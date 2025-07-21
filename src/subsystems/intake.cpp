#include "main.h"

void holdIntake(int intakePower) {
    //intakePower = ((controller.get_digital(DIGITAL_R1) - controller.get_digital(DIGITAL_R2)) * 127);
    intake.move(intakePower);
}