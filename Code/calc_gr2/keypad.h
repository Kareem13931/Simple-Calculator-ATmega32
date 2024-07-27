/*****************************************************************************************/
/***************************** Name    : Kareem Adel Othman ******************************/
/***************************** Date    : 7/11/2024          ******************************/
/***************************** SWC     : KPD                ******************************/
/***************************** Version : 1.0                ******************************/
/*****************************************************************************************/



#ifndef KEYPAD_H_
#define KEYPAD_H_


#define NOTPRESSED 0xff
#include "DIO.h"
#include "ports.h"
void keypad_vInit();
char keypad_u8check_press();


#endif /* KEYPAD_H_ */