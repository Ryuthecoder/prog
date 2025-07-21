#include "main.h"

pros::adi::Pneumatics hood('a', false); //hood
pros::adi::Pneumatics littlewill('b', false); //little will mech   

pros::Controller controller(pros::E_CONTROLLER_MASTER); // controller