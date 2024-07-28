#pragma once

#include <config.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ServoController.hpp>

// Servo
#define LIM_SUP_DEDAO 180
#define LIM_INF_DEDAO 0
#define LIM_SUP_DEDOS 180
#define LIM_INF_DEDOS 0
#define TIME_SERVOS 1000

#define T_ESPERA 1000


//Interface
#define MAX_STACK_SIZE 6
#define NUM_COLORS 7

ServoController servo_1 = ServoController(27);
ServoController servo_2 = ServoController(14);

class Funcoes{
    private:
        int index = 0;
        int stack[MAX_STACK_SIZE]; 
        int stackSize = 0;
        //int lastbutton = 1;
        int button = 1;
        unsigned long lastDebounceTime = 0;
        unsigned long lastDebounceTimeBotoes = 0;
        const unsigned long debounceDelay = 50;
        
    public:
        Funcoes(int index):index(index){}

        void draw_funcoes(TFT_eSprite &funcoes, int &index, TFT_eSprite &stackSprite);
        void track_position(int &index, int &button);
        void select(int &index, int &button);
        void init_screen(TFT_eSPI &d);
        void execStack(void);

};

void Funcoes::execStack(void){
    int i;
    for(i = 0; i < stackSize; i++){
        switch (stack[i]){
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
}


void Funcoes::init_screen(TFT_eSPI &d){
    d.fillScreen(TFT_WHITE);
    d.setTextColor(TFT_BLUE);
    d.setTextSize(4);
    // Define a cor e o tamanho do texto
    d.setTextDatum(TC_DATUM);
    d.setCursor(50, 90);
    String name = "O PROJETO\n     HAND ";
    for (int i = 0; i < name.length(); i++) {
        d.print(name[i]);
        delay(100);
    }
    delay(1500);
    d.fillScreen(TFT_WHITE);
    
}

void Funcoes::track_position(int &index, int &button){
    switch(button){
        case 1:
                index++;
                if (index > NUM_COLORS) {
                    index = 0;
                }
                break;
        case 2:
                index--;
                if (index < 0) {
                    index = NUM_COLORS;
                }
                break;
    }
}

void Funcoes::select(int &index, int &button) {
    if(button == 3){
            if(index <= 4 && stackSize < MAX_STACK_SIZE){
                stack[stackSize++] = index+1;
            }
            else{
                switch (index){
                    case 5:
                        if (stackSize > 0) {
                            stack[stackSize--] = 0;
                        }
                        break;
                    case 6:
                        stackSize = 0;
                        break;
                    case 7: 
                        //exec stack
                        execStack();
                        break;
                }
            }
    }

}


void Funcoes::draw_funcoes(TFT_eSprite &funcoes, int &index, TFT_eSprite &stackSprite){
    stackSprite.setTextFont(2);
    stackSprite.fillSprite(TFT_WHITE);
    stackSprite.setTextSize(2);

    stackSprite.setCursor(120, 10); 
    stackSprite.println(" Pilha ");
    stackSprite.setTextColor(TFT_BLACK);

    funcoes.setTextFont(2);
    funcoes.fillSprite(TFT_WHITE);
    funcoes.setTextSize(2);

    funcoes.setCursor(0, 10); 
    funcoes.setTextColor(TFT_BLACK);
    if (index == 0) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("1.Abrir mao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("pong");
    } else {
        funcoes.println("1.Abrir mao");
    }

    funcoes.setCursor(0, 40); 
    if (index == 1) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("2.Fechar mao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("2.Fechar mao");
    }

    funcoes.setCursor(0, 70); 
    if (index == 2) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("3.Abrir dedao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("3.Abrir dedao");
    }

    funcoes.setCursor(0, 100); 
     if (index == 3) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("4.Fechar dedao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("4.Fechar dedao");
    }

    funcoes.setCursor(0, 130); 
     if (index == 4) {
        funcoes.setTextColor(TFT_PINK, TFT_BLACK);
        funcoes.println("5.Esperar");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("5.Esperar");
    }

    
    funcoes.setCursor(0, 160); 
     if (index == 5) {
        funcoes.setTextColor(TFT_RED, TFT_BLACK);
        funcoes.println("Remover");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("Remover");
    }

     funcoes.setCursor(0, 190); 
     if (index == 6) {
        funcoes.setTextColor(TFT_GREEN, TFT_BLACK);
        funcoes.println("Executar");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("Executar");
    }
    funcoes.pushSprite(0, 0);

        // Redesenha a pilha na tela
    stackSprite.fillSprite(TFT_WHITE); 
    stackSprite.setCursor(0, 2); // Ajuste o posicionamento conforme necessário
    stackSprite.setTextColor(TFT_BLACK);
    stackSprite.println("Pilha");
    for (int i = 0; i < stackSize; i++) {
        stackSprite.setCursor(220, 20 + i * 25); // Ajuste o espaçamento conforme necessário
        stackSprite.setTextColor(TFT_BLACK);
        stackSprite.println(stack[i]);
    }
    stackSprite.pushSprite(230, 0);

}