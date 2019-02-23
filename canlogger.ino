#include <FlexCAN.h>

static CAN_message_t inMsg;

volatile int16_t hv_battery_current = 0;
volatile uint16_t hv_battery_voltage = 0; 
volatile float batt_current = 0.00;


void setup() {
  Serial.begin(115200);
  Can0.begin(500000); 
  Serial.println("Time,ID,Bus,Data");
  
}

void loop() {
  if (Can0.available()) {
    Can0.read(inMsg);
    if(inMsg.id == 711 && inMsg.len == 6 ){

  
        /*    
        int16_t voltage_math_var = 0; 
        voltage_math_var = inMsg.buf[2] << 11; 
        voltage_math_var += inMsg.buf[3] << 3;
        voltage_math_var += inMsg.buf[4] >> 5 ;

        for (int i = 11; i > -1; i--) {
            int tmp_cpy = voltage_math_var & 1;

            voltage_math_var = voltage_math_var >> 1; 

            tmp_cpy = tmp_cpy << i;

            hv_battery_voltage += tmp_cpy;
        } 

  
        hv_battery_voltage *= 0.125;
        */
      
      
      
      //log_msg(&inMsg,0);
      
      /*
      int16_t temp = 0;
      hv_battery_current = 1 & inMsg0.buf[3];
      for(int i = 7; i > -1; i--) {
          temp = inMsg0.buf[2] << i;
          temp &= 128; 
          temp >> 8 - i; 
          hv_battery_current += temp;  
      }
      for(int i = 7; i > 3; i--) {
          temp = inMsg0.buf[1] << i;
          temp &= 128; 
          temp >> 12 - i;
          hv_battery_current += temp; 
      }
      */
        char cur_sign = '+'; 
        if(inMsg.buf[1] > 31) {
          cur_sign = '-';
        }
        hv_battery_current = (inMsg.buf[1] & 15) << 8;
        hv_battery_current += inMsg.buf[2];
        //Serial.print("Binary hv battery current before is "); Serial.println(hv_battery_current, BIN); 
        hv_battery_current = 8192 - hv_battery_current; 
        hv_battery_current = hv_battery_current & 0b11111111;
        //Serial.print("Binary hv battery current after is "); Serial.println(hv_battery_current, BIN); 
      //hv_battery_voltage = inMsg0.buf[4] << 4;
      //hv_battery_voltage += inMsg0.buf[5] >> 4;
      //hv_battery_voltage *= 0.125; 
        batt_current = hv_battery_current * 0.15;
     //Serial.print("Reported Pack Voltage is:"); Serial.println(hv_battery_voltage);
        Serial.print("Reported Pack Current is:"); Serial.print(cur_sign); Serial.println(batt_current);
    

      
    }
  }
}


// prints the message to the console 
void log_msg(CAN_message_t* saveGHS1Msg, uint8_t origin_bus) {
    Serial.print(micros()); 
    Serial.print(",");
    Serial.print(saveGHS1Msg->id); 
    Serial.print(","); 
    Serial.print(origin_bus);
    Serial.print(","); 
    uint8_t count = saveGHS1Msg->len;
    for(size_t i = 0; i < count; i++) {    
        
        if(saveGHS1Msg->buf[i] < 16) {   // Need to check for the 0X case.
            Serial.print("0"); Serial.print(saveGHS1Msg->buf[i], HEX);
        } else {
            Serial.print(saveGHS1Msg->buf[i], HEX); 
        }
         
        //Serial.print(saveGHS1Msg->buf[i], DEC);
        //Serial.print(" ");
        
    }
    Serial.println();
}
