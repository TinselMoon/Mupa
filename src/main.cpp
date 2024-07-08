#include <Arduino.h>
#include "ServoController.hpp"
#include "led_rgb.h"


// Buttons
#define BUTTON_1 33
#define BUTTON_2 25
#define BUTTON_3 26

// Led
#define PIN_LED 13
#define AMARELO 16768256 
#define VERMELHO 16515843
#define VERDE 63240
#define AZUL 49911
led_rgb LED ;

int button_press = 0, last_button = 0, option = 0;

const int colors[] = {VERMELHO, VERDE, AZUL, AMARELO};

ServoController servo_1 = ServoController(27);
ServoController servo_2 = ServoController(14);

TaskHandle_t Task1;

void TaskLed(void *PvParameters);

int readButton(void);

void actHand(int option);

void setup() {
    servo_1.SetupServo();
    servo_2.SetupServo();

    LED.init();

    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);

    xTaskCreatePinnedToCore(
                    TaskLed,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */
}

void loop() {
    if((button_press = readButton()) && (button_press != last_button)){
        switch (button_press){
            case 1:
                if((option + 1) > 3){
                    option = 0;
                }
                else{
                    option++;
                }
                break;

            case 2:
                actHand(option);
                break;

            case 3:
                if((option - 1) < 0){
                    option = 2;
                }
                else{
                    option--;
                }
                break;
        }
    }
    last_button = button_press;
}

void TaskLed(void *PvParameters){

  while (1)
  {
    LED.latch(100,colors[option]);
  }
  
}

// Função mover servos
void actHand(int option){
    // Move apenas servo 1 em 180 graus
    if(option == 0){
        servo_1.control.write(180);
        delay(1000);
    }
    // Move apenas servo 2 em 180 graus
    else if(option == 1){
        servo_2.control.write(180);
        delay(1000);
    }
    // Move os dois servos em 180 graus
    else if (option == 2){
        servo_1.control.write(180);
        servo_2.control.write(180);
        delay(1000);
    }
    // Reposiciona os dois servos na posição inicial
    else{
        servo_1.control.write(0);
        servo_2.control.write(0);
        delay(1000);
    }
}

// Lê se um botão for pressionado
int readButton(void){
    int button_high = 0;
    int tempo = 0;
    if(!digitalRead(BUTTON_1) && !button_high){
        tempo = millis();
        button_high = 1;
    }
    if(!digitalRead(BUTTON_2) && !button_high){
        tempo = millis();
        button_high = 2;
    }
    if(!digitalRead(BUTTON_3) && !button_high){
        tempo = millis();
        button_high = 3;
    }
    while(button_high && (millis() - tempo) < 50){
        if(digitalRead(BUTTON_1) && digitalRead(BUTTON_2) && digitalRead(BUTTON_3)){
            return 0;
        }
    }
    return button_high;
}