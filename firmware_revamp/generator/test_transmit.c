#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "can_api.h"

//can be auto-generated
uint8_t mob = 0xFF;

//There is only one message
struct test_dashboard_t DASHBOARD_MSG;

void init() {
    // initialize a struct for each message, can be generated based on dbc file
    DASHBOARD_MSG = {
        .error_code = 0x00,
        .ready2_drive = 0x00,
        .steering_pos = 0x00,
        .start_button = 0x00,
    };
}

//TODOs:
// concurrency issues? interrupt + main loop both reading/writing the same set of bytes

int dashboard_set_error_code(uint8_t value) {
    //check that it is a legal value?
    DASHBOARD_MSG.error_code = value;
    s = test_dashboard_pack(DASHBOARD_RAW_ARR, &DASHBOARD_MSG, 8);
    return s;
}

int main() {
    can_msg_info* CAN_ARR = sample_CAN_init();
    int countdown = 10;
    while (1) {
        if (countdown <= 0) {
            return 0;
        }
        //something happens and we need to update a particular message
        //make simple signal setter functions that look like:
        int s = dashboard_set_error_code(0xFF);
        countdown--;
    }
}