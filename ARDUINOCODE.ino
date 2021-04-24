#include <Servo.h>

Servo valve;
Servo pitcher;
Servo relay;
int shotCount = 0;

void setup() {
  Serial.begin(9600);

  pinMode(10, INPUT);
  
  // TO CONSOLIDATED BOARD
  // pin 6: valve
  // pin 5: pitcher
  // pin 3: relay
  // pin 4: back end stop
  // pin 2: front endstop
  
  valve.attach(6);
  pitcher.attach(5);
  relay.attach(3);
  pinMode(4, INPUT);
  pinMode(2, INPUT);

  // FROM DECOUPLER
  // pin 8: stop pitching
  // pin 10: pitchup
  // pin 12: pitchdown
  // pin A3: reload

  pinMode(8, INPUT);
  pinMode(10, INPUT);
  pinMode(12, INPUT);
  valve.write(120);
}

void pitchup(){
  if (!digitalRead(4)){
    stoppitch();
  }else{
    pitcher.write(120);
  }
}

void pitchdown(){
  if (!digitalRead(2)){
    stoppitch();
  }else{
    pitcher.write(60);
  }
}

void stoppitch(){
  pitcher.write(90);
}

void feed(){
  valve.write(50);
  delay(750);
  valve.write(120);
}

void reload(){
  pitcher.write(135);
  while (digitalRead(4)){
    continue;
  }
  Serial.println("foo");
  stoppitch();
  delay(100);
  pitcher.write(30);
  delay(1100);
  stoppitch();
  feed();
}

void fire(){
  if (shotCount == 0){
    charge();
    delay(20000);
    discharge();
    delay(150);
    neutral();
  } else{
    charge();
    delay(10000);
    discharge();
    delay(150);
    neutral();
  }
  shotCount += 1;
  Serial.println(shotCount);
  reload();
}

void discharge(){
  relay.write(130);
}

void charge(){
  relay.write(65);
}

void neutral(){
  relay.write(90);
}

void loop() {
  if (digitalRead(8)){
    Serial.println("stop");
    Serial.println(A0);
    stoppitch();
  }else if (digitalRead(10)){
    pitchup();
    Serial.println("up");
    Serial.println(A1);
  }else if (digitalRead(12)){
    pitchdown();
    Serial.println("down");
    Serial.println(A2);
  }else if (analogRead(A3) > 500){
    fire();
    Serial.println("fire");
    Serial.println(A3);
  }
}
