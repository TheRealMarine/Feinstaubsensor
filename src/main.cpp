//////////////////////////////////////////////////
/////////////////////Includes/////////////////////
//////////////////////////////////////////////////

#include <Arduino.h>
#include "display4_ard_logo.cpp"

//////////////////////////////////////////////////
/////////////////////Defines//////////////////////
//////////////////////////////////////////////////

// ALARM

//Anzeigetexte
#define T_H                 0
#define T_rtest             1
#define T_Start             2
#define T_BeiTon            3
#define T_Taste             4
#define T_nochmal           5
#define T_12345             6
#define T_Feinstaub         7
#define T_daten             8
#define T_g                 9
#define T_m                 10
#define T_hoch3             11
//#define                   12
#define Z_oe           		0x94
#define Z_Pfeilnu      		0x00
#define Z_my           		0x01

//Festlegung der Texte die auf dem Display angezeigt werden sollen
 char const Display_Text0[] = {"H."};
 char const Display_Text1[] = {"rtest."};
 char const Display_Text2[] = {"Start."};
 char const Display_Text3[] = {"Bei Ton."};
 char const Display_Text4[] = {"Taste>."};
 char const Display_Text5[] = {"<nochmal."};
 char const Display_Text6[] = {"12345 Hz."};
 char const Display_Text7[] = {"Feinstaub-."};
 char const Display_Text8[] = {"daten:."};
 char const Display_Text9[] = {"g."};
 char const Display_Text10[] = {"m."};
 char const Display_Text11[] = {"³."};
 char const Display_Text12[] = {""};


//Zeiger
char const *ptr_hilfe;                //Hilfszeiger
char const *ptr_text[] = {Display_Text0, Display_Text1, Display_Text2, Display_Text3,
								Display_Text4, Display_Text5, Display_Text6, Display_Text7, Display_Text8, Display_Text9,
								Display_Text10, Display_Text11, Display_Text12
							   };

// ALARM

//////////////////////////////////////////////////
////////////////////Variables/////////////////////
//////////////////////////////////////////////////


char const pfeilnu[] =  {0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00};       // Zeichenmuster für einen Pfeil nach unten
char const my[] =       {0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x08, 0x08};       // Zeichenmuster für mü

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

void linieAnzeigen(int displayPos, String text) {
	
}

//////////////////////////////////////////////////
///////////////////////Main///////////////////////
//////////////////////////////////////////////////

/* Hier wird alles einmal ausgeführt vor dem Hauptprogramm. Demnach ist dies unsere Initialisierung. */
void setup() {
	lcd_init();

	// Port Ein-/Ausgänge
	DDRA = 0xFF; // Ausgang Display
	DDRB = 0xFF; // Ausgang Display
	DDRC = B00000000; // Eingang/Ausgang Feinstaubsensor NOCH ANPASSEN
}
/* Unser Hauptprogramm. Hier wird alles wiederholt ausgeführt solange kein Interrupt dies unterbrechen sollte. */
void loop() {
	display_pos(0x0A);
	write_char(Z_my);
	display_text(T_g);
	write_char(0x2F);
	display_text(T_m);
	display_text(T_hoch3);
	display_pos(0x4B);
	write_char(Z_Pfeilnu);
	write_instr(0x0C);
}