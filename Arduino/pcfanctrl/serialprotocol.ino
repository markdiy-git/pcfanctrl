/*
 * serialprotocol.ino
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
 
void write_u8(const uint8_t num)
{
  Serial.write(num);
}

void write_u16(const uint16_t num)
{
  uint8_t buffer[2] = { (uint8_t) (num >> 8), (uint8_t) (num & 0xff) };
  Serial.write((uint8_t*)&buffer,sizeof(uint8_t)*2);
}

void write_float(const float num)
{
  uint8_t buffer[4];
  uint8_t * pbyte = (uint8_t *)&num;
  for(int i=3;i>=0;i--){
    buffer[i] = * pbyte;
    pbyte++;
  }
  Serial.write((uint8_t*)&buffer, 4*sizeof(uint8_t));
}

uint8_t execMultiByteCmd;
size_t waitForData;
uint8_t parserBuffer[MAX_DATA_BYTES];

bool buffer_data(const uint8_t data, const size_t pos)
{
  bool bufferOverflow = (pos >= MAX_DATA_BYTES);

  // Write data to buffer if no overflow condition persist
  if ( !bufferOverflow )
  {
    parserBuffer[pos] = data;
  }

  return bufferOverflow;
}

void get_messages_from_serial()
{
  uint8_t command;
  if(Serial.available() > 0)
  {
    // The first byte received is the instruction
    uint8_t inputData = Serial.read();
    if (waitForData > 0){
      --waitForData;
      buffer_data(inputData, waitForData);
      if ( (waitForData == 0) && execMultiByteCmd ) { // got the whole message
        switch (execMultiByteCmd) {
          case SET_ZONE1_HIGHC:
            myConfig.temp1HighC = *(float *) &parserBuffer[0];
            break;
          case SET_ZONE1_MIDDLEC:
            myConfig.temp1MiddleC = *(float *) &parserBuffer[0];
            break;
          case SET_ZONE2_HIGHC:
            myConfig.temp2HighC = *(float *) &parserBuffer[0];
            break;
          case SET_ZONE2_MIDDLEC:
            myConfig.temp2MiddleC = *(float *) &parserBuffer[0];
            break;
          case SET_VOLTAGE_COEFFICIENT:
            myConfig.FansVoltageCoefficient = *(float *) &parserBuffer[0];
            break;
          case SET_FAN1_PWM_DEFAULT:
            myConfig.Fan1PWMDefault = parserBuffer[0];
            break;
          case SET_FAN2_PWM_DEFAULT:
            myConfig.Fan2PWMDefault = parserBuffer[0];
            break;
          case SET_FAN3_PWM_DEFAULT:
            myConfig.Fan3PWMDefault = parserBuffer[0];
            break;
          case SET_FAN1_PWM_LOW:
            myConfig.Fan1PWMLow = parserBuffer[0];
            break;
          case SET_FAN2_PWM_LOW:
            myConfig.Fan2PWMLow = parserBuffer[0];
            break;
          case SET_FAN3_PWM_LOW:
            myConfig.Fan3PWMLow = parserBuffer[0];
            break;
          case SET_FAN1_PWM_MIDDLE:
            myConfig.Fan1PWMMiddle = parserBuffer[0];
            break;
          case SET_FAN2_PWM_MIDDLE:
            myConfig.Fan2PWMMiddle = parserBuffer[0];
            break;
          case SET_FAN3_PWM_MIDDLE:
            myConfig.Fan3PWMMiddle = parserBuffer[0];
            break;
          case SET_FAN1_PWM_HIGH:
            myConfig.Fan1PWMHigh = parserBuffer[0];
            break;
          case SET_FAN2_PWM_HIGH:
            myConfig.Fan2PWMHigh = parserBuffer[0];
            break;
          case SET_FAN3_PWM_HIGH:
            myConfig.Fan3PWMHigh = parserBuffer[0];
            break;
        }
        execMultiByteCmd = 0;
      }
    } else {
      command = inputData;
      switch (command) {
      case SET_ZONE1_HIGHC:
      case SET_ZONE1_MIDDLEC:
      case SET_ZONE2_HIGHC:
      case SET_ZONE2_MIDDLEC:
      case SET_VOLTAGE_COEFFICIENT:
        waitForData = 4; // four data bytes needed
        execMultiByteCmd = command;
        break;
      case SET_FAN1_PWM_DEFAULT:
      case SET_FAN2_PWM_DEFAULT:
      case SET_FAN3_PWM_DEFAULT:
      case SET_FAN1_PWM_LOW:
      case SET_FAN2_PWM_LOW:
      case SET_FAN3_PWM_LOW:
      case SET_FAN1_PWM_MIDDLE:
      case SET_FAN2_PWM_MIDDLE:
      case SET_FAN3_PWM_MIDDLE:
      case SET_FAN1_PWM_HIGH:
      case SET_FAN2_PWM_HIGH:
      case SET_FAN3_PWM_HIGH:
        waitForData = 1; // one data bytes needed
        execMultiByteCmd = command;
        break;
      case GET_PROTOCOL_VERSION:
        write_u8(PROTOCOL_MAJOR_VERSION);
        write_u8(PROTOCOL_MINOR_VERSION);
        break;
      case GET_ZONE1_HIGHC:
        write_float(myConfig.temp1HighC);
        break;
      case GET_ZONE1_MIDDLEC:
        write_float(myConfig.temp1MiddleC);
        break;
      case GET_ZONE2_HIGHC:
        write_float(myConfig.temp2HighC);
        break;
      case GET_ZONE2_MIDDLEC:
        write_float(myConfig.temp2MiddleC);
        break;
      case GET_FAN1_PWM_DEFAULT:
        write_u8(myConfig.Fan1PWMDefault);
        break;
      case GET_FAN2_PWM_DEFAULT:
        write_u8(myConfig.Fan2PWMDefault);
        break;
      case GET_FAN3_PWM_DEFAULT:
        write_u8(myConfig.Fan3PWMDefault);
        break;
      case GET_FAN1_PWM_LOW:
        write_u8(myConfig.Fan1PWMLow);
        break;
      case GET_FAN2_PWM_LOW:
        write_u8(myConfig.Fan2PWMLow);
        break;
      case GET_FAN3_PWM_LOW:
        write_u8(myConfig.Fan3PWMLow);
        break;
      case GET_FAN1_PWM_MIDDLE:
        write_u8(myConfig.Fan1PWMMiddle);
        break;
      case GET_FAN2_PWM_MIDDLE:
        write_u8(myConfig.Fan2PWMMiddle);
        break;
      case GET_FAN3_PWM_MIDDLE:
        write_u8(myConfig.Fan3PWMMiddle);
        break;
      case GET_FAN1_PWM_HIGH:
        write_u8(myConfig.Fan1PWMHigh);
        break;
      case GET_FAN2_PWM_HIGH:
        write_u8(myConfig.Fan2PWMHigh);
        break;
      case GET_FAN3_PWM_HIGH:
        write_u8(myConfig.Fan3PWMHigh);
        break;
      case GET_ZONE1_TEMPC:
        write_float(zone1);
        break;
      case GET_ZONE2_TEMPC:
        write_float(zone2);
        break;
      case UPDATE_EEPROM:
        update_EEPROM();
        break;
      case GET_FAN1_RPM:
        write_u16(Fan1_RPM);
        break;
      case GET_FAN2_RPM:
        write_u16(Fan2_RPM);
        break;
      case GET_FAN3_RPM:
        write_u16(Fan3_RPM);
        break;
      case GET_FAN1_MAX_RPM:
        write_u16(myConfig.Fan1_Max_RPM);
        break;
      case GET_FAN2_MAX_RPM:
        write_u16(myConfig.Fan2_Max_RPM);
        break;
      case GET_FAN3_MAX_RPM:
        write_u16(myConfig.Fan3_Max_RPM);
        break;
      case GET_FAN_VOLTAGE:
        write_float(Fan_Voltage);
        break;
      }

    }
  }
}
