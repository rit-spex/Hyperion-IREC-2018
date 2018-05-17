/**
 * Hyperion - IREC
 * File: SysFwdr.cpp
 * Organization: RIT Space Exploration
 * Desc
 *    	Main program used to forward packets from LoRa Module to 
 *		desktop
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

/**
 * Convert int32_t into a float
 */
float convert_int32_float(int32_t inputvalue) {
	float f;
	memcpy(&f, &inputvalue, sizeof(f));
	return f;
}

/**
 * Create the header section of the data string to be sent over
 * a Serial connection.
 * Args:
 * 		buff: the source string
 * 		hdr: the header struct
 * Format:
 * 		[DataType],[F1],[F2],[F3],[F4],[Time]
 * Return:
 * 		The length of the compiled string
 */
int write_header(char* buff, unpk_header hdr){

	char temp[10] = {'\0'};
	
	itoa(hdr.data_type, temp, 10);
	strcat(buff, temp);

	for(int i = 0; i < 4; i++){
		strcat(buff, ",");
		if((hdr.flags >> 3-i) & 1){
			strcat(buff, "1");
		} else {
			strcat(buff, "0");
		}
	}

	strcat(buff, ",");
	itoa(hdr.time, temp, 10);
	strcat(buff, temp);

	return strlen(buff);
}

/**
 * Individual csv converter from LSM9DS1 packet to data string
 * Args:
 * 		buff: the source string
 * 		data: raw data struct
 * Format:
 * 		[Header],[ax],...,[gx],...,[mx]
 * Returns:
 * 		The length of the string constructed
 */
int CtCSV_LSM9DS1(char* buff, LSM9DS1_Packet data){
	
	LSM9DS1_Data data_frame = data.data;

	write_header(buff, data.header);

	int32_t temp_arry[9] = {data_frame.ax, data_frame.ay, data_frame.az,
							data_frame.gx, data_frame.gy, data_frame.gz,
							data_frame.mx, data_frame.my, data_frame.mz};
	
	char temp[10] = {'\0'};

	for(int i = 0; i < 10; i++){
		strcat(buff, ",");
		float res = convert_int32_float(temp_arry[i]);
		dtostrf(res, 1, 3, temp);
		strcat(buff, temp);
	}

	return strlen(buff);
}

/**
 * Construct a string to be sent over serial for the BME280
 * Args:
 * 		buff: the string to be constructed
 * 		data: raw data structure
 * Format:
 * 		[Header],[temp],[press],[hum],[alt]
 * Return:
 * 		The size of the constructed string
 */
int CtCSV_BME280(char* buff, BME280_Packet data){

	BME280_Data data_frame = data.data;

	write_header(buff, data.header);

	int32_t temp_arry[4] = {data_frame.temperature, data_frame.pressure,
							data_frame.humidity, data_frame.altitude};
	
	char temp[10] = {'\0'};

	for(int i = 0; i < 4; i++){
		strcat(buff, ",");
		float res = convert_int32_float(temp_arry[i]);
		dtostrf(res, 1, 3, temp);
		strcat(buff, temp);
	}

	return strlen(buff);
}

int CtCSV_CCS811(char* buff, CCS811_Packet data);
int CtCSV_LIS331(char* buff, LIS311_Packet data);
int CtCSV_PFSL(char* buff, PFSL_Packet data);

/**
 * Grabs source from transmission, unpacks data then 
 * converts packed data a comma seperated string.
 * CSV format: 
 * [type],[flag1(1,0)],[flag2(1,0)],[flag3(1,0)],[flag4(1,0)],[time(seconds)],[data],[data]...
 * Arguments
 * 		buff, the buffer which the csv string will be placed.
 * 		source, the raw bytes from the payload.
 * Return
 * 		The number of bytes within the buff
 */
int convert_to_csv(char* buff, const uint8_t* source){

	switch(IRECHYPERP::typeofData(source)){
		case NULLDATAt:
		{
			//No data
		}
		break;
		case LSM9DS1t:
		{
			LSM9DS1_Packet packet = IRECHYPERP::unpack_LSM9DS1(source);
			CtCSV_LSM9DS1(buff, packet);
		}
		break;
		case BME280t:
		{
			BME280_Packet packet = IRECHYPERP::unpack_BME280(source);
			CtCSV_BME280(buff, packet);
		}
		break;
		case CCS811t:
		{
			CCS811_Packet packet = IRECHYPERP::unpack_CCS811(source);
			//CtCSV_CCS811(buff, packet);
		}
		break;
		case LIS331t:
		break;
		case PFSLt:
		break;
		case Orent:
		break;
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
		char buff_strng[INITAL_STR_SIZE] = {'\0'};

		uint8_t len = sizeof(buff_raw_msg);

    	if (rf95.recv(buff_raw_msg, &len)){
			convert_to_csv(buff_strng, buff_raw_msg);
			Serial.println(buff_strng);
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

	init_LoRa();
}

int main(){

	init_sys();

	while(true){
		// Place loop code here
		handle_incomming_messages();
	}
}
