#include <Arduino.h>

//Makros//
#define BEFEHL()(PORTB&=0xFE)               //PB0='0'->RS=0 ->Befehl
#define ZEICHEN()(PORTB|=0x01)              //PB0='1'->Zeichen
#define FREIGABE()(PORTB|=0x02)             //PB1=1->E=1->Datenbus freigeben
#define SPERREN()(PORTB&=0xFD)              //PB1=0->E=0->Datenbus sperren 


                        
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
    delay(1);                               //Warte 1ms
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
    delay(1);                               //Warte 1ms
}          

void display_pos (unsigned char pos)
{
     delay(1);
     pos+=0x80;                          //um 0x80 erh�hen (->Datenblatt)
     write_instr(pos);                   //Cursor auf pos setzen
     
}

void lcd_init ()
{
    delay(15);                              //Warte 15ms
    write_instr(0x33);                      //Datenbusbreite 8 Bit
    delay(5);                               //Warte 5ms
    write_instr(0x33);                      //Datenbusbreite 8 Bit
    delay(1);                               //Warte 1ms
    write_instr(0x33);                      //Datenbusbreite 8 Bit
    delay(1);   
    write_instr(0x22);                      //4 Bits
    delay(1);
    write_instr(0x28);                      //4 Bit, 2 Zeilen, 5x8 dots   
    delay(1);
    write_instr(0x0F);                      //LCD ein, Cursor ein, Zeichen blinkt
    delay(1);
    write_instr(0x01);                      //DDRAM l�schen, Adresse 0x00
    delay(1);                                
    write_instr(0x06);                      //DDRAM-Adresse +1 nach Schreibvorgang, Display nicht schieben
    delay(1);                               //Warte 1ms
}