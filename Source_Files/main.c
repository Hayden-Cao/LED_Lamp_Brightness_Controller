#include <xc.h>
#include "pic18f4321-Config.h"
#include <stdbool.h>
#include <pic18f4321.h>
#include "Inits.h"
#include "Calcs.h"

#define _XTAL_FREQ 8000000
#define rot_enc_dt PORTBbits.RB0
#define rot_enc_clk PORTBbits.RB1
#define rot_enc_sw PORTBbits.RB2

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
    Interrupt_init();
    
    // Rotary Encoder Pins = Input
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    
    
    
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
            
            pwm_val = PWM_calc(num_digits, data_in);
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


