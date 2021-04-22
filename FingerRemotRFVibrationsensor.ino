#include <rm4.h>
#include <Adafruit_Fingerprint.h> 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
int count_benar=0;
byte status_jari;
#define READY 0
#define BENAR 1
int tambahA = 0;
int tambahB = 0;
int tambahC = 0;
int tambahD = 0;
byte status_tambahanA;
byte status_tambahanB;
byte status_tambahanC;
byte status_tambahanD;
#define Anambah 1
#define Bnambah 1
#define Cnambah 1
#define Dnambah 1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
static const int kData0Pin = A0;
static const int kData1Pin = A1;
static const int kData2Pin = A2;
static const int kData3Pin = A3;
static const int vib_pin = 11;
RM4 remote(kData0Pin, kData1Pin, kData2Pin, kData3Pin); 
const byte relay1Mesin = 8;
const byte relay2Alarm = 9;
const byte relay3Sensor = 10;
//const byte relay4Lampu = 11;
void setup()  
{
  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  pinMode(relay1Mesin, OUTPUT);
  pinMode(relay2Alarm, OUTPUT);
  pinMode(relay3Sensor, OUTPUT);
  //pinMode(relay4Lampu, OUTPUT);

  digitalWrite(relay1Mesin, HIGH);
  digitalWrite(relay2Alarm, HIGH);
  digitalWrite(relay3Sensor, LOW);
  //digitalWrite(relay4Lampu, HIGH);
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
    tambahB ++;
    if (tambahB == 4) tambahB = 0;
    status_tambahanB = Bnambah;
    switch (tambahB){
    case 1:
    Serial.println("MESIN Urip & SENSOR Mati");
    digitalWrite(relay1Mesin, LOW);
    digitalWrite(relay3Sensor, HIGH);
    digitalWrite(relay2Alarm, HIGH);
    delay(500);
    break;

    delay(1000);

    default:
    delay(500);
    digitalWrite(relay1Mesin, HIGH);
    digitalWrite(relay3Sensor, LOW);
    Serial.println("MESIN Mati SENSOR Urip");
    } 
  }
  //Tombol A
   if (button_code == 2) {
    tambahA ++;
    if (tambahA == 4) tambahA = 0;
    status_tambahanA = Anambah;
    switch (tambahA){
    case 1:
    Serial.println("ALARM Urip");
    digitalWrite(relay2Alarm, LOW);
    delay(500);
    break;

    delay(1000);

    default:
    delay(500);
    digitalWrite(relay2Alarm, HIGH);
    Serial.println("ALARM Mati");
    }
  }
  //Tombol C
   if (button_code == 8) {
    tambahC ++;
    if (tambahC == 4) tambahC = 0;
    status_tambahanC = Cnambah;
    switch (tambahC){
    case 1:
    digitalWrite(relay1Mesin, HIGH);
    digitalWrite(relay3Sensor, LOW);
    Serial.println("MESIN Mati SENSOR Urip");
    delay(500);
    break;
    } 
  }
  //Tombol D
   if (button_code == 4) {
    tambahD ++;
    if (tambahD == 4) tambahD = 0;
    status_tambahanD = Dnambah;
    switch (tambahD){
    case 1:
    Serial.println("ALARM Mati");
    digitalWrite(relay2Alarm, HIGH);
    delay(500);
    break;
    } 
  }
 Serial.println(button_code);
  
 delay(100);
 int val;
  val=digitalRead(vib_pin);
  if(val==HIGH)
  {
      Serial.println("ALARM Urip");
      digitalWrite(relay2Alarm,LOW);
      }
    /*else if(val==LOW)
    {
      Serial.println("ALARM Mati");
      digitalWrite(relay2Alarm,HIGH);
      }*/
      Serial.println(val);
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
    break;
 
    default:
    delay(1500);
    digitalWrite(relay1Mesin, HIGH);
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
