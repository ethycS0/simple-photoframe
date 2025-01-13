#include"system.h"

#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/vector.h>

static volatile uint64_t ticks = 0;

void sys_tick_handler(void) {
        ticks++;
}

uint64_t system_get_ticks(void) {
        return ticks;
}

static void rcc_setup(void) {
        rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]); 
}

static void systick_setup(void) {
        systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
        systick_counter_enable();
        systick_interrupt_enable();
}

void system_setup(void) {
        systick_setup();
        rcc_setup();
}

void system_delay(uint64_t ms) {
        uint64_t END_TIME= system_get_ticks() + ms;
        while(system_get_ticks() > END_TIME) {
                // spin
        }
}
