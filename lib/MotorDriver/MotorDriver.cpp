#include "MotorDriver.h"
#include "CanMessages.h"
#include <Arduino.h>


MotorDriver::MotorDriver(CanManager* can_manager, motor_id_t id) {
    this->can = can_manager;
    this->motor_id = id;
}

void MotorDriver::avancer(uint8_t speed) {
    MOTOR cmd; 
    cmd.motor_id = this->motor_id;
    cmd.etat = true;
    cmd.forward = true;
    cmd.speed = speed;

    this->can->send(cmd);
}

void MotorDriver::reculer(uint8_t speed) {
    MOTOR cmd;
    cmd.motor_id = this->motor_id;
    cmd.etat = true;
    cmd.forward = false;
    cmd.speed = speed;

    this->can->send(cmd); 
}

void MotorDriver::stopper() {
    MOTOR cmd;
    cmd.motor_id = this->motor_id;
    cmd.etat = false;
    cmd.forward = false;
    cmd.speed = 0;

    this->can->send(cmd); 
}

void MotorDriver::init() {
    MOTOR cmd;
    cmd.motor_id = this->motor_id;
    cmd.etat = false;
    cmd.forward = true;
    cmd.speed = 0;

    this->can->send(cmd);
}