 #include <Servo.h>
  
  Servo myservo;
  
  void setup() {
    myservo.attach(9);
    // put your setup code here, to run once:
    Serial.begin(9600);
    myservo.write(90);
  }

  int acc=0;
  int magneto=0;
  String input_acc="",input_magneto="";
  char temp =' ';
  void loop() {
    // put your main code here, to run repeatedly:
    if (Serial.available() > 0)
    {
       temp = Serial.read();
      if(temp=='a')
      {
        while(temp!='m')
        {
          temp = Serial.read();
          if ((temp >= 48 && temp < 58))
          {
            input_acc += temp;
            //Serial.println("input");
          } 
        }
        delay(10);
        acc = input_acc.toInt();
        delay(10);
       // Serial.println(input);
        input_acc="";
        //delay(10);
      }

      if(temp=='m')
      {
        while(temp!='\n')
        {
          temp = Serial.read();
          if ((temp >= 48 && temp < 58))
          {
            input_magneto += temp;
            //Serial.println("input");
          } 
        }
        delay(10);
        magneto = input_magneto.toInt();
        delay(10);
        //Serial.println(input);
        input_magneto="";

      }
      temp=' ';
      Serial.print(acc); Serial.print('\t');
      Serial.println(magneto);
      delay(100);
    }
  }
