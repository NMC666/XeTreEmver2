#include "XeDoLine.h"

XeDoLine::XeDoLine(int s1, int s2, int s3, int s4, int s5,
                   int m1_in1, int m1_in2, int ena,
                   int m2_in1, int m2_in2, int enb , int trig , int echo) {
  _S_pins[0] = s1;
  _S_pins[1] = s2;
  _S_pins[2] = s3;
  _S_pins[3] = s4;
  _S_pins[4] = s5;

  _M1_IN1 = m1_in1; _M1_IN2 = m1_in2; _ENA = ena;
  _M2_IN1 = m2_in1; _M2_IN2 = m2_in2; _ENB = enb;
  _trigPin = trig;
  _echoPin = echo;

  _Kp = 1.0; _Ki = 0.00001; _Kd = 20.0;
  _baseSpeed = 80;
  _lastError = 0; _integral = 0;
  _t_prev = micros();
}

void XeDoLine::batdau() {
  for (int i = 0; i < 5; i++) pinMode(_S_pins[i], INPUT);
  pinMode(_M1_IN1, OUTPUT); pinMode(_M1_IN2, OUTPUT); pinMode(_ENA, OUTPUT);
  pinMode(_M2_IN1, OUTPUT); pinMode(_M2_IN2, OUTPUT); pinMode(_ENB, OUTPUT);
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
  dunglai();
}

void XeDoLine::chay() {
  _t = micros();
  if ((unsigned long)(_t - _t_prev) >= 20000) {
    _dt = (_t - _t_prev) / 1000000.0;
    _t_prev = _t;
    kiemtravatcan();
    doccam();
    PID();
    
  }
}

void XeDoLine::doccam() {
  int vitri = 0, dem = 0;
  for (int i = 0; i < 5; i++) {
    _sensors[i] = digitalRead(_S_pins[i]);
    if (_sensors[i] == 0) {
      vitri += (i - 2) * 100;
      dem++;
    }
  }
  if (dem > 0)
    _error = (float)vitri / dem;
  else
    _error = (_lastError > 0) ? 200 : -200;
}

void XeDoLine::PID() {
  _integral += _error * _dt;
  _integral = constrain(_integral, -500, 500);
  float dao = (_dt > 0) ? (_error - _lastError) / _dt : 0;

  float hieu_chinh = _Kp * _error + _Ki * _integral + _Kd * dao;
  hieu_chinh = constrain(hieu_chinh, -60, 60);

  int tocdo = _baseSpeed + (abs(_error) * 0.3);
  tocdo = constrain(tocdo, 50, _baseSpeed);

  int trai = tocdo - hieu_chinh;
  int phai = tocdo + hieu_chinh;

  trai = constrain(trai, 0, 255);
  phai = constrain(phai, 0, 255);

  chaydongco(trai, phai);
  _lastError = _error;
}

void XeDoLine::chaydongco(int trai, int phai) {
  digitalWrite(_M1_IN1, HIGH);
  digitalWrite(_M1_IN2, LOW);
  analogWrite(_ENA, trai);

  digitalWrite(_M2_IN1, HIGH);
  digitalWrite(_M2_IN2, LOW);
  analogWrite(_ENB, phai);
}

void XeDoLine::dunglai() {
  digitalWrite(_M1_IN1, LOW);
  digitalWrite(_M1_IN2, LOW);
  analogWrite(_ENA, 0);

  digitalWrite(_M2_IN1, LOW);
  digitalWrite(_M2_IN2, LOW);
  analogWrite(_ENB, 0);
}

void XeDoLine::caiPID(float kp, float ki, float kd) {
  _Kp = kp; _Ki = ki; _Kd = kd;
}

void XeDoLine::tocdoban(int speed) {
  _baseSpeed = speed;
}
void XeDoLine::banhphaitien(int speed){
  digitalWrite(_M1_IN1, HIGH);
  analogWrite(_ENA, speed);
}
void XeDoLine::banhphailui(int speed){
  digitalWrite(_M1_IN2, HIGH);
  analogWrite(_ENA, speed);
}void XeDoLine::banhtraitien(int speed){
  digitalWrite(_M2_IN1, HIGH);
  analogWrite(_ENB, speed);
}
void XeDoLine::banhtralui(int speed){
  digitalWrite(_M2_IN2, HIGH);
  analogWrite(_ENB, speed);
} 
long XeDoLine::dokhoangcach() {
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  long duration = pulseIn(_echoPin, HIGH, 20000); 
  long distance = duration * 0.034 / 2;

  return distance; 
}
void XeDoLine::kiemtravatcan() {
  long kc = dokhoangcach();  
  if (kc > 0 && kc < 5) {   
    dunglai();               
  }
}


