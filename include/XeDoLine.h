#ifndef XEDOLINE_H
#define XEDOLINE_H

#include <Arduino.h>

class XeDoLine {
  public:
    XeDoLine(int s1, int s2, int s3, int s4, int s5,
             int m1_in1, int m1_in2, int ena,
             int m2_in1, int m2_in2, int enb,
             int trigPin, int echoPin);

    void batdau();         
    void chay();            
    void dunglai();         

    void caiPID(float kp, float ki, float kd); 
    void tocdoban(int speed);                  
    void banhphaitien(int speed);
    void banhphailui(int speed);
    void banhtraitien(int speed);
    void banhtralui(int speed);
    long dokhoangcach();
    void kiemtravatcan();

  private:
    int _sensors[5];
    int _S_pins[5];
    int _M1_IN1, _M1_IN2, _ENA;
    int _M2_IN1, _M2_IN2, _ENB;
    int _trigPin, _echoPin;

    float _Kp, _Ki, _Kd;
    float _error, _lastError, _integral;
    int _baseSpeed;

    unsigned long _t, _t_prev;
    float _dt;

    void doccam();      
    void PID();         
    void chaydongco(int trai, int phai);
};

#endif
