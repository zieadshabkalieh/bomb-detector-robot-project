#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);
#define buzer 2
#define sensore 3
#define tr 6
#define ec 7
#define ENA 8
#define in2 9
#define in1 10
#define in4 11
#define in3 12
#define ENB 13

#include <NewPing.h>
NewPing sonar(tr, ec, 400);
int dis;
bool prev;
char receivedCommand;

void setup() {
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Set the address for the receiver
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();

  pinMode(buzer, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(sensore, INPUT_PULLUP);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Serial.begin(9600);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedCommand, sizeof(receivedCommand));
    Serial.println(receivedCommand);

    int sensore_val = digitalRead(sensore);

    // Check if the line sensor detects the border
    if (sensore_val == LOW) {
      // Change direction when the border is detected
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(500);  // Adjust the delay based on your requirements
      // Stop the motors
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);

      // Buzzer pattern for border detection
      digitalWrite(buzer, HIGH);
      delay(500);
      digitalWrite(buzer, LOW);
      delay(500);
      digitalWrite(buzer, HIGH);
      delay(500);
      digitalWrite(buzer, LOW);
      delay(500);
    } else {
      // Continue with the existing obstacle avoidance logic

      int dis = sonar.ping_cm();
      analogWrite(ENA, map(dis, 0, 200, 20, 255));
      analogWrite(ENB, map(dis, 0, 200, 20, 255));
      Serial.println(dis);

      // Motor control based on received command
      if (receivedCommand == 'f' && dis > 40) {
           digitalWrite(in1,LOW);
   digitalWrite(in2,HIGH);
   digitalWrite(in3,LOW);
   digitalWrite(in4,HIGH);
      } else if (receivedCommand == 'b') {
           analogWrite(ENA,255);
   analogWrite(ENB,255);
   digitalWrite(in1,HIGH);
   digitalWrite(in2,LOW);
   digitalWrite(in3,HIGH);
   digitalWrite(in4,LOW);
   delay(100);
      } else if (receivedCommand == 'l') {
            digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
      } else if (receivedCommand == 'r') {
            digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
      } else if (receivedCommand == 's' || receivedCommand == 'S') {
            digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
      }
    }
  }
}
