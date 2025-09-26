/*
 * Simulate a priority queue with a bit flags register
 * Interrupts (highest priority from left to right):
 * Bits: 7   6    5   4   3     2   1   0
 * IRQ:  ADC UART SPI EXT TIMER I2C PWM GPIO
 * Bit = 1 -> interrupt pending
 * Bit = 0 -> no interrupt
 */
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

typedef uint8_t t_flags;

// fixed priority 
#define F_GPIO  (1 << 0)
#define F_PWM   (1 << 1)
#define F_I2C   (1 << 2)
#define F_TIMER (1 << 3)
#define F_EXT   (1 << 4)
#define F_SPI   (1 << 5)
#define F_UART  (1 << 6)
#define F_ADC   (1 << 7)

typedef void (*handler_ptr)();

void sleep(){
    for(volatile int i = 0; i<100000000; i++); 
}
void ADC_handler()   { printf("[ADC] Conversion complete → reading value from ADC buffer\n"); sleep(); }
void UART_handler()  { printf("[UART] Data received → reading from RX FIFO\n"); sleep();}
void SPI_handler()   { printf("[SPI] Transfer done → fetching received byte\n"); sleep(); }
void EXT_handler()   { printf("[EXT] External interrupt triggered → checking pin state\n"); sleep(); }
void TIMER_handler() { printf("[TIMER] Timer overflow → updating system tick\n"); sleep(); }
void I2C_handler()   { printf("[I2C] Transfer complete → releasing I2C bus\n"); sleep(); }
void PWM_handler()   { printf("[PWM] Cycle complete → updating duty cycle register\n"); sleep(); }
void GPIO_handler()  { printf("[GPIO] Pin change detected → updating input state\n"); sleep(); }

// mapping the handlers in a funcptr lookup table
handler_ptr handler_table[] = {
    GPIO_handler,
    PWM_handler,
    I2C_handler,
    TIMER_handler,
    EXT_handler,
    SPI_handler,
    UART_handler,
    ADC_handler
};

// most significant bit 
uint8_t MSG(t_flags* flags)
{
    uint8_t msg_flag = *flags;
    // setting all bits after msg to 1
    // since its a 8 bits int 7 shift right covers all bits
    msg_flag |= (msg_flag >> 1);
    msg_flag |= (msg_flag >> 2);
    msg_flag |= (msg_flag >> 4);

    // prevent overflow with mask 
    msg_flag = ((msg_flag + 1) >> 1) | (msg_flag & (1 << ((sizeof(msg_flag) * CHAR_BIT)-1)));
    return msg_flag;
}

int priority_encoder(t_flags* flags)
{
   // find priority interupt with the MSB 
    uint8_t interupt = MSG(flags);
   
    if (interupt){
        *flags &= ~interupt;
        int idx = __builtin_ctz(interupt);  // convert mask -> index, counts the trailing 0s 
        handler_table[idx]();

    }

    return 0;
    
}

void add_interupt(uint8_t flag, t_flags* flags){
    // add an interupt to flag register
    if(*flags & flag){
        printf("io task already suspended");
    }else{
        *flags &= ~flag;
    }
}

int main(void) {

    // simulate random task order
    int max = UINT8_MAX;
    
    srand(time(NULL));

    volatile uint8_t val = (uint8_t) rand() % (max + 1);
    
    printf("rand: %d \n", val);
    
    t_flags flag_register = val;

    while(flag_register != 0){
        priority_encoder(&flag_register);
    }

    return 0;
}
