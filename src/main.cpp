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
    MOTEUR msg;
    memcpy(&msg, frame.data, frame.length);
    
    uint32_t pin_in1;
    uint32_t pin_in2;

    if (msg.motor_id == RIGHT) {
        pin_in1 = DC_1_IN1;
        pin_in2 = DC_1_IN2;
    } else {
        pin_in1 = DC_2_IN1;
        pin_in2 = DC_2_IN2;
    }

    if (msg.speed == 0) {
        analogWrite(pin_in1, 0);
        analogWrite(pin_in2, 0);
        digitalWrite(pin_in1, LOW);
        digitalWrite(pin_in2, LOW);
        return;
    }

    if (msg.etat) {
        digitalWrite(pin_in2, LOW);         
        analogWrite(pin_in1, msg.speed);
    } 
    else {
        digitalWrite(pin_in1, LOW);
        analogWrite(pin_in2, msg.speed);
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
    can.onReceive(MOTEUR::ID, set_motor);
}

void loop() {
    can.update();
}