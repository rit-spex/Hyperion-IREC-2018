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
  // put your setup code here, to run once:
  pinMode(PARA_POP, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(PARA_ANG);

  if(sensorValue > ANG_THRES && trigger == false){
    deltaTime = millis();
    trigger = true;
  }
  
  if(millis()-deltaTime > 10000 && trigger && deltaTimePop-millis() < 2000){
    digitalWrite(PARA_POP, HIGH);
    
    if(switchs){
      deltaTimePop = millis();
      switchs = false;
    }
  }
  
  if(trigger){
    digitalWrite(LED, HIGH);
  }
}
