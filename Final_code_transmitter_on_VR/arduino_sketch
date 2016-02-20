#include <Wire.h>
#include <mylib.h>

#define address 0x1E //I2c address
#define SensorPin1      A1  //Accelerometer connected here
#define filterSamples   13  // filterSamples should  be an odd number, no smaller than 3

int sensSmoothArray1 [filterSamples];   // array for holding raw sensor values for Acc
int sensSmoothArray2 [filterSamples];   // array for holding raw sensor values for Magneto

int rawData1, smoothData1;  // variables for sensor1 data
int rawData2, smoothData2;  // variables for sensor2 data
int rawData2_x,rawData2_y,rawData2_z;

int acc_mean=0,acc_up=0,acc_down=0; //positions for calibration
int magneto_mean=0,magneto_left=0;
int magneto_right=0;

char acc_mag[10];

mylib digital1; //For acc
mylib digital2; //For Magneto

int button = 7; //button pin on protosnap
int red = 3;
int green = 5;
int blue = 6;

int serial_switch = A0; //switch to turn on serial comm

void setup(){
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(red, OUTPUT); 
  pinMode(green, OUTPUT); 
  pinMode(blue, OUTPUT); 
  //pinMode(serial_switch,INPUT);
  Wire.begin();
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();

  digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  digitalWrite(blue,LOW);
}
int mapped_value_acc;
int mapped_value_magneto;
int flag=0;
int quad=1; //flag for 4th quadrant

void loop(){       
//  Serial.print(magneto_left);Serial.print('\t');
//  Serial.print(magneto_right); Serial.print('\t');
//  Serial.println(magneto_mean);

//Serial.print(magneto_left); Serial.print("\t");
// Serial.print(magneto_right); Serial.print("\t");
// Serial.print(smoothData2);Serial.print("\t");
// Serial.println(mapped_value_magneto);
 if(digitalRead(button)==LOW)
 {
  switch (flag)
  {
    case 0:
    magneto_mean = digital2.calib_magneto();
    acc_mean = digital1.calib_acc();
//    Serial.print("Mag mean: ");
//    Serial.print(magneto_mean);
    delay(100);
    digital1.green_led_blink();
    flag++;
    break;

    case 1:
    magneto_left = digital2.calib_magneto();
//    Serial.print("Mag left: ");
//    Serial.print(magneto_left);
    delay(100);
    digital1.green_led_blink();
    flag++;
    break;

    case 2:
    magneto_right = digital2.calib_magneto();
//    Serial.print("Mag right: ");
//    Serial.print(magneto_right);
    delay(100);
    digital1.green_led_blink();
    flag++;
    break;

    case 3:
    acc_up = digital1.calib_acc();
    delay(1000);
    digital1.green_led_blink();
    flag++;
    break;

    case 4:
    acc_down = digital1.calib_acc();
    delay(1000);
    digital1.green_led_blink();
    flag++;
    break;
   }
  
  }

  rawData1 = analogRead(SensorPin1); //acc data 
  
  Wire.beginTransmission(address); //Read values from Magneto
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

  Wire.requestFrom(address, 6);
  if (6 <= Wire.available()) {
    rawData2_x = Wire.read() << 8; //X msb
    rawData2_x |= Wire.read(); //X lsb

    rawData2_z = Wire.read() << 8; //X msb
    rawData2_z |= Wire.read();

    rawData2_y = Wire.read() << 8; //X msb
    rawData2_y |= Wire.read();
  }
  
 rawData2 = atan2(rawData2_y,rawData2_x)*RAD_TO_DEG; //Conversion to degrees
if(rawData2<0)
rawData2 = 180+(180+rawData2);

if((magneto_left>260 && magneto_right<80) || (magneto_left>260 && magneto_right>360))
{
  
  if(flag==4)
  {
    digital1.violet_special();
    magneto_right = 360 + magneto_right;
    flag++;
    delay(1000);
  }
  
  if(rawData2<80)
  { 
    rawData2 = 360 + rawData2;
    smoothData2 = digital2.digitalSmooth(rawData2, sensSmoothArray2);
    mapped_value_magneto = map(smoothData2,magneto_left,magneto_right,10,170);
    mapped_value_magneto = constrain(mapped_value_magneto,10,170);
    }
  else
  {
  smoothData2 = digital2.digitalSmooth(rawData2, sensSmoothArray2); 
  mapped_value_magneto = map(smoothData2,magneto_left,magneto_right,10,170);
  mapped_value_magneto = constrain(mapped_value_magneto,10,170);
  }
}

else
{
  //digital1.normal_working();
  smoothData2 = digital2.digitalSmooth(rawData2, sensSmoothArray2); 
  mapped_value_magneto = map(smoothData2,magneto_left,magneto_right,10,170);
  mapped_value_magneto = constrain(mapped_value_magneto,10,170);
}
    smoothData1 = digital1.digitalSmooth(rawData1, sensSmoothArray1);  //Smooth acc values
    mapped_value_acc = map(smoothData1,acc_up,acc_down,10,170);
    mapped_value_acc = constrain(mapped_value_acc,10,170);
    
 sprintf(acc_mag,"a%dm%d",mapped_value_acc,mapped_value_magneto);
 delay(40);

// Serial.print(magneto_left); Serial.print("\t");
// Serial.print(magneto_right); Serial.print("\t");
// Serial.print(smoothData2);Serial.print("\t");
// Serial.println(mapped_value_magneto);
 
if(analogRead(serial_switch)==1023)
{
digital1.transmitting_led();
Serial.println(acc_mag);
delay(20);
}
else
{
  if(flag==6)
  digital1.violet_special();
  else
  digital1.normal_working();
}

delay(100);

}
