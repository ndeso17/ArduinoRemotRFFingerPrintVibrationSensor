#include <rm4.h>
#include <Adafruit_Fingerprint.h> 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
int count_benar=0;
byte status_jari;
#define READY 0
#define BENAR 1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
static const int vib_pin = 10;
static const int kData0Pin = 4;
static const int kData1Pin = 5;
static const int kData2Pin = 6;
static const int kData3Pin = 7;
RM4 remote(kData0Pin, kData1Pin, kData2Pin, kData3Pin); 
const byte relay1Mesin = 8;
const byte relay2Alarm = 9;
const byte relay3Sensor = 11;
void setup()  
{
  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  //pinMode(vib_pin,INPUT);
  pinMode(relay1Mesin, OUTPUT);
  pinMode(relay2Alarm, OUTPUT);
  pinMode(relay3Sensor, OUTPUT);

  digitalWrite(relay1Mesin, HIGH);
  digitalWrite(relay2Alarm, HIGH);
  digitalWrite(relay3Sensor, LOW);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor FingerPrint Ketemu Boss...!");
  } else {
    Serial.println("FingerPrint Ora Ketemu Boss... :(");
    while (1) { delay(1); }
  }
  Serial.println("Ngenteni Sidik Jari Sing Pas...");
}

void loop()                     // run over and over again
{
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.

  pinMode(vib_pin,INPUT);
  const int button_code = remote.buttonCode();
  //Tombol B
   if (button_code == 1) {
    Serial.println("MESIN Urip & SENSOR Mati");
    digitalWrite(relay1Mesin, LOW);
    digitalWrite(relay3Sensor, HIGH);
    } 
  //Tombol A
   if (button_code == 4) {
    Serial.println("ALARM Urip");
    digitalWrite(relay2Alarm, LOW);
    }
  //Tombol C
   if (button_code == 8) {
    Serial.println("MESIN Mati & SENSOR Urip");
    digitalWrite(relay1Mesin, HIGH);
    digitalWrite(relay3Sensor, LOW);
    } 
  //Tombol D
   if (button_code == 2) {
    Serial.println("ALARM Mati");
    digitalWrite(relay2Alarm, HIGH);
    } 
 Serial.println(button_code);
  
 delay(100);
 int val;
  val=digitalRead(vib_pin);
  if(val==1)
  {
      Serial.println("ALARM Urip");
      digitalWrite(relay2Alarm,LOW);
      }
   }
   
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    count_benar ++;
    if ( count_benar == 4) count_benar = 0;
    status_jari = BENAR;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  // found a match!
  Serial.print("Driji Ketemu, ID Driji #"); Serial.print(finger.fingerID); 
  Serial.print(" Pas e "); Serial.println(finger.confidence); 
  switch(status_jari){
    case READY:
    getFingerprintID();
    delay(50);
    break;
    case BENAR:
    switch(count_benar){
    case 1:
    delay(1500);
    Serial.println("Mesin Urip");
    digitalWrite(relay1Mesin, LOW);
    digitalWrite(relay3Sensor, HIGH);
    break;
 
    case 2:
    delay(1500);
    digitalWrite(relay1Mesin, HIGH);
    digitalWrite(relay3Sensor, LOW);
    Serial.println("Mesin Mati");
    break;
  }
  break;
  }
  /*digitalWrite(R_MESIN, LOW);
  delay(5000);
  digitalWrite(R_MESIN, HIGH);*/
  return finger.fingerID;
}
