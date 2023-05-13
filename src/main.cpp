//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

#include <display.h> // Vorgebene Display Library (leicht verändert wegen Port änderung)
#include <SoftwareSerial.h> // Library zur auslesung von Daten mit einem SoftwareSerial objekt (2 Wire Protocol)
#include <Arduino.h> // Reguläre Arduino Library

//////////////////////////////////////////////////
/////////////////////Defines//////////////////////
//////////////////////////////////////////////////

#define Zeile_1 0x00 // Zeilen Definition des Displays
#define Zeile_2 0x40 // Zeilen Definition des Displays
#define Zeile_3 0x0A // Zeilen Definition des Displays
#define Zeile_4 0x4A // Zeilen Definition des Displays
#define FeinstaubsensorRxD 52 // Definition des Digital Pins
#define FeinstaubsensorTxD 53 // Definition des Digital Pins
#define TemperatursensorOWP 15 // Definition des Digital Pins

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

SoftwareSerial Feinstaubsensor (FeinstaubsensorRxD, FeinstaubsensorTxD); // Deklarierung des Feinstaubsensors an RxD Pin 52 und TxD Pin 53 als SoftwareSerial objekt

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
	text.remove(0, 1);				 // Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
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
	for (uint8_t i = 0; i <= 7; i++) // Zeile auswählen und um eine weitere Zeile erhöhen
	{
		write_char(my[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (uint8_t i = 0; i <= 7; i++) // Zeile auswählen und um eine weitere Zeile erhöhen
	{
		write_char(hoch3[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (uint8_t i = 0; i <= 7; i++) // Zeile auswählen und um eine weitere Zeile erhöhen
	{
		write_char(grad[i]); // Ausgewählte Zeile an CGRAM senden
	}
}

/**
 * @brief Es speichert anhand des deklarierten SoftwareSerial Ports die eingehenden Daten
 * 		  und verarbeitet diese zu den verschiedenen Feinstaubarten. Die Werte werden in
 * 	   	  folgenden Globale Variablen gespeichert:
 *		  Feinstaubsensor_PM25
 *		  Feinstaubsensor_PM10
 *
 * @return Gibt 1 (true) zurück wenn Parität besteht
 */
bool FeinstaubsensorMessung()
{
	while (Feinstaubsensor.available()) // Wenn Daten zum abrufen Verfügbar sind
	{
		if (Feinstaubsensor.read() == 0xAA) // überprüfen ob erster Byte korrekt ist (muss 0xAA entsprechen laut Datenblatt)
		{
			FeinstaubsensorDaten[0] = 0xAA; // Wenn korrekt, diesen in das Array eintragen
			for (uint8_t i = 1; i < 10; i++) // Alle weiteren Bytes nacheinander einlesen
			{
				FeinstaubsensorDaten[i] = Feinstaubsensor.read();
			}
		}
	}
	uint8_t ParityByte = FeinstaubsensorDaten[8]; // Parität Byte an Stelle 8 im Array
	uint8_t CalculatedParityByte = FeinstaubsensorDaten[2] + FeinstaubsensorDaten[3] + FeinstaubsensorDaten[4] + FeinstaubsensorDaten[5] + FeinstaubsensorDaten[6] + FeinstaubsensorDaten[7];
	if (ParityByte == CalculatedParityByte) // Nur wenn Parität besteht die gerade ausgelesenen Daten eintragen, ansonsten Daten der letzten Messung verwenden
	{
		Feinstaubsensor_PM10 = ((FeinstaubsensorDaten[3] * 256) + FeinstaubsensorDaten[2]) / 10; // Die beiden ausgelesenen Bytes des Feinstaubsensors zu dem korrekten Feinstaubwert verarbeiten (PM10)
		Feinstaubsensor_PM25 = ((FeinstaubsensorDaten[5] * 256) + FeinstaubsensorDaten[4]) / 10; // Die beiden ausgelesenen Bytes des Feinstaubsensors zu dem korrekten Feinstaubwert verarbeiten (PM2.5)
		return true; // Parität
	}
	else
	{
		return false; // Keine Parität
	}
}

/**
 * @brief Sie misst die Temperatur und Luftfeuchtigkeit des DHT22 Sensors und speichert diese
 * 		  in folgenden globalen Variablen:
 * 		  Temperatursensor_Temperatur
 * 		  Temperatursensor_Luftfeuchtigkeit
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
		uint8_t live;
		startTime = micros(); // Zeit bei Start speichern

		do
		{
			live = (uint64_t)(micros() - startTime); // Misst wie lange der TemperatursensorOWP Pin auf 1 (HIGH) ist
		} while (digitalRead(TemperatursensorOWP) == (i & 1) ? HIGH : LOW);

		if (i >= 0 && (i & 1))
		{
			data <<= 1; // Verschiebt das letzte Bit eins nach links
			if (live > 30) // Wenn der Pin über 30 us auf 1 (HIGH) ist, ist es eine 1 als data
			{
				data |= 1; // Setzt das korrekte Bit auf 1
			}
		}

		switch (i) // Daten in korrekte Variablen eintragen je nach empfangenen Bit
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

	uint8_t hTemp = rawTemperature >> 8; // Erste (Higher) 8 Bits von 16 Bits
	rawTemperature = rawTemperature << 8; // Bewege 8 Bits nach links
	uint8_t lTemp = rawTemperature >> 8; // Letzte (Lower) 8 Bits von 16 Bits

	uint8_t hHumidity = rawHumidity >> 8; // Erste (Higher) 8 Bits von 16 Bits
	rawHumidity = rawHumidity << 8; // Bewege 8 Bits nach links
	uint8_t lHumidity = rawHumidity >> 8; // Letzte (Lower) 8 Bits von 16 Bits

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

/**
 * @brief Sie misst die Temperatur und Luftfeuchtigkeit des DHT22 Sensors und speichert diese
 * 		  in folgenden globalen Variablen:
 * 		  Temperatursensor_Temperatur
 * 		  Temperatursensor_Luftfeuchtigkeit
 * 
 * @return Gibt 1 (true) zurück wenn Parität besteht
 */
void ZusatzFunktionen()
{
	// Wenn luftfeuchtigkeit über 70% ist, Feinstaubsensor deaktivieren da dieser sonnst beschädigt wird (siehe datenblatt)
	if (Temperatursensor_Luftfeuchtigkeit >= 70) {
		pinMode(PORTB1, OUTPUT); // Eingang des Feinstaubsensors deaktivieren
	} else {
		pinMode(PORTB1, INPUT); // Eingang des Feinstaubsensors reaktivieren
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
	//Serial.begin(9600); // Serial Konsole für DEBUG

	// Weitere Inits
	lcd_init(); // Display initialisieren
	sonderzeichen(); // Sonderzeichen in CGRAM schreiben
	write_instr(0x0C); // Cursor des Displays deaktivieren

	// Sensoren
	Feinstaubsensor.begin(9600); // Feinstaubsensor mit der Übertragungsrate (Baudrate) von 9600 starten

	FeinstaubsensorMessung(); // Daten des Feinstaubsensors auslesen (Hier vor Delay in der Init damit die Daten beim Start des Arduino korrekt verarbeitet werden)
	delay(1000); // Kurzer Delay (1 Sekunde) um sicher zu gehen das alles korrekt initialisiert ist
}

/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop()
{
	Serial.println(Temperatursensor_Luftfeuchtigkeit);
	FeinstaubsensorMessung(); // Daten des Feinstaubsensors auslesen
	write_instr(0x01); // Display Inhalt löschen

	// "Feinstaubdaten" anzeigen
	display_pos(Zeile_1);
	display_text("Feinstaub-"); // "Feinstaub-" anzeigen
	display_pos(Zeile_2);
	display_text("daten:"); // "daten:" anzeigen

	// PM2.5 sowie den Wert davon anzeigen
	display_pos(Zeile_3);
	display_text("PM2.5:"); // "PM2.5:" anzeigen
	display_pos(Zeile_4);
	display_text((String)Feinstaubsensor_PM25); // Wert von PM2.5 ausgeben als umgewandelten String
	write_char(Z_my); // "µ" anzeigen
	display_text("g/m"); // "g/m" anzeigen
	write_char(Z_hoch3); // "³" anzeigen

	delay(4000); // Zeit zwischen den verschiedenen Anzeigen
	write_instr(0x01); // Display Inhalt löschen

	// "Feinstaubdaten" anzeigen
	display_pos(Zeile_1);
	display_text("Feinstaub-"); // "Feinstaub-" anzeigen
	display_pos(Zeile_2);
	display_text("daten:"); // "daten:" anzeigen

	// PM10 sowie den Wert davon anzeigen
	display_pos(Zeile_3);
	display_text("PM10:"); // "PM10:" anzeigen
	display_pos(Zeile_4);
	display_text((String)Feinstaubsensor_PM10); // Wert von PM10 ausgeben als umgewandelten String
	write_char(Z_my); // "µ" anzeigen
	display_text("g/m"); // "g/m" anzeigen
	write_char(Z_hoch3); // "³" anzeigen

	TemperatursensorMessung(); // Daten des Temperatursensors auslesen
	delay(4000); // Zeit zwischen den verschiedenen Anzeigen

	float x	= (float)Temperatursensor_Temperatur / 10; // Wandelt Ganzzahl zu Gleitkommazahl um und setzt das Komma eins nach links
	String temp = (String)x; // Wandelt Gleitkommazahl sowie das dazugehörige Komma zu Text um
	temp.remove(temp.length() - 1); // Entfernt letzte zahl da immer null
	String humi = (String)Temperatursensor_Luftfeuchtigkeit; // Wandelt Luftfeuchtigkeit als Ganzzahl zu Text um

	write_instr(0x01); // Display Inhalt löschen

	// "Temperatur" sowie den Wert anzeigen
	display_pos(Zeile_1);
	display_text("Temperatur:"); // "Temperatur:" anzeigen
	display_pos(Zeile_2);
	display_text(temp); // Wert von Temperatursensor_Temperatur ausgeben
	write_char(Z_grad); // "°" anzeigen
	display_text("C"); // "C" anzeigen

	// "Humidity" sowie den Wert anzeigen
	display_pos(Zeile_3);
	display_text("Humidity:"); // "Humidity:" anzeigen
	display_pos(Zeile_4);
	display_text(humi); // Wert von Temperatursensor_Luftfeuchtigkeit ausgeben
	display_text("%"); // "%" anzeigen

	delay(4000); // Zeit zwischen den verschiedenen Anzeigen
	ZusatzFunktionen();
}