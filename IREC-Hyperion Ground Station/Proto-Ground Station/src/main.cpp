/**
 * Hyperion - IREC
 * File: main.cpp
 * Organization: RIT Space Exploration
 * Desc
 *    	Main proto-ground station responsible for arming and disarming the payload.
 */

#include <SPI.h>
#include <IREC_Hyperion_Protocol.h>
#include <RH_RF95.h>

#define BAUD_RATE 115200
#define INITAL_STR_SIZE 200
#define MAX_MSG_LEN_RECV 100

// SPI pins
#define RFM95_RST     6   // "A"
#define RFM95_CS      5   // "B"
#define RFM95_INT     4    // "C"

#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

bool Armed = false;
bool sent_update = false;

/**
 * Convert int32_t into a float
 */
float convert_int32_float(int32_t inputvalue) {
	float f;
	memcpy(&f, &inputvalue, sizeof(f));
	return f;
}

void send_command(){

    if(!sent_update){
        rf95.waitPacketSent();

        uint16_t time = millis() / 1000;
        uint8_t buff[HEADER_SIZE] = {0};

        if(Armed){
            char flags[4] = {1, 0, 0, 0};
            IRECHYPERP::createCMMNDFrame(buff, flags, time);
        } else {
            char flags[4] = {0, 1, 0, 0};
            IRECHYPERP::createCMMNDFrame(buff, flags, time);      
        }

        rf95.send(buff, HEADER_SIZE);

        sent_update = true;
    }
}

/**
 * Recieve incomming messages then handle them according
 * to which data type they contain.
 */
void handle_incomming_messages(){

	if (rf95.available()){ 

		digitalWrite(LED_BUILTIN, HIGH);

    	uint8_t buff_raw_msg[MAX_MSG_LEN_RECV];

		uint8_t len = sizeof(buff_raw_msg);

    	if (rf95.recv(buff_raw_msg, &len)){
            int type = IRECHYPERP::typeofData(buff_raw_msg);

            if(type == CMMNDt){
                CMMND_Packet packet = IRECHYPERP::unpack_CMMND(buff_raw_msg);

                if(packet.header.flags & 1){
                    Serial.println("PAYLOAD IS !DISARMED!");
                }
                if((packet.header.flags >> 1) & 1){
                    Serial.println("PAYLOAD IS !ARMED!");
                }

                send_command();
                
            } else if(type == INFOt){
                
                Info_Packet packet = IRECHYPERP::unpack_Info(buff_raw_msg);

                Serial.println(packet.data);
            }
    	}
		digitalWrite(LED_BUILTIN, LOW);
  	}
}

/**
 * Init function for the lora.
 */
int init_LoRa(){

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
	  return 1;
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
	  return 1;
  }

  // maximum power
  rf95.setTxPower(23, false);

  return 0;
}

/**
 * Main initilize function for the SysFwdr program.
 */
void init_sys(){
	// Place startup code here
	Serial.begin(BAUD_RATE);
	// Pin mode for led
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	if(init_LoRa()) Serial.println("LORA INIT FAILURE!");
}

int main(){

	init_sys();

	while(true){
		// Place loop code here
        String usr;

		handle_incomming_messages();

        if(Serial.available()){
            usr = Serial.readString();

            if(usr.equals("ARM")){
                Armed = true;
                Serial.println("Arming Payload!");
                sent_update = false;
            }
            if(usr.equals("DISARM")){
                Armed = false;
                Serial.println("Disarming Payload!");
                sent_update = false;
            }
        }
	}
}
