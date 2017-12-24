
/*********************************************************************
Monochrome OLEDs based on SSD1306 drivers
128x64 size display using I2C
Bekotes: 
 VCC 3.3v
 GND gnd
 SCL A5
 SDA A4

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_64.h>

#include <Servo.h>
#include <Metro.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


const int pinMotor = 7;
const int pinPotmeter = A0;
const int pinRotationSignal = 2;

const double increment = 0.5f; //0.25f

Servo myservo;
double motValue;
long rotationCounter;
unsigned long time;
unsigned long lastTime;
unsigned long rpm;

Metro ledMetro = Metro(1000); 


void setup() {
  
  pinMode(pinRotationSignal, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinRotationSignal), rotation, FALLING);
  rotationCounter = 0;

  myservo.attach(pinMotor);
  motValue = 0;
  Serial.begin(9600);  

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.println("Fschnell Laboratory");
  display.println("");
  display.println("Lab Centrifuge V1.1");
  display.println("               2017");
  display.display();
  delay(3000);
  
  time = millis();
  lastTime = time;
}


void rotation() {
  
  //Ha a rotation signal és az utolso rotation signal idő diff > 3 millis, akkor tortent igazi rotation
  if (time - lastTime > 3){
    lastTime = time;
    rotationCounter++;
  }
  
  time = millis();
}

void loop() {
  int potValue = analogRead(pinPotmeter);
  
  //Serial.print(potValue);
  //Serial.print(" * ");
  
  potValue = map(potValue, 0, 742, 30, 140);
  
  if (potValue < 1){
    motValue = 0;
  } else {
    double diff = potValue - motValue;
    if (diff > 1){
      motValue = motValue + increment;
    } 
    else if (diff < -1){
      motValue = motValue - increment;
    }
  }
  
  //Serial.print(potValue);
  //Serial.print(" - ");
  //Serial.println((int) motValue);
  
  myservo.write((int) motValue);

  
 if (ledMetro.check() == 1) {
   rpm = rotationCounter*60;
   //Serial.println(rot);
   rotationCounter = 0;
 }  

  display.clearDisplay();
  display.setTextSize(3);  
  display.setCursor(1,1); //4,2
  display.println(rpm);
  display.display();
  
  delay(15); 
}


