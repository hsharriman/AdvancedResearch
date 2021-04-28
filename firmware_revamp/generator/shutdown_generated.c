#include "shutdown_generated.h"

/* RAW ARRAY DEFINITIONS */
uint8_t* BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR;


/* MESSAGE STRUCT DEFINITIONS */
struct shutdown_brakelight_bspd_shutdown_t BRAKELIGHT_BSPD_SHUTDOWN_MSG;


/* BOARD-SPECIFIC CAN FUNCTIONALITY. */
void shutdown_CAN_init() {
    // Initialize each struct as empty
    // BRAKELIGHT_BSPD_SHUTDOWN_MSG
    BRAKELIGHT_BSPD_SHUTDOWN_MSG = {
        .error_code = OxFF,
        .brake_analog_voltage_msb = OxFF,
        .brake_analog_voltage_lsb = OxFF,
        .brake_gate = OxFF,
        .bspdsense = OxFF,

    };

    // Allocate array of structs that will be shared with can_api
    CAN_msg_array = malloc(sizeof(can_msg_info*) * 1);
    CAN_info_len = 1;

    // Add one struct per TX messsage
    //BRAKELIGHT_BSPD_SHUTDOWN struct initialization
    BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR = malloc(sizeof(uint8_t) * 8);
    struct can_msg_info MESSAGE = {
        .raw_arr = BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR,
        .countdown = 100,
        .cycle_time = 100,
        .mob = 0, // TODO?
        .ident = 11.0,
        .length = 5.0,
    };
    can_info_arr[0] = &MESSAGE;


    // Call can_api init
    CAN_init(mode);
}

/* BOARD-SPECIFIC MESSAGE SETTERS, ONE PER SIGNAL PER TX MESSAGE */
// Update sig: error_code of msg: brakelight_bspd_shutdown
int set_brakelight_bspd_shutdown_error_code (uint8_t value) {
    //check that it is a legal value
    if (shutdown_brakelight_bspd_shutdown_error_code_is_in_range(value) == 0) {
        return 0;
    }
    // update the struct and the raw array
    BRAKELIGHT_BSPD_SHUTDOWN_MSG.error_code = value;
    s = shutdown_brakelight_bspd_shutdown_pack(BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR, &BRAKELIGHT_BSPD_SHUTDOWN_MSG, 8);
    return s;
};
// Update sig: brake_analog_voltage_msb of msg: brakelight_bspd_shutdown
int set_brakelight_bspd_shutdown_brake_analog_voltage_msb (uint8_t value) {
    //check that it is a legal value
    if (shutdown_brakelight_bspd_shutdown_brake_analog_voltage_msb_is_in_range(value) == 0) {
        return 0;
    }
    // update the struct and the raw array
    BRAKELIGHT_BSPD_SHUTDOWN_MSG.error_code = value;
    s = shutdown_brakelight_bspd_shutdown_pack(BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR, &BRAKELIGHT_BSPD_SHUTDOWN_MSG, 8);
    return s;
};
// Update sig: brake_analog_voltage_lsb of msg: brakelight_bspd_shutdown
int set_brakelight_bspd_shutdown_brake_analog_voltage_lsb (uint8_t value) {
    //check that it is a legal value
    if (shutdown_brakelight_bspd_shutdown_brake_analog_voltage_lsb_is_in_range(value) == 0) {
        return 0;
    }
    // update the struct and the raw array
    BRAKELIGHT_BSPD_SHUTDOWN_MSG.error_code = value;
    s = shutdown_brakelight_bspd_shutdown_pack(BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR, &BRAKELIGHT_BSPD_SHUTDOWN_MSG, 8);
    return s;
};
// Update sig: brake_gate of msg: brakelight_bspd_shutdown
int set_brakelight_bspd_shutdown_brake_gate (uint8_t value) {
    //check that it is a legal value
    if (shutdown_brakelight_bspd_shutdown_brake_gate_is_in_range(value) == 0) {
        return 0;
    }
    // update the struct and the raw array
    BRAKELIGHT_BSPD_SHUTDOWN_MSG.error_code = value;
    s = shutdown_brakelight_bspd_shutdown_pack(BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR, &BRAKELIGHT_BSPD_SHUTDOWN_MSG, 8);
    return s;
};
// Update sig: bspdsense of msg: brakelight_bspd_shutdown
int set_brakelight_bspd_shutdown_bspdsense (uint8_t value) {
    //check that it is a legal value
    if (shutdown_brakelight_bspd_shutdown_bspdsense_is_in_range(value) == 0) {
        return 0;
    }
    // update the struct and the raw array
    BRAKELIGHT_BSPD_SHUTDOWN_MSG.error_code = value;
    s = shutdown_brakelight_bspd_shutdown_pack(BRAKELIGHT_BSPD_SHUTDOWN_RAW_ARR, &BRAKELIGHT_BSPD_SHUTDOWN_MSG, 8);
    return s;
};

