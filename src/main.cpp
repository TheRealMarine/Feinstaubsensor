//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

#include <display.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

//////////////////////////////////////////////////
/////////////////////Defines//////////////////////
//////////////////////////////////////////////////

#define Zeile_1 0x00 // Zeilen Definition des Displays
#define Zeile_2 0x40 // Zeilen Definition des Displays
#define Zeile_3 0x0A // Zeilen Definition des Displays
#define Zeile_4 0x4A // Zeilen Definition des Displays
#define FeinstaubsensorRxD 52 // Definition des Digital Pin
#define FeinstaubsensorTxD 53 // Definition des Digital Pin
#define TemperatursensorOWP 15 // Definition des Digital Pin

SoftwareSerial Feinstaubsensor (52, 53); // Deklarierung des Feinstaubsensors an RxD Pin 52 und TxD Pin 53 als SoftwareSerial objekt

//////////////////////////////////////////////////
////////////////////Variablen/////////////////////
//////////////////////////////////////////////////

uint8_t Feinstaubsensor_PM25 = 0; // PM2.5 Wert des Feinstaubssensors
uint8_t Feinstaubsensor_PM10 = 0; // PM10 Wert des Feinstaubssensors
uint16_t Temperatursensor_Luftfeuchtigkeit = 0; // Luftfeuchtigkeit des Temperatursensors
uint16_t Temperatursensor_Temperatur = 0; // Temperatur des Temperatursensors
uint8_t FeinstaubsensorDaten[10];

//////////////////////////////////////////////////
////////////////////Funktionen////////////////////
//////////////////////////////////////////////////

/**
 * @brief Diese Funktion dient der vereinfachten Ausgabe von Text.
 *
 * @param text
 * 		  Text welcher auf dem Display ausgegeben werden soll.
 */
void display_text(String text)
{
	text = text + "$";
	char character = text.charAt(0); // Ersten Buchstaben/Zeichen auswählen
	text.remove(0);				 // Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
	while (character != '$')		 // Nur solange der ausgewählte Buchstabe/Zeichen nicht das endungszeichen "$" ist, write_char ausführen
	{
		write_char(character);		// Ausgewählter Buchstabe/Zeichen auf display ausgeben
		character = text.charAt(0); // Ersten Buchstaben/Zeichen auswählen
		text.remove(0, 1);			// Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
	}
}

/**
 * @brief Programmiert Sonderzeichen in das CGRAM des Displays.
 */
void sonderzeichen()
{
	write_instr(0x40); // Die CGRAM auf Addresse 0 stellen
	for (uint8_t i = 0; i <= 7; i++)
	{							// Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(pfeilnu[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (uint8_t i = 0; i <= 7; i++)
	{					   // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(my[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (uint8_t i = 0; i <= 7; i++)
	{						  // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(hoch3[i]); // Ausgewählte Zeile an CGRAM senden
	}
}

/**
 * @brief Es speichert anhand des deklarierten SoftwareSerial Ports die eingehenden Daten
 * 		  und verarbeitet diese zu den verschiedenen Feinstaubarten. Die Werte werden in
 * 	   	  folgenden Globale Variablen gespeichert:
 *		  SDS_PM25
 *		  SDS_PM10
 *
 * @return Gibt 1 (true) zurück wenn Parität besteht
 */
bool FeinstaubsensorMessung() // TODO BUGFIX: Liest daten viermal oder siebenmal aus und nicht einmal
{
	while (Feinstaubsensor.available())
	{
		if (Feinstaubsensor.read() == 0xAA)
		{
			FeinstaubsensorDaten[0] = 0xAA;
			for (int i = 1; i < 10; i++)
			{
				FeinstaubsensorDaten[i] = Feinstaubsensor.read();
			}
		}
	}
	unsigned char ParityBit = FeinstaubsensorDaten[8];
	unsigned char CalculatedParityBit = FeinstaubsensorDaten[2] + FeinstaubsensorDaten[3] + FeinstaubsensorDaten[4] + FeinstaubsensorDaten[5] + FeinstaubsensorDaten[6] + FeinstaubsensorDaten[7];
	if (ParityBit == CalculatedParityBit)
	{
		Feinstaubsensor_PM10 = ((FeinstaubsensorDaten[3] *256) + FeinstaubsensorDaten[2])/10;
		Feinstaubsensor_PM25 = ((FeinstaubsensorDaten[5] *256) + FeinstaubsensorDaten[4])/10;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Sie misst die Temperatur und Luftfeuchtigkeit des DHT22 Sensors und speichert diese in globalen Variablen.
 * 
 * @return Gibt 1 (true) zurück wenn Parität besteht
 */
bool TemperatursensorMessung()
{
	// Start Signal
	pinMode(TemperatursensorOWP, OUTPUT);
	digitalWrite(TemperatursensorOWP, LOW);
	delay(18);
	digitalWrite(TemperatursensorOWP, HIGH);
	pinMode(TemperatursensorOWP, INPUT);
	digitalWrite(TemperatursensorOWP, HIGH);

	// Daten auslesen
	uint16_t rawHumidity = 0;
	uint16_t rawTemperature = 0;
	uint8_t checkSum = 0;
	uint16_t data = 0;
	uint32_t startTime;

	for (int8_t i = -3; i < 80; i++)
	{
		byte live;
		startTime = micros();

		do
		{
			live = (uint64_t)(micros() - startTime);
		} while (digitalRead(TemperatursensorOWP) == (i & 1) ? HIGH : LOW);

		if (i >= 0 && (i & 1))
		{
			data <<= 1; // Verschiebt das letzte Bit eins nach links
			if (live > 30) // Wenn der Pin über 30 us auf HIGH ist, ist es eine 1
			{
				data |= 1; // Setzt das korrekte Bit auf 1
			}
		}

		switch (i)
		{
		case 31:
			rawHumidity = data;
			break;
		case 63:
			rawTemperature = data;
			break;
		case 79:
			checkSum = data;
			data = 0;
			break;
		}
	}

	uint8_t hTemp = rawTemperature >> 8; // First (Higher) 8 Bits of 16 Bits
	rawTemperature = rawTemperature << 8; // Move 8 Bits to left
	uint8_t lTemp = rawTemperature >> 8; // Second (Lower) 8 Bits of 16 Bits

	uint8_t hHumidity = rawHumidity >> 8; // First (Higher) 8 Bits of 16 Bits
	rawHumidity = rawHumidity << 8; // Move 8 Bits to left
	uint8_t lHumidity = rawHumidity >> 8; // Second (Lower) 8 Bits of 16 Bits

	uint8_t calculatedCheckSum = hTemp + lTemp + hHumidity + lHumidity;
	if (checkSum == calculatedCheckSum) // Nur wenn Parität besteht die gerade ausgelesenen Daten eintragen, ansonsten Daten der letzten Messung verwenden
	{
		Temperatursensor_Temperatur = hTemp + lTemp;
		Temperatursensor_Luftfeuchtigkeit = hHumidity + lHumidity;
		return true;
	} else {
		return false;
	}
}
//////////////////////////////////////////////////
///////////////////////Main///////////////////////
//////////////////////////////////////////////////

/* Hier wird alles einmal ausgeführt vor dem Hauptprogramm. Demnach ist dies unsere Initialisierung. */
void setup()
{
	// Port Ein-/Ausgänge
	DDRA = B11111111; // Ausgang Display
	DDRC = B11111111; // Ausgang Display
	DDRB = B11111101; // Eingang/Ausgang Feinstaubsensor
	DDRJ = B11111110; // Eingang Temperatursensor
	PORTB = B11111101; // LED's ausmachen

	// Terminal/Konsole
	Serial.begin(9600);

	// Weitere Inits
	lcd_init();
	sonderzeichen();

	// Sensoren
	Feinstaubsensor.begin(9600);

	FeinstaubsensorMessung();
	delay(1000);
}

/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop()
{
	FeinstaubsensorMessung();

	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(Zeile_1);
	display_text("Feinstaub-"); // "Feinstaub-"
	display_pos(Zeile_2);
	display_text("daten:"); // "daten:"

	// PM2.5 sowie den Wert davon anzeigen
	display_pos(Zeile_3);
	display_text("PM2.5:"); // "PM2.5:"
	display_pos(Zeile_4);
	display_text((String)Feinstaubsensor_PM25); // Wert von PM2.5 ausgeben als umgewandelten String
	write_instr(0x0C); // Cursor aus

	delay(4000);

	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(Zeile_1);
	display_text("Feinstaub-"); // "Feinstaub-"
	display_pos(Zeile_2);
	display_text("daten:"); // "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(Zeile_3);
	display_text("PM10:"); // "PM10:"
	display_pos(Zeile_4);
	display_text((String)Feinstaubsensor_PM10); // Wert von PM10 ausgeben als umgewandelten String
	write_instr(0x0C); // Cursor aus

	TemperatursensorMessung();
	delay(4000);

	float x	= (float)Temperatursensor_Temperatur / 10; // Setzt das Komma eins nach links
	float y = (float)Temperatursensor_Luftfeuchtigkeit / 10; // Setzt das Komma eins nach links
	String temp = (String)x; // Wandelt Float sowie das Komma zu Text um
	String humi = (String)y; // Wandelt Float sowie das Komma zu Text um
	temp.remove(temp.length() - 1); // Entfernt letzte null da immer null
	humi.remove(humi.length() - 1); // Entfernt letzte null da immer null

	write_instr(0x01); // Display löschen

	// "Temperatur" sowie den Wert anzeigen
	display_pos(Zeile_1);
	display_text("Temperatur:"); // "Temperatur"
	display_pos(Zeile_2);
	display_text(temp); // Wert von Temperatursensor_Temperatur ausgeben

	// "Humidity" sowie den Wert anzeigen
	display_pos(Zeile_3);
	display_text("Humidity:"); // "Humidity:" anzeigen
	display_pos(Zeile_4);
	display_text(humi); // Wert von Temperatursensor_Luftfeuchtigkeit ausgeben
	write_instr(0x0C); // Cursor aus

	delay(4000);
}