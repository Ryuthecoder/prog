#include "main.h"
#include "lemlib/api.hpp" 
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
 

// controller 
//pros::Controller controller(pros::E_CONTROLLER_MASTER);

// MotorGroup
pros::MotorGroup leftMotors({-5, 4, -3},
                            pros::MotorGearset::blue); 
pros::MotorGroup rightMotors({6, -9, 7}, pros::MotorGearset::blue); 

//pneumatics
/*pros::adi::Pneumatics hood('a', false); //hood

pros::adi::Pneumatics littlewill('b', false); //little will mech */



// Inertial Sensor port number 
pros::Imu imu(7);

// tracking wheels

pros::Rotation horizontalEnc(20);

pros::Rotation verticalEnc(-11);

lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, 0);

lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              1, // ? inch track width figure out 
                              lemlib::Omniwheel::NEW_275, 
                              450, // drivetrain rpm
                              2 // horizontal drift (figure out)
);

// lateral motion controller 
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
Runs initialization code
this occurs as soon as the program is started
all other competition modes are blocked by initialize
it is recommended to keep execution time for this mode under a few seconds*/
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, it can be changed using the following line
    // lemlib::bufferedStdout().setRate(...);
    // If I use bluetooth or a wired connection, I will want to have a rate of 10ms

 

    //brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}


//Runs while the robot is disabled

void disabled() {}


//runs after initialize if the robot is connected to field control

void competition_initialize() {}

// get a path used for pure pursuit
ASSET(PLACEHOLDER_pathjerryio_txt); // replace "." with "_" to comply with C++ 

//runs during auton

void autonomous() {
    //pure pursuit path
	chassis.follow(PLACEHOLDER_pathjerryio_txt, 20, 11000);

}


// driver control
void opcontrol() {
    // controller 
    
    // loop to continuously update motors im still exploring options

    

    while (true) {
        
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        
        chassis.arcade(leftY, rightX);
        
        /**if (leftY == 0 && rightX == 0) {
            // if the joystick is not moving, stop the chassis
            chassis.setBrakeMode();
        } else {
            // otherwise, move the chassis
            chassis.arcade(leftY, rightX);
        }*/

        chassis.arcade(leftY, rightX); 

        toggleHood();

        holdIntake((controller.get_digital(DIGITAL_R1) - controller.get_digital(DIGITAL_R2)) * 127);
        
        pros::delay(10);

    }
}