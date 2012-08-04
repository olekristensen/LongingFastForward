#include "delay_x.h"

/****
 * PINOUT
 * 
 * Arduino         Prosilica            Port Config
 * GND      <--->  01 Ext GND           PSU GND
 * NC (PSU) <--->  02 Ext Power (12V)   PSU + 12V
 * 4        <--->  03 Camera Out 4      FrameReadout
 * 5        <--->  04 Camera In 1       Sync1 (Trigger)
 * 3        <--->  05 Camera Out 3      Imaging
 * 2        <--->  06 Camera Out 1      FrameTriggerReady
 * GND      <--->  07 Camera In GND     GND
 * TX3      <--->  08 RxD RS232 In      Serial >
 * RX3      <--->  09 RxD RS232 Out     Serial <
 * 5V       <--->  10 Camera Out Power  
 * 6        <--->  11 Camera In 2       Sync2 
 * 7        <--->  12 Camera Out 2      Exposing
 * 
 ***/

const boolean debug = false;

const int pinOut_trigger          = 5; 
const int pinOut_sync2            = 6; 

const int pinIn_frameTriggerReady = 2;
const int pinIn_imaging           = 3;
const int pinIn_frameReadout      = 4;
const int pinIn_exposing          = 7;

const int maxBrackets = 10;

int numberBrackets = 2;
int bracketEv = 4;

double bracketedExposuresMillis[maxBrackets];
double bracketedExposuresMicros[maxBrackets];
unsigned long bracketedExposuresFactor[maxBrackets];

unsigned long baseExposure = 150;
unsigned long baseExposureTarget = 150;
boolean makeBracketCalculation = false;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String commandString = "";
String numberString = "";

int exposureNumber = 0;

volatile boolean trig = false;

char inByte = 0;
char inChar[64] = "";


void setup() {
  pinMode(pinOut_trigger, OUTPUT);      
  pinMode(pinIn_frameTriggerReady, INPUT);
  pinMode(pinIn_imaging, INPUT);   
  pinMode(pinIn_frameReadout, INPUT);
  pinMode(pinIn_exposing, INPUT);

  Serial.begin(9600);
  inputString.reserve(128);
  commandString.reserve(128);
  numberString.reserve(24);
  
  calculateBrackets();
  attachInterrupt(0,trigger,RISING);
  if(digitalRead(pinIn_frameTriggerReady) == HIGH){
    trigger();
  }
}

void loop()
{

  if(trig){
    trig = false;
    double lDelayMs = bracketedExposuresMillis[exposureNumber];
    double lDelayUs = bracketedExposuresMicros[exposureNumber];
    if(lDelayMs < 1){
      noInterrupts();
    }
    digitalWrite(pinOut_trigger, HIGH);
    _delay_ms(lDelayMs);
    _delay_us(lDelayUs);
    digitalWrite(pinOut_trigger, LOW);
    interrupts();
    exposureNumber = ++exposureNumber%numberBrackets;
    lDelayMs = bracketedExposuresMillis[exposureNumber];
    lDelayUs = bracketedExposuresMicros[exposureNumber];
    double preDelayUs = max(1000.0-lDelayUs, 0.0);
    double preDelayMs = max(38.525-lDelayMs, 0.0);
    _delay_ms(preDelayMs);
    _delay_us(preDelayUs);
  }

  processSerial();

  if(stringComplete){

    while (inputString.indexOf("\n") > 0){
      int commandEnd = inputString.indexOf("\n");
      commandString = inputString.substring(0,commandEnd+1);
      inputString = inputString.substring(commandEnd+1, inputString.length());

      // reset bracketing sequence
      if(commandString.substring(0,2) == "0>"){
        exposureNumber = 0;
        Serial.print("0>");
        Serial.print(exposureNumber);
        Serial.println("<");
      }

      // set number of brackets
      if(commandString.substring(0,2) == "N>"){
        numberString = commandString.substring(2,commandString.length()-1);
        int _numberBrackets = numberString.toInt();
        numberString = "";
        setNumberBrackets(_numberBrackets);
        Serial.print("N>");
        Serial.print(numberBrackets);
        Serial.println("<");
      }

      // set EV interval of brackets
      if(commandString.substring(0,2) == "V>"){
        numberString = commandString.substring(2,commandString.length()-1);
        int _bracketEv = numberString.toInt();
        numberString = "";
        setBracketEv(_bracketEv);
        Serial.print("V>");
        Serial.print(bracketEv);
        Serial.println("<");
      }

      // set base exposure of brackets
      if(commandString.substring(0,2) == "E>"){
        numberString = commandString.substring(2,commandString.length()-1);
        if(numberString.length() > 0){
          char charStr[20];
          numberString.toCharArray(charStr, 20);
          numberString = "";
          unsigned long _baseExposure = strtoul(charStr, NULL, 10);
          setBaseExposure(_baseExposure);
        }
        Serial.print("E>");
        Serial.print(baseExposure);
        Serial.println("<");
      }

      // set base exposure running average target of brackets
      if(commandString.substring(0,2) == "T>"){
        unsigned long _baseExposureTarget = 0;
        numberString = commandString.substring(2,commandString.length()-1);
        if(numberString.length() > 0){
          char charStr[20];
          numberString.toCharArray(charStr, 20);
          numberString = "";
          _baseExposureTarget = strtoul(charStr, NULL, 10);
          setBaseExposureTarget(_baseExposureTarget);
        }
        Serial.print("T>");
        Serial.print(baseExposureTarget);
        Serial.println("<");
      }

      // print status
      if(commandString.substring(0,2) == "S>"){
        Serial.println("S>");
        for(int i= 0; i < numberBrackets; i++){
          Serial.print("S> Exponent: 2^");
          Serial.print((i*bracketEv));
          Serial.print("\tFactor: ");
          Serial.print(bracketedExposuresFactor[i]);
          Serial.print("\tExposure Millis: ");
          Serial.print(bracketedExposuresMillis[i]);
          Serial.print("\t + Micros: ");
          Serial.println(bracketedExposuresMicros[i]);
        }
        Serial.println("<");
      }
      commandString = "";
    }
    Serial.println("!");
    inputString = "";
    stringComplete = false;
    Serial.flush();
  }

  if(baseExposure != baseExposureTarget && exposureNumber == 0){
    baseExposure = (baseExposure* 0.97) + (baseExposureTarget * 0.03);
    if(max(baseExposure,baseExposureTarget)-min(baseExposure,baseExposureTarget) < 1){
      baseExposure = baseExposureTarget;
    }
    makeBracketCalculation = true;
  }

  if(makeBracketCalculation && exposureNumber == 0){
    calculateBrackets();
    makeBracketCalculation=false;
  }

}

void trigger(){
  trig = true;
}

void calculateBrackets(){
  /*  Serial.println();
   Serial.print(numberBrackets);
   Serial.print(" Exposures bracketed with ");
   Serial.print(bracketEv);
   Serial.println(" EVs");
   Serial.print("Base Exposure: ");
   Serial.print(baseExposure);
   Serial.println(" microseconds"); 
   */
  for(int i= 0; i < numberBrackets; i++){
    //    Serial.print("Exponent: 2^");
    //    Serial.print((i*bracketEv));
    bracketedExposuresFactor[i] =  (1ul<<(i*bracketEv));
    bracketedExposuresMillis[i] = (baseExposure*bracketedExposuresFactor[i])/1000;
    bracketedExposuresMicros[i] = (baseExposure*bracketedExposuresFactor[i])%1000;
    /*    Serial.print("\tFactor: ");
     Serial.print(bracketedExposuresFactor[i]);
     Serial.print("\tExposure Millis: ");
     Serial.print(bracketedExposuresMillis[i]);
     Serial.print("\t + Micros: ");
     Serial.println(bracketedExposuresMicros[i]);
     */
  }
}

void setNumberBrackets(int _numberBrackets){
  if(maxBrackets > _numberBrackets){
    numberBrackets = _numberBrackets;
    makeBracketCalculation=true;
  }
}

void setBracketEv(int _bracketEv){
  bracketEv = _bracketEv;
  makeBracketCalculation=true;
}


void setBaseExposure(unsigned long _baseExposure){
  baseExposure = _baseExposure;
  makeBracketCalculation=true;
}

void setBaseExposureTarget(unsigned long _baseExposureTarget){
  baseExposureTarget = _baseExposureTarget;
  makeBracketCalculation=true;
}

void processSerial(){
  while (Serial.available() > 0) {
    int lengthOfCommand = Serial.readBytesUntil('\n', inChar, 64);
    if(lengthOfCommand > 0){
      inChar[lengthOfCommand] = '\n';
      inChar[lengthOfCommand+1] = '\0';
      inputString += inChar;
      stringComplete = true;
    }
 }
}



