/**
 * @file genericData.h
 * @author Stu Steiner
 * @date 29 Dec 2015
 * @brief The basic genericData.h file contains the prototypes for your specific type functions
 *
 * The genericData.h specifies the standard set of functions required for any type to
 * work with your generic linked list.
 *
 * @note This file will never be changed
 */

#ifndef GENERICDATA_H
#define GENERICDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requiredIncludes.h"

/**
 * @brief Prints the specific types contents.
 *
 * The printType function is called via function pointer
 * Its focus is to print the specific type formatted for
 * the type
 *
 * @param passedIn - The void * passed in representing the specific data
 *
 * @warning - The passed in void * passedIn is checked - exit(-99) if NULL
 */
void printType(void *passedIn);


/**
 * @brief Builds and returns a single specific data type.
 *
 * Reads the initial data from the file and then builds
 * an object of that specific data type.
 *
 * @note The carriage return will be removed when reading from the file
 *
 * @param fin - The FILE * representing the open file
 * @return void * - Representing an object of the specific type
 *
 * @warning - The passed in FILE * fin is checked - exit(-99) if NULL
 */
void *buildType(FILE *fin);


/**
 * @brief Builds and returns a single specific data type.
 *
 * Reads the initial data from the keyboard by prompting the user
 * for the specific information. An object of that specific data type
 * is constructed.
 *
 * @note The carriage return will be removed when reading from the keyboard
 *
 * @param fin - The FILE * representing stdin
 * @return void * - Representing an object of the specific type
 *
 * @warning - The passed in FILE * fin is checked - exit(-99) if NULL
 */
void *buildType_Prompt(FILE *fin);


/**
 * @brief Compares two objects of the specific data type.
 *
 * Compares two object of the specific data type and emulates the
 * behavior of the Java compareTo method indicating a sense of order.
 * The data checked is determined by the specifications provided by
 * the user.
 *
 * @note The passed in items will need to be cast to the appropriate
 * specific data type.
 *
 * @param p1 - The void * representing an object of the specific data type
 * @param p2 - The void * representing an object of the specific data type
 * @return int - Representing order < 0 indicates p1 is less than p2,
 * > 0 indicates p1 is greater than p2 and == 0 indicates p1 == p2 for contents
 *
 * @warning - The passed in void * p1 is checked - exit(-99) if NULL
 * @warning - The passed in void * p2 is checked - exit(-99) if NULL
 */
int compare(const void *p1, const void *p2);


/**
 * @brief Cleans up all dynamically allocated memory for the specific data type
 *
 * Cleans up and frees all the dynamically allocated memory for the
 * specific data type.  Each pointer in the specific data type is set
 * to NULL after it has been freed.
 *
 * @param passedIn - The void * passed in representing the specific data
 *
 * @warning - The passed in void * passedIn is checked - exit(-99) if NULL
 */
void cleanType(void *passedIn);


#endif // GENERICDATA_H
