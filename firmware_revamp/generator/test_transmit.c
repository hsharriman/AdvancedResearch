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

//for now, assume that you know exactly which messages you expect to receive.
void sample_CAN_init() {
    // assert that NUM_MSGS is not greater than 6
    // literally all of this can be automated based on the YAML files.
    //do this for each TX/RX message
        can_msg_info* can_info_arr = malloc(sizeof(can_msg_info*) * NUM_MSGS);
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
    CAN_init(mode, can_info_arr, NUM_MSGS);
    // know all TX/RX messages that will be sent on this board in advance via yaml. we can use this to GENERATE NAMED ARRAYS
    // that correspond to the name of each message that will be sent, so it is very simple to set a message.
}

//TODOs:
// concurrency issues? interrupt + main loop both reading/writing the same set of bytes
// generate those named arrays
// generate code that's like this for each TX/RX message.