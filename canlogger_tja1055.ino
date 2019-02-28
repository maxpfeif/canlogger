/*  Setup to handle interfacing with CAN FT using an NXP TJA1055. 
 *  This also requires the use of EN and ~STDBY pins. 
*/
#include <FlexCAN.h>

static CAN_message_t inMsg0;
static CAN_message_t inMsg1;

#define TJA1055_EN 5
#define TJA1055_STDBY 6 


void setup() {
  Serial.begin(115200);
  
  Can0.begin(50000); // FT CAN runs at 50kbaud 
  
  Can1.begin(500000);  

  pinMode(TJA1055_EN, OUTPUT);
  pinMode(TJA1055_STDBY, OUTPUT); 

  // Wakeup the Transciever for logging 
  digitalWrite(TJA1055_EN, HIGH);
  digitalWrite(TJA1055_STDBY, HIGH);

  Serial.println("Time, ID, Data");
  
}

void loop() {
  bool got_data = false;
  if (Can0.available()) {
    Can0.read(inMsg0); 
    if(inMsg0.id == 0xA3D4003){
      log_msg(&inMsg0);
    }  
  } 
  if(Can1.available()) {
    Can1.read(inMsg1);
    //Serial.printf("                                                       %010d,%06x,%016x\n", millis(), inMsg1.id, inMsg1.buf);
  }
  
}


// prints the message to the console 
void log_msg(CAN_message_t* saveMsg) {
    Serial.print(micros()); 
    Serial.print(",");
    Serial.print(saveMsg->id, HEX); 
    Serial.print(",");  
    uint8_t count = saveMsg->len;
    for(size_t i = 0; i < count; i++) {       
        if(saveMsg->buf[i] < 16) {   // Need to check for the 0X case.
            Serial.print("0"); Serial.print(saveMsg->buf[i], HEX);
        } else {
            Serial.print(saveMsg->buf[i], HEX); 
        }
    }
    Serial.println();
}

void log_busses(CAN_message_t* saveBus1, CAN_message_t* saveBus2) {
  Serial.printf("%010d,%06x,%016x\n", millis(), saveBus2->id, saveBus2->buf);

}
