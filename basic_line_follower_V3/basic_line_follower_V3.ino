#include<EEPROM.h>
#include<Servo.h>
#include<NewPing.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);

//for servo control
Servo handle;
Servo grab;
int hpos, gpos;

//for sonar sensor
#define trf 32
#define ecf 34
#define trr 28
#define ecr 30
#define trl 24
#define ecl 26
int sf = 0, sl = 0, sr = 0;
int object_boundary = 15, wall_boundary = 15, midpoint = 12, turn = 400;

NewPing sonarl(trl, ecl, 90);
NewPing sonarf(trf, ecf, 90);
NewPing sonarr(trr, ecr, 90);

// for motor driver
#define pwma 2
#define pwmb 7
#define n1 4
#define n2 3
#define n3 6
#define n4 5
#define switchin 51
#define switchout 53

//for line follow
int spl = 25;
int spr = 22;
int btd = 60;
int mtd = 200;
#define dt 10
#define br 80
int k = 0;
int base = 40;
int peak = 160;
int cl = base;
int d = 2;
int brake = cl / d;
int counter = 0;
int tcount = 0;
int mode = 1;

//for 30degeree & 90 degree && other logic
int k30 = 0;
int k90 = 0;
int mov = 0;
int cross = 0;
unsigned long mi1 = 0, mi2 = 0, mi3 = 0, mi4 = 0, m81 = 0, m82 = 0 ;


//for ir sensor logic
int bin = 0;
int sum = 0;
int sensor[] = { 0, 0, 0, 0, 0, 0};
int bina[] = {1, 2, 4, 8, 16, 32 };

//for calibration & store
#define calout 23
#define calin 25
int minimum[] = {1024, 1024, 1024, 1024, 1024, 1024};
int maximum[] = {0, 0, 0, 0, 0, 0};
int trash[] = {0, 0, 0, 0, 0, 0};
float cal_p = 0.5;

//for debug light
#define d30 47
#define d90 43

// for motor driver

void mos(int a , int b) {
  if (a >= 0) {

    digitalWrite(n1, HIGH);
    digitalWrite(n2, LOW);
  }
  else if (a < 0) {
    a = -a;
    digitalWrite(n1, LOW);
    digitalWrite(n2, HIGH);
  }
  if (b >= 0) {

    digitalWrite(n3, HIGH);
    digitalWrite(n4, LOW);
  }
  else if (b < 0) {
    b = -b;
    digitalWrite(n3, LOW);
    digitalWrite(n4, HIGH);
  }
  analogWrite(pwma, a);
  analogWrite(pwmb, b);
  return ;

}
void motorSpeed(int speedA, int speedB) {
  digitalWrite(n1, HIGH);
  digitalWrite(n3, HIGH);
  digitalWrite(n2, LOW);
  digitalWrite(n4, LOW);
  analogWrite(pwma, speedA);
  analogWrite(pwmb, speedB);
  return ;
}

void motorSpeedL(int speedA, int speedB) {
  if (speedA >= 80 && speedB >= 80) {
    analogWrite(pwma, speedA);
    analogWrite(pwmb, speedB);
  }
  else {
    analogWrite(pwma, 150);
    analogWrite(pwmb, 150);
  }
  digitalWrite(n1, HIGH);
  digitalWrite(n2, LOW);
  digitalWrite(n3, LOW);
  digitalWrite(n4, HIGH);
  return ;
}

void motorSpeedR(int speedA, int speedB) {
  if (speedA >= 80 && speedB >= 80) {
    analogWrite(pwma, speedA);
    analogWrite(pwmb, speedB);
  }
  else {
    analogWrite(pwma, 150);
    analogWrite(pwmb, 150);
  }
  digitalWrite(n1, LOW);
  digitalWrite(n2, HIGH);
  digitalWrite(n3, HIGH);
  digitalWrite(n4, LOW);
  return ;
}
void motorSpeedB(int speedA, int speedB) {
  digitalWrite(n1, LOW);
  digitalWrite(n3, LOW);
  digitalWrite(n2, HIGH);
  digitalWrite(n4, HIGH);
  analogWrite(pwma, speedA);
  analogWrite(pwmb, speedB);
}

void motorSpeedS() {
  digitalWrite(n2, LOW);
  digitalWrite(n3, LOW);
  digitalWrite(n1, LOW);
  digitalWrite(n4, LOW);
  analogWrite(pwma, 0);
  analogWrite(pwmb, 0);
  return ;
}




void cal() {
  digitalWrite(calout, HIGH);
  motorSpeedR(250, 250);
  for (int c = 0; c < 10000 ; c++)
    for (int i = 0; i < 6; i++) {
      sensor[i] = analogRead(i + 10);
      maximum[i] = max(maximum[i], sensor[i]);
      minimum[i] = min(minimum[i], sensor[i]);
    }
  motorSpeed(0, 0);
  delay(500);
  for (int i = 0; i < 6; i++) trash[i] = ( maximum[i] + minimum[i]) * cal_p;
  for (int i = 0; i < 6; i++) {
    EEPROM.write(i, trash[i] / 5 );
    delay(10);
  }
  digitalWrite(calout, LOW);
  return;
}






void setup()
{
  TCCR3B = 3;
  TCCR4B = 3;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  //  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  pinMode(calin, INPUT);
  pinMode(calout, OUTPUT);
  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(n1, OUTPUT);
  pinMode(n2, OUTPUT);
  pinMode(n3, OUTPUT);
  pinMode(n4, OUTPUT);
  pinMode(d30 , OUTPUT);
  pinMode(d90 , OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(switchout , OUTPUT);
  pinMode(switchin , INPUT);
  digitalWrite(switchout, HIGH);
  digitalWrite(13, LOW);

  for (int i = 0; i < 6; i++) trash[i] = EEPROM.read(i) * 5;

  spl = EEPROM.read(6);
  spr = EEPROM.read(7);
  hpos = EEPROM.read(8);
  gpos = EEPROM.read(9);
  counter = EEPROM.read(10);
  btd = EEPROM.read(11);
  mtd = EEPROM.read(12);
  d = EEPROM.read(13);
  base = EEPROM.read(14) * d;
  peak = EEPROM.read(15) * d;
  cl = base;
  brake = cl / d;
  object_boundary = EEPROM.read(16);
  wall_boundary = EEPROM.read(17);
  midpoint = EEPROM.read(18);
  cal_p = EEPROM.read(19) * 0.1;

  //  handle.attach(8);                         //this is for when grabber was attached to bot
  //  grab.attach(9);
  //  grab.write(140);
  //  handle.write(0);
  //  delay(1500);
  //  grab.write(gpos);
  //  delay(500);
  //  handle.write(hpos);

  int p = map(analogRead(8), 740, 1023, 0, 100);
  if (p <= 40) {
    while (digitalRead(calin) == LOW && digitalRead(switchin) == LOW) text_battery_low();
    while (digitalRead(calin) == HIGH || digitalRead(switchin) == HIGH);
  }
  sust_cracker_nut();

  Serial.begin(38400);
}


// for ir sensor function

void check()
{
  sum = 0;
  bin = 0;
  //for black line(1)-- white surface(0)
  for (int i = 0; i < 6; i++) {
    sensor[i] = analogRead(i + 10);
    if (mode == 1) (sensor[i] > trash[i]) ? sensor[i] = 1 : sensor[i] = 0 ;
    else if (mode == 0) (sensor[i] < trash[i]) ? sensor[i] = 1 : sensor[i] = 0 ;
    sum += sensor[i];
    bin += sensor[i] * bina[i];
  }

  //  debug();

  return;
}

void sonar() {
  sl = sonarl.ping_cm();
  sf = sonarf.ping_cm();
  sr = sonarr.ping_cm();
}

void debug() {
  for (int i = 5; i >= 0 ; i--) {
    Serial.print(sensor[i]);
  }
  Serial.print("  ");
  Serial.print(mov);
  Serial.print("  ");
  Serial.print(sl);
  Serial.print("  ");
  Serial.print(sf);
  Serial.print("  ");
  Serial.println(sr);
}




void loop()
{
  int r = 0;
  int t = 0;
  while (r == 0) {
    r = menu_function1();
    if (r != 0) {
      if (r == 1) {
        delay(1000);
        line_follow();
        motorSpeedS();
      }
      else if (r == 2) {
        text_callibrating();
        for (int i = 0; i < 6; i++) trash[i] = EEPROM.read(i) * 5;
      }
      else if (r == 3) {
        while (digitalRead(calin) == LOW) analog_reading_display();
      }
      else if (r == 4) {
        while (digitalRead(calin) == LOW) digital_reading_display();
      }
      else if (r == 5) {
        while (digitalRead(calin) == LOW) sonar_reading_display();
      }
      else if (r == 6) {
        display.clearDisplay();
        text("STARTING..", 04, 24);
        display.display();
        delay(500);
        while (digitalRead(calin) == LOW) mos(10 * spr, 10 * spl);
        while (digitalRead(calin) == HIGH)mos(0, 0);
      }
      sust_cracker_nut();
      delay(500);
    }

    r = menu_function2();
    if (r != 0) {
      if (r == 1) counter_adjust();
      else if (r == 2) speed_adjust();
      else if (r == 3) braking_adjust();
      else if (r == 4) {
        display.clearDisplay();
        text("ACTIVATED!", 8, 24);
        display.display();
        remote_control();
      }
      sust_cracker_nut();
      delay(500);
    }

    t++;
    delay(1);
    if (t > 3000) {
      text_battery_check();
      for (int i = 0; i < 500 && digitalRead(calin) == LOW && digitalRead(switchin) == LOW ; i++) delay(1);
      if (digitalRead(calin) == LOW && digitalRead(switchin) == LOW) sust_cracker_nut();
      break;
    } //for showing battery level after every 3 second
  } //while loop

  //    sl = sonarl.ping_cm();
  //  sf = sonarf.ping_cm();
  //  sr = sonarr.ping_cm();
  //    check();
  //    if(sl>5 && sl<20) digitalWrite(calout, HIGH);
  //  if(sf>2 && sf<20) digitalWrite(calout, HIGH);
  //  if(sr>5 && sr<20) digitalWrite(calout, HIGH);
}
