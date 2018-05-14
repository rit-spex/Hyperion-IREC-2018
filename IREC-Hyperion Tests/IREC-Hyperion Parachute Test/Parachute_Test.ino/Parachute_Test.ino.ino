#define PARA_ANG 14
#define PARA_POP 15
#define ANG_THRES 800
#define LED 13

int triggerCounter = 0;

int sensorValue = 0;

int deltaTime = 0;

int deltaTimePop = 0;

bool trigger = false;

bool switchs = true;

void setup() {

  pinMode(PARA_POP, OUTPUT);
  digitalWrite(PARA_POP, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {

  sensorValue = analogRead(PARA_ANG);

  if(sensorValue > ANG_THRES && trigger == false){
    triggerCounter++;
    delay(1000);

    if(triggerCounter >= 5){
      deltaTime = millis();
      digitalWrite(LED, HIGH);
      trigger = true;
    } 
  }
  
  if(trigger && switchs){
    switchs = false;
    trigger = false;
    delay(10000);
    digitalWrite(PARA_POP, HIGH);
    delay(2000);
    digitalWrite(LED, LOW);
    digitalWrite(PARA_POP, LOW);
  }
}
