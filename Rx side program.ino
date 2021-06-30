#include <LiquidCrystal.h>
#include <stdio.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9);
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
// defines pins numbers
//New GPS GY-GPS6MV2
int rf1 = 10;
int rf2 = 11;
int rf3 = 12;
int rf4 = 13;
int shifter = A0;
int relay   = A1;
int buzzer  = A2;
int rtr1=0;
int memsx=0,memsy=0;
 unsigned char rcv,count,gchr='x',gchr1='x',robos='s';
 unsigned char panics='x';
 char rcvmsg[10],pastnumber[11];
 //char pastnumber1[11],pastnumber2[11];//pastnumber3[11];
 char gpsval[50];
// char dataread[100] = "";
// char lt[15],ln[15];
int i=0,k=0,lop=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String gpsString="";
char *test="$GPRMC";
//int hbtc=0,hbtc1=0,rtrl=0;
unsigned char gv=0,msg1[10],msg2[11];
 float lati=0,longi=0;
unsigned int lati1=0,longi1=0;
unsigned char flat[5],flong[5];
unsigned char finallat[8],finallong[9];
 int ii=0,rchkr=0;
float tempc=0,weight=0;
float vout=0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int sti=0;
void okcheck()
{
  unsigned char rcr;
  do{
      rcr = Serial.read();
    }while(rcr != 'K');
}
void okcheck1()
{
  unsigned char rcr;
  do{
      rcr = mySerial.read();
    }while(rcr != 'K');
}
void things_send()
{
unsigned char recr;
 mySerial.write("AT+CIPMUX=1\r\n");delay(2000);
 mySerial.write("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");delay(4000);          //OK  LINKED
 mySerial.write("AT+CIPSEND=4,77\r\n");  delay(3000); 
 mySerial.write("GET https://api.thingspeak.com/update?api_key=8XUKE2ELJ10RLN83&");
}
void things_sendlt()
{
unsigned char recr;
 mySerial.write("AT+CIPMUX=1\r\n");delay(2000);
 mySerial.write("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");delay(4000);          //OK  LINKED
 mySerial.write("AT+CIPSEND=4,79\r\n");  delay(3000); 
 mySerial.write("GET https://api.thingspeak.com/update?api_key=CB8YL3GZYT8ON7ER&");
}
void things_sendln()
{
unsigned char recr;
 mySerial.write("AT+CIPMUX=1\r\n");delay(2000);
 mySerial.write("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");delay(4000);          //OK  LINKED
 mySerial.write("AT+CIPSEND=4,80\r\n");  delay(3000); 
 mySerial.write("GET https://api.thingspeak.com/update?api_key=CB8YL3GZYT8ON7ER&");
}
void things_rcv()
{
unsigned char recr;
  mySerial.write("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");  delay(4000);  
  mySerial.write("AT+CIPSEND=4,73\r\n");    
          /*
          do{
         recr = Serial.read();
        }while(recr != '>');*/  delay(3000);
//https://api.thingspeak.com/channels/412507/fields/2.json?results=10000
 mySerial.write("GET https://api.thingspeak.com/channels/449069/fields/3.json?results=1");//39

}

void things_done()
{    
 mySerial.write("\r\n\r\n");  delay(4000);
}


void send_link()
 {
    Serial.write("AT+CMGS=\"");
    Serial.write(pastnumber);
    Serial.write("\"\r\n");  delay(2500);  
    Serial.write("https://www.google.co.in/search?client=opera&q=");
    for(ii=0;ii<=6;ii++){Serial.write(finallat[ii]);}
    Serial.write("%2C");
    for(ii=0;ii<=7;ii++){Serial.write(finallong[ii]);}
    Serial.write(0x1A);delay(4000);delay(4000);
 }


 void beep()
 {
  digitalWrite(buzzer,LOW);delay(1000);delay(1000);digitalWrite(buzzer,HIGH);
 }
 
void setup() 
{
 // unsigned char rcr;
 Serial.begin(9600);//serialEvent();
 mySerial.begin(9600);


  pinMode(rf1, INPUT);
  pinMode(rf2, INPUT);
  pinMode(rf3, INPUT);
  pinMode(rf4, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(shifter, OUTPUT);


  digitalWrite(relay, LOW);
  digitalWrite(buzzer, HIGH);
  digitalWrite(shifter, LOW);
   
  lcd.begin(16, 2);lcd.cursor();
  
  
  lcd.print("Smart Helmet-Rx");
  
  
    digitalWrite(shifter, HIGH);
   get_gps();
   gps_convert();
    digitalWrite(shifter, LOW);

    lcd.clear();
    lcd.setCursor(0,0);
     for(ii=0;ii<=6;ii++) lcd.write(finallat[ii]);
     
    lcd.setCursor(0,1);
     for(ii=0;ii<=7;ii++) lcd.write(finallong[ii]);    
    
    lcd.setCursor(0,2);
    lcd.write("Lat:");
     for(ii=0;ii<=6;ii++) lcd.write(finallat[ii]);
     
     lcd.setCursor(0,3);
    lcd.write("Long:");
     for(ii=0;ii<=7;ii++) lcd.write(finallong[ii]);   
  delay(1500); 
     gsminit();
  delay(1500); 
  wifiinit();
  delay(1500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("A:");      //2,0
    lcd.setCursor(8,0);
    lcd.print("H:");     //10,0
   
    lcd.setCursor(0,1);
    lcd.print("MEMS:");     //6,1

      inputString = "";
      stringComplete = false;              
        sti=0;

      //serialEvent();  
}

char helms='x',alcs='x',memss='x';
void loop() 
{
       if(digitalRead(rf1) == LOW)
         {
           lcd.setCursor(2,0);lcd.print("ON ");
           alcs='1';
         }
       if(digitalRead(rf1) == HIGH)
         {
           lcd.setCursor(2,0);lcd.print("OFF");
           alcs='0';
         }


       
       if(digitalRead(rf2) == LOW)
         {
           lcd.setCursor(10,0);lcd.print("Wear  ");
           helms='w';
         }
       if(digitalRead(rf2) == HIGH)
         {
           lcd.setCursor(10,0);lcd.print("NOWear");
           helms='n';
         }  


      memsx = analogRead(A3);
       delay(500);
      memsy = analogRead(A4);   
       delay(500);

     if((memsx > 310 && memsx < 390) && (memsy > 310 && memsy < 390))
       {
        lcd.setCursor(6,1);lcd.print("Stable");   
        memss='s';
       }
      else
       {
        lcd.setCursor(6,1);lcd.print("Moved "); 
        memss='m';
       } 
        
 
        
     if(alcs == '0' && helms == 'w' && memss == 's')
       {
        digitalWrite(relay, HIGH);   
       }
     else
       {
        digitalWrite(relay, LOW);
       }   




      if(alcs == '1')
        {
          beep();
                   lcd.setCursor(13,1);lcd.print("1  ");
           things_send();
           mySerial.write("field2=00100");//converts0(tempc);
           things_done();
      
           for(rtr1=0;rtr1<15;rtr1++){lcd.setCursor(10,1);convertk(rtr1);delay(950);}
   
            Serial.write("AT+CMGS=\"");
            Serial.write(pastnumber);
            Serial.write("\"\r\n");  delay(3000);  
            Serial.write("Alc-ON");
            Serial.write("  https://www.google.co.in/search?client=opera&q=");
            for(ii=0;ii<=6;ii++){Serial.write(finallat[ii]);}
            Serial.write("%2C");
            for(ii=0;ii<=7;ii++){Serial.write(finallong[ii]);}
            Serial.write(0x1A);delay(4000);delay(5000);
                   lcd.setCursor(13,1);lcd.print("   ");
          }

      if(helms == 'n')
        {
          beep();
                   lcd.setCursor(13,1);lcd.print("2  ");
           things_send();
           mySerial.write("field3=00100");//converts0(tempc);
           things_done();
      
           for(rtr1=0;rtr1<15;rtr1++){lcd.setCursor(10,1);convertk(rtr1);delay(950);}
   
            Serial.write("AT+CMGS=\"");
            Serial.write(pastnumber);
            Serial.write("\"\r\n");  delay(3000);  
            Serial.write("Helmet Nowear");
            Serial.write("   https://www.google.co.in/search?client=opera&q=");
            for(ii=0;ii<=6;ii++){Serial.write(finallat[ii]);}
            Serial.write("%2C");
            for(ii=0;ii<=7;ii++){Serial.write(finallong[ii]);}
            Serial.write(0x1A);delay(4000);delay(5000);
                   lcd.setCursor(13,1);lcd.print("   ");
         }

      if(memss == 'm')
        {
          beep();
                   lcd.setCursor(13,1);lcd.print("3  ");
           things_send();
           mySerial.write("field4=00100");//converts0(tempc);
           things_done();
      
           for(rtr1=0;rtr1<15;rtr1++){lcd.setCursor(10,1);convertk(rtr1);delay(950);}
   
            Serial.write("AT+CMGS=\"");
            Serial.write(pastnumber);
            Serial.write("\"\r\n");  delay(3000);  
            Serial.write("MEMS Moved Acc  ");
            Serial.write("   https://www.google.co.in/search?client=opera&q=");
            for(ii=0;ii<=6;ii++){Serial.write(finallat[ii]);}
            Serial.write("%2C");
            for(ii=0;ii<=7;ii++){Serial.write(finallong[ii]);}
            Serial.write(0x1A);delay(4000);delay(5000);
                   lcd.setCursor(13,1);lcd.print("   ");
         }  
}    


void serialEvent() 
{
  while (Serial.available()) 
        {
         
         char inChar = (char)Serial.read();
           //sti++;
           //inputString += inChar;
          if(inChar == '*')
            {sti=1;
              inputString += inChar;
             //  stringComplete = true;
             // gchr = inputString[sti-1] 
            }
          if(sti == 1)
            {
                inputString += inChar;
            }
          if(inChar == '#')
            {sti=0;
              stringComplete = true;      
            }
        }
}





/*
void serialEvent() 
{
  while (Serial.available()) 
        {
         
         char inChar = (char)Serial.read();
           if(inChar == '*')
             {
              gchr = Serial.read();
             }
           if(inChar == '#')
             {
              gchr1 = Serial.read();
             }        
        }
}*/


int readSerial(char result[]) 
{
  int i = 0;
  while (1) 
  {
    while (Serial.available() > 0) 
    {
      char inChar = Serial.read();
      if (inChar == '\n') 
         {
          result[i] = '\0';
          Serial.flush();
          return 0;
         }
      if (inChar != '\r') 
         {
          result[i] = inChar;
          i++;
         }
    }
  }
}


int readSerial1(char result[]) 
{
  int i = 0;
  while (1) 
  {
    while (Serial.available() > 0) 
    {
      char inChar = Serial.read();
      if (inChar == '*') 
         {
          result[i] = '\0';
          Serial.flush();
          return 0;
         }
      if (inChar != '*') 
         {
          result[i] = inChar;
          i++;
         }
    }
  }
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   //while (gps.available()>0)            //Serial incoming data from GPS

    while (Serial.available() > 0)
   {
    //char inChar = (char)gps.read();
    char inChar = (char)Serial.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
  
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
 
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=19;i<=20;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=20;i<=21;i++)         
      lat_minut+=gpsString[i];
     for(i=23;i<=24;i++)         
      lat_minut+=gpsString[i];


       
  String log_degree="";
    for(i=31;i<=33;i++)
      log_degree+=gpsString[i];
  String log_minut="";
    for(i=34;i<=35;i++)
      log_minut+=gpsString[i];
    for(i=37;i<=38;i++)
      log_minut+=gpsString[i];


    
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}

void gps_convert()
{
  if(gps_status)
  {
 // Serial.println(gpsString);

  if(gpsString[0] == '$' && gpsString[1] == 'G' && gpsString[2] == 'P' && gpsString[3] == 'R' && gpsString[4] == 'M' && gpsString[5] == 'C')
    {
     // Serial.println("Don11111111111111111111111111111111111111111111111111111\r\n");      
    //  Serial.write(gpsString[18]);Serial.write(gpsString[19]);Serial.write(gpsString[20]);Serial.write(gpsString[21]);Serial.write(gpsString[22]);
     //lcd.setCursor(0,0);
     for(ii=0;ii<9;ii++)
       {
        //lcd.write(gpsString[19+ii]);    
        msg1[ii] = gpsString[19+ii];
        //Serial.write(msg1[ii]);
       }
       //Serial.println("\r\n");
     //lcd.setCursor(0,1);
      for(ii=0;ii<10;ii++)
       {
        //lcd.write(gpsString[32+ii]);
        msg2[ii] = gpsString[32+ii];    
       // Serial.write(msg2[ii]);
       }

// Serial.println(msg1);
// Serial.println(msg2);

       //lati = (((msg1[2]-48)*100000) +((msg1[3]-48)*10000) + ((msg1[5]-48)*1000) + ((msg1[6]-48)*100) + ((msg1[7]-48)*10) + (msg1[8]-48)); 
       //longi = (((msg2[3]-48)*100000) + ((msg2[4]-48)*10000) + ((msg2[6]-48)*1000) + ((msg2[7]-48)*100) + ((msg2[8]-48)*10) + (msg2[9]-48));

       lati = (((msg1[2]-48)*1000) + ((msg1[3]-48)*100) + ((msg1[5]-48)*10) + (msg1[6]-48)); 
       longi = (((msg2[3]-48)*1000) + ((msg2[4]-48)*100) + ((msg2[6]-48)*10) + (msg2[7]-48));

      // converts(lati);Serial.write("-");
      // converts(longi);Serial.write("\r\n");
       
       lati = (lati/60);  longi = (longi/60);

       lati = (lati*100); longi = (longi*100);
       lati1 = lati;      longi1 = longi;
       
// Serial.write("After ");
  //      converts(lati1);Serial.write("-");
    //   converts(longi1);Serial.write("\r\n");
 

              convlat(lati); convlong(longi);
        finallat[0] = msg1[0];
        finallat[1] = msg1[1];
        finallat[2] = '.';
        finallat[3] = flat[0]; finallat[4] = flat[1];finallat[5] = flat[2];finallat[6] = flat[3];finallat[7] = '\0';


        finallong[0] = msg2[0];
        finallong[1] = msg2[1];
        finallong[2] = msg2[2];
        finallong[3] = '.';
        finallong[4] = flong[0];finallong[5] = flong[1];finallong[6] = flong[2];finallong[7] = flong[3];finallong[8] = '\0'; 

   
    }
  }  
}


 void convlat(unsigned int value)  
      {
             unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
    g=g|0x30;              
      h=h|0x30;
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flat[0] = c;
               flat[1] = e;
               flat[2] = g;
             flat[3] = h;


          }

 void convlong(unsigned int value)  
      {
            unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
    g=g|0x30;              
      h=h|0x30;
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flong[0] = c;
               flong[1] = e;
               flong[2] = g;
             flong[3] = h;


          }



void wifiinit()
{
  
  mySerial.write("AT\r\n");         delay(2000);
  mySerial.write("ATE0\r\n");         okcheck1();delay(2000);
  mySerial.write("AT+CWMODE=3\r\n");         delay(2000);  
  mySerial.write("AT+CWJAP=\"iotserver\",\"iotserver123\"\r\n");
  okcheck1();
  mySerial.write("AT+CIPMUX=1\r\n");delay(3000);
 
  lcd.clear();
  lcd.print("Connected");
  
}

void gsminit()
{
  Serial.write("AT\r\n");                   okcheck();
  Serial.write("ATE0\r\n");                 okcheck();
  Serial.write("AT+CMGF=1\r\n");            okcheck();
  Serial.write("AT+CNMI=1,2,0,0\r\n");      okcheck();
  Serial.write("AT+CSMP=17,167,0,0\r\n");   okcheck();
    
  lcd.clear();
  lcd.print("SEND MSG STORE");
  lcd.setCursor(0,1);
  lcd.print("MOBILE NUMBER");  
  do{
     rcv = Serial.read();
    }while(rcv != '*');
     readSerial(pastnumber);pastnumber[10] = '\0';
     
     /*
  pastnumber1[0] = pastnumber[0];pastnumber1[1] = pastnumber[1];pastnumber1[2] = pastnumber[2];pastnumber1[3] = pastnumber[3];pastnumber1[4] = pastnumber[4];pastnumber1[5] = pastnumber[5];
  pastnumber1[6] = pastnumber[6];pastnumber1[7] = pastnumber[7];pastnumber1[8] = pastnumber[8];pastnumber1[9] = pastnumber[9];pastnumber1[10] = '\0';
    */
/*
  pastnumber3[0] = pastnumber[20];pastnumber3[1] = pastnumber[21];pastnumber3[2] = pastnumber[22];pastnumber3[3] = pastnumber[23];pastnumber3[4] = pastnumber[24];pastnumber3[5] = pastnumber[25];
  pastnumber3[6] = pastnumber[26];pastnumber3[7] = pastnumber[27];pastnumber3[8] = pastnumber[28];pastnumber3[9] = pastnumber[29];pastnumber3[10] = '\0';
*/
  lcd.clear();
  lcd.print(pastnumber);

                       delay(3000);
    Serial.write("AT+CMGS=\"");
    Serial.write(pastnumber);
    Serial.write("\"\r\n"); delay(3000);
    Serial.write("Reg\r\n");
    Serial.write(0x1A);    
    delay(4000);  delay(5000);  

}


void converts(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   Serial.write(a);
   Serial.write(c);
   Serial.write(e); 
   Serial.write(g);
   Serial.write(h);
}

void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   //lcd.write(a);
   //lcd.write(c);
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}

void convertk(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
  // lcd.write(a);
  // lcd.write(c);
  // lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}

void converts0(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   mySerial.write(a);
   mySerial.write(c);
   mySerial.write(e); 
   mySerial.write(g);
   mySerial.write(h);
}
