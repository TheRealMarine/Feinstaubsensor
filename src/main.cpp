//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

#include <display4_ard_logo.h>
#include <display.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <Arduino.h>

//////////////////////////////////////////////////
/////////////////////Defines//////////////////////
//////////////////////////////////////////////////

#define Zeile_1 0x00
#define Zeile_2 0x40
#define Zeile_3 0x0A
#define Zeile_4 0x4A

SoftwareSerial SDS_FeinstaubsensorPort = SoftwareSerial(PORTC5/*rxPin*/, PORTC6/*txPin*/); // Feinstaubsensor
DHT DHT_Temperatursensor (0, DHT22); // DHT_Temperatursensor

//////////////////////////////////////////////////
////////////////////Variables/////////////////////
//////////////////////////////////////////////////

float SDS_PM25 = 0; // PM2.5 Wert des Feinstaubssensors 
float SDS_PM10 = 0; // PM10 Wert des Feinstaubssensors 
float DHT_Luftfeuchtigkeit = 0; // DHT_Luftfeuchtigkeit des DHT_Temperatursensors
float DHT_Temperatur = 0; // DHT_Temperatur des DHT_Temperatursensors

//////////////////////////////////////////////////
/////////////////////Methoden/////////////////////
//////////////////////////////////////////////////

/* 
* Name: display_text 
* Erstellt von Mirko
* 
* Funktion: Diese Funktion dient der vereinfachten Ausgabe von Text.
* 
* Input:
*		String: Text welcher auf dem Display ausgegeben werden soll.
*				WICHTIG: Das Zeichen "$" endet die Ausgabe. Dieser muss mit in dem String stehen!
*
* Output: -
*/
void display_text(String text) {
	char character = text.charAt(0); // Ersten Buchstaben/Zeichen auswählen
	text.remove(0, 1); // Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
	while(character != '$') { // Nur solange der ausgewählte Buchstabe/Zeichen nicht das endungszeichen "$" ist, write_char ausführen
		write_char(character); // Ausgewählter Buchstabe/Zeichen auf display ausgeben
		character = text.charAt(0); // Ersten Buchstaben/Zeichen auswählen
		text.remove(0, 1); // Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
		Serial.print(character);
	}
}

/* 
* Name: sonderzeichen
* Erstellt von Patrick
* 
* Funktion: Programmiert Sonderzeichen in das CGRAM des Displays.
* 
* Input: -
*
* Output: -
*/
void sonderzeichen() {
	write_instr(0x40); // Die CGRAM auf Addresse 0 stellen
	for (int i = 0; i <= 7; i++) { // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(pfeilnu[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (int i = 0; i <= 7; i++) { // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(my[i]); // Ausgewählte Zeile an CGRAM senden
	}
}

/* 
* Name: FeinstaubsensorMessung 
* Erstellt von Patrick
* 
* Funktion: Es speichert anhand des deklarierten SoftwareSerial Ports die eingehenden Daten
* 			und verarbeitet diese zu den verschiedenen Feinstaubarten. Die Werte werden in
* 			folgenden Globale Variablen gespeichert:
*			SDS_PM25
*			SDS_PM10
* 
* Input: -
*
* Output: -
*/
void FeinstaubsensorMessung() {

}

/* 
* Name: display_text 
* Erstellt von Mirko
* 
* Funktion: Diese Funktion dient der Ausgabe von Text.
* 
* Input: -
*
* Output: -
*/
void DHT_TemperatursensorMessung() {
	DHT_Temperatur = DHT_Temperatursensor.readTemperature();
	DHT_Luftfeuchtigkeit = DHT_Temperatursensor.readHumidity();
	Serial.println(DHT_Temperatur);
	Serial.println(DHT_Luftfeuchtigkeit);
}

//////////////////////////////////////////////////
///////////////////////Main///////////////////////
//////////////////////////////////////////////////

/* Hier wird alles einmal ausgeführt vor dem Hauptprogramm. Demnach ist dies unsere Initialisierung. */
void setup() {
	lcd_init();
	sonderzeichen(); // vlt mit init umtauschen?

	// Port Ein-/Ausgänge
	DDRA = 0xFF; // Ausgang Display
	DDRB = 0xFF; // Ausgang Display
	DDRC = B01000000; // Eingang/Ausgang Feinstaubsensor NOCH ANPASSEN
	
	// Sensoren
	DHT_Temperatursensor.begin();

	// Terminal/Konsole
	Serial.begin(9600);

	delay(1000); // 1 Sekunde warten bevor das Hauptprogramm dauerhaft ausgeführt wird damit alles ordungsgemäß funktioniert
}
/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop() {
	FeinstaubsensorMessung();

	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(0x00);					
	display_text("Feinstaub-$");		// "Feinstaub-"
	display_pos(0x40);					
	display_text("daten:$"); 		// "daten:"

	// PM2.5 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("PM2.5:$"); // "PM2.5:"
	display_pos(0x4A);
	display_text(String(SDS_PM25)+"$"); // Wert von PM2.5 ausgeben

	delay(2000);

	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(0x00);					
	display_text("Feinstaub-$");		// "Feinstaub-"
	display_pos(0x40);					
	display_text("daten:$"); 		// "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("PM10:$"); // "PM10:"
	display_pos(0x4A);
	display_text(String(SDS_PM10)+"$"); // Wert von PM10 ausgeben

	delay(2000);
	DHT_TemperatursensorMessung();

	write_instr(0x01); // Display löschen

	// "DHT_Temperatur" anzeigen
	display_pos(0x00);					
	display_text("DHT_Temperatur$");		// "Feinstaub-"
	display_pos(0x40);					
	display_text(String(DHT_Temperatur)+"$"); 		// "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("Humidity:$"); // "PM10:"
	display_pos(0x4A);
	display_text(String(DHT_Luftfeuchtigkeit)+"$"); // Wert von PM10 ausgeben

	delay(2000);
}