 #include <Servo.h>


Servo servo;
#define SERVO_PIN 8
char ctrlCode = 0; 
#define STEP_SIZE 9
int curPos = 90;

void turnLeft();
void turnStraight();
void turnRight();

typedef struct {
  char ctrl_code;
  void* execute;
} Action;

Action actions[] = {
  {'l', &turnLeft},
  {'r', &turnRight},
  {'s', &turnStraight},
};

void updateMotor() {
  servo.write(curPos);
}

void turnLeft() {
  if(curPos >= 180 - STEP_SIZE)
    return;
    
  curPos += STEP_SIZE;
  updateMotor();
}

void turnRight() {
  if(curPos <= STEP_SIZE)
    return;
    
  curPos -= STEP_SIZE;
  updateMotor();
}

void turnStraight() {
  curPos = 90;
  updateMotor();
}

void on_action_recv(char ctrl_code) {
  for(int i = 0; i < sizeof(actions)/sizeof(Action); i++) {
    if(ctrl_code == actions[i].ctrl_code) {
      reinterpret_cast<void(*)()>(actions[i].execute)();
    }
  }
}

void setup()
{
    delay(3000);
    Serial.begin(9600);                      
    pinMode(SERVO_PIN, OUTPUT);  
    servo.attach(SERVO_PIN);
}

void loop()
{
   if(Serial.available() > 0)      
   {
      ctrlCode = Serial.read();      
      on_action_recv(ctrlCode);
   }
}

