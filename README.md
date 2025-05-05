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


