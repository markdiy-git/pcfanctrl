/*
 * pcfanctrl.h
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

#define  FAN_ON                    HIGH
#define  FAN_OFF                   LOW

#define  FAN1_POWER                12   // pin D12, Power Control
#define  FAN2_POWER                8    // pin D8, Power Control
#define  FAN3_POWER                4    // pin D4, Power Control

#define  FAN1_PWM                  11   // pin D11, PWM Control
#define  FAN2_PWM                  10   // pin D10, PWM Control
#define  FAN3_PWM                  6    // pin D6, PWM Control

#define  FAN1_SENSE                7    // pin D7
#define  FAN2_SENSE                0    // pin D0
#define  FAN3_SENSE                1    // pin D1

#define  SYS_MODE_BUTTON           9    // pin D9

#define  FAN_VOLTAGE               A0   // pin A0

#define  FAN_RPM_MEASURE_DURATION  10000L

#define  SYS_NORMAL                0
#define  SYS_FAN_TEST              1

#define  EEPROM_INIT_VALUE         0x12
#define  EEPROM_BASE_ADDRESS       0
#define  EEPROM_DATA_BASE_ADDRESS  1

// Define the protocol commands that can be sent and received
#define   GET_PROTOCOL_VERSION     0
#define   SET_ZONE1_HIGHC          1
#define   GET_ZONE1_HIGHC          2
#define   SET_ZONE1_MIDDLEC        3
#define   GET_ZONE1_MIDDLEC        4
#define   SET_ZONE2_HIGHC          5
#define   GET_ZONE2_HIGHC          6
#define   SET_ZONE2_MIDDLEC        7
#define   GET_ZONE2_MIDDLEC        8
#define   SET_FAN1_PWM_DEFAULT     9
#define   GET_FAN1_PWM_DEFAULT     10
#define   SET_FAN2_PWM_DEFAULT     11
#define   GET_FAN2_PWM_DEFAULT     12
#define   SET_FAN3_PWM_DEFAULT     13
#define   GET_FAN3_PWM_DEFAULT     14
#define   SET_FAN1_PWM_LOW         15
#define   GET_FAN1_PWM_LOW         16
#define   SET_FAN2_PWM_LOW         17
#define   GET_FAN2_PWM_LOW         18
#define   SET_FAN3_PWM_LOW         19
#define   GET_FAN3_PWM_LOW         20
#define   SET_FAN1_PWM_MIDDLE      21
#define   GET_FAN1_PWM_MIDDLE      22
#define   SET_FAN2_PWM_MIDDLE      23
#define   GET_FAN2_PWM_MIDDLE      24
#define   SET_FAN3_PWM_MIDDLE      25
#define   GET_FAN3_PWM_MIDDLE      26
#define   SET_FAN1_PWM_HIGH        27
#define   GET_FAN1_PWM_HIGH        28
#define   SET_FAN2_PWM_HIGH        29
#define   GET_FAN2_PWM_HIGH        30
#define   SET_FAN3_PWM_HIGH        31
#define   GET_FAN3_PWM_HIGH        32
#define   UPDATE_EEPROM            33
#define   GET_ZONE1_TEMPC          34
#define   GET_ZONE2_TEMPC          35
#define   GET_FAN_VOLTAGE          36
#define   GET_FAN1_RPM             37
#define   GET_FAN2_RPM             38
#define   GET_FAN3_RPM             39
#define   GET_FAN1_MAX_RPM         40
#define   GET_FAN2_MAX_RPM         41
#define   GET_FAN3_MAX_RPM         42
#define   TEST_FAN_MAX_RPM         43
#define   SET_VOLTAGE_COEFFICIENT  44

#define   MAX_DATA_BYTES           16
#define   PROTOCOL_MAJOR_VERSION   1
#define   PROTOCOL_MINOR_VERSION   0

#define   ZONE1_LM75_ADDRESS       0x48
#define   ZONE2_LM75_ADDRESS       0x49

struct MyConfig {
  float temp1HighC;
  float temp1MiddleC;
  float temp2HighC;
  float temp2MiddleC;
  float FansVoltageCoefficient;
  uint8_t Fan1PWMLow;
  uint8_t Fan1PWMMiddle;
  uint8_t Fan1PWMHigh;
  uint8_t Fan1PWMDefault;
  uint8_t Fan2PWMLow;
  uint8_t Fan2PWMMiddle;
  uint8_t Fan2PWMHigh;
  uint8_t Fan2PWMDefault;
  uint8_t Fan3PWMLow;
  uint8_t Fan3PWMMiddle;
  uint8_t Fan3PWMHigh;
  uint8_t Fan3PWMDefault;
  uint16_t Fan1_Max_RPM;
  uint16_t Fan2_Max_RPM;
  uint16_t Fan3_Max_RPM;
};
