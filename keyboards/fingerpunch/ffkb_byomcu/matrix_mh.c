/* Copyright 2021 wwwMADwww <4d4144h@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"
#include <string.h> /* memset */
#include <unistd.h> /* close */


int matrixArraySize = MATRIX_ROWS * sizeof(matrix_row_t);
matrix_row_t oldMatrix[MATRIX_ROWS];

uint8_t keyMatrixRowPins[MATRIX_ROWS] = MATRIX_PIN_ROWS;


/* Initial matrix state, no columns active */
void keyMatrixClear(void) {

    for(int i = 0; i < MATRIX_COLS; i++) {
        matrix_io_delay();
        writePinHigh(MATRIX_PIN_REG_SHIFT);
        writePinLow(MATRIX_PIN_REG_SHIFT);
    }

    matrix_io_delay();
    writePinHigh(MATRIX_PIN_REG_STORE);
    writePinLow(MATRIX_PIN_REG_STORE);

}


void keyMatrixInit(void) {

    for (int i = 0; i < MATRIX_ROWS ; i++) {
        setPinInputHigh(keyMatrixRowPins[i]);
    }

    setPinOutput(MATRIX_PIN_REG_DATA);
    writePinLow(MATRIX_PIN_REG_DATA);

    setPinOutput(MATRIX_PIN_REG_STORE);
    writePinLow(MATRIX_PIN_REG_STORE);

    setPinOutput(MATRIX_PIN_REG_SHIFT);
    writePinLow(MATRIX_PIN_REG_SHIFT);

    keyMatrixClear();

}



/*
    setting first column active.
    assuming that keyMatrixClear was called right before this or keyMatrixShift was called at least MATRIX_COLS times
*/
void keyMatrixReset(void) {

    writePinLow(MATRIX_PIN_REG_DATA);
    
    matrix_io_delay();
    writePinHigh(MATRIX_PIN_REG_SHIFT);
    writePinLow(MATRIX_PIN_REG_SHIFT);

    matrix_io_delay();
    writePinHigh(MATRIX_PIN_REG_DATA);

    matrix_io_delay();
    writePinHigh(MATRIX_PIN_REG_STORE);
    writePinLow(MATRIX_PIN_REG_STORE);

}



/* setting next column active */
void keyMatrixShift(void) {
    
    matrix_io_delay();
    writePinHigh(MATRIX_PIN_REG_SHIFT);
    writePinLow(MATRIX_PIN_REG_SHIFT);
    
    matrix_io_delay();
    writePinHigh(MATRIX_PIN_REG_STORE);
    writePinLow(MATRIX_PIN_REG_STORE);
}



/* 
 * read rows states based on the single active column, which is set in keyMatrixShift,
 * and return bits in right order
 *
 * example: all rows pressed for a given column
 *
 * uint8_t res = 0;
 * 0000000 |= ((readPin(D4) & 1) ? 0 : 1) << (5); (0100000)
 * 0100000 |= ((readPin(F7) & 1) ? 0 : 1) << (4); (0010000)
 * 0110000 |= ((readPin(F6) & 1) ? 0 : 1) << (3); (0001000)
 * 0111000 |= ((readPin(F5) & 1) ? 0 : 1) << (2); (0000100)
 * 0111100 |= ((readPin(F4) & 1) ? 0 : 1) << (1); (0000010)
 * 0111110 |= ((readPin(D2) & 1) ? 0 : 1) << (0); (0000001)
 * res = 01111111
 */
uint8_t keyMatrixGetColRowValues(void) {

    uint8_t res = 0;
    uint8_t readValue = 0;

    for (int r = 0; r < MATRIX_ROWS; r++) {
        readValue = readPin(keyMatrixRowPins[r]);
        xprintf("Reading from row pin (r=%d, pin=%s)\nreadValue: %08b\nprocessed value: %08b\n", r, keyMatrixRowPins[r], readValue, ((readValue & 1) ? 0 : 1) << ((MATRIX_ROWS - 1) - r));
        res |= ((readValue & 1) ? 0 : 1) << ((MATRIX_ROWS - 1) - r);
    }

    return res;
}


/*==========================================================================
============================================================================
==========================================================================*/



void matrix_init_custom(void) {  

    memset(oldMatrix, 0, matrixArraySize);
    keyMatrixInit();

}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {

    bool changed = false;
    memset(current_matrix, 0, matrixArraySize);

    keyMatrixReset();
    
    for (int c = 0; c < MATRIX_COLS; c++) {

        uint8_t rowValues = keyMatrixGetColRowValues();
        xprintf("rowValues %d: %08b\n", c, rowValues);

        for (int r = 0; r < MATRIX_ROWS; r++) {
            xprintf("current_matrix processed value for c=%d, r=%d: %08b\n", c, r, ((rowValues & (1 << r)) ? 1 : 0) << c);
            current_matrix[r] |= ((rowValues & (1 << r)) ? 1 : 0) << c;
            xprintf("current_matrix[%d]: %08b\n", r, current_matrix[r]);
        }
    
        keyMatrixShift();
    }


    changed = memcmp(current_matrix, oldMatrix, matrixArraySize) != 0;
    memcpy(oldMatrix, current_matrix, matrixArraySize);

    return changed;

}

/* example loop through for loops for col and rows in matrix_scan_custom above
current_matrix[0] |= ((00000010 & (0000001)) ? 1 : 0) << 0; 00000000
current_matrix[1] |= ((01000000 & (0000010)) ? 1 : 0) << 0; 00000000
current_matrix[2] |= ((00000100 & (0000100)) ? 1 : 0) << 0; 00000001
current_matrix[3] |= ((00000000 & (0001000)) ? 1 : 0) << 0; 00000000
current_matrix[4] |= ((00000000 & (0010000)) ? 1 : 0) << 0; 00000000
current_matrix[5] |= ((00000000 & (0100000)) ? 1 : 0) << 0; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000000
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

00000000 |= ((00010000 & (1 << 0)) ? 1 : 0) << 1; 00000000
00000000 |= ((00000010 & (1 << 1)) ? 1 : 0) << 1; 00000010
00000001 |= ((00000000 & (1 << 2)) ? 1 : 0) << 1; 00000000
00000000 |= ((00000000 & (1 << 3)) ? 1 : 0) << 1; 00000000
00000000 |= ((00000000 & (1 << 4)) ? 1 : 0) << 1; 00000000
00000000 |= ((00000000 & (1 << 5)) ? 1 : 0) << 1; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000010
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

00000000 |= ((00000000 & (1 << 0)) ? 1 : 0) << 2; 00000000
00000010 |= ((00000100 & (1 << 1)) ? 1 : 0) << 2; 00000100
00000001 |= ((00000000 & (1 << 2)) ? 1 : 0) << 2; 00000000
00000000 |= ((00000000 & (1 << 3)) ? 1 : 0) << 2; 00000000
00000000 |= ((00000000 & (1 << 4)) ? 1 : 0) << 2; 00000000
00000000 |= ((00000000 & (1 << 5)) ? 1 : 0) << 2; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

current_matrix[0] |= ((00000000 & (1 << 0)) ? 1 : 0) << 3; 00000000
current_matrix[1] |= ((00000000 & (1 << 1)) ? 1 : 0) << 3; 00000000
current_matrix[2] |= ((00000000 & (1 << 2)) ? 1 : 0) << 3; 00000000
current_matrix[3] |= ((00000000 & (1 << 3)) ? 1 : 0) << 3; 00000000
current_matrix[4] |= ((00000000 & (1 << 4)) ? 1 : 0) << 3; 00000000
current_matrix[5] |= ((00000000 & (1 << 5)) ? 1 : 0) << 3; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

current_matrix[0] |= ((00000000 & (1 << 0)) ? 1 : 0) << 4; 00000000
current_matrix[1] |= ((00000000 & (1 << 1)) ? 1 : 0) << 4; 00000000
current_matrix[2] |= ((00000000 & (1 << 2)) ? 1 : 0) << 4; 00000000
current_matrix[3] |= ((00000000 & (1 << 3)) ? 1 : 0) << 4; 00000000
current_matrix[4] |= ((00000000 & (1 << 4)) ? 1 : 0) << 4; 00000000
current_matrix[5] |= ((00000000 & (1 << 5)) ? 1 : 0) << 4; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

current_matrix[0] |= ((00000000 & (1 << 0)) ? 1 : 0) << 5; 00000000
current_matrix[1] |= ((00000000 & (1 << 1)) ? 1 : 0) << 5; 00000000
current_matrix[2] |= ((00000000 & (1 << 2)) ? 1 : 0) << 5; 00000000
current_matrix[3] |= ((00000000 & (1 << 3)) ? 1 : 0) << 5; 00000000
current_matrix[4] |= ((00000000 & (1 << 4)) ? 1 : 0) << 5; 00000000
current_matrix[5] |= ((00000000 & (1 << 5)) ? 1 : 0) << 5; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

current_matrix[0] |= ((00000000 & (1 << 0)) ? 1 : 0) << 6; 00000000
current_matrix[1] |= ((00000000 & (1 << 1)) ? 1 : 0) << 6; 00000000
current_matrix[2] |= ((00000000 & (1 << 2)) ? 1 : 0) << 6; 00000000
current_matrix[3] |= ((00000000 & (1 << 3)) ? 1 : 0) << 6; 00000000
current_matrix[4] |= ((00000000 & (1 << 4)) ? 1 : 0) << 6; 00000000
current_matrix[5] |= ((00000000 & (1 << 5)) ? 1 : 0) << 6; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

current_matrix[0] |= ((00000000 & (1 << 0)) ? 1 : 0) << 7; 00000000
current_matrix[1] |= ((00000000 & (1 << 1)) ? 1 : 0) << 7; 00000000
current_matrix[2] |= ((00000000 & (1 << 2)) ? 1 : 0) << 7; 00000000
current_matrix[3] |= ((00000000 & (1 << 3)) ? 1 : 0) << 7; 00000000
current_matrix[4] |= ((00000000 & (1 << 4)) ? 1 : 0) << 7; 00000000
current_matrix[5] |= ((00000000 & (1 << 5)) ? 1 : 0) << 7; 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000

current_matrix[0] = 00000000
current_matrix[1] = 00000110
current_matrix[2] = 00000001
current_matrix[3] = 00000000
current_matrix[4] = 00000000
current_matrix[5] = 00000000
*/