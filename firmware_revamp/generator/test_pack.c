#include <stdio.h>
#include <stdlib.h>
#include "test.h"

struct test_dashboard_t CAN_MSG = {
    .error_code = 0x00,
    .ready2_drive = 0x00,
    .steering_pos = 0x00,
    .start_button = 0x00,
};

int main() {
    int s = 1;
    uint8_t* RAW_ARR = malloc(sizeof(uint8_t) * 8);
    printf("setting message\n");
    CAN_MSG.error_code = 0xFF;
    CAN_MSG.ready2_drive = 0xFF;
    printf("CAN message: %d,%d, %d, %d\n", CAN_MSG.error_code, CAN_MSG.ready2_drive, CAN_MSG.steering_pos, CAN_MSG.start_button);

    s = test_dashboard_pack(RAW_ARR, &CAN_MSG, 8);
    for (int i=0; i<8; i++) {
        printf("%d, ", RAW_ARR[i]);
    }
    printf("\n");
    RAW_ARR[3] = 0xFF;

    int err = test_dashboard_unpack(&CAN_MSG, RAW_ARR, 8);
    printf("CAN message: %d,%d, %d, %d\n", CAN_MSG.error_code, CAN_MSG.ready2_drive, CAN_MSG.steering_pos, CAN_MSG.start_button);

    return 0;
}