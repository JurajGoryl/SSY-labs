##  SSY  Projekt – LWM + HX711
https://github.com/JurajGoryl/SSY-labs/tree/main/projekt%20SSY%20hotovo
**Juraj Goryl | 240913**

Tento projekt bol vypracovaný v rámci predmetu MPC-SSY. Ide o váhový systém využívajúci prevodník HX711 a bezdrôtovú komunikáciu cez LWM.

---

## Funkcionalita

Po spustení sa zobrazí menu v UART termináli (napr. PuTTY, 38400 baud), ktoré umožňuje ovládať jednotlivé funkcie stlačením čísla:

* `1` – Kalibrácia pomocou známeho závažia
* `2` – Odoslanie váhy na Gateway
* `0` – Vyčistenie terminálu


![image](https://github.com/user-attachments/assets/98809833-21b0-4c00-a1cc-d729ac80165e)

---

## KALIBRÁCIA POMOCU ZNÁMEHO ZÁVAŽIA

Táto možnosť spustí proces kalibrácie:

1. Používateľ sa vyzve, aby sa ničím nedotýkal.
2. Systém načíta 1000 vzoriek a vypočíta medián ako **nulovú hodnotu**.
3. Následne sa vyzve položiť 200g závažie a potvrdiť ENTERom.
4. Z ďalších 1000 vzoriek sa vypočíta **skalovací faktor**.
5. Váha potom periodicky zobrazuje meranú hmotnosť v gramoch.


![Capture2](https://github.com/user-attachments/assets/5ed088cb-cfda-4a31-b421-09cec1b3a0c8)

---

## ODOSLANIE NA GATEWAY

Po stlačení tlačidla `2` sa aktuálna hmotnosť načíta a odošle na bezdrôtovú gateway cez LWM protokol.  Nasledujúca cast kódu je vytvorenie bloku na odoslanie pomocou LWM.

```c
static void appSendData(void)
{
#ifdef NWK_ENABLE_ROUTING
    appMsg.parentShortAddr = NWK_RouteNextHop(0, 0);
#else
    appMsg.parentShortAddr = 0;
#endif

    tmp_value = Hx711_read();

    float weight_grams = tmp_value - read_median(1000);

    appMsg.sensors.weight = (int)weight_grams; //ADC_get(3);

#if defined(APP_COORDINATOR)
    appUartSendMessageHR((uint8_t *)&appMsg, sizeof(appMsg));
    SYS_TimerStart(&appDataSendingTimer);
    appState = APP_STATE_WAIT_SEND_TIMER;
#else
    appNwkDataReq.dstAddr = 0;
    appNwkDataReq.dstEndpoint = APP_ENDPOINT;
    appNwkDataReq.srcEndpoint = APP_ENDPOINT;
    appNwkDataReq.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY;
    appNwkDataReq.data = (uint8_t *)&appMsg;
    appNwkDataReq.size = sizeof(appMsg);
    appNwkDataReq.confirm = appDataConf;

    HAL_LedOn(APP_LED_DATA);
    NWK_DataReq(&appNwkDataReq);

    appState = APP_STATE_WAIT_CONF;
#endif
}
```

&#x9;

&#x20;

&#x20;&#x20;

&#x20;&#x20;


![unnamed](https://github.com/user-attachments/assets/32ceb37d-ae77-43ae-995a-9d6c146b2393)




---

## Opis Funkcií (kód)

### `read_median()` – Výpočet mediánu z viacerých hodnôt

```c
unsigned long read_median(uint8_t times) {
    if (times == 0) return 0;

    unsigned long values[times];
    for (uint8_t i = 0; i < times; i++) {
        values[i] = Hx711_read();
        _delay_ms(2);
    }

    qsort(values, times, sizeof(unsigned long), compare_ul);

    if (times % 2 == 0)
        return (values[times / 2 - 1] + values[times / 2]) / 2;
    else
        return values[times / 2];
}
```

Používa sa na stabilnejšie čítanie počas kalibrácie.

---

### `kalibracia()` – Kalibrácia váhy pomocou 200g

```c
void kalibracia() {
    UART_SendStringNewLine(">>> prebieha kalibracia <<<");
    UART_SendStringNewLine("nedotykaj sa");
    _delay_ms(2000);

    unsigned long zero_offset = read_median(1000);
    UART_SendStringNewLine("nula nastavena");

    UART_SendStringNewLine("poloz 200g zavazie a stlac ENTER");
    while (!(UART_DataAvailable()));
    UART_GetChar();
    _delay_ms(2000);

    unsigned long loaded_value = read_median(1000);
    long diff = loaded_value - zero_offset;
    float known_weight = 200.0;
    float scale = known_weight / diff;

    UART_SendStringNewLine("HOTOVO");

    while (1) {
        unsigned long value = Hx711_read();
        long diff = value - zero_offset;
        float weight = diff * scale;

        char buf[30];
        sprintf(buf, "Vaha: %.2f g", weight);
        UART_SendStringNewLine(buf);

        _delay_ms(500);
    }
}
```

Kalibračná funkcia so známym závažím.

---

### `Hx711_init()` a `Hx711_read()` – Inicializácia a čítanie

```c
void Hx711_init(void) {
    ADDO_DDR &= ~(1 << ADDO_BIT);
    ADDO_PORT |= (1 << ADDO_BIT);
    ADSK_DDR |= (1 << ADSK_BIT);
    ADSK_PORT &= ~(1 << ADSK_BIT);
}

unsigned long Hx711_read(void) {
    unsigned long Count = 0;
    uint8_t i;

    while (ADDO_PIN & (1 << ADDO_BIT));

    for (i = 0; i < 24; i++) {
        ADSK_PORT |= (1 << ADSK_BIT);
        Count = Count << 1;
        ADSK_PORT &= ~(1 << ADSK_BIT);

        if (ADDO_PIN & (1 << ADDO_BIT)) Count++;
    }

    ADSK_PORT |= (1 << ADSK_BIT);
    Count ^= 0x800000;
    ADSK_PORT &= ~(1 << ADSK_BIT);

    return Count;
}
```

Inicializácia prevodníka a čítanie dát z HX711.

---
