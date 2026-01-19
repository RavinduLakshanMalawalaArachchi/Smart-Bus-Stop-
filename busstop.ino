#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo gateServo;

#define PIR_PIN 1
#define RED_LED 3
#define GREEN_LED 4
#define WHITE_LED 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define BUZZER_PIN 8
#define SERVO_PIN 9
#define LDR_PIN A0

long duration;
int distance;
int ldrValue;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(0); 

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Bus Halt");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {

  int pirState = digitalRead(PIR_PIN);
  if (pirState == HIGH) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  ldrValue = analogRead(LDR_PIN);
  if (ldrValue < 400) {
    digitalWrite(WHITE_LED, HIGH); 
  } else {
    digitalWrite(WHITE_LED, LOW); 
  }

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > 0 && distance <= 20) {
    digitalWrite(BUZZER_PIN, HIGH);  
  } else {
    digitalWrite(BUZZER_PIN, LOW);   
  }

  if (distance > 0 && distance <= 10) {
    gateServo.write(90);              
  } else {
    gateServo.write(0);              
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bus Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  if (distance > 0 && distance <= 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bus Arrived");
    lcd.setCursor(0, 1);
    lcd.print("Gate Open");
  } 
  else if (distance > 10 && distance <= 20) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bus Near");
    lcd.setCursor(0, 1);
    lcd.print("Prepare Stop");
  }

  delay(500);
}
