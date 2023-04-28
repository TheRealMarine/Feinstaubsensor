//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

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

//DHT DHT_Temperatursensor (0, DHT22); // DHT_Temperatursensor

//////////////////////////////////////////////////
////////////////////Variables/////////////////////
//////////////////////////////////////////////////

float SDS_PM25 = 0; // PM2.5 Wert des Feinstaubssensors 
float SDS_PM10 = 0; // PM10 Wert des Feinstaubssensors 
float DHT_Luftfeuchtigkeit = 0; // DHT_Luftfeuchtigkeit des DHT_Temperatursensors
float DHT_Temperatur = 0; // DHT_Temperatur des DHT_Temperatursensors
unsigned char zahl;
unsigned char Feinstaubsensor_10byte[10];

//////////////////////////////////////////////////
/////////////////////Methoden/////////////////////
//////////////////////////////////////////////////

/**
 * @brief Diese Funktion dient der vereinfachten Ausgabe von Text.
 * 
 * @param text 
 * 		  Text welcher auf dem Display ausgegeben werden soll.
 */
void display_text(String text) {
	//Serial.print(text);
	char character = text.charAt(0); // Ersten Buchstaben/Zeichen auswählen
	//Serial.print(character);
	text.remove(0, 1); // Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
	while(character != '$') { // Nur solange der ausgewählte Buchstabe/Zeichen nicht das endungszeichen "$" ist, write_char ausführen
		//Serial.print(character);
		write_char(character); // Ausgewählter Buchstabe/Zeichen auf display ausgeben
		character = text.charAt(0); // Ersten Buchstaben/Zeichen auswählen
		text.remove(0, 1); // Entferne ersten Buchstaben/Zeichen welcher ausgewählt wurde
	}
}

/**
 * @brief Programmiert Sonderzeichen in das CGRAM des Displays.
 */
void sonderzeichen() {
	write_instr(0x40); // Die CGRAM auf Addresse 0 stellen
	for (int i = 0; i <= 7; i++) { // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(pfeilnu[i]); // Ausgewählte Zeile an CGRAM senden
	}

	for (int i = 0; i <= 7; i++) { // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(my[i]); // Ausgewählte Zeile an CGRAM senden
	}

	/*for (int i = 0; i <= 7; i++) { // Zeile auswählen und um eine weitere Zeile erhöhen
		write_char(hoch3[i]); // Ausgewählte Zeile an CGRAM senden
	}*/
}

/**
 * @brief Es speichert anhand des deklarierten SoftwareSerial Ports die eingehenden Daten
 * 		  und verarbeitet diese zu den verschiedenen Feinstaubarten. Die Werte werden in
 * 	   	  folgenden Globale Variablen gespeichert:
 *		  SDS_PM25
 *		  SDS_PM10
 * 
 */


/**
 * @brief Sie misst die Temperatur und Luftfeuchtigkeit des DHT22 Sensors und speichert diese in globalen Variablen.
 * 
 */
void DHT_TemperatursensorMessung() {
	//DHT_Temperatur = DHT_Temperatursensor.readTemperature();
	//DHT_Luftfeuchtigkeit = DHT_Temperatursensor.readHumidity();
	//Serial.println(DHT_Temperatur);
	//Serial.println(DHT_Luftfeuchtigkeit);
}

void rs232_rec(void) {
    int k;
  	int i;
  
  	for(i = 0; i <= 9; i++) {
      	while (PINC1==1) {}                                  //Auf Startbit warten  
    
        delayMicroseconds(110);                                      //Auf die "Mitte" von Bit0 warten (104+104/2)
          zahl = 0;
          for(k=0;k <= 7;k++) {                                //Wiederhole 8x    
            zahl = zahl >> 1;                            //Inhalt von zahl rechtsschieben ("alte" Bits sichern). An der Stelle zahl.7 wird eine 0 nachgeschoben
            if (PINC1==1) {                                 //Falls RxD=1, zahl.7 setzen                 
                zahl = zahl | 0b10000000;
            }
            delayMicroseconds(104);                                //Warte bis zur "Mitte" des n?chsten Bits
        }
        Feinstaubsensor_10byte[i] = zahl;
		//Serial.print(zahl);
    } 
	Serial.print(Feinstaubsensor_10byte[0]);
	Serial.println(Feinstaubsensor_10byte[1]);
	Serial.print(Feinstaubsensor_10byte[3]);
	Serial.println(Feinstaubsensor_10byte[2]);
	Serial.println();
 
}

//////////////////////////////////////////////////
///////////////////////Main///////////////////////
//////////////////////////////////////////////////

/* Hier wird alles einmal ausgeführt vor dem Hauptprogramm. Demnach ist dies unsere Initialisierung. */
void setup() {
	// Port Ein-/Ausgänge
	DDRA = 0xFF; // Ausgang Display
	DDRC = 0xFF; // Ausgang Display
	DDRB = 0xFD; // Eingang/Ausgang Feinstaubsensor

	
	// Sensoren
	//DHT_Temperatursensor.begin();

	// Terminal/Konsole
	Serial.begin(9600);

	// Weitere Inits
	lcd_init();
	sonderzeichen();

	// Softwareserials
	// Temperatur/Humidity sensor

	delay(1000); // 1 Sekunde warten bevor das Hauptprogramm dauerhaft ausgeführt wird damit alles ordungsgemäß funktioniert
}

/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop() {
	
	//rs232_rec();
	//SDS_PM25 = (Feinstaubsensor_10byte[3]*256)+(Feinstaubsensor_10byte[2]/10);

	write_instr(0x01); // Display löschen


	//display_pos(Zeile_1);
	//display_text("Startbit$");
	//while (PINC != 0) {  };
	//write_instr(0x01);


	// "Feinstaubdaten" anzeigen
	display_pos(0x00);					
	display_text("Feinstaub-$"); // "Feinstaub-"
	display_pos(0x40);					
	display_text("daten:$"); // "daten:"

	// PM2.5 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("PM2.5:$"); // "PM2.5:"
	display_pos(0x4A);
	display_text(String(SDS_PM25)+"$"); // Wert von PM2.5 ausgeben

	delay(4000);

	write_instr(0x01); // Display löschen

	// "Feinstaubdaten" anzeigen
	display_pos(0x00);					
	display_text("Feinstaub-$"); // "Feinstaub-"
	display_pos(0x40);					
	display_text("daten:$"); // "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("PM10:$"); // "PM10:"
	display_pos(0x4A);
	display_text(String(SDS_PM10)+"$"); // Wert von PM10 ausgeben

	delay(4000);
	DHT_TemperatursensorMessung();

	write_instr(0x01); // Display löschen

	// "DHT_Temperatur" anzeigen
	display_pos(0x00);					
	display_text("DHT_Temperatur$"); // "Feinstaub-"
	display_pos(0x40);					
	display_text(String(DHT_Temperatur)+"$"); // "daten:"

	// PM10 sowie den Wert davon anzeigen
	display_pos(0x0A);
	display_text("Humidity:$"); // "PM10:"
	display_pos(0x4A);
	display_text(String(DHT_Luftfeuchtigkeit)+"$"); // Wert von PM10 ausgeben

	delay(4000);
	
}