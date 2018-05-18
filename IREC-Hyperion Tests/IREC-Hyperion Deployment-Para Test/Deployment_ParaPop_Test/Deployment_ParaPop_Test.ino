
#define DEPLOY_SWITCH_01 25 
#define DEPLOY_SWITCH_02 26 
#define DEPLOY_SWITCH_03 27 
#define DEPLOY_SWITCH_04 28 

#define EMATCH_1_ARM    23
#define EMATCH_1_FIRE   21

#define DEPLOYMENT_ERROR_SPEED -20 // -20 m/s

#define SWITCH_DEBUFF 5000

#define PARA_BLAST_TIME 2000

uint32_t deployment_time = 0;
bool parachute_deploy = false;

uint32_t cnt = 0;
bool toggle = false;

float get_rate_of_climb(){
  return 0;
}

bool correct_orientation_para(){
  return true;
}

/**
 * Arms Parachute on payload deployment
 */
void arm_parachute(){

  digitalWriteFast(EMATCH_1_ARM, HIGH);
}

/**
 * Set deployment_time and arm subsystems.
 */
void set_deployment(){

  arm_parachute();
  // arm impact dampers TODO

  deployment_time = millis();
}

bool R_check_deployment(){

  int open_cnt = 0;
  static unsigned int switch_debuff = 0;

  if(digitalReadFast(DEPLOY_SWITCH_01) == HIGH) open_cnt += 1;
  if(digitalReadFast(DEPLOY_SWITCH_02) == HIGH) open_cnt += 1;
  if(digitalReadFast(DEPLOY_SWITCH_03) == HIGH) open_cnt += 1;
  if(digitalReadFast(DEPLOY_SWITCH_04) == HIGH) open_cnt += 1;

  if(open_cnt > 2){
    switch_debuff += 1;
    
  } else if (open_cnt == 2){
    // Anomaly case where 2 switches are open and 2 switches are still
    // closed.
    if (get_rate_of_climb() < DEPLOYMENT_ERROR_SPEED) switch_debuff += 1;
    else switch_debuff = 0;
  } else {
    switch_debuff = 0;
  }

  if(switch_debuff >= SWITCH_DEBUFF){
    set_deployment(); // Set time deployed
    return true;
  }

  return false;
}

void R_deploy_parachute(){

  static uint32_t deployed_time_para = 0;

  if(!deployed_time_para){
    // Fire the two ematches
    digitalWriteFast(EMATCH_1_FIRE, HIGH);
    deployed_time_para = millis();
  }

  if(millis() - deployed_time_para >= PARA_BLAST_TIME){
    // Return the fire pins to low.
    digitalWriteFast(EMATCH_1_FIRE, LOW);
    return;
  }
}

void setup() {
  // put your setup code here, to run once:

  pinMode(DEPLOY_SWITCH_01, INPUT);
  pinMode(DEPLOY_SWITCH_02, INPUT);
  pinMode(DEPLOY_SWITCH_03, INPUT);
  pinMode(DEPLOY_SWITCH_04, INPUT);

  pinMode(EMATCH_1_ARM, OUTPUT);
  digitalWriteFast(EMATCH_1_ARM, LOW);
  pinMode(EMATCH_1_FIRE, OUTPUT);
  digitalWriteFast(EMATCH_1_FIRE, LOW);

  pinMode(22, OUTPUT);
  digitalWriteFast(22, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(cnt > 45000){
    cnt = 0;
    if(toggle){
      toggle = false;
      digitalWriteFast(22, HIGH);
    } else {
       toggle = true;
      digitalWriteFast(22, LOW);     
    }
  }

  cnt += 1;
  
  if(R_check_deployment()){
    delay(4000);
    R_deploy_parachute();
  }
}
