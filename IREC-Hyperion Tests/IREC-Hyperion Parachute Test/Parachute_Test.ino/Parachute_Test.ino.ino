#define PARA_ANG 14
#define PARA_POP 15
#define ANG_THRES 800
#define LED 13

int sensorValue = 0;

int deltaTime = 0;

int deltaTimePop = 0;

bool trigger = false;

bool switchs = true;

void setup() {

  pinMode(PARA_POP, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {

  sensorValue = analogRead(PARA_ANG);

  if(sensorValue > ANG_THRES && trigger == false){
    deltaTime = millis();
    trigger = true;
  }
  
  if(millis()-deltaTime > 10000 && trigger && millis()-deltaTimePop < 2000){
    digitalWrite(PARA_POP, HIGH);
    
    if(switchs){
      deltaTimePop = millis();
      switchs = false;
    }
  }
  
  if(millis()-deltaTimePop < 2000 && !switchs){
    digitalWrite(LED, HIGH);
  }
}
