#!/usr/bin/env python
#
# GrovePi Example for using the Grove Temperature & Humidity Sensor Pro 
# (http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro)
#
# The GrovePi connects the Raspberry Pi and Grove sensors.  
# You can learn more about GrovePi here:  http://www.dexterindustries.com/GrovePi
#
# Have a question about this example?  Ask on the forums here:  http://forum.dexterindustries.com/c/grovepi
#
'''
## License
The MIT License (MIT)
GrovePi for the Raspberry Pi: an open source platform for connecting Grove Sensors to the Raspberry Pi.
Copyright (C) 2017  Dexter Industries
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
'''
import grovepi
import math
import time,sys
import decimal
import json 
from grove_rgb_lcd import *

#begin by connecting the grove light sensor to port A0
#SIG, NC, VCC, GND
light_sensor = 0

#now connect the various led lights
blue_led = 4
red_led = 6
green_led = 5

#establish threshold for light sensor
threshold = 10


# Connect the Grove Temperature & Humidity Sensor Pro to digital port D7
#adding a bit more specificity since there are multiple sensors in this setup
temp_sensor = 7  # The Sensor goes on digital port 7.

# temp_humidity_sensor_type
# Grove Base Kit comes with the blue sensor.
blue = 0    # The Blue colored sensor.
white = 1   # The White colored sensor.

        
#adding data string for the JSON database
data = {}
data['weather'] = []

while True:
    try: 
        # This example uses the blue colored sensor. 
        # The first parameter is the port, the second parameter is the type of sensor.
        #data was added infront of temp and humidity to specify string
        [temp,humidity] = grovepi.dht(temp_sensor,blue)  
        
        grovepi.pinMode(temp_sensor,"INPUT")
        #one input for 3 separate outputs
        grovepi.pinMode(green_led,"OUTPUT") 
        grovepi.pinMode(blue_led, "OUTPUT")
        grovepi.pinMode(red_led, "OUTPUT")
        
        if math.isnan(temp) == False and math.isnan(humidity) == False:
            #convert celsius to fahrenheit
            temp = ((temp * 9) / 5) + 32
        
            #round the number so it will display on the LCD
            round_temp = round(temp, 2) #rounds to 2 decimal spaces
        
            print("temp = %.02f F humidity =%.02f%%"%(round_temp, humidity)) #what will display in the terminal
            time.sleep(30) #30 seconds between readings
    
            #what will display on the LCD
            setRGB(0,128,64) #sets color to the backlight; this creates a light blue
            setText("Temp: %.02f F\nHumidity: %.02f%%"%(round_temp,humidity)) #displays to the LCD
            time.sleep(30) #refreshes every 30 seconds
        
#now to ensure that the lights only turn on when certain requirements met
# green only = 60-85 degrees AND with humidity less than 80%
# blue only = 85-95 degrees AND with humidity less than 80%
# red only = temp is over 95 degrees
# green and blue = humidity over 80%
        
        if round_temp in range(64,85) and humidity in range(0,81): 
            grovepi.digitalWrite(green_led,1) 
        else: 
            grovepi.digitalWrite(green_led,0)
        
        if round_temp in range(85,95) and humidity in range(0,81):
            grovepi.digitalWrite(blue_led,1)
        else: 
            grovepi.digitalWrite(blue_led,0)
        
        if round_temp > 95:
            grovepi.digitalWrite(red_led,1)
        else:
            grovepi.digitalWrite(red_led,0)
            
        if humidity > 80:
            grovepi.digitalWrite(green_led and blue_led,1)
        else: 
            grovepi.digitalWrite(green_led and blue_led,0)

            
#ensure that the data will only record during the day
#meaning it will only record if it's light out

        # Get sensor value
        lightsensor_value = grovepi.analogRead(light_sensor)

        #calculate resistance of sensor in K
        resistance = (float)(1023 - lightsensor_value) * 10 / lightsensor_value 

        if resistance < threshold: 
            #adding JSON print
            data['weather'].append([round_temp,humidity])
        
            #adding JSON file named final1.json
            with open("final1.json", "w") as outfile:
                json.dump(data['weather'], outfile, indent=4)
            time.sleep(1800) #takes readings every 30 minutes (1800 seconds = 30 minutes)
        else:
            print("It's night time")

            
            
    except IOError:
        print("Error")

            
        #to display in HTML5 on static canvas, ensure your json file is in the same folder as the static canvas html5
        #when you open the canvas, a data graph will display the temp/humidity readouts
