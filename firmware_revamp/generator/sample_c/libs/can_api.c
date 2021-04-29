#include "can_api.h"

/*----- Interrupt(s) -----*/
// *pg 76 of datasheet*
#define UPDATE_STATUS                      0
// Timer counters
uint8_t gTimerFlag = 0x00;
// uint8_t gClock_prescale = 0x00;  // Used for update timer



ISR(TIMER0_COMPA_vect) {
	// Only send CAN msgs every 20 cycles
	// Timer Counter0 compare match A
 gTimerFlag |= _BV(UPDATE_STATUS);
 }

    for (int i=0; i<CAN_info_len; i++) {
        can_msg_info* msg = CAN_msg_array[i];
        msg->countdown = msg->countdown - gClock_prescale;
        if (msg->countdown <= 0) { // time to send the message
            CAN_transmit(msg->mob, msg->ident, msg->length, msg->raw_arr);
            msg->countdown = msg->cycle_time;
            for (int i=0; i<8; i++) {
                printf("%d, ", msg->raw_arr[i]);
            }
            printf(" sent\n");
        }
    }
}


//Should be making use of generated code, be defined in the generated file, not can_api
ISR(CAN_INT_vect) { //pin inturpt vector //notification from the CAN

/*----- Brake Light Mailbox -----*/
    // TODO in init function, need to set macros
	CANPAGE = (BRAKE_LIGHT_MBOX << MOBNB0); //1st mailbox from a certain person/board ... This is all your people on imessage
	if(bit_is_set(CANSTMOB, RXOK)) { //is there something in the mail box to be read
		//different letters
		// can_recv_msg[0] = CANMSG;   // PANIC PANIC
		// can_recv_msg[1] = CANMSG;   // brake analog voltage MSB       // subjectlines of the letter, have to read the letters in order
		// can_recv_msg[2] = CANMSG;   // brake analog voltage LSB
		// can_recv_msg[3] = CANMSG;   // is brake pressed? CF
		// can_recv_msg[4] = CANMSG;   // BSPD sense
		// can_recv_msg[5] = CANMSG;   // TSMS sense
		// can_recv_msg[6] = CANMSG;   // left e-stop sense
		// can_recv_msg[7] = CANMSG;   // GLVMS sense
        // TODO unpack the message using generated code, this should all go in the generated file not can_api

		if(can_recv_msg[3] == 0xFF) { //if the specific letter says something
			gFlag |= _BV(BRAKE_PRESSED);           //trip flag // scribbling on your hand
		} else {
			gFlag &= ~_BV(BRAKE_PRESSED);
		}

		if(can_recv_msg[3] == 0xFF) { //if the specific letter says something
			PORT_BRAKE_LED |= _BV(BRAKE_LED);           //trip flag // scribbling on your hand
		} else {
			PORT_BRAKE_LED &= ~_BV(BRAKE_LED);
		}

		//Setup to Receive Again
		CANSTMOB = 0x00;
		CAN_wait_on_receive(BRAKE_LIGHT_MBOX, CAN_ID_BRAKE_LIGHT, CAN_LEN_BRAKE_LIGHT, CAN_MSK_SINGLE);
	}


/*----- BMS Core Mailbox -----*/
	CANPAGE = (BMS_CORE_MBOX << MOBNB0);
	if(bit_is_set(CANSTMOB, RXOK)) {
		can_recv_msg[0] = CANMSG; // PANICPANIC
		can_recv_msg[1] = CANMSG; // Relay Status
		can_recv_msg[2] = CANMSG; // Temperature
		can_recv_msg[3] = CANMSG; // SoC % Estimate
		can_recv_msg[4] = CANMSG; // BMS OK!
		can_recv_msg[5] = CANMSG; // Regen Enabled
		can_recv_msg[6] = CANMSG; // Current Limiting Enabled
		can_recv_msg[7] = CANMSG; // Cell Balancing Status

		OCR1B = can_recv_msg[3]; //SoC for PWM


		// Grab BMS fault light
		if(can_recv_msg[4] == 0x00) {
			gFlag |= _BV(BMS_LIGHT); // If BMS shutdown is low, turn light pin high

		}
		//Setup to Receive Again
		CANSTMOB = 0x00;
		CAN_wait_on_receive(BMS_CORE_MBOX, CAN_ID_BMS_CORE, CAN_LEN_BMS_CORE, CAN_MSK_SINGLE);
	}


/*----- AIRs Control Sense Mailbox -----*/
	CANPAGE = (AIR_CONTROL_SENSE_MBOX << MOBNB0); //repeat with mailbox 1 to listen for BMS and IMD
	if(bit_is_set(CANSTMOB, RXOK)) {
		can_recv_msg[0] = CANMSG; // PANIC PANIC
		can_recv_msg[1] = CANMSG; // Main Pack Sense
		can_recv_msg[2] = CANMSG; // HVD Sense
		can_recv_msg[3] = CANMSG; // IMD Sense
		can_recv_msg[4] = CANMSG; // BMS Sense
		can_recv_msg[5] = CANMSG; // BMS Status
		can_recv_msg[6] = CANMSG; // IMD Status


		// Grab IMD status
		if(can_recv_msg[6] == 0x00) {
			gFlag |= _BV(IMD_STATUS); //If IMD status is low, set light pin high
		}

		// If IMD shutdown is true, make IMD_PIN high (if IMD goes low within 2 seconds of car on)
		// make sure these latch (don't turn off until board is turned off)

		//Setup to Receive Again
		CANSTMOB = 0x00;
		CAN_wait_on_receive(AIR_CONTROL_SENSE_MBOX, CAN_ID_AIR_CONTROL_SENSE, CAN_LEN_AIR_CONTROL_SENSE, CAN_MSK_SINGLE);
	}


/*----- AIRs Control Critical Mailbox -----*/
	CANPAGE = (AIR_CONTROL_CRITICAL_MBOX << MOBNB0); //repeat with mailbox 1 to listen for BMS and IMD
	if(bit_is_set(CANSTMOB, RXOK)) {
		can_recv_msg[0] = CANMSG; // error code, can panic PANIC
		can_recv_msg[1] = CANMSG; // Precharge Started/complete
		can_recv_msg[2] = CANMSG; // High Side AIR
		can_recv_msg[3] = CANMSG; // Low Side AIR
		can_recv_msg[4] = CANMSG; // HV Check
		can_recv_msg[5] = CANMSG; // Debugging


		if(can_recv_msg[1] == 0xFF) {
			gFlag |= _BV(PRECHARGE); //
		}
		else {
			gFlag &= ~_BV(PRECHARGE);
			gFlag &= ~_BV(STATUS_START);
			gCAN_MSG[1] = 0x00;
		}

		if(can_recv_msg[4] == 0xFF) {
			PORT_HV_LED |= _BV(HV_LED); //if the HV Check is High then turn on HV LED
		}
		else {
			PORT_HV_LED &= ~_BV(HV_LED); //if the HV Check is 0 then turn the HV LED off
		}

		// If IMD shutdown is true, make IMD_PIN high (if IMD goes low within 2 seconds of car on)
		// make sure these latch (don't turn off until board is turned off)

		//Setup to Receive Again
		CANSTMOB = 0x00;
		CAN_wait_on_receive(AIR_CONTROL_CRITICAL_MBOX, CAN_ID_AIR_CONTROL_CRITICAL, CAN_LEN_AIR_CONTROL_CRITICAL, CAN_MSK_SINGLE);
	}


/*----- Throttle Mailbox -----*/
	CANPAGE = (THROTTLE_MBOX << MOBNB0);
	if(bit_is_set(CANSTMOB, RXOK)) {
		can_recv_msg[0] = CANMSG;   // PANIC PANIC
		can_recv_msg[1] = CANMSG;   // Throttle Position
		can_recv_msg[2] = CANMSG;   // BOTs sense
		can_recv_msg[3] = CANMSG;   // inertia Switch Sense
		can_recv_msg[4] = CANMSG;   // Driver E-Stop Sense

		OCR1A = can_recv_msg[1]; //Throttle for PWM


		//Setup to Receive Again
		CANSTMOB = 0x00;
		CAN_wait_on_receive(THROTTLE_MBOX, CAN_ID_THROTTLE, CAN_LEN_THROTTLE, CAN_MSK_SINGLE);
	}


}

/*----- Functions -----*/
void initTimer(void) {
	TCCR0A = _BV(WGM01);    // Set up 8-bit timer in CTC mode
	TCCR0B = 0x05;          // clkio/1024 prescaler
	TIMSK0 |= _BV(OCIE0A);
	OCR0A = 0x27;           //Makes timer run at ~100Hz
}

uint8_t CAN_init (uint8_t mode)
{
    // Software reset; necessary for all CAN
    // stuff.
    CANGCON = _BV(SWRES);

    // CAN prescaler timing prescaler set to 0
    CANTCON = 0x00;

    // Set Error passive state
    CANGSTA |= _BV(ERRP);

    // Set BAUD rate
    

    // 500 kbps
    CANBT1 = 0x00;
    CANBT2 = 0x04;
    CANBT3 = 0x12;
    
    /* 
    // 250 kbps
    CANBT1 = 0x00;
    CANBT2 = 0x0C;
    CANBT3 = 0x36;
    */

    // Allow all interrupts & receive interrupts
    CANGIE |= _BV(ENIT) | _BV(ENRX);

    // compatibility with future chips
    CANIE1 = 0;

    // enable interrupts on all MObs
    CANIE2 = ( _BV(IEMOB0) | _BV(IEMOB1) |
               _BV(IEMOB2) | _BV(IEMOB3) |
               _BV(IEMOB4) | _BV(IEMOB5) );


    // All MObs come arbitrarily set up at first,
    // must reset all in order to make them useable
    int8_t mob;
    for( mob=0; mob<6; mob++ ){
        // Selects Message Object 0-5
        // This changes the MOb that is selected
        CANPAGE = ( mob << 4 );

        // Disable mob
        CANCDMOB = 0x00;

        // Clear mob status register;
        CANSTMOB = 0x00;
    }

    // Set up as Enabled mode
    //  instead of standby
    //  Necessary in order to get CAN
    //  communication
    if (mode == CAN_ENABLED)
    {
        CANGCON |= _BV( ENASTB );
    }
    else if( mode == CAN_LISTEN )
    {
        CANGCON |= _BV(LISTEN) | _BV( ENASTB );
    }

    return 0;
}

// TODO: refactor this function to take a struct instead
uint8_t CAN_transmit (uint8_t mob, uint16_t ident, uint8_t msg_length, uint8_t msg[])
{
    // Check that the MOb is free
    if( bit_is_set(CANEN2, mob) ){
        return CAN_ERR_MOB_BUSY;
    }

    // Select CAN mob based on input MOb;
    // also resets the CANPAGE
    CANPAGE = (mob << MOBNB0);

    // Clean CAN status for this MOb
    CANSTMOB = 0x00;

    // Set MOb ID
    CANIDT1 = (uint8_t) (ident >> 3);
    CANIDT2 = (uint8_t) (ident << 5);
    CANIDT3 = 0x00;
    CANIDT4 = 0x00; // Data frame

    // Set mask to 0x00
    // Not used by Tx but good practice
    CANIDM1 = 0x00;
    CANIDM2 = 0x00;
    CANIDM3 = 0x00;
    CANIDM4 = 0x00;

    // Set the message
    uint8_t i;
    for(i=0; i < msg_length; i++){
        CANMSG = msg[i];
    }

    // Send the message
    //CANCDMOB = _BV(CONMOB0) | (msg_length << DLC0);
    CANCDMOB = 0x00;
    CANCDMOB = (0x01 << CONMOB0) | (msg_length << DLC0);

    // TODO: Have interrupt to check status of transmitted message?

    return 0;
}


uint8_t CAN_transmit_success (uint8_t mob)
{
    // Check busy status
    if (bit_is_set(CANEN2, mob))
    {
        return CAN_ERR_MOB_BUSY;
    }

    // Check if OK transmission
    if (bit_is_set(CANSTMOB, TXOK))
    {
        CANSTMOB &= ~_BV(TXOK);
    }

    if (CANSTMOB != 0x00)
    {
        return CAN_ERR_UNKNOWN;
    }

    return 0;
}


uint8_t CAN_wait_on_receive (uint8_t mob, uint16_t ident, uint8_t msg_length, uint16_t mask)
{
    // Check that the MOb is free
    if( bit_is_set(CANEN2, mob) ){
        return CAN_ERR_MOB_BUSY;
    }

    // Select CAN mob based on input MOb
    CANPAGE = (mob << MOBNB0);

    // Clean CAN status for this MOb
    CANSTMOB = 0x00;

    // Set MOb ID
    CANIDT1 = (uint8_t) (ident >> 3); // node ID
    CANIDT2 = (uint8_t) (ident << 5);
    CANIDT3 = 0x00;
    CANIDT4 = 0x00; // Data frame

    // Set up MASK
    CANIDM1 = (uint8_t) (mask >> 3);
    CANIDM2 = (uint8_t) (mask << 5);
    CANIDM3 = 0x00;
    CANIDM4 = (_BV(RTRMSK) | _BV(IDEMSK)); // Use what is set above

    // Begin waiting for Rx
    //CANCDMOB = _BV(CONMOB1) | (msg_length << DLC0);
    CANCDMOB = 0x00;
    CANCDMOB = (0x02 << CONMOB0) | (msg_length << DLC0);

    return 0;
}


uint8_t CAN_read_received (uint8_t mob, uint8_t msg_length, uint8_t *msg)
{
    // Keep track of errors
    uint8_t error_value = 0;

    // Select MOb
    CANPAGE = (mob << MOBNB0);

    // Check to see if RXOK flag is set
    if (bit_is_set(CANSTMOB, RXOK))
    {
        // Reset RXOK if it is set
        CANSTMOB &= ~_BV(RXOK);
    }
    else
    {
        // Update the error value
        error_value = CAN_ERR_NO_RX_FLAG;
    }

    // Check if DLC is as expected
    if (bit_is_set(CANSTMOB, DLCW))
    {
        error_value = CAN_ERR_DLCW;

        // Set message length to the correct DLC
        msg_length = (0xF | CANCDMOB);
    }

    // Grab messages anyway
    for (int i = 0; i < msg_length; i++)
    {
        msg[i] = CANMSG;
    }

    // Reset entire status register
    CANSTMOB = 0x00;

    return error_value;
}
