#ifndef __BEEP_H
#define __BEEP_H     
#include "sys.h"
//硬件连接看原理图
#define BEEP PAout(3)  

void BEEP_Init(void);         
void Sound(u16 frq);
void Sound2(u16 time);
void play_music(void);
void play_successful(void);
void play_failed(void);

#endif
