/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by cantools version 35.5.0 Mon May 10 14:54:37 2021.
 */

#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef EINVAL
#    define EINVAL 22
#endif

/* Frame ids. */
#define SHUTDOWN_BRAKELIGHT_BSPD_SHUTDOWN_FRAME_ID (0x0bu)

/* Frame lengths in bytes. */
#define SHUTDOWN_BRAKELIGHT_BSPD_SHUTDOWN_LENGTH (5u)

/* Extended or standard frame types. */
#define SHUTDOWN_BRAKELIGHT_BSPD_SHUTDOWN_IS_EXTENDED (0)

/* Frame cycle times in milliseconds. */


/* Signal choices. */


/**
 * Signals in message brakelight_bspd_shutdown.
 *
 * All signal values are as on the CAN bus.
 */
struct shutdown_brakelight_bspd_shutdown_t {
    /**
     * Range: 0..1 (0..1 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t error_code;

    /**
     * Range: 0..1 (0..1 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t brake_analog_voltage_msb;

    /**
     * Range: 0..1 (0..1 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t brake_analog_voltage_lsb;

    /**
     * Range: 0..1 (0..1 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t brake_gate;

    /**
     * Range: 0..1 (0..1 -)
     * Scale: 1
     * Offset: 0
     */
    uint8_t bspdsense;
};

/**
 * Pack message brakelight_bspd_shutdown.
 *
 * @param[out] dst_p Buffer to pack the message into.
 * @param[in] src_p Data to pack.
 * @param[in] size Size of dst_p.
 *
 * @return Size of packed data, or negative error code.
 */
int shutdown_brakelight_bspd_shutdown_pack(
    uint8_t *dst_p,
    const struct shutdown_brakelight_bspd_shutdown_t *src_p,
    size_t size);

/**
 * Unpack message brakelight_bspd_shutdown.
 *
 * @param[out] dst_p Object to unpack the message into.
 * @param[in] src_p Message to unpack.
 * @param[in] size Size of src_p.
 *
 * @return zero(0) or negative error code.
 */
int shutdown_brakelight_bspd_shutdown_unpack(
    struct shutdown_brakelight_bspd_shutdown_t *dst_p,
    const uint8_t *src_p,
    size_t size);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool shutdown_brakelight_bspd_shutdown_error_code_is_in_range(uint8_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool shutdown_brakelight_bspd_shutdown_brake_analog_voltage_msb_is_in_range(uint8_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool shutdown_brakelight_bspd_shutdown_brake_analog_voltage_lsb_is_in_range(uint8_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool shutdown_brakelight_bspd_shutdown_brake_gate_is_in_range(uint8_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool shutdown_brakelight_bspd_shutdown_bspdsense_is_in_range(uint8_t value);


#ifdef __cplusplus
}
#endif

#endif
