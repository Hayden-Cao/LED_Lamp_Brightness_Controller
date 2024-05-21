#include <xc.h>
#include "pic18f4321-Config.h"
#include <stdbool.h>
#include <pic18f4321.h>
#include "Inits.h"

void UART_init()
{
    // 9600 Baud Rate
    // SPBRG value is from the PIC18F4321 Datasheet
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    BAUDCONbits.BRG16 = 0;
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 1;
    SPBRG = 51; 
}

void PWM_init()
{
    
    // Digital I/O
    ADCON1 = 0x0F;
    T2CON = 0x01;
    // 2 KHz PWM signal
    // PWM period = (PR2 + 1) * (Tosc) * TMR2 prescaler
    // Tosc = (1 / 8 Mhz), TMR2 Prescaler = 4
    PR2 = 249;
    CCPR1L = 0;
    // PWM Mode
    CCP1CON = 0x0C;
    TMR2 = 0;
    TRISCbits.TRISC2 = 0;
}


void Interrupt_init()
{
    // Interrupts enabled with priority
    // UART Receive is given low priority
    // Rotary Encoder is given high priority
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    IPR1bits.RCIP = 0;
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
}
