#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

class ServoController
{
private:



public:

  Servo control;
  int servo_pin;

  ServoController(int _servo_pin){
    
    servo_pin = _servo_pin;

  }  
  void SetupServo() {

    //Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    control.setPeriodHertz(50);    // standard 50 hz servo
    control.attach(servo_pin);  
  }
};