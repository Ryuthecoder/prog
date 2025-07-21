#include "main.h"



/*bool hoodToggle = false; //hood toggle state
bool littleWillToggle = false; //little will toggle state */



void toggleHood() {
    if(controller.get_digital_new_press(DIGITAL_A)) {
        hood.toggle();
    }
}

