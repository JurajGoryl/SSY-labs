https://github.com/JurajGoryl/SSY-labs/tree/main/projekt%20SSY%20hotovo

JURAJ GORYL 240913

Ako projekt boli zadané váhy s HX711 prevodníkom a LWM.



FUNKCIONALITA:

Po spustení kódu sa nám ponúkne menu kde si môžemem vybrať z viacerých možností, ktoré vykonávajú nejaké funkcie po stlačení správneho tlačidla sa vykoná danú funkcionalita. 



1 – Kalibrácia pomocou známeho závažia

2 – Odoslanie váhy na Gateway

0 – Vyčistenie terminálu

![Capture](https://github.com/user-attachments/assets/0e1dccb7-7d48-4f2a-88dd-2bf75ea1ef38)



KALIBRÁCIA POMOCU ZNÁMEHO ZÁVAŽIA.

Táto možnosť spustí kalibráciu pomocou známeho závažia. Kalibrácia prebieha tak, že najskôr je užívateľ vyzvaný na to aby sa ničoho nedotýkal. V tom okamihu je načítavyných viacero hodnôt, z ktorých je následne pomocou funkcie počítajúcej medián vypočítaná základná ADC hodnota váhy. Následne je užívateľ vyzvaný na to aby položil závažie známej váhy (200g) a stlačil ENTER na potvrdenie. Po stlačení ENTERu sa znova načítava viacej hodnôt s ktrých sa vypočíta mediám. Následne sa v konzole začne vypisovať aktuálna hmotnosť v gramoch.

![Capture2](https://github.com/user-attachments/assets/45a9815e-5cac-4550-8e8f-06f0be22ecf6)


ODOSLANIE NA GATEWAY
po stlačení tlačidla 2 sa zoberie aktuálna hodnota hmotnosti a pošle sa na Gateway 

![unnamed](https://github.com/user-attachments/assets/9deea029-01ce-4761-bec5-2061600c963b)

OPIS FUNKCIÍ:

unsigned long read_median(uint8_t times) 
{
	if (times == 0) return 0;

	unsigned long values[times];

	for (uint8_t i = 0; i < times; i++) 
	{
		values[i] = Hx711_read();
		_delay_ms(2);
	}

	qsort(values, times, sizeof(unsigned long), compare_ul);  

	if (times % 2 == 0) 
	{
		return (values[times / 2 - 1] + values[times / 2]) / 2;
	}
	else 
	{
		return values[times / 2];
	}
}

Vypočíta medián pre väčšiu presnoť kalibrácie.



void kalibracia() 
{
	UART_SendStringNewLine(">>> prebieha kalibraica <<<");
	UART_SendStringNewLine("nedotykaj sa");
	_delay_ms(2000);
	unsigned long zero_offset = read_median(1000);
	UART_SendStringNewLine("nula nastavena");

	// calibration of known mass
	UART_SendStringNewLine("poloz 200g zavazie a stlac ENTER");
	while (!(UART_DataAvailable()));  //waiting for input
	UART_GetChar();  //reading ENTER
	_delay_ms(2000);

	unsigned long loaded_value = read_median(1000);
	long diff = loaded_value - zero_offset;

	float known_weight = 200.0; // set known weight
	float scale = known_weight / diff;

	UART_SendStringNewLine("HOTOVO");

	while (1) 
	{
		unsigned long value = Hx711_read();
		long diff = value - zero_offset;
		float weight = diff * scale;

		char buf[30];
		sprintf(buf, "Vaha: %.2f g", weight);
		UART_SendStringNewLine(buf);

		_delay_ms(500);
	}
}

Kalibračná funkcia so známim závažím.


void Hx711_init(void) {
	ADDO_DDR &= ~(1 << ADDO_BIT);  // Set ADDO as input
	ADDO_PORT |= (1 << ADDO_BIT);  // Enable pull-up (if needed)
	
	ADSK_DDR |= (1 << ADSK_BIT);   // Set ADSK as output
	ADSK_PORT &= ~(1 << ADSK_BIT); // Start ADSK low
}

unsigned long Hx711_read(void) 
{
	unsigned long Count = 0;
	uint8_t i;

	// Wait until ADDO goes low (indicates data ready)
	while (ADDO_PIN & (1 << ADDO_BIT));

	// Read 24-bit data
	for (i = 0; i < 24; i++) 
	{
		ADSK_PORT |= (1 << ADSK_BIT);  // Set ADSK high
		Count = Count << 1;
		ADSK_PORT &= ~(1 << ADSK_BIT); // Set ADSK low

		if (ADDO_PIN & (1 << ADDO_BIT)) 
		{
			Count++;
		}
	}

	// Send extra clock pulse to set device mode
	ADSK_PORT |= (1 << ADSK_BIT);
	Count ^= 0x800000; // Convert signed 24-bit value
	ADSK_PORT &= ~(1 << ADSK_BIT);

	return Count;
}

Inicializácia prevodníka a odčítane aktuálnej hodnoty.
