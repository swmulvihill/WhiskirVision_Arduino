#include <SD.h>   //For talking to SD Card
#include <Wire.h>  //For RTC
#include "RTClib.h" //For RTC

//Define pins
//SD card is on standard SPI pins
//RTC is on Standard I2C Pins
const int CS_PIN   = 53; // LOOK

//Default rate of 25ms
int refresh_rate = 25;

//Define RTC object
RTC_DS1307 RTC;

//Initialize variables

File dataFile;

#define SENSOR1 22
#define SENSOR2 23
#define SENSOR3 24
#define SENSOR4 25
#define SENSOR5 26
#define SENSOR6 27
#define SENSOR7 32
#define SENSOR8 33
#define SENSOR9 34
#define SENSOR10 35
#define SENSOR11 36
#define SENSOR12 37
#define LEDPIN 6
#define SWITCH 10 

// variables will change:
bool sensorState[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; //CHANGED FROM INT TO BOOLEAN
bool lastState[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

//Variable for reading the Switch Status
int SWITCHSTATE ;

//define integers for sensor change
bool SensorChange[12] = {0,0,0,0,0,0,0,0,0,0,0,0};


void setup() 
{
  // initialize the sensor pin 24 as an input:
  pinMode(SENSOR1, INPUT);     
  digitalWrite(SENSOR1, HIGH); // turn on the pullup  // initialize the sensor pin 25 as an input:
  pinMode(SENSOR2, INPUT);     
  digitalWrite(SENSOR2, HIGH); // initialize the sensor pin 26 as an input:
  pinMode(SENSOR3, INPUT);     
  digitalWrite(SENSOR3, HIGH); // initialize the sensor pin 27 as an input:
  pinMode(SENSOR4, INPUT);     
  digitalWrite(SENSOR4, HIGH); // initialize the sensor pin 28 as an input:
  pinMode(SENSOR5, INPUT);     
  digitalWrite(SENSOR5, HIGH); // initialize the sensor pin 29 as an input:
  pinMode(SENSOR6, INPUT);     
  digitalWrite(SENSOR6, HIGH);  // initialize the sensor pin 30 as an input:
  pinMode(SENSOR7, INPUT);     
  digitalWrite(SENSOR7, HIGH);  // initialize the sensor pin 31 as an input:
  pinMode(SENSOR8, INPUT);     
  digitalWrite(SENSOR8, HIGH);  // initialize the sensor pin 32 as an input:
  pinMode(SENSOR9, INPUT);     
  digitalWrite(SENSOR9, HIGH);  // initialize the sensor pin 33 as an input:
  pinMode(SENSOR10, INPUT);     
  digitalWrite(SENSOR10, HIGH);  // initialize the sensor pin 34 as an input:
  pinMode(SENSOR11, INPUT);     
  digitalWrite(SENSOR11, HIGH);  // initialize the sensor pin 35 as an input:
  pinMode(SENSOR12, INPUT);     
  digitalWrite(SENSOR12, HIGH); 
  


// initialize the LED pin as an output:
  pinMode (LEDPIN, OUTPUT);

// initialize the SWITCH pin as an output:
  pinMode (SWITCH, INPUT);

// setpin 19 as high (5V) and 18 as low 
  pinMode(19, OUTPUT);     
  pinMode(18, OUTPUT);
  digitalWrite(19, HIGH);
  digitalWrite(18, LOW);
  
  Serial.begin(9600);
  Serial.println(F("Initializing Card"));
 
 //CS pin, and pwr/gnd pins are outputs
  pinMode(CS_PIN, OUTPUT);
 
  //Initiate the I2C bus and the RTC library
  Wire.begin();
  RTC.begin();
 
  //If RTC is not running, set it to the computer's compile time
  if (! RTC.isrunning())
  {
    Serial.println(F("RTC is NOT running!"));
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 
  //Initialize SD card
  if (!SD.begin(CS_PIN))
  {
    Serial.println(F("Card Failure")); //SLIGHT CHANGE TO INCLUDE ELSE (return command and card ready is outside loop
  }
    else {
    Serial.println(F("Card Ready"));
    return;
    }

  //Read the configuration information (speed.txt)
  File commandFile = SD.open("speed.txt");
  if (commandFile)
  {
    Serial.println(F("Reading Command File"));
  
    while(commandFile.available())
    {
      refresh_rate = commandFile.parseInt(); //SOMETHING STRANGE IS HAPPENING HEREEEEE
    }
    Serial.print(F("Refresh Rate = "));
    Serial.print(refresh_rate);
    Serial.println(F("ms"));
    commandFile.close();
  }
  else
  {
    Serial.println(F("Could not read command file."));
    return;
  }



}

void loop()
{
  SWITCHSTATE = digitalRead(SWITCH); //read input value
  if (SWITCHSTATE == LOW)
 {
   digitalWrite(LEDPIN, LOW);
 } 
  else 
 {
   digitalWrite(LEDPIN, HIGH);
  
// read the state of the sensor value:
  sensorState[1] = digitalRead(SENSOR1);
  sensorState[2] = digitalRead(SENSOR2);
  sensorState[3] = digitalRead(SENSOR3);
  sensorState[4] = digitalRead(SENSOR4);
  sensorState[5] = digitalRead(SENSOR5);
  sensorState[6] = digitalRead(SENSOR6);
  sensorState[7] = digitalRead(SENSOR7);
  sensorState[8] = digitalRead(SENSOR8);
  sensorState[9] = digitalRead(SENSOR9);
  sensorState[10] = digitalRead(SENSOR10);
  sensorState[11] = digitalRead(SENSOR11);
  sensorState[12] = digitalRead(SENSOR12);
  //Get the current date and time info and store in strings


for(int i=0; i<12; i++){
    if (sensorState[i] == HIGH)
  {
   SensorChange[i] = 1;
  }
  else 
  {
   SensorChange[i] = 0;
  }
}


// EXTRA ATTENTION
dataFile = SD.open("log1.csv", FILE_WRITE);
if (dataFile)
  {
    for(int i=0; i<12; i++){
      dataFile.print(SensorChange[i]);
    }
      dataFile.flush(); //Data isn't actually written until we close the connection!

      //Print same thing to the screen for debugging
      Serial.println();
      for(int i=0; i<12; i++){
      Serial.print(SensorChange[i]); // REMOVE TO TEST
      }
  }

  
for(int i=0; i<12; i++){
  lastState[i] = sensorState[i];
}

  delay(refresh_rate);
  }
}
