#include <xc.h>
#include "pic18f4321-Config.h"
#include <stdbool.h>
#include <pic18f4321.h>

#define _XTAL_FREQ 8000000
#define rot_enc_dt PORTBbits.RB0
#define rot_enc_clk PORTBbits.RB1


void UART_init();
uint8_t RX_Parse(uint8_t index);

void PWM_init(void);
void PWM_calc(uint8_t num_digits);

void __interrupt(low_priority) RX_ISR();
void __interrupt(high_priority) Rotary_Encoder_ISR();


// global variables for the ISRs to change
char data_in[4];
uint8_t index_count = 0;
uint8_t pwm_val = 0;
bool RX_got = false;


void main(void)
{
    // 8 MHz Clock Frequency
    OSCCON = 0x70;
    PWM_init();
    UART_init();
    
    
    // Rotary Encoder Pins = Input
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    
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
    
    uint8_t last_pwm_val = 0;
    uint8_t num_digits = 0;
    
    for(;;)
    {
        num_digits = 0;
        while(data_in[num_digits] != '\0')
            num_digits++;

        // RX_got flags when there is a UART transmission
        // PWM_calc will only get called when there is a UART transmission
        
        if(RX_got)
        {
            pwm_val = 0;
            PWM_calc(num_digits);
            CCPR1L = pwm_val;
            RX_got = false;
        } 
        else
        {
            CCPR1L = pwm_val;
        }
        
        
        // TMR2 controls the PWM signal
        PIR1bits.TMR2IF = 0;
        T2CONbits.TMR2ON = 1;
        while(!PIR1bits.TMR2IF);
        
    }
    
}


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

void PWM_calc(uint8_t num_digits)
{
    uint8_t temp = 0;
    uint8_t multiplier = 0;

    // Calculates what pwm_val based on the given digits
    // switch case will swap between the cases of 1, 2, and 3 digits inputted
    switch(num_digits)
    {
        case 1:
            pwm_val = RX_Parse(0);
            break;

        case 2:

            for (uint8_t i = 0; i < 2; i++)
            {
                temp = RX_Parse(i);

                if (i == 0)
                {
                    multiplier = 10;
                } 
                else
                {
                    multiplier = 1;
                }

                pwm_val += temp * multiplier;
            }
            break;

        case 3:

            for (uint8_t i = 0; i < 3; i++)
            {
                temp = RX_Parse(i);

                if (i == 0)
                {
                    multiplier = 100;
                } 
                else if (i == 1)
                {
                    multiplier = 10;
                } 
                else
                {
                    multiplier = 1;
                }

                pwm_val += temp * multiplier;
            }

                break;
        default: ;

    }
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

void __interrupt(low_priority) RX_ISR()
{
    RX_got = true;
    char error;
    PIR1bits.RCIF = 0;
    
    // FERR and OERR are dealt following the 
    // procedure from the datasheet
    
    if (RCSTAbits.FERR)
    {
        error = RCREG;
    }
    else if(RCSTAbits.OERR)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }  
    else
    {      
        // only store the data if the ASCII value of the data from the UART
        // if the data is '0'-'9' or '\0'
        if((RCREG >= 48 && RCREG <= 57) || RCREG == 0)
        {
            data_in[index_count] = RCREG;

            // 0 == null character '\0'
            // Index is reset when the null character is receieved
            if(data_in[index_count] == 0)
            {
                index_count = 0;
            } 
            else
            {
                index_count++;
            }
        }
    }
       
}

void __interrupt(high_priority) Rotart_Encoder_ISR()
{   
    INTCONbits.INT0F = 0;
    
    
    // clockwise rotation increases pwm_val
    // counterclockwise rotation decrease pwm_val
    // pwm_val capped at 0 <= pwm_val <= 255
    if(rot_enc_clk != rot_enc_dt)
    {
        if(pwm_val == 255)
        {
            pwm_val = 255;
        } else
        {
            pwm_val++;
        }
    } 
    else
    {
        if(pwm_val == 0)
        {
            pwm_val = 0;
        } 
        else
        {
            pwm_val--;
        }
    }
}


uint8_t RX_Parse(uint8_t index)
{
    uint8_t dec_val = 0;
    
    // parses a decimal value from its ASCII form
    
    switch(data_in[index])
    {
        case '0': dec_val = 0; break;
        case '1': dec_val = 1; break;
        case '2': dec_val = 2; break;
        case '3': dec_val = 3; break;
        case '4': dec_val = 4; break;
        case '5': dec_val = 5; break;
        case '6': dec_val = 6; break;
        case '7': dec_val = 7; break;
        case '8': dec_val = 8; break;
        case '9': dec_val = 9; break;
        default: break;
    }
    
    return dec_val;
}
