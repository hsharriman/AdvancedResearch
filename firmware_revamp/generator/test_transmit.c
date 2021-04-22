#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "can_api.h"

//can be auto-generated
const NUM_MSGS = 1;
int cycle_time = 10;
uint8_t mob = 0xFF;
uint16_t ident = 2;
int msg_length = 8;

//There is only one message
struct test_dashboard_t DASHBOARD_MSG;

void init() {
    // initialize a struct for each message
    DASHBOARD_MSG = {
        .error_code = 0x00,
        .ready2_drive = 0x00,
        .steering_pos = 0x00,
        .start_button = 0x00,
    };
    //from board side, where are we tracking the RAW array for each message -> struct?
    //TODO: information needed from the dbc to get this working nicely:
        // names of each message, their frequency, their message ID, their message length

}

//for now, assume that you know exactly which messages you expect to receive.
can_msg_info* sample_CAN_init() {
    // assert that NUM_MSGS is not greater than 6
    // literally all of this can be automated based on the YAML files.
    //do this for each TX/RX message
    can_msg_info* can_info_arr = malloc(sizeof(can_msg_info*) * NUM_MSGS);
    for (int i=0; i<NUM_MSGS; i++) {
        uint8_t* RAW_ARR = malloc(sizeof(uint8_t) * 8);
        struct can_msg_info A_MESSAGE = {
            .raw_arr = RAW_ARR,
            .countdown = cycle_time, // comes from YAML....
            .cycle_time = cycle_time,
            .mob = mob, // arbitrary
            .ident = ident, // YAML
            .length = msg_length,  //YAML
        };
        can_info_arr[0] = &A_MESSAGE;
    }
    //IF WE ARE USING RTOS THEN WE JUST NEED TO SET THINGS IN STRUCTS, THE CAN API SIDE WILL BE FED THE APPROPRIATE INFO FROM THE RELEVANT STRUCT
    CAN_init(mode, can_info_arr, NUM_MSGS);
    // know all TX/RX messages that will be sent on this board in advance via yaml. we can use this to GENERATE NAMED ARRAYS
    // that correspond to the name of each message that will be sent, so it is very simple to set a message.
    return CAN_info_arr;
}

//TODOs:
// concurrency issues? interrupt + main loop both reading/writing the same set of bytes
// generate those named arrays
// generate code that's like this for each TX/RX message.

int main() {
    can_msg_info* CAN_ARR = sample_CAN_init();
    int countdown = 10;
    while (1) {
        if (countdown <= 0) {
            return 0;
        }
        //something happens and we need to update a particular message
        
        countdown--;
    }
}