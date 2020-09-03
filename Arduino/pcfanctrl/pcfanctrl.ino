/*
 * pcfanctrl.ino
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

#include <Temperature_LM75_Derived.h>
#include <EEPROM.h>
#include "pcfanctrl.h"


#define  FAN1_USE_PWM_CTRL
#define  FAN2_USE_PWM_CTRL
//#define  USE_FAN3
//#define  FAN3_USE_PWM_CTRL
//#define  USEPWM13



float zone1 = 0;
float zone2 = 0;

unsigned long lastTime = 0;

unsigned long lastRPMTime = 0;
volatile uint16_t Fan1_Sense_Counter = 0;
volatile uint16_t Fan2_Sense_Counter = 0;
volatile uint16_t Fan3_Sense_Counter = 0;
uint16_t Fan1_RPM = 0;
uint16_t Fan2_RPM = 0;
uint16_t Fan3_RPM = 0;
uint16_t Fan1_Max_RPM = 0;
uint16_t Fan2_Max_RPM = 0;
uint16_t Fan3_Max_RPM = 0;
float Fan_Voltage = 0;

uint8_t sys_run_mode = SYS_NORMAL;
unsigned long test_mode_button_down_ts = 0;
unsigned long entry_test_mode_ts = 0;

Generic_LM75_9_to_12Bit temperature1(ZONE1_LM75_ADDRESS);
Generic_LM75_9_to_12Bit temperature2(ZONE2_LM75_ADDRESS);

MyConfig myConfig;

/*
 * Read the values stored in the EEPROM.
 */
void read_EEPROM_values() {
  EEPROM.get(EEPROM_DATA_BASE_ADDRESS, myConfig);
}

/*
 * Initialize the EEPROM and set the values to Default.
 */
void init_EEPROM() {
  myConfig.temp1HighC = 35.0f;
  myConfig.temp1MiddleC = 34.0f;
  myConfig.temp2HighC = 38.0f;
  myConfig.temp2MiddleC = 35.0f;
  myConfig.FansVoltageCoefficient = 0.0275f;
  myConfig.Fan1PWMLow = 20;
  myConfig.Fan1PWMMiddle = 70;
  myConfig.Fan1PWMHigh = 99;
  myConfig.Fan1PWMDefault = 50;
  myConfig.Fan2PWMLow = 20;
  myConfig.Fan2PWMMiddle = 70;
  myConfig.Fan2PWMHigh = 99;
  myConfig.Fan2PWMDefault = 50;
  myConfig.Fan3PWMLow = 20;
  myConfig.Fan3PWMMiddle = 70;
  myConfig.Fan3PWMHigh = 99;
  myConfig.Fan3PWMDefault = 50;
  myConfig.Fan1_Max_RPM = 0;
  myConfig.Fan2_Max_RPM = 0;
  myConfig.Fan3_Max_RPM = 0;
  EEPROM.put(EEPROM_DATA_BASE_ADDRESS, myConfig);
  EEPROM.put(EEPROM_BASE_ADDRESS, EEPROM_INIT_VALUE);
}

/*
 * save config to EEPROM.
 */
void update_EEPROM() {
  EEPROM.put(EEPROM_DATA_BASE_ADDRESS, myConfig);
}




void Measure_Fan_Voltage(){
  uint16_t result = 0;
  uint16_t highest = 0;
  uint16_t lowest = 1024;
  for (uint8_t i=0; i < 6; i++){
    int val = analogRead(FAN_VOLTAGE);
    if(val > highest)
      highest = val;
    if(val<lowest)
      lowest = val;
    result += val;
  }
  result = (result - highest - lowest) / 4;
    
  Fan_Voltage =  result * myConfig.FansVoltageCoefficient;
}

void Fan1_Sense_ISR(){
  Fan1_Sense_Counter++;
}

void Fan2_Sense_ISR(){
  Fan2_Sense_Counter++;
}

void Fan3_Sense_ISR(){
  Fan3_Sense_Counter++;
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(57600);
  
  // EEPROM, read stored data or init
  uint8_t writtenBefore;
  EEPROM.get(EEPROM_BASE_ADDRESS, writtenBefore);
  if (writtenBefore != EEPROM_INIT_VALUE) {
    init_EEPROM();
  } else {
    read_EEPROM_values();
  }
  
  Wire.begin();
  temperature1.setResolution(Generic_LM75_9_to_12Bit_Compatible::Resolution_11_bits);
  temperature2.setResolution(Generic_LM75_9_to_12Bit_Compatible::Resolution_11_bits);
  
  
#if defined FAN1_USE_PWM_CTRL ||defined FAN2_USE_PWM_CTRL
  Setup_Timer1_PWM();
#endif
  
  // Set the PWM and Power pins as output.
  pinMode( FAN1_POWER, OUTPUT);
  pinMode( FAN2_POWER, OUTPUT);

#ifdef FAN1_USE_PWM_CTRL
  pinMode( FAN1_PWM, OUTPUT);
  Set_FAN1_PWM(myConfig.Fan1PWMDefault);
#endif

#ifdef FAN2_USE_PWM_CTRL
  pinMode( FAN2_PWM, OUTPUT);
  Set_FAN2_PWM(myConfig.Fan2PWMDefault);
#endif

  // Set Power pins output HIGH, turn on the fan's power.
  digitalWrite( FAN1_POWER, FAN_ON);
  digitalWrite( FAN2_POWER, FAN_ON);

  pinMode( FAN1_SENSE, INPUT);
  pinMode( FAN2_SENSE, INPUT);
  pinMode( FAN3_SENSE, INPUT);
  lastRPMTime = millis();
  attachInterrupt(digitalPinToInterrupt(FAN1_SENSE),Fan1_Sense_ISR,FALLING);
  attachInterrupt(digitalPinToInterrupt(FAN2_SENSE),Fan2_Sense_ISR,FALLING);
  attachInterrupt(digitalPinToInterrupt(FAN3_SENSE),Fan3_Sense_ISR,FALLING);

#ifdef USE_FAN3
  pinMode( FAN3_POWER, OUTPUT);
  digitalWrite( FAN3_POWER, FAN_ON);
  #ifdef FAN3_USE_PWM_CTRL
      pinMode( FAN3_PWM, OUTPUT);
      Setup_Timer4_PWM();
      Set_FAN3_PWM(myConfig.Fan3PWMDefault);
  #endif
#endif

  analogReference(INTERNAL);

  pinMode( SYS_MODE_BUTTON, INPUT_PULLUP);
  
  
  delay(3000);
}

void Fan_RPM_Process(){
    unsigned long curTime = millis();
    detachInterrupt(digitalPinToInterrupt(FAN1_SENSE));
    detachInterrupt(digitalPinToInterrupt(FAN2_SENSE));
    detachInterrupt(digitalPinToInterrupt(FAN3_SENSE));
    
    //Fan_RPM = Fan_Sense_Counter * 60/(curTime - lastRPMTime)/1000.0/2
    float timesec = (float)(curTime - lastRPMTime )/500.0;
    timesec = 60.0/timesec;

    Fan1_RPM = Fan1_Sense_Counter * timesec;
    Fan2_RPM = Fan2_Sense_Counter * timesec;
    Fan3_RPM = Fan3_Sense_Counter * timesec;


    if(sys_run_mode == SYS_FAN_TEST){
      if(Fan1_RPM>Fan1_Max_RPM)
        Fan1_Max_RPM = Fan1_RPM;

      if(Fan2_RPM>Fan2_Max_RPM)
        Fan2_Max_RPM = Fan2_RPM;

      if(Fan3_RPM>Fan3_Max_RPM)
        Fan3_Max_RPM = Fan3_RPM;
    }
    
    Fan1_Sense_Counter = 0;
    Fan2_Sense_Counter = 0;
    Fan3_Sense_Counter = 0;

    lastRPMTime = millis();
    attachInterrupt(digitalPinToInterrupt(FAN1_SENSE),Fan1_Sense_ISR,FALLING);
    attachInterrupt(digitalPinToInterrupt(FAN2_SENSE),Fan2_Sense_ISR,FALLING);
    attachInterrupt(digitalPinToInterrupt(FAN3_SENSE),Fan3_Sense_ISR,FALLING);
}

void Test_Mode_Process(){
  unsigned long curTime = millis();
  if(curTime - entry_test_mode_ts > 60000){
      myConfig.Fan1_Max_RPM = Fan1_Max_RPM;
      myConfig.Fan2_Max_RPM = Fan2_Max_RPM;
      myConfig.Fan3_Max_RPM = Fan3_Max_RPM;
      update_EEPROM();
      entry_test_mode_ts = 0;
      test_mode_button_down_ts = 0;
      sys_run_mode = SYS_NORMAL;
      return;
  }
  digitalWrite(FAN1_POWER, FAN_ON);
  digitalWrite(FAN2_POWER, FAN_ON);
#ifdef FAN1_USE_PWM_CTRL
  Set_FAN1_PWM(100);
#endif

#ifdef FAN2_USE_PWM_CTRL
  Set_FAN2_PWM(100);
#endif

#ifdef USE_FAN3
  digitalWrite(FAN3_POWER, FAN_ON);
  #ifdef FAN3_USE_PWM_CTRL
    Set_FAN3_PWM(100);
  #endif
#endif
}

void Normal_Mode_Process(){
  if(zone1>myConfig.temp1MiddleC)
    digitalWrite(FAN1_POWER, FAN_ON);
  else
    digitalWrite(FAN1_POWER, FAN_OFF);

#ifdef FAN1_USE_PWM_CTRL
  if(zone1>myConfig.temp1HighC)
    Set_FAN1_PWM(myConfig.Fan1PWMHigh);
  else
    Set_FAN1_PWM(myConfig.Fan1PWMDefault);
#endif
    
  if((zone2>myConfig.temp2MiddleC && zone1>myConfig.temp1HighC) || zone2>myConfig.temp2HighC)
    digitalWrite(FAN2_POWER, FAN_ON);
  else if(zone1<myConfig.temp1MiddleC && zone2<myConfig.temp2MiddleC )
    digitalWrite(FAN2_POWER, FAN_OFF);

#ifdef FAN2_USE_PWM_CTRL
  if(zone2>myConfig.temp2HighC || zone1>myConfig.temp1HighC)
    Set_FAN2_PWM(myConfig.Fan2PWMHigh);
  else
    Set_FAN2_PWM(myConfig.Fan2PWMDefault);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

  get_messages_from_serial();

  unsigned long curTime = millis();
  if(curTime - lastRPMTime >= FAN_RPM_MEASURE_DURATION){
    Fan_RPM_Process();

  }
  
  if(curTime - lastTime >= 1000){

    Measure_Fan_Voltage();
    
    zone1 = temperature1.readTemperatureC();
    zone2 = temperature2.readTemperatureC();
    
    uint8_t pinval = digitalRead(SYS_MODE_BUTTON);
    
    if(LOW == pinval and sys_run_mode == SYS_NORMAL and test_mode_button_down_ts==0){
      test_mode_button_down_ts = millis();
    }else if(LOW == pinval  and sys_run_mode == SYS_NORMAL and curTime - test_mode_button_down_ts > 3000){
      entry_test_mode_ts = millis();
      sys_run_mode = SYS_FAN_TEST;
    }else if(HIGH == pinval){
      test_mode_button_down_ts = 0;
    }
    

    if(sys_run_mode == SYS_FAN_TEST){
      Test_Mode_Process();
    }

    if(sys_run_mode == SYS_NORMAL){
      Normal_Mode_Process();
    }

    lastTime = millis();
  }

}
