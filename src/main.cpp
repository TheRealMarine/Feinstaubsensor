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

SoftwareSerial FeinstaubsensorPort = SoftwareSerial(PORTC5/*rxPin*/, PORTC6/*txPin*/); // Feinstaubsensor
DHT Temperatursensor (0, DHT22);

//////////////////////////////////////////////////
////////////////////Variables/////////////////////
//////////////////////////////////////////////////

int value_PM25 = 0; // PM2.5 Wert des Feinstaubssensors 
int value_PM10 = 0; // PM10 Wert des Feinstaubssensors 
float luftfeuchtigkeit = 0; // Luftfeuchtigkeit des Temperatursensors
float temperatur = 0; // Temperatur des Temperatursensors

//////////////////////////////////////////////////
/////////////////////Methoden/////////////////////
//////////////////////////////////////////////////

/*
void display_text(int text_nr) {
	unsigned char tab;                      	//Lokale Zï¿œhlvariable tab deklarieren/

	ptr_hilfe = ptr_text[text_nr];          	//Text-Zeiger auf Anfangsadresse von Display_Texttext_nr
												//setzen und Adresse an Hilfe-Zeiger ï¿œbergeben
	tab = 0;
	while ( *(ptr_hilfe + tab) != '$' )     	//Wiederhole solange der Hilfe-Zeiger nicht auf dem
		{
												//Zeichen "$" steht

		write_char(*(ptr_hilfe + tab));     	//Zeichen an der Position des Hilfe-Zeigers an die Anzeige
		tab++;                              	//senden Adresse des Hilfezeigers umn 1 erhï¿œhen
		}
}
*/

/* 
* Name: display_text 
* 
* Funktion: Diese Funktion dient der Ausgabe von Text.
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
	}
}

void sonderzeichen() // Trägt in das CGRAM ein
{
	write_instr(0x40); //CGRAM auf 0 stellen
	for (int i = 0; i <= 7; i++) { //Zeilen 0...7
		write_char(pfeilnu[i]); //Zeilen 0...7 des Sonderzeichen an das CGRAM senden
	}

	for (int i = 0; i <= 7; i++) { //Zeilen 0...7
		write_char(my[i]); //Zeilen 0...7 des Sonderzeichen an das CGRAM senden
	}
}

void FeinstaubsensorMessung() {

}

void TemperatursensorMessung() {
	temperatur = Temperatursensor.readTemperature();
	luftfeuchtigkeit = Temperatursensor.readHumidity();
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
	Temperatursensor.begin();
}
/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop() {

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
	display_text(String(value_PM25)+"$"); // Wert von PM2.5 ausgeben

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
	display_text(String(value_PM10)+"$"); // Wert von PM10 ausgeben

	delay(2000);

	write_instr(0x01); // Display löschen

	// "Temperatur" anzeigen
	display_pos(0x00);					
	display_text("Temperatur$");		// "Feinstaub-"
	display_pos(0x40);					
	display_text(String(temperatur)+"$"); 		// "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("Humidity:$"); // "PM10:"
	display_pos(0x4A);
	display_text(String(luftfeuchtigkeit)+"$"); // Wert von PM10 ausgeben

	delay(2000);
}