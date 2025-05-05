/*
 * commands.c
 *
 * WSNDemo command handler implementation
 */

#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "nwk.h"
#include "sysTimer.h"
#include "halLed.h"
#include "commands.h"

// ----------------------------
// Definície a konfigurácia
// ----------------------------
#define APP_CMD_UART_BUFFER_SIZE    16
#define APP_CMD_PENDING_TABLE_SIZE  5
#define APP_CMD_INVALID_ADDR        0xffff

#define LED_IDENTIFY                0
#define APP_CMD_ENDPOINT            15

// ----------------------------
// Typy
// ----------------------------
typedef enum {
  APP_CMD_UART_STATE_IDLE,
  APP_CMD_UART_STATE_SYNC,
  APP_CMD_UART_STATE_DATA,
  APP_CMD_UART_STATE_MARK,
  APP_CMD_UART_STATE_CSUM,
} AppCmdUartState_t;

typedef struct PACK {
  uint8_t commandId;
  uint64_t dstAddr;
} AppCmdUartHeader_t;

typedef struct PACK {
  uint8_t commandId;
  uint64_t dstAddr;
  uint16_t duration;
  uint16_t period;
} AppCmdUartIdentify_t;

typedef struct PACK {
  uint8_t id;
} AppCmdHeader_t;

typedef struct PACK {
  uint8_t id;
  uint16_t duration;
  uint16_t period;
} AppCmdIdentify_t;

typedef struct PACK {
  uint16_t addr;
  uint8_t size;
  bool ready;
  union {
    uint8_t payload;
    AppCmdHeader_t header;
    AppCmdIdentify_t identify;
  };
} AppCmdPendingTableEntry_t;

// ----------------------------
// Lokálne premenné
// ----------------------------
static AppCmdUartState_t appCmdUartState = APP_CMD_UART_STATE_IDLE;
static uint8_t appCmdUartPtr;
static uint8_t appCmdUartBuf[APP_CMD_UART_BUFFER_SIZE];
static uint8_t appCmdUartCsum;

static SYS_Timer_t appCmdIdentifyDurationTimer;
static SYS_Timer_t appCmdIdentifyPeriodTimer;

static AppCmdPendingTableEntry_t appCmdPendingTable[APP_CMD_PENDING_TABLE_SIZE];
static AppCmdPendingTableEntry_t *appCmdInProgress;
static NWK_DataReq_t appCmdDataReq;

// ----------------------------
// Prototypy lokálnych funkcií
// ----------------------------
static void appCmdUartProcess(uint8_t *data, uint8_t size);
static void appCmdBuffer(uint16_t addr, uint8_t *data, uint8_t size);
static void appCmdDataRequest(AppCmdPendingTableEntry_t *entry);
static void appCmdDataConf(NWK_DataReq_t *req);
static void appCmdCheckPendingTable(void);
static bool appCmdDataInd(NWK_DataInd_t *ind);
static bool appCmdHandle(uint8_t *data, uint8_t size);
static void appCmdIdentifyDurationTimerHandler(SYS_Timer_t *timer);
static void appCmdIdentifyPeriodTimerHandler(SYS_Timer_t *timer);

// ----------------------------
// Implementácia
// ----------------------------

/**
 * Inicializácia správy príkazov.
 */
void APP_CommandsInit(void)
{
  appCmdIdentifyDurationTimer.mode = SYS_TIMER_INTERVAL_MODE;
  appCmdIdentifyDurationTimer.handler = appCmdIdentifyDurationTimerHandler;

  appCmdIdentifyPeriodTimer.mode = SYS_TIMER_PERIODIC_MODE;
  appCmdIdentifyPeriodTimer.handler = appCmdIdentifyPeriodTimerHandler;

  appCmdInProgress = NULL;
  appCmdDataReq.dstAddr = 0;
  appCmdDataReq.dstEndpoint = APP_CMD_ENDPOINT;
  appCmdDataReq.srcEndpoint = APP_CMD_ENDPOINT;
  appCmdDataReq.options = NWK_OPT_ENABLE_SECURITY;
  appCmdDataReq.confirm = appCmdDataConf;

  for (uint8_t i = 0; i < APP_CMD_PENDING_TABLE_SIZE; i++) {
    appCmdPendingTable[i].addr = APP_CMD_INVALID_ADDR;
    appCmdPendingTable[i].ready = false;
  }

  NWK_OpenEndpoint(APP_CMD_ENDPOINT, appCmdDataInd);
}

/**
 * Ozna?enie zariadenia ako pripravené na príkaz.
 */
bool APP_CommandsPending(uint16_t addr)
{
  for (uint8_t i = 1; i < APP_CMD_PENDING_TABLE_SIZE; i++) {
    if (addr == appCmdPendingTable[i].addr) {
      appCmdPendingTable[i].ready = true;
      appCmdCheckPendingTable();
      return true;
    }
  }
  return false;
}

/**
 * Spracovanie prijatého bajtu cez UART.
 */
void APP_CommandsByteReceived(uint8_t byte)
{
  switch (appCmdUartState) {
    case APP_CMD_UART_STATE_IDLE:
      if (0x10 == byte) {
        appCmdUartPtr = 0;
        appCmdUartCsum = byte;
        appCmdUartState = APP_CMD_UART_STATE_SYNC;
      }
      break;

    case APP_CMD_UART_STATE_SYNC:
      appCmdUartCsum += byte;
      appCmdUartState = (0x02 == byte) ? APP_CMD_UART_STATE_DATA : APP_CMD_UART_STATE_IDLE;
      break;

    case APP_CMD_UART_STATE_DATA:
      appCmdUartCsum += byte;
      if (0x10 == byte) {
        appCmdUartState = APP_CMD_UART_STATE_MARK;
      } else {
        appCmdUartBuf[appCmdUartPtr++] = byte;
        if (appCmdUartPtr == APP_CMD_UART_BUFFER_SIZE)
          appCmdUartState = APP_CMD_UART_STATE_IDLE;
      }
      break;

    case APP_CMD_UART_STATE_MARK:
      appCmdUartCsum += byte;
      if (0x10 == byte) {
        appCmdUartBuf[appCmdUartPtr++] = byte;
        appCmdUartState = (appCmdUartPtr == APP_CMD_UART_BUFFER_SIZE) ? APP_CMD_UART_STATE_IDLE : APP_CMD_UART_STATE_DATA;
      } else if (0x03 == byte) {
        appCmdUartState = APP_CMD_UART_STATE_CSUM;
      } else {
        appCmdUartState = APP_CMD_UART_STATE_IDLE;
      }
      break;

    case APP_CMD_UART_STATE_CSUM:
      if (byte == appCmdUartCsum)
        appCmdUartProcess(appCmdUartBuf, appCmdUartPtr);
      appCmdUartState = APP_CMD_UART_STATE_IDLE;
      break;

    default:
      break;
  }
}

// --- Lokálne pomocné funkcie ---

static void appCmdUartProcess(uint8_t *data, uint8_t size)
{
  if (size < sizeof(AppCmdUartHeader_t))
    return;

  AppCmdUartHeader_t *header = (AppCmdUartHeader_t *)data;

  if (APP_COMMAND_ID_IDENTIFY == header->commandId) {
    AppCmdUartIdentify_t *uartCmd = (AppCmdUartIdentify_t *)data;
    AppCmdIdentify_t cmd = {
      .id = APP_COMMAND_ID_IDENTIFY,
      .duration = uartCmd->duration,
      .period = uartCmd->period
    };
    appCmdBuffer(header->dstAddr, (uint8_t *)&cmd, sizeof(AppCmdIdentify_t));
  }
}

static void appCmdBuffer(uint16_t addr, uint8_t *data, uint8_t size)
{
  if (APP_ADDR == addr) {
    appCmdHandle(data, size);
  } else if (addr & NWK_ROUTE_NON_ROUTING) {
    for (uint8_t i = 1; i < APP_CMD_PENDING_TABLE_SIZE; i++) {
      if (APP_CMD_INVALID_ADDR == appCmdPendingTable[i].addr) {
        appCmdPendingTable[i].addr = addr;
        appCmdPendingTable[i].size = size;
        appCmdPendingTable[i].ready = false;
        memcpy(&appCmdPendingTable[i].payload, data, size);
        break;
      }
    }
  } else {
    if (APP_CMD_INVALID_ADDR == appCmdPendingTable[0].addr) {
      appCmdPendingTable[0].addr = addr;
      appCmdPendingTable[0].size = size;
      appCmdPendingTable[0].ready = true;
      memcpy(&appCmdPendingTable[0].payload, data, size);
      appCmdCheckPendingTable();
    }
  }
}

static void appCmdDataRequest(AppCmdPendingTableEntry_t *entry)
{
  appCmdInProgress = entry;
  appCmdDataReq.dstAddr = entry->addr;
  appCmdDataReq.data = &entry->payload;
  appCmdDataReq.size = entry->size;
  NWK_DataReq(&appCmdDataReq);
}

static void appCmdDataConf(NWK_DataReq_t *req)
{
  appCmdInProgress->addr = APP_CMD_INVALID_ADDR;
  appCmdInProgress->ready = false;
  appCmdInProgress = NULL;

  appCmdCheckPendingTable();
  (void)req;
}

static void appCmdCheckPendingTable(void)
{
  if (appCmdInProgress)
    return;

  for (uint8_t i = 0; i < APP_CMD_PENDING_TABLE_SIZE; i++) {
    if (appCmdPendingTable[i].ready)
      appCmdDataRequest(&appCmdPendingTable[i]);
  }
}

static bool appCmdDataInd(NWK_DataInd_t *ind)
{
  return appCmdHandle(ind->data, ind->size);
}

static bool appCmdHandle(uint8_t *data, uint8_t size)
{
  if (size < sizeof(AppCmdHeader_t))
    return false;

  AppCmdHeader_t *header = (AppCmdHeader_t *)data;

  if (APP_COMMAND_ID_IDENTIFY == header->id) {
    AppCmdIdentify_t *req = (AppCmdIdentify_t *)data;

    if (sizeof(AppCmdIdentify_t) != size)
      return false;

    SYS_TimerStop(&appCmdIdentifyDurationTimer);
    SYS_TimerStop(&appCmdIdentifyPeriodTimer);

    appCmdIdentifyDurationTimer.interval = req->duration;
    SYS_TimerStart(&appCmdIdentifyDurationTimer);

    appCmdIdentifyPeriodTimer.interval = req->period;
    SYS_TimerStart(&appCmdIdentifyPeriodTimer);

    HAL_LedOn(LED_IDENTIFY);
    NWK_Lock();
    return true;
  }

  return false;
}

static void appCmdIdentifyDurationTimerHandler(SYS_Timer_t *timer)
{
  NWK_Unlock();
  HAL_LedOn(LED_IDENTIFY);
  SYS_TimerStop(&appCmdIdentifyPeriodTimer);
  (void)timer;
}

static void appCmdIdentifyPeriodTimerHandler(SYS_Timer_t *timer)
{
  HAL_LedToggle(LED_IDENTIFY);
  (void)timer;
}
