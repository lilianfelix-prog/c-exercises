#include <stdio.h>
#include <stdint.h>
/*
 * Compile with : arm-none-eabi-gcc -S isr.c -o isr.s
 *
 */

#define INTERRUPT_S1  0x1
#define INTERRUPT_S2  0x2
#define STATUS_REG    0x600C0780
#define REG(x)        ((volatile uint32_t*) x)
#define T             1000000
#define TT            10000
/*
 * Using gcc ARM ISR syntax.
 * Switch register contexts and saves registers.
 * Multiple source interrupt handler
 */

void isr() __attribute__ ((interrupt ("IRQ")))
{
    uint8_t status =  *REG(STATUS_REG);
    uint32_t i;
    if(status & INTERRUPT_S1){
        i = T;
        while(i--){
            asm volatile("nop");
        }
    }
    if(status & INTERRUPT_S2){
        i = TT;
        while(i--){
            asm volatile("nop");
        }
    }
}

int main(void)
{
    isr();

    return 0;
}
