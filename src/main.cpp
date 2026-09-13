#include <Arduino.h>
#include "CanMessages.h"

CanManager can;

// Motor 1
#define DC_1_SLEEP PA5 
#define DC_1_IN1 PA6
#define DC_1_IN2 PA7

// Motor 2
#define DC_2_SLEEP PA0
#define DC_2_IN1 PA1
#define DC_2_IN2 PA2

void set_motor(can_frame_t frame) {
    MOTOR msg;
    memcpy(&msg, frame.data, frame.length);
    
    // INIT
    if (msg.launch) {
        digitalWrite(DC_1_IN2, LOW);
        analogWrite(DC_1_IN1, 255);
        delay(3000);
        
        analogWrite(DC_1_IN1, 0);
        digitalWrite(DC_1_IN1, LOW);
        delay(1000);
        
        digitalWrite(DC_2_IN2, LOW);
        analogWrite(DC_2_IN1, 255);
        delay(3000);
        
        analogWrite(DC_2_IN1, 0);
        digitalWrite(DC_2_IN1, LOW);
        return;
    }
    
    int abs_speed = abs(msg.speed);
    if (abs_speed > 255) {
        abs_speed = 255;
    }

    // RIGHT
    if (msg.motor_id == BOTH || msg.motor_id == RIGHT) {
        if (msg.speed > 0) { // Marche avant
            digitalWrite(DC_1_IN2, LOW);         
            analogWrite(DC_1_IN1, abs_speed);
        } else if (msg.speed < 0) { // Marche arrière
            digitalWrite(DC_1_IN1, LOW);
            analogWrite(DC_1_IN2, abs_speed);
        } else { // Arrêt
            analogWrite(DC_1_IN1, 0);
            analogWrite(DC_1_IN2, 0);
            digitalWrite(DC_1_IN1, LOW);
            digitalWrite(DC_1_IN2, LOW);
        }
    }
    
    // LEFT
    if (msg.motor_id == BOTH || msg.motor_id == LEFT) {
        if (msg.speed > 0) { 
            digitalWrite(DC_2_IN2, LOW);         
            analogWrite(DC_2_IN1, abs_speed);
        } else if (msg.speed < 0) { 
            digitalWrite(DC_2_IN1, LOW);
            analogWrite(DC_2_IN2, abs_speed);
        } else { 
            analogWrite(DC_2_IN1, 0);
            analogWrite(DC_2_IN2, 0);
            digitalWrite(DC_2_IN1, LOW);
            digitalWrite(DC_2_IN2, LOW);
        }
    }
}

void setup() {
    // Motor 1 (RIGHT)
    pinMode(DC_1_SLEEP, OUTPUT);
    pinMode(DC_1_IN1, OUTPUT);
    pinMode(DC_1_IN2, OUTPUT);
    
    digitalWrite(DC_1_SLEEP, HIGH);
    digitalWrite(DC_1_IN1, LOW);
    digitalWrite(DC_1_IN2, LOW);

    // Motor 2 (LEFT)
    pinMode(DC_2_SLEEP, OUTPUT);
    pinMode(DC_2_IN1, OUTPUT);
    pinMode(DC_2_IN2, OUTPUT);
    
    digitalWrite(DC_2_SLEEP, HIGH); 
    digitalWrite(DC_2_IN1, LOW);
    digitalWrite(DC_2_IN2, LOW);

    can.init();
    can.onReceive(MOTOR::ID, set_motor);
}

void loop() {
    can.update();
}
