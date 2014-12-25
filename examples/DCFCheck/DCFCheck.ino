#include <DCF77.h>
#include <Time.h>  

#define DCF_PIN 2
#define DCF_INTERRUPT 0

#define BLINK_PIN 13

#define ON_RISING_FLANK true

bool last = LOW;

DCF77 DCF(DCF_PIN, DCF_INTERRUPT, ON_RISING_FLANK); 

void setup() {
  pinMode(BLINK_PIN, OUTPUT);
  
  Serial.begin(9600); 
  DCF.Start();
  DCF.setSplitTime(105, 205);
}

void loop() {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime!=0) {
    setTime(DCFtime);
    digitalClockDisplay();
  }     
  
  if (DCF.isPulse()) {
    if (ON_RISING_FLANK && last == LOW) {
      Serial.println();
    }
    last = HIGH;
    digitalWrite(BLINK_PIN, HIGH);
    Serial.print("1");
  } else {
    if (!ON_RISING_FLANK && last == HIGH) {
      Serial.println();
    }
    last = LOW;
    digitalWrite(BLINK_PIN, LOW);
    Serial.print("0");
  }
  
  delay(10);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.println(); 
  Serial.println(); 
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
