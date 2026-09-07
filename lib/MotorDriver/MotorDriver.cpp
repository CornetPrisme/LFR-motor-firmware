#include "MotorDriver.h"
#include "CanMessages.h"
#include <Arduino.h>


MotorDriver::MotorDriver(CanManager* can_manager, motor_id_t id) {
    this->can = can_manager;
    this->motor_id = id;
}

void MotorDriver::set_speed(int16_t speed) {
    MOTOR cmd; 
    cmd.motor_id = this->motor_id;
    cmd.launch = false;
    cmd.speed = speed;

    this->can->send(cmd);
}

void MotorDriver::init() {
    MOTOR cmd; 
    cmd.motor_id = this->motor_id;
    cmd.launch = true;

    this->can->send(cmd);
}