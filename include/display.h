//Anzeigetexte
#define DDisplay_Text0		0
#define DDisplay_Text1		1
#define DDisplay_Text2		2
#define DDisplay_Text3		3

#define Z_oe           		0x94
#define Z_Pfeilnu      		0x00
#define Z_my           		0x01
//#define Z_hoch3             0x02

// Festlegung der Texte die auf dem Display angezeigt werden sollen
char const Display_Text0[] = {"Feinstaub-$"};
char const Display_Text1[] = {"daten:$"};
char const Display_Text2[] = {"g/m$"};
char const Display_Text3[] = {"PM2.5:$"};



// Zeiger
char const *ptr_hilfe; //Hilfszeiger
char const *ptr_text[] = {Display_Text0, Display_Text1, Display_Text2};

// Sonderzeichen
char const pfeilnu[] =  {0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00};       // Zeichenmuster für einen Pfeil nach unten
char const my[] =       {0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x08, 0x08};       // Zeichenmuster für mü
//char const hoch3[] =    {0x18, 0x04, 0x1C, 0x04, 0x18, 0x00, 0x00, 0x00};       // Zeichenmuster für ³