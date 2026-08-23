#pragma once
#include <Arduino.h>
#include "CanMessages.h"

class MotorDriver {
    private:
        CanManager* can;
        motor_id_t motor_id;
    public:
        MotorDriver(CanManager* can_manager, motor_id_t id);

        void init();
        void avancer(uint8_t speed);
        void reculer(uint8_t speed);
        void stopper();
};