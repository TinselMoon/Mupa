#include <Arduino.h>
#include "ServoController.hpp"
#include "led_rgb.h"

#include <TFT_eSPI.h>
#include <SPI.h>

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

led_rgb LED;

// TFT Screen
#define TFT_GREY 0x5AEB
#define LOOP_PERIOD 35 // Display updates every 35 ms

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 4

TFT_eSPI tft = TFT_eSPI();


int button_press = 0, last_button = 0, option = 0;
int lastOption = 1;

const int colors[] = {VERMELHO, VERDE, AZUL, AMARELO};

ServoController servo_1 = ServoController(27);
ServoController servo_2 = ServoController(14);

TaskHandle_t Task1;

void TaskLed(void *PvParameters);

int readButton(void);

void actHand(void);

void displayOptions(void);

void setup() {
    servo_1.SetupServo();
    servo_2.SetupServo();

    LED.init();

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);

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
                actHand();
                break;

            case 3:
                if((option - 1) < 0){
                    option = 3;
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
    LED.print_color(colors[option]);
    if(lastOption != option){
        displayOptions();
        lastOption = option;
    }
  }
  
}

void displayOptions(void){
    tft.setTextColor(TFT_BLACK,TFT_WHITE);
    switch (option){
    case 0:
        tft.drawCentreString("Mode 2", 240, 60, FONT_SIZE);
        tft.drawCentreString("Mode 3", 80, 180, FONT_SIZE);
        tft.drawCentreString("Mode 4", 240, 180, FONT_SIZE);
        tft.setTextColor(TFT_WHITE,TFT_BLACK);
        tft.drawCentreString("Mode 1", 80, 60, FONT_SIZE);
        break;
    case 1:
        tft.drawCentreString("Mode 1", 80, 60, FONT_SIZE);
        tft.drawCentreString("Mode 3", 80, 180, FONT_SIZE);
        tft.drawCentreString("Mode 4", 240, 180, FONT_SIZE);
        tft.setTextColor(TFT_WHITE,TFT_BLACK);
        tft.drawCentreString("Mode 2", 240, 60, FONT_SIZE);
        break;
    case 2:
        tft.drawCentreString("Mode 1", 80, 60, FONT_SIZE);
        tft.drawCentreString("Mode 2", 240, 60, FONT_SIZE);
        tft.drawCentreString("Mode 4", 240, 180, FONT_SIZE);
        tft.setTextColor(TFT_WHITE,TFT_BLACK);
        tft.drawCentreString("Mode 3", 80, 180, FONT_SIZE);
        break;
    case 3:
        tft.drawCentreString("Mode 1", 80, 60, FONT_SIZE);
        tft.drawCentreString("Mode 2", 240, 60, FONT_SIZE);
        tft.drawCentreString("Mode 3", 80, 180, FONT_SIZE);
        tft.setTextColor(TFT_WHITE,TFT_BLACK);
        tft.drawCentreString("Mode 4", 240, 180, FONT_SIZE);
        break;
    }
}

// Função mover servos
void actHand(void){
    switch(option) {
        // Move apenas servo 1 em 180 graus
        case 0:
            servo_1.control.write(180);
            break;
        // Move apenas servo 2 em 180 graus
        case 1:
            servo_2.control.write(180);
            break;
        // Move os dois servos em 180 graus
        case 2:
            servo_1.control.write(180);
            servo_2.control.write(180);
            break;
        // Reposiciona os dois servos na posição inicial
        case 3:
            servo_1.control.write(0);
            servo_2.control.write(0);
            break;
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