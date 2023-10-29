#include <SoftwareSerial.h>        // 블루투스 시리얼 통신 라이브러리 추가
#define TRIG 6
#define ECHO 7

int motor1pin1 = 4; // 오른쪽 바퀴
int motor1pin2 = 5;
int speedmotor1 = 11; // 속도 제어

int motor2pin1 = 12; // 왼쪽 바퀴
int motor2pin2 = 13;
int speedmotor2 = 10; // 속도 제어

int left_led = 2;   // 좌측 방향지시등
int right_led = 3;  // 우측 방향지시등
int light_led = 8;     // 라이트

char mode;

void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);  //bluetooth 통신을 위한 Serial1 선언
  
  pinMode(motor1pin1, OUTPUT);  
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(speedmotor1, OUTPUT);
  pinMode(speedmotor2, OUTPUT);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(left_led, OUTPUT);
  pinMode(right_led, OUTPUT);
  pinMode(light_led, OUTPUT);
}

void loop() {
  long duration, distance;

  // 초음파 센서 동작
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn (ECHO, HIGH);  //초음파의 왕복 시간 측정
  distance = duration * 17 / 1000;  //시간을 거리로 바꾸기 위한 연산

/*  Serial.print("\nDistance : ");
  Serial.print(distance);
  Serial.println(" Cm");
*/
  // 블루투스 모듈 작동 확인
  if (Serial1.available()) {

    Serial.write(mode = Serial1.read()); // 앱 버튼 값 받아오기
     Serial.print(" ");
    Serial.println(mode);  //mode값 확인
   
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }


  // 조도 센서 측정
  int cds = analogRead(A1);  
  Serial.println(cds);  
  // 라이트 밝기 조절   
  int ledoutput = map(cds, 180, 875, 0, 255); //180~875까지 나오는 조도센서 값을 0~255사이로 맵핑
  if(cds > 300){ //조도센서 값이 300 이상일때 (어두울때)
  analogWrite(light_led, ledoutput);  //어두움의 정도에 따라 LED의 밝기 조절
  }
  else 
{
  analogWrite(light_led, 0); //일정 이상 밝을 때 LED OFF
   }

 
  if(mode == '4') {  // 좌측 방향지시등 작동
    analogWrite(left_led, 120);
    delay(1000);
    analogWrite(left_led, 0);
    delay(1000);
    analogWrite(left_led, 120);
    delay(1000);
    analogWrite(left_led, 0);
    delay(1000);
    analogWrite(left_led, 120);
    delay(1000);
    analogWrite(left_led, 0);
    delay(1000);
    analogWrite(left_led, 120);
    delay(1000);
    analogWrite(left_led, 0);
    mode =0;
  }
  else if(mode == '6'){  // 우측 방향지시등 작동
    analogWrite(right_led, 120);
    delay(1000);
    analogWrite(right_led, 0);
    delay(1000);
    analogWrite(right_led, 120);
    delay(1000);
    analogWrite(right_led, 0);
    delay(1000);
    analogWrite(right_led, 120);
    delay(1000);
    analogWrite(right_led, 0);
    delay(1000);
    analogWrite(right_led, 120);
    delay(1000);
    analogWrite(right_led, 0);
    mode=0;
  }

  // 모터 동작
  // 앞에 물체가 있는 경우(급정지) -> 후진 또는 정지만 동작
  if(distance < 15)
  {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    delay(100);

    // 후진 동작
    if(mode == '8'){ // 후진 => 직진
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(speedmotor1, 150);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(speedmotor2, 150);

    delay(200);
    }
    else if(mode == '7') { // 후진 => 좌회전(오른쪽바퀴 후진, 왼쪽바퀴 전진)
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(speedmotor1, 100);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(speedmotor2, 150);
    delay(200);
    }
    else if(mode == '9') { // 후진 => 우회전(오른쪽바퀴 전진, 왼쪽바퀴 후진)
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(speedmotor1, 150);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(speedmotor2, 100);
    delay(200);      
    }

    // 정지
    else if(mode == '5'){ 
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);

      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
      delay(200);  
    }      
  }
  else{ // distance가 15 이상일 경우
    // 전진 동작
    if(mode == '2'){ // 전진 => 직진
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(speedmotor1, 150);
   
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(speedmotor2, 150);
    delay(200);
    }
    else if(mode == '1') { // 전진 => 좌회전(오른쪽바퀴 전진, 왼쪽바퀴 후진)
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(speedmotor1, 100);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(speedmotor2, 150);

    delay(200);
    }
    else if(mode == '3') { // 전진 => 우회전(오른쪽바퀴 후진, 왼쪽바퀴 전진)
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(speedmotor1, 150);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(speedmotor2, 100);

    delay(200); 
    }

    // 후진 동작
    else if(mode == '8'){ // 후진 => 직진
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(speedmotor1, 150);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(speedmotor2, 170);

    delay(200);
    }
    else if(mode == '7') { // 후진 => 좌회전(오른쪽바퀴 후진, 왼쪽바퀴 전진)
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(speedmotor1, 100);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(speedmotor2, 150);
    delay(200);
    }
    else if(mode == '9') { // 후진 => 우회전(오른쪽바퀴 전진, 왼쪽바퀴 후진)
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(speedmotor1, 150);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(speedmotor2, 100);
    delay(200);      
    }

    // 정지
    else if(mode == '5'){ 
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);

      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
      delay(200);  
    }
}
}
