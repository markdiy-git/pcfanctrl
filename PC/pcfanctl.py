#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  pcfanctl.py
#  
#  Copyright 2020 markdiy <https://markdiy.blogspot.com>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  

import serial
import time
import struct
import sys


SERIALPORT      = '/dev/ttyACM0'
SERIALBAUDRATE  = 57600
DEBUG           = 1

class PCFanCtrl:
    
    GET_PROTOCOL_VERSION   =  0
    SET_ZONE1_HIGHC        =  1
    GET_ZONE1_HIGHC        =  2
    SET_ZONE1_MIDDLEC      =  3
    GET_ZONE1_MIDDLEC      =  4
    SET_ZONE2_HIGHC        =  5
    GET_ZONE2_HIGHC        =  6
    SET_ZONE2_MIDDLEC      =  7
    GET_ZONE2_MIDDLEC      =  8
    SET_FAN1_PWM_DEFAULT   =  9
    GET_FAN1_PWM_DEFAULT   = 10
    SET_FAN2_PWM_DEFAULT   = 11
    GET_FAN2_PWM_DEFAULT   = 12
    SET_FAN3_PWM_DEFAULT   = 13
    GET_FAN3_PWM_DEFAULT   = 14
    SET_FAN1_PWM_LOW       = 15
    GET_FAN1_PWM_LOW       = 16
    SET_FAN2_PWM_LOW       = 17
    GET_FAN2_PWM_LOW       = 18
    SET_FAN3_PWM_LOW       = 19
    GET_FAN3_PWM_LOW       = 20
    SET_FAN1_PWM_MIDDLE    = 21
    GET_FAN1_PWM_MIDDLE    = 22
    SET_FAN2_PWM_MIDDLE    = 23
    GET_FAN2_PWM_MIDDLE    = 24
    SET_FAN3_PWM_MIDDLE    = 25
    GET_FAN3_PWM_MIDDLE    = 26
    SET_FAN1_PWM_HIGH      = 27
    GET_FAN1_PWM_HIGH      = 28
    SET_FAN2_PWM_HIGH      = 29
    GET_FAN2_PWM_HIGH      = 30
    SET_FAN3_PWM_HIGH      = 31
    GET_FAN3_PWM_HIGH      = 32
    UPDATE_EEPROM          = 33
    GET_ZONE1_TEMPC        = 34
    GET_ZONE2_TEMPC        = 35
    GET_FAN_VOLTAGE        = 36
    GET_FAN1_RPM           = 37
    GET_FAN2_RPM           = 38
    GET_FAN3_RPM           = 39
    GET_FAN1_MAX_RPM       = 40
    GET_FAN2_MAX_RPM       = 41
    GET_FAN3_MAX_RPM       = 42
    TEST_FAN_MAX_RPM       = 43
    SET_VOLTAGE_COEFFICIENT =  44
    
    def __init__(self,SerailPort,Baudrate,Timeout=1):
        self.port = serial.Serial(port=SerailPort,baudrate=Baudrate,timeout=Timeout)
        self.ProtoMajVer  = None
        self.ProtoMinVer  = None
        self.Zone1HighC   = None
        self.Zone2HighC   = None
        self.Zone1MiddleC = None
        self.Zone2MiddleC = None
        self.Zone1TempC   = None
        self.Zone2TempC   = None
        self.FAN1PWMDefault = None
        self.FAN2PWMDefault = None
        self.FAN3PWMDefault  = None
        self.FAN1PWMHigh = None
        self.FAN2PWMHigh = None
        self.FAN3PWMHigh = None
        self.FAN1RPM = None
        self.FAN2RPM = None
        self.FAN3RPM = None
        self.FAN1MaxRPM = None
        self.FAN2MaxRPM = None
        self.FAN3MaxRPM = None
        self.FanVoltage = None
        
        
    def write(self, message):
        if DEBUG>=2:
            print("Writing data to serial port:", message.hex())
        self.port.write(message)
        self.port.flushOutput()
    
    def read_float(self):
        data = self.port.read(4)
        if data and DEBUG>=2:
            print("Read from serial port:", data.hex())
            print("float data :", struct.unpack('!f',data))
        return struct.unpack('!f',data)[0]
    
    def read_uint8(self):
        return struct.unpack('!B',self.port.read(1))[0]
    
    def read_uint16(self):
        return struct.unpack('!H',self.port.read(2))[0]

    def write_float(self, data):
        self.port.write(struct.pack('!f', data))
        self.port.flushOutput()
    
    def write_uint8(self, data):
        self.port.write(struct.pack('!B', data))
        self.port.flushOutput()
    
    def write_uint16(self, data):
        self.port.write(struct.pack('!H', data))
        self.port.flushOutput()

    def GetProtocolVersion(self):
        self.write(self.GET_PROTOCOL_VERSION.to_bytes(1, byteorder = sys.byteorder))
        #self.port.flushOutput()
        self.ProtoMajVer = self.read_uint8()
        self.ProtoMinVer = self.read_uint8()
        if DEBUG : print('Protocol Version:', self.ProtoMajVer, self.ProtoMinVer)
        return (self.ProtoMajVer, self.ProtoMinVer)

    def SetZone1HighC(self,data):
        self.write(self.SET_ZONE1_HIGHC.to_bytes(1, byteorder = sys.byteorder))
        self.write_float(data)
    
    def GetZone1HighC(self):
        self.write(self.GET_ZONE1_HIGHC.to_bytes(1, byteorder = sys.byteorder))
        self.port.flushOutput()
        self.Zone1HighC = self.read_float()
        if DEBUG : print('Zone1HighC:', self.Zone1HighC)
        return self.Zone1HighC
    
    def SetZone1MiddleC(self,data):
        self.write(self.SET_ZONE1_MIDDLEC.to_bytes(1, byteorder = sys.byteorder))
        self.write_float(data)
    
    def GetZone1MiddleC(self):
        self.write(self.GET_ZONE1_MIDDLEC.to_bytes(1, byteorder = sys.byteorder))
        self.port.flushOutput()
        self.Zone1MiddleC = self.read_float()
        if DEBUG : print('Zone1MiddleC:', self.Zone1MiddleC)
        return self.Zone1MiddleC
    
    def SetZone2HighC(self,data):
        self.write(self.SET_ZONE2_HIGHC.to_bytes(1, byteorder = sys.byteorder))
        self.write_float(data)
    
    def GetZone2HighC(self):
        self.write(self.GET_ZONE2_HIGHC.to_bytes(1, byteorder = sys.byteorder))
        self.port.flushOutput()
        self.Zone2HighC = self.read_float()
        if DEBUG : print('Zone2HighC:', self.Zone2HighC)
        return self.Zone2HighC
    
    def SetZone2MiddleC(self,data):
        self.write(self.SET_ZONE2_MIDDLEC.to_bytes(1, byteorder = sys.byteorder))
        self.write_float(data)
    
    def GetZone2MiddleC(self):
        self.write(self.GET_ZONE2_MIDDLEC.to_bytes(1, byteorder = sys.byteorder))
        self.port.flushOutput()
        self.Zone2MiddleC = self.read_float()
        if DEBUG : print('Zone2MiddleC:', self.Zone2MiddleC)
        return self.Zone2MiddleC
    
    def SetFAN1PWMDefault(self,data):
        self.write(self.SET_FAN1_PWM_DEFAULT.to_bytes(1, byteorder = sys.byteorder))
        self.write_uint8(data)
    
    def SetFAN1PWMHigh(self,data):
        self.write(self.SET_FAN1_PWM_HIGH.to_bytes(1, byteorder = sys.byteorder))
        self.write_uint8(data)
    
    def GetFAN1PWMDefault(self):
        self.write(self.GET_FAN1_PWM_DEFAULT.to_bytes(1, byteorder = sys.byteorder))
        self.FAN1PWMDefault = self.read_uint8()
        if DEBUG : print('FAN1PWMDefault:', self.FAN1PWMDefault)
        return self.FAN1PWMDefault
    
    def GetFAN1PWMHigh(self):
        self.write(self.GET_FAN1_PWM_HIGH.to_bytes(1, byteorder = sys.byteorder))
        self.FAN1PWMHigh = self.read_uint8()
        if DEBUG : print('FAN1PWMHigh:', self.FAN1PWMHigh)
        return self.FAN1PWMHigh
    
    def SetFAN2PWMDefault(self,data):
        self.write(self.SET_FAN2_PWM_DEFAULT.to_bytes(1, byteorder = sys.byteorder))
        self.write_uint8(data)
    
    def SetFAN2PWMHigh(self,data):
        self.write(self.SET_FAN2_PWM_HIGH.to_bytes(1, byteorder = sys.byteorder))
        self.write_uint8(data)
    
    def GetFAN2PWMDefault(self):
        self.write(self.GET_FAN2_PWM_DEFAULT.to_bytes(1, byteorder = sys.byteorder))
        self.FAN2PWMDefault = self.read_uint8()
        if DEBUG : print('FAN2PWMDefault:', self.FAN2PWMDefault)
        return self.FAN2PWMDefault
    
    def GetFAN2PWMHigh(self):
        self.write(self.GET_FAN2_PWM_HIGH.to_bytes(1, byteorder = sys.byteorder))
        self.FAN2PWMHigh = self.read_uint8()
        if DEBUG : print('FAN2PWMHigh:', self.FAN2PWMHigh)
        return self.FAN2PWMHigh
    
    def SetFAN3PWMDefault(self,data):
        self.write(self.SET_FAN3_PWM_DEFAULT.to_bytes(1, byteorder = sys.byteorder))
        self.write_uint8(data)
    
    def GetFAN3PWMDefault(self):
        self.write(self.GET_FAN3_PWM_DEFAULT.to_bytes(1, byteorder = sys.byteorder))
        self.FAN3PWMDefault = self.read_uint8()
        if DEBUG : print('FAN3PWMDefault:', self.FAN3PWMDefault)
        return self.FAN3PWMDefault
    
    def GetFAN3PWMHigh(self):
        self.write(self.GET_FAN3_PWM_HIGH.to_bytes(1, byteorder = sys.byteorder))
        self.FAN3PWMHigh = self.read_uint8()
        if DEBUG : print('FAN3PWMHigh:', self.FAN3PWMHigh)
        return self.FAN3PWMHigh
    
    def SetFAN3PWMHigh(self,data):
        self.write(self.SET_FAN3_PWM_HIGH.to_bytes(1, byteorder = sys.byteorder))
        self.write_uint8(data)
    
    def GetFAN1RPM(self):
        self.write(self.GET_FAN1_RPM.to_bytes(1, byteorder = sys.byteorder))
        self.FAN1RPM = self.read_uint16()
        if DEBUG : print('FAN1RPM:', self.FAN1RPM)
        return self.FAN1RPM
    
    def GetFAN2RPM(self):
        self.write(self.GET_FAN2_RPM.to_bytes(1, byteorder = sys.byteorder))
        self.FAN2RPM = self.read_uint16()
        if DEBUG : print('FAN2RPM:', self.FAN2RPM)
        return self.FAN2RPM
    
    def GetFAN3RPM(self):
        self.write(self.GET_FAN3_RPM.to_bytes(1, byteorder = sys.byteorder))
        self.FAN3RPM = self.read_uint16()
        if DEBUG : print('FAN3RPM:', self.FAN3RPM)
        return self.FAN3RPM
    
    def GetFAN1MaxRPM(self):
        self.write(self.GET_FAN1_MAX_RPM.to_bytes(1, byteorder = sys.byteorder))
        self.FAN1MaxRPM = self.read_uint16()
        if DEBUG : print('FAN1MaxRPM:', self.FAN1MaxRPM)
        return self.FAN1MaxRPM
    
    def GetFAN2MaxRPM(self):
        self.write(self.GET_FAN2_MAX_RPM.to_bytes(1, byteorder = sys.byteorder))
        self.FAN2MaxRPM = self.read_uint16()
        if DEBUG : print('FAN2MaxRPM:', self.FAN2MaxRPM)
        return self.FAN2MaxRPM
    
    def GetFAN3MaxRPM(self):
        self.write(self.GET_FAN3_MAX_RPM.to_bytes(1, byteorder = sys.byteorder))
        self.FAN3MaxRPM = self.read_uint16()
        if DEBUG : print('FAN3MaxRPM:', self.FAN3MaxRPM)
        return self.FAN3MaxRPM
    
    def GetFanVoltage(self):
        self.write(self.GET_FAN_VOLTAGE.to_bytes(1, byteorder = sys.byteorder))
        self.FanVoltage = self.read_float()
        if DEBUG : print('FanVoltage:', round(self.FanVoltage,2))
        return self.FanVoltage

    def GetZone1TempC(self):
        self.write(self.GET_ZONE1_TEMPC.to_bytes(1, byteorder = sys.byteorder))
        self.port.flushOutput()
        self.Zone1TempC = self.read_float()
        if DEBUG : print('Zone1TempC:', round(self.Zone1TempC,2))
        return self.Zone1TempC
    
    def GetZone2TempC(self):
        self.write(self.GET_ZONE2_TEMPC.to_bytes(1, byteorder = sys.byteorder))
        self.port.flushOutput()
        self.Zone2TempC = self.read_float()
        if DEBUG : print('Zone2TempC:', round(self.Zone2TempC,2))
        return self.Zone2TempC
    
    def SetVoltageCoefficient(self,data):
        self.write(self.SET_VOLTAGE_COEFFICIENT.to_bytes(1, byteorder = sys.byteorder))
        self.write_float(data)
        
    def UpdateEEPROM(self):
        self.write(self.UPDATE_EEPROM.to_bytes(1, byteorder = sys.byteorder))
    
    def close(self):
        return self.port.close()



def main(args):
    
    fanCtrl = PCFanCtrl(SERIALPORT,SERIALBAUDRATE)

    print('\nGet Protocol Version:')
    proto_maj,proto_min = fanCtrl.GetProtocolVersion()
    
    #fanCtrl.SetZone1HighC(34.9)
    #fanCtrl.SetZone1MiddleC(25.6)
    #fanCtrl.SetZone2HighC(37.9)
    #fanCtrl.SetZone2MiddleC(34.9)
    fanCtrl.SetFAN1PWMDefault(75)
    fanCtrl.SetFAN2PWMDefault(60)
    #fanCtrl.SetFAN1PWMDefault(90)
    #fanCtrl.SetFAN2PWMHigh(99)
    #fanCtrl.SetFAN2PWMDefault(50)
    #fanCtrl.SetFAN3PWMDefault(50)
    #fanCtrl.SetVoltageCoefficient(0.027)
    fanCtrl.UpdateEEPROM()
    #time.sleep(1)
    
    print('\n\nCurrent Config:')
    fanCtrl.GetZone1HighC()
    fanCtrl.GetZone1MiddleC()
    fanCtrl.GetZone2HighC()
    fanCtrl.GetZone2MiddleC()
    fanCtrl.GetFAN1PWMDefault()
    fanCtrl.GetFAN2PWMDefault()
    fanCtrl.GetFAN3PWMDefault()
    fanCtrl.GetFAN1PWMHigh()
    fanCtrl.GetFAN2PWMHigh()
    fanCtrl.GetFAN3PWMHigh()
    
    fanCtrl.GetFAN1MaxRPM()
    fanCtrl.GetFAN2MaxRPM()
    
    print('\n\nZone1 Zone2 Current Temperature:')
    
    while True:
        try:
            time.sleep(5)
            fanCtrl.GetZone1TempC()
            fanCtrl.GetZone2TempC()
            fanCtrl.GetFAN1RPM()
            fanCtrl.GetFAN2RPM()
            fanCtrl.GetFanVoltage()
        except KeyboardInterrupt:
            print("Closing the connection")
            fanCtrl.close()

            break
        
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
