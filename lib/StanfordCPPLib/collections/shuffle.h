/*
 * File: shuffle.h
 * ---------------
 * This file contains implementation of a shuffling function that operates on
 * a 1-D and 2-D array, Vector, or Grid of any type.
 *
 * @author Marty Stepp
 * @since 2014/02/01
 */

#include <private/init.h>   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include <private/initstudent.h>   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _shuffle_h
#define _shuffle_h

#include <string>

#define INTERNAL_INCLUDE 1
#include <random.h>
#undef INTERNAL_INCLUDE

/*
 * Randomly rearranges the elements of the given array up to the given length.
 * Precondition: The array pointer points to a valid non-null array in memory,
 * and that array contains at least 'length' elements.
 */
template <typename T>
void shuffle(T* array, int length) {
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

/*
 * Randomly rearranges the elements of the given 2-D array up to the given
 * number of rows and columns.
 * Precondition: The array pointer points to a valid non-null 2-D array in
 * memory, and that array contains at least the given number of rows/columns.
 */
template <typename T>
void shuffle(T** array2d, int rows, int cols) {
    int length = rows * cols;
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            int r1 = i / cols;
            int c1 = i % cols;
            int r2 = j / cols;
            int c2 = j % cols;

            T temp = array2d[r1][c1];
            array2d[r1][c1] = array2d[r2][c2];
            array2d[r2][c2] = temp;
        }
    }
}

/*
 * Randomly rearranges the characters of the given string and returns the
 * rearranged version.
 */
std::string shuffle(std::string s);

#endif // _shuffle_h
