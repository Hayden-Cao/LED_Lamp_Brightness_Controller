#include <xc.h>
#include "pic18f4321-Config.h"
#include <stdbool.h>
#include <pic18f4321.h>
#include "Calcs.h"

uint8_t RX_Parse(uint8_t index, char* data_in)
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

uint8_t PWM_calc(uint8_t num_digits, char* data_in)
{
    uint8_t temp = 0;
    uint8_t multiplier = 0;
    uint8_t pwm_val = 0;

    // Calculates what pwm_val based on the given digits
    // switch case will swap between the cases of 1, 2, and 3 digits inputted
    switch(num_digits)
    {
        case 1:
            pwm_val = RX_Parse(0, data_in);
            break;

        case 2:

            for (uint8_t i = 0; i < 2; i++)
            {
                temp = RX_Parse(i, data_in);

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
                temp = RX_Parse(i, data_in);

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
    
    return pwm_val;
}
