
#include "Arduino.h"
#include "mylib.h"
#include "Wire.h"

#define address 0x1E //I2c address
#define SensorPin1      A1  //Accelerometer connected here
#define filterSamples   13
#define SensorPin1      A1

int red_led = 3;
int blue_led = 6;
int green_led = 5;	//led pins

int mylib::digitalSmooth(int rawIn, int *sensSmoothArray)
{
  int j, k, temp, top, bottom;
  long total;
  static int i;
 // static int raw[filterSamples];
  static int sorted[filterSamples];
  boolean done;

  i = (i + 1) % filterSamples;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
  sensSmoothArray[i] = rawIn;                 // input new data into the oldest slot

  // Serial.print("raw = ");

  for (j=0; j<filterSamples; j++){     // transfer data array into anther array for sorting and averaging
    sorted[j] = sensSmoothArray[j];
  }

  done = 0;                // flag to know when we're done sorting              
  while(done != 1){        // simple swap sort, sorts numbers from lowest to highest
    done = 1;
    for (j = 0; j < (filterSamples - 1); j++){
      if (sorted[j] > sorted[j + 1]){     // numbers are out of order - swap
        temp = sorted[j + 1];
        sorted [j+1] =  sorted[j] ;
        sorted [j] = temp;
        done = 0;
      }
    }
  }

/*
  for (j = 0; j < (filterSamples); j++){    // print the array to debug
    Serial.print(sorted[j]); 
    Serial.print("   "); 
  }
  Serial.println();
*/

  // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
  bottom = max(((filterSamples * 15)  / 100), 1); 
  top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));   // the + 1 is to make up for asymmetry caused by integer rounding
  k = 0;
  total = 0;
  for ( j = bottom; j< top; j++){
    total += sorted[j];  // total remaining indices
    k++; 
    // Serial.print(sorted[j]); 
    // Serial.print("   "); 
  }

//  Serial.println();
//  Serial.print("average = ");
//  Serial.println(total/k);
  return total / k;    // divide by number of samples 
}

int mylib::calib_magneto()
{
	int calib_x=0,calib_y=0,calib_z=0;
	int mean_x =0,mean_y=0;
	int temp;
        for(int j=0;j<10;j++)
    {
         Wire.beginTransmission(address); //Read values from Magneto
         Wire.write(0x03); //select register 3, X MSB register
         Wire.endTransmission();

         Wire.requestFrom(address, 6);
         if (6 <= Wire.available()) 
         {
           calib_x = Wire.read() << 8; //X msb
           calib_x |= Wire.read(); //X lsb
	   calib_z = Wire.read() << 8;
	   calib_z |= Wire.read();
	   calib_y = Wire.read() << 8;
	   calib_y |= Wire.read();
           mean_x+=calib_x;
	   mean_y+=calib_y;
           calib_x=0;
	   calib_y=0;
         } 
         
    }
    mean_x = mean_x/10;
    mean_y = mean_y/10;
    temp = atan2(mean_y,mean_x)*RAD_TO_DEG;
    if(temp<0)
	return (180+(180+temp));
    else
	return temp;
	//return (atan2(mean_y,mean_x)*RAD_TO_DEG);
}

int mylib::calib_acc()
{
	int calib=0;
	int mean = 0;

        for(int j=0;j<10;j++)
    {
           calib = analogRead(SensorPin1); 
           mean+=calib;
           calib=0;
     }
 
    mean = mean/10;
    return mean;
         
}

void mylib::green_led_blink()
{
    digitalWrite(green_led,LOW);
    digitalWrite(red_led,HIGH);
    digitalWrite(blue_led,HIGH);
    delay(500); 
    digitalWrite(green_led,LOW);
    digitalWrite(red_led,LOW);
    digitalWrite(blue_led,LOW);
}

void mylib::violet_special()
{
    digitalWrite(red_led,LOW);
    digitalWrite(green_led,HIGH);
    digitalWrite(blue_led,LOW);
}

void mylib::normal_working()
{
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, LOW);
    digitalWrite(blue_led, LOW);
}

void mylib::transmitting_led()
{
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, HIGH);
    digitalWrite(blue_led, HIGH);
}
