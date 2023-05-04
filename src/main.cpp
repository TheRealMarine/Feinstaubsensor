//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

#include <display.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

//////////////////////////////////////////////////
/////////////////////Defines//////////////////////
//////////////////////////////////////////////////

#define Zeile_1 0x00
#define Zeile_2 0x40
#define Zeile_3 0x0A
#define Zeile_4 0x4A

SoftwareSerial Feinstaubsensor (52, 53); // Deklarierung des Feinstaubsensors (SDS) an RxD Pin 52 und TxD Pin 53 als SoftwareSerial objekt
SoftwareSerial Temperatursensor (15, 14); // Deklarierung des Temperatursensors (DHT22) an RxD Pin 15 und TxD Pin 14 als SoftwareSerial objekt

//////////////////////////////////////////////////
////////////////////Variablen/////////////////////
//////////////////////////////////////////////////

float SDS_PM25 = 0; // PM2.5 Wert des Feinstaubssensors
float SDS_PM10 = 0; // PM10 Wert des Feinstaubssensors
float DHT_Luftfeuchtigkeit = 0; // Luftfeuchtigkeit des DHT_Temperatursensors
float DHT_Temperatur = 0; // Temperatur des DHT_Temperatursensors
unsigned char Feinstaubsensor_10byte[10]; // Alle Daten des Feinstaubsensors

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
	for (int i = 0; i <= 7; i++)
	{							// Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(pfeilnu[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (int i = 0; i <= 7; i++)
	{					   // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(my[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (int i = 0; i <= 7; i++)
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
 */
void FeinstaubsensorMessung() // TODO BUGFIX: Liest daten viermal oder siebenmal aus und nicht einmal
{
	while (Feinstaubsensor.available())
	{
		if (Feinstaubsensor.read() == 0xAA)
		{
			Feinstaubsensor_10byte[0] = 0xAA;
			for (int i = 1; i < 10; i++)
			{
				Feinstaubsensor_10byte[i] = Feinstaubsensor.read();
			}
		}
	}
}

/**
 * @brief Sie misst die Temperatur und Luftfeuchtigkeit des DHT22 Sensors und speichert diese in globalen Variablen.
 * 
 */
bool doesFeinstaubsensorHaveParity() {
	unsigned char ParityBit = Feinstaubsensor_10byte[8];
	unsigned char CalculatedParityBit = Feinstaubsensor_10byte[2] + Feinstaubsensor_10byte[3] + Feinstaubsensor_10byte[4] + Feinstaubsensor_10byte[5] + Feinstaubsensor_10byte[6] + Feinstaubsensor_10byte[7];
	if (ParityBit == CalculatedParityBit)
	{
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
 */
bool doesTemperatursensorHaveParity() {
	unsigned char ParityBit = Feinstaubsensor_10byte[8];
	unsigned char CalculatedParityBit = Feinstaubsensor_10byte[2] + Feinstaubsensor_10byte[3] + Feinstaubsensor_10byte[4] + Feinstaubsensor_10byte[5] + Feinstaubsensor_10byte[6] + Feinstaubsensor_10byte[7];
	if (ParityBit == CalculatedParityBit)
	{
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
 */
void TemperatursensorMessung() {
	while (Temperatursensor.available())
	{
		Serial.println(Temperatursensor.read());
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

	// Terminal/Konsole
	Serial.begin(9600);

	// Weitere Inits
	lcd_init();
	sonderzeichen();

	// Sensoren
	Feinstaubsensor.begin(9600);
	Temperatursensor.begin(9600);

}

/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop()
{
	TemperatursensorMessung();

	/* FEINSTAUB GEHT!
	FeinstaubsensorMessung();
	int F = ((Feinstaubsensor_10byte[3] * 256) + Feinstaubsensor_10byte[2]) / 10;
	Serial.print("Real Value PM2.5: ");
	Serial.println(F);
	Serial.print("High Byte PM2.5: ");
	Serial.println(Feinstaubsensor_10byte[3]);
	Serial.print("Low Byte PM2.5: ");
	Serial.println(Feinstaubsensor_10byte[2]);
	int FF = ((Feinstaubsensor_10byte[5] * 256) + Feinstaubsensor_10byte[4]) / 10;
	Serial.print("Real Value PM10: ");
	Serial.println(FF);
	Serial.print("High Byte PM10: ");
	Serial.println(Feinstaubsensor_10byte[5]);
	Serial.print("Low Byte PM10: ");
	Serial.println(Feinstaubsensor_10byte[4]);
	Serial.println();
	Serial.print("Has Parity ? : ");
	if (doesFeinstaubsensorHaveParity()) { Serial.println("Yes"); } else { Serial.println("No"); }
	Serial.println("-------");
	delay(5000);
	*/
	/*
	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(Zeile_1);
	display_text("Feinstaub-$"); // "Feinstaub-"
	display_pos(Zeile_2);
	display_text("daten:$"); // "daten:"

	// PM2.5 sowie den Wert davon anzeigen
	display_pos(Zeile_3);
	display_text("PM2.5:$"); // "PM2.5:"
	display_pos(Zeile_4);
	display_text(String(SDS_PM25) + "$"); // Wert von PM2.5 ausgeben
	write_instr(0x0C); // Cursor aus

	delay(4000);

	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(Zeile_1);
	display_text("Feinstaub-$"); // "Feinstaub-"
	display_pos(Zeile_2);
	display_text("daten:$"); // "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(Zeile_3);
	display_text("PM10:$"); // "PM10:"
	display_pos(Zeile_4);
	display_text(String(SDS_PM10)+"$"); // Wert von PM10 ausgeben
	write_instr(0x0C); // Cursor aus

	delay(4000);

	write_instr(0x01); // Display löschen

	// "Temperatur" sowie den Wert anzeigen
	display_pos(Zeile_1);
	display_text("Temperatur:$"); // "Temperatur"
	display_pos(Zeile_2);
	display_text(String(DHT_Temperatur)+"$"); // Wert von DHT_Temperatur ausgeben

	// "Humidity" sowie den Wert anzeigen
	display_pos(Zeile_3);
	display_text("Humidity:$"); // "Humidity:" anzeigen
	display_pos(Zeile_4);
	display_text(String(DHT_Luftfeuchtigkeit)+"$"); // Wert von DHT_Luftfeuchtigkeit ausgeben
	write_instr(0x0C); // Cursor aus

	delay(4000);
	*/
}