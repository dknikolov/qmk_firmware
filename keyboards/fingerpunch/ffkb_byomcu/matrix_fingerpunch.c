// Copyright 2022 @sadekbaroudi (Sadek Baroudi)
// SPDX-License-Identifier: GPL-3.0-or-later
#include "quantum.h"
#include <string.h> /* memset */
#include <unistd.h> /* close */

#if (!defined(SHIFTREG_MATRIX_COL_LATCH) || !defined(SHIFTREG_MATRIX_COL_CLK) || !defined(SHIFTREG_MATRIX_COL_DATA))
#    error Missing shift register I/O pin definitions
#endif

int matrixArraySize = MATRIX_ROWS * sizeof(matrix_row_t);
matrix_row_t oldMatrix[MATRIX_ROWS];

#define SHIFTREG_OUTPUT_BITS 8
pin_t rowPinsSR[MATRIX_ROWS] = MATRIX_ROW_PINS_SR;

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

/**
 * Set the entire shift register to be full of inactive bits
 */
void clearColumns(void) {
    // Set column to inactive
    // writePinHigh(SHIFTREG_MATRIX_COL_DATA); // flip flop to test
    writePinLow(SHIFTREG_MATRIX_COL_DATA);

    // Loop through the columns, and clock to push a bit into the register
    for (int c = 0; c < SHIFTREG_OUTPUT_BITS; c++) {
        matrix_io_delay();
        pulsePinHigh(SHIFTREG_MATRIX_COL_CLK);
    }

    // Latch the register so that it's now all clear in the store
    matrix_io_delay();
    pulsePinHigh(SHIFTREG_MATRIX_COL_LATCH);
}

/**
 * Enable the first column. We assume that the shift register is full of inactive column bits!
 * If you are unsure, call clearColumns() before calling this
 */
void enableFirstColumn(void) {
    // Activate the data pin so the first column is enabled
    writePinHigh(SHIFTREG_MATRIX_COL_DATA); // flip flop to test
    // writePinLow(SHIFTREG_MATRIX_COL_DATA);

    matrix_io_delay();
    pulsePinHigh(SHIFTREG_MATRIX_COL_CLK);

    // Deactivate the data pin so when we shift, we're shifting in inactive columns
    // writePinHigh(SHIFTREG_MATRIX_COL_DATA); // flip flop to test
    writePinLow(SHIFTREG_MATRIX_COL_DATA);

    // Latch so we're good to go!
    pulsePinHigh(SHIFTREG_MATRIX_COL_LATCH);
}

/**
 * Shift the register by one to activate the next column
 * Assuming that enableFirstColumn has already been called, and you are iterating through the columns
 */
void nextColumn(void) {
    pulsePinHigh(SHIFTREG_MATRIX_COL_CLK);
    matrix_io_delay();
    pulsePinHigh(SHIFTREG_MATRIX_COL_LATCH);
}

/*
 * override of the qmk intialization function
 */
void matrix_init_custom(void) {
    // Set up the initial states for all the row pins
    for (int r = 0; r < MATRIX_ROWS; r++) {
        // Note: This needs to use the internal pull down resistors, and atmegas do *not* support that
        setPinInputLow(rowPinsSR[r]);
    }

    // Set the latch to low by default, and specify as an output pin
    writePinLow(SHIFTREG_MATRIX_COL_LATCH);
    setPinOutput(SHIFTREG_MATRIX_COL_LATCH);

    // Set the clock to low by default, and specify as an output pin
    writePinLow(SHIFTREG_MATRIX_COL_CLK);
    setPinOutput(SHIFTREG_MATRIX_COL_CLK);

    // Set the data pin to inactive by default, and specify as an output pin
    // writePinHigh(SHIFTREG_MATRIX_COL_DATA); // flip flop to test
    writePinLow(SHIFTREG_MATRIX_COL_DATA);
    setPinOutput(SHIFTREG_MATRIX_COL_DATA);

    // Since it's the init, deactivate all the columns. We'll activate once we get to the matrix scan
    clearColumns();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    // Keep track of if something was modified
    bool matrix_has_changed = false;

    // reset the current matrix, as we'll be updating and comparing to the old matrix
    memset(current_matrix, 0, matrixArraySize);

    // Set the first column to be active for the first loop
    enableFirstColumn();
    matrix_io_delay();

    // Loop through the columns, activating one at a time, and read the rows, and place in the new current_matrix
    // bool rowPinActive = false;
    for (int c = 0; c < MATRIX_COLS; c++) {
        for (int r = 0; r < MATRIX_ROWS; r++) {
            current_matrix[r] |= ((readPin(rowPinsSR[r]) ? 1 : 0) << c);
        }
        nextColumn();
        matrix_io_delay();
    }

    matrix_has_changed = memcmp(current_matrix, oldMatrix, matrixArraySize) != 0;
    memcpy(oldMatrix, current_matrix, matrixArraySize);

    // matrix_print();
    // xprintf("matrix_has_changed: %d\n", matrix_has_changed);

    return matrix_has_changed;
}

/*
current_matrix {
      1  0  2  4  3  5  6  7
 0   {1, 0, 0, 0, 0, 0, 0, 0},
 1   {0, 0, 0, 0, 0, 0, 0, 0},
 2   {0, 0, 0, 0, 0, 0, 0, 0},
 3   {0, 0, 0, 0, 0, 0, 0, 0},
 4   {0, 0, 0, 0, 0, 0, 0, 0},
 5   {0, 0, 0, 0, 0, 0, 0, 0}
}
*/