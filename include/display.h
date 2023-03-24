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

#define Z_oe           		0x94
#define Z_Pfeilnu      		0x00
#define Z_my           		0x01
//#define Z_hoch3             0x02

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


//Zeiger
char const *ptr_hilfe;                //Hilfszeiger
char const *ptr_text[] = {Display_Text0, Display_Text1, Display_Text2, Display_Text3, Display_Text4,
						  Display_Text5, Display_Text6, Display_Text7, Display_Text8, Display_Text9,
						  Display_Text10};

// ALARM




char const pfeilnu[] =  {0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00};       // Zeichenmuster für einen Pfeil nach unten
char const my[] =       {0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x08, 0x08};       // Zeichenmuster für mü
//char const hoch3[] =    {0x18, 0x04, 0x1C, 0x04, 0x18, 0x00, 0x00, 0x00};       // Zeichenmuster für ³