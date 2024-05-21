/* 
 * File:   Calcs.h
 * Author: caoha
 *
 * Created on May 20, 2024, 11:03 AM
 */

#ifndef CALCS_H
#define	CALCS_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t RX_Parse(uint8_t index, char* data_in);
uint8_t PWM_calc(uint8_t num_digits, char* data_in);


#ifdef	__cplusplus
}
#endif

#endif	/* CALCS_H */

