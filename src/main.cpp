#include <Arduino.h>
#include "CanManager.h"
#include "pins.h"

CAN_STRUCT(MOTOR, 291, 
    uint8_t motor_id;
    int16_t speed;        
)

CanManager can;

static const uint32_t SLEEP_PINS[NB_MOTOR] = {DC_1_SLEEP, DC_2_SLEEP};
static const uint32_t IN1_PINS[NB_MOTOR] = {DC_1_IN1, DC_2_IN1};
static const uint32_t IN2_PINS[NB_MOTOR] = {DC_1_IN2, DC_2_IN2};
static int current_speeds[NB_MOTOR] = {0, 0};
static int target_speeds[NB_MOTOR]  = {0, 0};

void set_motor(MOTOR msg){
    if (msg.motor_id < NB_MOTOR) {
        target_speeds[msg.motor_id] = msg.speed;
    }
}

void apply_speed() {
    for (uint8_t i = 0; i < NB_MOTOR; i++) {
        
        if (current_speeds[i] != target_speeds[i]) {
            current_speeds[i] = target_speeds[i];
            
            int speed = current_speeds[i];
            int abs_speed = abs(speed);
            if (abs_speed > 255) abs_speed = 255;

            if (speed > 0) {
                digitalWrite(IN2_PINS[i], LOW);
                analogWrite(IN1_PINS[i], abs_speed);
            } 
            else if (speed < 0) {
                digitalWrite(IN1_PINS[i], LOW);
                analogWrite(IN2_PINS[i], abs_speed);
            } 
            else {
                analogWrite(IN1_PINS[i], 0);
                analogWrite(IN2_PINS[i], 0);
                digitalWrite(IN1_PINS[i], LOW);
                digitalWrite(IN2_PINS[i], LOW);
            }
        }
    }
}

void setup() {
    can.init();

    for (uint8_t i = 0; i < NB_MOTOR; i++) {
        pinMode(SLEEP_PINS[i], OUTPUT);
        pinMode(IN1_PINS[i], OUTPUT);
        pinMode(IN2_PINS[i], OUTPUT);
        
        digitalWrite(SLEEP_PINS[i], HIGH);
        digitalWrite(IN1_PINS[i], LOW);
        digitalWrite(IN2_PINS[i], LOW);
    }
    can.onReceive<MOTOR>(set_motor);
}

void loop() {
    can.update();
    apply_speed();
}