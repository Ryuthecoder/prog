#include "main.h"


pros::adi::Pneumatics hood('a', false); //hood
pros::adi::Pneumatics littlewill('b', false); //little will mech   

pros::MotorGroup intake({-1, 2, 8}, pros::MotorGearset::blue); // intake motors

pros::Controller controller(pros::E_CONTROLLER_MASTER); // controller