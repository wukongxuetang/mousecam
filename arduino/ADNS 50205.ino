#include <SPI.h>

int SCLK = 5;
int SDIO = 6;
int NCS  = 7;

void setup() {
  Serial.begin(9600);
   
  pinMode(SCLK, OUTPUT);
  pinMode(SDIO, OUTPUT);
  pinMode(NCS, OUTPUT);
  
  mouse_reset();
  delay(10);
}

void loop() {
  char img[225];
  for (int i=0;i<225;i++){
      img[i]=readLoc(0x0b);
      img[i] &= 0x7F;
      img[i]+=1;
      Serial.print(img[i], DEC);
      Serial.print(",");
      delay(2);
  }  
  Serial.println();
  delay(500);
}

void mouse_reset(){
  // Initiate chip reset
  digitalWrite(NCS, LOW);
  pushbyte(0x3a);
  pushbyte(0x5a);
  digitalWrite(NCS, HIGH);
  delay(10);
  // Set 1000cpi resolution
  digitalWrite(NCS, LOW);
  pushbyte(0x0d);
  pushbyte(0x01);
  digitalWrite(NCS, HIGH);
}

unsigned int readLoc(uint8_t addr){
  unsigned int ret=0;
  digitalWrite(NCS, LOW);
  pushbyte(addr);
  ret=pullbyte();
  digitalWrite(NCS, HIGH);
  return(ret);
}

void pushbyte(uint8_t c){
  pinMode(SDIO, OUTPUT);
  for(unsigned int i=0x80;i;i=i>>1){
    digitalWrite(SCLK, LOW);
    digitalWrite(SDIO, c & i);
    digitalWrite(SCLK, HIGH);
  }
}

unsigned int pullbyte(){
  unsigned int ret=0;
  pinMode(SDIO, INPUT);
  for(unsigned int i=0x80; i>0; i>>=1) {
    digitalWrite(SCLK, LOW);
    ret |= i*digitalRead(SDIO);
    digitalWrite(SCLK, HIGH);
  }
  pinMode(SDIO, OUTPUT);
  return(ret);
}
