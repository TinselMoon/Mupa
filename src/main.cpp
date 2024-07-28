#include <Arduino.h>
#include "ServoController.hpp"
#include "led_rgb.h"
#include "screenController.h"

#include <TFT_eSPI.h>
#include <SPI.h>

// Buttons
/*#define BUTTON_1 33
#define BUTTON_2 25
#define BUTTON_3 26*/

// Led
// Codigo das cores são encontrados a partir da conversão do código hexadecimal para decimal
#define PIN_LED 13
#define NUM_COLORS 7
#define AMARELO 16768256 
#define VERMELHO 16515843
#define VERDE 63240
#define AZUL 49911
#define MAGENTA 16711935
#define CIANO 3407871
#define LARANJA 16093234
#define LIMAO 13434777

// Movimento
/*#define LIM_SUP_DEDAO 180
#define LIM_INF_DEDAO 0
#define LIM_SUP_DEDOS 180
#define LIM_INF_DEDOS 0
#define TIME_SERVOS 1000

#define T_ESPERA 1000*/

led_rgb LED;

/*// TFT Screen
#define TFT_GREY 0x5AEB
#define LOOP_PERIOD 35 // Display updates every 35 ms

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 4*/

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite funcoes = TFT_eSprite(&tft);
TFT_eSprite stack = TFT_eSprite(&tft);

int index_ = 0;

Funcoes funcao(index_);


// Functions stack
//#define MAX_STACK_FUNCS 6
//int funcs[MAX_STACK_FUNCS] = {0}, stack = 0;

// Button
int button_press = 0, last_button = 1, option = 0;
int lastOption = 1;

const int colors[] = {VERMELHO, VERDE, AZUL, AMARELO, MAGENTA, CIANO, LARANJA, LIMAO};

/*ServoController servo_1 = ServoController(27);
ServoController servo_2 = ServoController(14);
*/
TaskHandle_t Task1;

//SemaphoreHandle_t xMutex;

void TaskLed(void *PvParameters);

int readButton(void);

/*void actHand(void);

void displayOptions(void);

void execStack(void);*/

void setup() {
    Serial.begin(115200);
    servo_1.SetupServo();
    servo_2.SetupServo();

    LED.init();

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);

    funcoes.setColorDepth(8);
    funcoes.createSprite(200, 240);

    stack.setColorDepth(8);
    stack.createSprite(70, 240);  
    funcao.init_screen(tft);

    /*pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);*/
    pinMode(botao::button_1, INPUT_PULLUP);
    pinMode(botao::button_2, INPUT_PULLUP);
    pinMode(botao::button_3, INPUT_PULLUP);

    //xMutex = xSemaphoreCreateMutex();

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
    
    /*if((button_press = readButton()) && (button_press != last_button)){
        //while(xSemaphoreTake(xMutex, portMAX_DELAY) != pdTRUE);
        switch (button_press){
            case 1:
                if((option + 1) > NUM_COLORS){
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
                    option = NUM_COLORS;
                }
                else{
                    option--;
                }
                break;
        }
        //xSemaphoreGive(xMutex);
    }
    last_button = button_press;*/
}

void TaskLed(void *PvParameters){
  while (1)
  {
    //while(xSemaphoreTake(xMutex, portMAX_DELAY) != pdTRUE);
    LED.print_color(colors[index_]);
    /*if(lastOption != option){
        displayOptions();
        lastOption = option;
    }*/
    //xSemaphoreGive(xMutex);
  }
}
/*// Teste da tela
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
}*/

/*void execStack(void){
    int i;
    for(i = 0; i < stack; i++){
        switch (funcs[i]){
            //Func abrir mao
            case 1:
                servo_1.control.write(LIM_SUP_DEDAO);
                servo_2.control.write(LIM_SUP_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func fecha mao
            case 2:
                servo_1.control.write(LIM_INF_DEDAO);
                servo_2.control.write(LIM_INF_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func abrir dedao
            case 3:
                servo_2.control.write(LIM_SUP_DEDAO);
                delay(TIME_SERVOS);
                break;
            //Func fechar dedao
            case 4:
                servo_2.control.write(LIM_INF_DEDAO);
                delay(TIME_SERVOS);
                break;
            //Func espera
            case 5:
                delay(T_ESPERA);
                break;
        }
    }
}*/

// Função mover servos
/*void actHand(void){
    if(stack+1 < MAX_STACK_FUNCS && option <= 4){
        funcs[stack++] = option+1;
    }
    else{
        switch (option){
            case 5:
                // Remove ultimo item da stack
                if(stack > 0){
                    stack--;
                }
                break;
            case 6:
                //Limpa as funções da stack
                stack = 0;
                break;
            case 7:
                //executa as funções da stack
                execStack();
                break;
        }
    }
}*/

// Lê se um botão for pressionado
int readButton(void){
    int button_high = 0;
    int tempo = 0;
    if(!digitalRead(botao::button_1) && !button_high){
        tempo = millis();
        button_high = 1;
    }
    if(!digitalRead(botao::button_2) && !button_high){
        tempo = millis();
        button_high = 2;
    }
    if(!digitalRead(botao::button_3) && !button_high){
        tempo = millis();
        button_high = 3;
    }
    while(button_high && (millis() - tempo) < 50){
        if(digitalRead(botao::button_1) && digitalRead(botao::button_2) && digitalRead(botao::button_3)){
            return 0;
        }
    }
    return button_high;
}