#include <Arduino.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MakeNJIT";
const char* password = "makenjit25";
const char* serverURL = "https://b0ab-128-235-85-14.ngrok-free.app/trigger-intruder";
#define num_rows 4
#define num_cols 4

#define BUZZER_PIN 3
#define SS_PIN 17
#define RST_PIN 4
#define MISO_PIN 6
#define MOSI_PIN 7
#define SCK_PIN 15
#define SUCCESS_LED 14
#define FAIL_LED 13

#define trig1 35
#define echo1 36
#define trig2 48
#define echo2 45
#define THRESHOLD 15

bool insideLow = false;
bool outsideLow = false;
int insideLowCount = 0;
int outsideLowCount = 0;
char flipflopvar = ' ';

int count = 0;
String x = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String pass = "";
String correct_pass = "";

bool RFID_set = false;

byte row_pins[num_rows] = {1, 2, 42, 41};
byte col_pins[num_cols] = {40, 39, 38, 37};
char keys[num_rows][num_cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad k = Keypad(makeKeymap(keys), row_pins, col_pins, num_rows, num_cols);

void unlockSuccessEvent();
void unlockFailEvent();

void flipflop(char input){
  if(flipflopvar == ' ' || flipflopvar == input){
      flipflopvar = input;
  } else {
    if(flipflopvar == 'o' && input == 'i')
      printf("Going Inside\n");
    if(flipflopvar == 'i' && input == 'o')
      printf("Going Outside\n");
    flipflopvar = ' ';
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi Connected");
}

void loop() {
  if(RFID_set) {
    if(correct_pass.length() < 4) {
      char key = k.getKey();
      if(key) {
        correct_pass.concat(key);
      }
    }
    else {
      digitalWrite(trig1, LOW);
      delayMicroseconds(2);
      digitalWrite(trig1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig1, LOW);
      float distance1 = (pulseIn(echo1, HIGH) * 0.0343) / 2;
    
      digitalWrite(trig2, LOW);
      delayMicroseconds(2);
      digitalWrite(trig2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig2, LOW);
      float distance2 = (pulseIn(echo2, HIGH) * 0.0343) / 2;
    
      if(distance1 < THRESHOLD){
        insideLow = true;
        if(insideLowCount < 20) insideLowCount++;
      } else {
        insideLow = false;
        insideLowCount = 0;
      }
    
      if(distance2 < THRESHOLD){
        outsideLow = true;
        if(outsideLowCount < 20) outsideLowCount++;
      } else {
        outsideLow = false;
        outsideLowCount = 0;
      }
    
      if(outsideLowCount == 3) flipflop('o');
      if(insideLowCount == 3) flipflop('i');
    
      delay(5);
      
      if (!mfrc522.PICC_IsNewCardPresent()) {
        char key = k.getKey();
        if(key) {
          pass.concat(key);
        }
        if(pass.length() == 4) {
          if(pass.equals(correct_pass)) {
            unlockSuccessEvent();
          }
          else {
            unlockFailEvent();
          }
          pass.clear();
        }
        return;
      }
      // Select one of the cards
      if (!mfrc522.PICC_ReadCardSerial())  {
        return;
      }
      String content= "";
      byte letter;
      for (byte i = 0; i < mfrc522.uid.size; i++)  {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      content.toUpperCase();
      if (content.substring(1).equalsIgnoreCase(x.substring(1))) //change here the UID of the card/cards that you want to give access
      {
        unlockSuccessEvent();
      }
     
      else {
        unlockFailEvent();
      }

      pass.clear();
      printf("Password: %s Correct Password: %s%c", pass, correct_pass, '\n');time_t start = time(NULL);
    }
  }
  else {
    if (!mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }

    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      x.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      x.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    RFID_set = true;
  }
  printf("Password: %s Correct Password: %s%c", pass, correct_pass, '\n');
}

void setPass(String s) {
  correct_pass = s;
}

void unlockSuccessEvent() {
  tone(BUZZER_PIN, 1000); // Play 1000 Hz tone
  digitalWrite(SUCCESS_LED, HIGH); // Turn Unlock indicator LED on
  delay(500);             // Let it play for 200ms
  //implement unlocking
  digitalWrite(SUCCESS_LED, LOW); // Turn Unlock indicator LED OFF
  noTone(BUZZER_PIN);     // Stop the tone
  delay(500);             // Wait before next beep
}

void unlockFailEvent() {
  tone(BUZZER_PIN, 500); // Play 500 Hz tone
  digitalWrite(FAIL_LED, HIGH);
  delay(500);             // Let it play for 200ms
  noTone(BUZZER_PIN);     // Stop the tone
  digitalWrite(FAIL_LED, LOW);
  delay(500);             // Wait before next beep

  triggerIntruder(); 
}

void triggerIntruder() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");
        int responseCode = http.POST("{}");
    
        if (responseCode > 0) {
          Serial.println("📤 Alert sent!");
          Serial.println(http.getString());
        } else {
          Serial.print("❌ Error sending alert: ");
          Serial.println(responseCode);
        }
    
        http.end();
      } else {
        Serial.println("❌ WiFi not connected");
      }
}