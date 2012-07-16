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

unsigned long microsLastTrigger = 0ul;

double bracketedExposuresMillis[maxBrackets];
double bracketedExposuresMicros[maxBrackets];
unsigned long bracketedExposuresFactor[maxBrackets];

unsigned long baseExposure = 250;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int exposureNumber = 0;

volatile boolean trig = false;

char inByte = 0;

void setup() {
  pinMode(pinOut_trigger, OUTPUT);      
  pinMode(pinIn_frameTriggerReady, INPUT);
  pinMode(pinIn_imaging, INPUT);   
  pinMode(pinIn_frameReadout, INPUT);
  pinMode(pinIn_exposing, INPUT);

  Serial.begin(9600);
  inputString.reserve(200);

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
    microsLastTrigger = micros();
    noInterrupts();
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

  if(stringComplete){

    // reset bracketing sequence
    if(inputString.substring(0,2) == "0>"){
      exposureNumber = 0;
      Serial.print("0>");
      Serial.print(exposureNumber);
      Serial.println("!");
    }

    // set number of brackets
    if(inputString.substring(0,2) == "N>"){
      String _s = inputString.substring(2,inputString.length()-1);
      int _numberBrackets = _s.toInt();
      setNumberBrackets(_numberBrackets);
      Serial.print("N>");
      Serial.print(numberBrackets);
      Serial.println("!");
    }

    // set EV interval of brackets
    if(inputString.substring(0,2) == "V>"){
      String _s = inputString.substring(2,inputString.length()-1);
      int _bracketEv = _s.toInt();
      setBracketEv(_bracketEv);
      Serial.print("V>");
      Serial.print(bracketEv);
      Serial.println("!");
    }

    // set base exposure of brackets
    if(inputString.substring(0,2) == "E>"){
      String _s = inputString.substring(2,inputString.length()-1);
      char charStr[20];
      _s.toCharArray(charStr, 20);
      unsigned long _baseExposure = strtoul(charStr, NULL, 10);
      setBaseExposure(_baseExposure);
      Serial.print("E>");
      Serial.print(baseExposure);
      Serial.println("!");
    }

    // print status
    if(inputString.substring(0,2) == "S>"){
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
      Serial.println("!");
    }
    inputString = "";
    stringComplete = false;
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
    calculateBrackets();
  }
}

void setBracketEv(int _bracketEv){
  bracketEv = _bracketEv;
  calculateBrackets();
}


void setBaseExposure(unsigned long _baseExposure){
  baseExposure = _baseExposure;
  calculateBrackets();
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

