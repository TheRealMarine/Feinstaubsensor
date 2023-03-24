//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

#include <Arduino.h>
#include <display4_ard_logo.h>
#include <display.h>

//////////////////////////////////////////////////
/////////////////////Defines//////////////////////
//////////////////////////////////////////////////



//////////////////////////////////////////////////
////////////////////Variables/////////////////////
//////////////////////////////////////////////////



//////////////////////////////////////////////////
/////////////////////Methoden/////////////////////
//////////////////////////////////////////////////

void display_text(int text_nr)
{
	unsigned char tab;                      	//Lokale Zï¿œhlvariable tab deklarieren/

	ptr_hilfe = ptr_text[text_nr];          	//Text-Zeiger auf Anfangsadresse von Display_Texttext_nr
												//setzen und Adresse an Hilfe-Zeiger ï¿œbergeben
	tab = 0;
	while ( *(ptr_hilfe + tab) != '.' )     	//Wiederhole solange der Hilfe-Zeiger nicht auf dem
		{
												//Zeichen "." steht

		write_char(*(ptr_hilfe + tab));     	//Zeichen an der Position des Hilfe-Zeigers an die Anzeige
		tab++;                              	//senden Adresse des Hilfezeigers umn 1 erhï¿œhen
		}
}

void sonderzeichen()
{
	write_instr(0x40);                      	//CGRAM auf 0 stellen
	for (int i = 0; i <= 7; i++) {                	//Zeilen 0...7
		//write_char(my[i]);                    //Zeilen 0...7 des Sonderzeichen an das CGRAM senden
		write_char(pfeilnu[i]);
	}

	for (int k = 0; k <= 7; k++) {               	//Zeilen 0...7
		//write_char(pfeilnu[k]);             	//Zeilen 0...7 des Sonderzeichen an das CGRAM senden
		write_char(my[k]);             			//Zeilen 0...7 des Sonderzeichen an das CGRAM senden
	}

//	for (int i = 0; i <= 7; i++) {               	//Zeilen 0...7
//		//write_char(pfeilnu[k]);             	//Zeilen 0...7 des Sonderzeichen an das CGRAM senden
//		write_char(hoch3[i]);             			//Zeilen 0...7 des Sonderzeichen an das CGRAM senden
//	}
}

void linieAnzeigen(int displayPos, String text) {
	
}

//////////////////////////////////////////////////
///////////////////////Main///////////////////////
//////////////////////////////////////////////////

/* Hier wird alles einmal ausgeführt vor dem Hauptprogramm. Demnach ist dies unsere Initialisierung. */
void setup() {
	lcd_init();
	sonderzeichen();

	// Port Ein-/Ausgänge
	DDRA = 0xFF; // Ausgang Display
	DDRB = 0xFF; // Ausgang Display
	DDRC = B00000000; // Eingang/Ausgang Feinstaubsensor NOCH ANPASSEN
}
/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop() {

	write_instr(0x01);
	display_pos(0x00);
	display_text(T_Feinstaub);
	display_pos(0x40);
	display_text(T_daten);
	write_instr(0x0C);

	display_pos(0x0A);
	write_char(Z_my);
	display_text(T_g);
	write_char(0x2F);
	display_text(T_m);
	display_text(T_hoch3);
	display_pos(0x4B);
	write_char(Z_Pfeilnu);
	write_instr(0x0C);

	delay(5000);
}