#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"

#define BEEP PEout(1)	// BEEP,·äÃùÆ÷½Ó¿Ú	

void beep_Init();
void beep_ring();
#endif