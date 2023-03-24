#ifndef _DISPLAY_INCLUDED_
#define _DISPLAY_INCLUDED_


#pragma used+
void write_char(unsigned char);            
void write_instr(unsigned char);
void lcd_init(void);
void display_pos(unsigned char pos);
#pragma used-
#pragma library display4_ard_logo.lib

#endif
