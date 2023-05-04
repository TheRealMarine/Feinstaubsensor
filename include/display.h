#include <Arduino.h>

#define Z_Pfeilnu      0x00
#define Z_my           0x01

char const pfeilnu[] =  {0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00};
char const my[] =       {0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x08, 0x08};
char const hoch3[] =    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Makros//
#define BEFEHL()(PORTC&=0xFE)               //PB0='0'->RS=0 ->Befehl
#define ZEICHEN()(PORTC|=0x01)              //PB0='1'->Zeichen
#define FREIGABE()(PORTC|=0x02)             //PB1=1->E=1->Datenbus freigeben
#define SPERREN()(PORTC&=0xFD)              //PB1=0->E=0->Datenbus sperren 


                        
//Methoden
        
void write_char(unsigned char data)
{            
    unsigned char hbyte,lbyte;
    
    hbyte=data;                             //hbyte: data direkt �bernehmen
    lbyte=data<<4;                          //lbyte: data 4 mal links schieben
    ZEICHEN();                              //Vorbereitung: Gleich wird ein ZEICHEN gesendet
    PORTA=hbyte;                            //hbyte senden
    FREIGABE();                             //Display freigeben
    SPERREN();                              //Display sperren
    PORTA=lbyte;                            //lbyte senden
    FREIGABE();                             //Display freigeben
    SPERREN();                              //Display sperren
    _delay_ms(1);                            //Warte 1ms
}     
    
    
void write_instr(unsigned char instr)
{   
    
    unsigned char hbyte,lbyte;      
             
    hbyte=instr;                            //hbyte: data direkt �bernehmen
    lbyte=instr<<4;                         //lbyte: data 4 mal links schieben
    BEFEHL();                               //Vorbereitung: Gleich wird ein BEFEHL gesendet
    PORTA=hbyte;                            //hbyte senden
    FREIGABE();                             //Display freigeben
    SPERREN();                              //Display sperren
    PORTA=lbyte;                            //lbyte senden
    FREIGABE();                             //Display freigeben
    SPERREN();                              //Display sperren
    _delay_ms(1);                            //Warte 1ms
}         

void display_pos (unsigned char pos)
{
     _delay_ms(1);
     pos+=0x80;                          //um 0x80 erh�hen (->Datenblatt)
     write_instr(pos);                   //Cursor auf pos setzen
     
}

void lcd_init (void)
{
    _delay_ms(15);                           //Warte 15ms
    write_instr(0x33);                      //Datenbusbreite 8 Bit
    _delay_ms(5);                            //Warte 5ms
    write_instr(0x33);                      //Datenbusbreite 8 Bit
    _delay_ms(1);                            //Warte 1ms
    write_instr(0x33);                      //Datenbusbreite 8 Bit
    _delay_ms(1);   
    write_instr(0x22);                      //4 Bits
    _delay_ms(1);
    write_instr(0x28);                      //4 Bit, 2 Zeilen, 5x8 dots   
    _delay_ms(1);
    write_instr(0x0F);                      //LCD ein, Cursor ein, Zeichen blinkt
    _delay_ms(1);
    write_instr(0x01);                      //DDRAM l�schen, Adresse 0x00
    _delay_ms(1);                                
    write_instr(0x06);                      //DDRAM-Adresse +1 nach Schreibvorgang, Display nicht schieben
    _delay_ms(1);                            //Warte 1ms
}