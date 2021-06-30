
#include <stdio.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(A4, A5);

// defines pins numbers
//New GPS GY-GPS6MV2
int rf1 = 8;// 4 channels
int rf2 = 9;
int alc  = 12;
int hel  = 13;
void setup() 
{
  pinMode(alc, INPUT);
  pinMode(hel, INPUT);  
  digitalWrite(hel, HIGH);   
}
void loop() 
{
       if(digitalRead(alc) == LOW)
         {
           digitalWrite(rf1,LOW);
         }
       if(digitalRead(alc) == HIGH)
         {
      
           digitalWrite(rf1,HIGH);
         }
       if(digitalRead(hel) == LOW)
         {
           
           digitalWrite(rf2,LOW);
         }
       if(digitalRead(hel) == HIGH)
         {
           
           digitalWrite(rf2,HIGH);
         }  
           
}    
