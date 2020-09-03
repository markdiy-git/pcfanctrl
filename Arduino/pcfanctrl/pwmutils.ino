/*
 * pwmutils.ino
 * 
 * Copyright 2020 markdiy <https://markdiy.blogspot.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
/**********************************************************
 * Fast PWM on pins 10,11 (TIMER 1)
 * TOP Value in OCR1A
 * Use 25kHz PWM frequencies
 * 
 **********************************************************/
#define  T1PWMTOP 639 //25KHz PWM, 16MHz /1 /(639+1) = 25KHz

/*
 * Configure timer1 PWM channels,clock,mode,TOP
 */
void Setup_Timer1_PWM()
{
  // TCCR1A configuration
  // Channel A: 00, disabled D9
  // Channel B: 10, COM1B1, enabled D10, Clear on Compare Match OCR1B, Set when TCNT1 = 0x0000
  // Channel C: 10, COM1C1, enabled D11, Clear on Compare Match OCR1C, Set when TCNT1 = 0x0000
  // PWM  mode: 11, WGM11 WGM10
  TCCR1A = _BV(COM1B1) | _BV(COM1C1) | _BV(WGM11) | _BV(WGM10);
  
  // TCCR1B configuration
  // PWM   mode: 1111, WGM13 WGM12 WGM11 WGM10, Fast PWM and TOP Value in OCR1A
  // Clock mode:  001, CS10, clk/1
  TCCR1B=_BV(WGM13) | _BV(WGM12) | _BV(CS10);  
  
  OCR1A = T1PWMTOP;
  // TCCR1C configuration
  TCCR1C=0;
}

/*
 * Set PWM to D10
 */
void Set_FAN2_PWM(const int value)
{
  uint16_t pwmval = map(value, 0, 100, 0, T1PWMTOP);
  pwmval = constrain(pwmval,0,T1PWMTOP);
  OCR1B=pwmval;   // Set PWM value
}

/*
 * Set PWM to D11
 */
void Set_FAN1_PWM(const int value)
{
  uint16_t pwmval = map(value, 0, 100, 0, T1PWMTOP);
  pwmval = constrain(pwmval,0,T1PWMTOP);
  OCR1C=pwmval;   // Set PWM value
}

/**********************************************************
 * Fast PWM on pins 6, 13 (High Speed TIMER 4)
 * TOP Value in OCR4C
 * Use 25kHz PWM frequencies
/**********************************************************/
#define  T4PWMTOP 239 //25KHz PWM, 48MHz /8 /(239+1) = 25KHz

/*
 * Configure the PWM clock and PWM4A,PWM4D
 */
void Setup_Timer4_PWM()
{
    // PLL Configuration
    // PLL Postcaler Factor for High-Speed Timer: PLLTM1,PLLTM0 = 11 , PLL Output frequency /division factor 2
    PLLFRQ=(PLLFRQ&0xCF)|_BV(PLLTM1)|_BV(PLLTM0);

    // TCCR4B configuration
    // Clock Select: CS43,CS42,CS41,CS40 = 0100, 48MHz /8
    TCCR4B= _BV(CS42);

    // TCCR4D configuration
    // Waveform Generation Mode: WGM41,WGM40 = 00 , Fast PWM,TOP at OCR4C
    TCCR4D=0;

    // TCCR4C configuration
    // Compare Output Mode: COM4D1,COM4D0 = 10 , Clear on Compare Match,Set when TCNT4 = 0x000,OC4D Pin Connected
    // Pulse Width Modulator D Enable: PWM4D = 1
    TCCR4C= _BV(COM4D1) | _BV(PWM4D);

#ifdef USEPWM13
    // TCCR4A configuration
    // Compare Output Mode: COM4A1,COM4A0 = 10 , Clear on Compare Match,Set when TCNT4 = 0x000,OC4A Pin Connected
    // Pulse Width Modulator A Enable: PWM4A = 1 
    TCCR4A = _BV(COM4A1) | _BV(PWM4A);
#else
    TCCR4A = 0;
#endif

    // TOP count for Timer 4 FAST PWM
    OCR4C=T4PWMTOP;

}

/*
 * Set PWM to D6 (Timer4 D)
 */
void Set_FAN3_PWM(const int value)
{
  uint16_t pwmval = map(value, 0, 100, 0, T4PWMTOP);
  pwmval = constrain(pwmval,0,T4PWMTOP);
  uint8_t highByte = (pwmval >> 8) & 0x3;
  uint8_t lowByte = pwmval & 0xff;
  TC4H = highByte;
  OCR4D=lowByte;   // Set PWM value
}

#ifdef USEPWM13
/*
 * Set PWM to D13 (Timer4 A)
 */
void Set_PWM_13(const int value)
{
  uint16_t pwmval = map(value, 0, 100, 0, T4PWMTOP);
  pwmval = constrain(pwmval,0,T4PWMTOP);
  uint8_t highByte = (pwmval >> 8) & 0x3;
  uint8_t lowByte = pwmval & 0xff;
  TC4H = highByte;
  OCR4A=lowByte;   // Set PWM value
}
#endif
