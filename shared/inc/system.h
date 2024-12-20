#ifndef SHARED_INC_SYSTEM
#define SHARED_INC_SYSTEM

#include"common-defines.h"

#define CPU_FREQ        (84000000)
#define SYSTICK_FREQ    (1000)

void system_setup(void);
uint64_t system_get_ticks(void);
void system_delay(uint64_t ms); 


#endif
