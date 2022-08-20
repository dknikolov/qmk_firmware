// Copyright 2018-2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-3.0-or-later
#include "quantum.h"

#if (!defined(SHIFTREG_MATRIX_COL_LATCH) || !defined(SHIFTREG_MATRIX_COL_CLK) || !defined(SHIFTREG_MATRIX_COL_DATA))
#    error Missing shift register I/O pin definitions
#endif

uint8_t rowPinsSR[MATRIX_ROWS] = MATRIX_ROW_PINS_SR;

void pulsePinHigh(pin_t pin) {
    writePinHigh(pin);
    matrix_io_delay();
    writePinLow(pin);
}

void pulsePinLow(pin_t pin) {
    writePinLow(pin);
    matrix_io_delay();
    writePinHigh(pin);
}

static uint8_t read_rows(void) {
    uint8_t result = 0;
    for (int r = 0; r < MATRIX_ROWS; r++) {
        result |= (readPin(rowPinsSR[r]) << r);
    }
    return result;
}

void activate_column(int shiftBy) {
    writePinLow(SHIFTREG_MATRIX_COL_LATCH);
    // c < 8 to cover all 8 bits in the byte
    for (int c; c < 8; c++) {
        if (c == shiftBy) {
            writePinHigh(SHIFTREG_MATRIX_COL_DATA);
        } else {
            writePinLow(SHIFTREG_MATRIX_COL_DATA);
        }
        pulsePinHigh(SHIFTREG_MATRIX_COL_CLK);
        matrix_io_delay();
    }
    writePinLow(SHIFTREG_MATRIX_COL_DATA);
    matrix_io_delay();
    pulsePinHigh(SHIFTREG_MATRIX_COL_LATCH);
}

void matrix_init_custom(void) {
    // Set up the initial states for all the pins
    for (int r = 0; r < MATRIX_ROWS; r++) {
        setPinInput(rowPinsSR[r]);
        // setPinInputHigh(rowPinsSR[r]);
    }

    writePinLow(SHIFTREG_MATRIX_COL_LATCH);
    setPinOutput(SHIFTREG_MATRIX_COL_LATCH);

    writePinLow(SHIFTREG_MATRIX_COL_CLK);
    setPinOutput(SHIFTREG_MATRIX_COL_CLK);

    writePinLow(SHIFTREG_MATRIX_COL_DATA);
    setPinOutput(SHIFTREG_MATRIX_COL_DATA);
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    // Keep track of if something was modified
    bool matrix_has_changed = false;

    for (int c = 0; c < MATRIX_COLS; c++) {
        activate_column(c);
        matrix_io_delay();

        for (int r = 0; r < MATRIX_ROWS; r++) {
            matrix_row_t row_rx = read_rows();

            if (current_matrix[r] != row_rx) {
                matrix_has_changed = true;
                current_matrix[r] = row_rx;
            }
        }
    }
    return matrix_has_changed;
}
