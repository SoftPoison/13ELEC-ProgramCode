#ifndef _SOFT_UTILS_H
#define _SOFT_UTILS_H

#include "application.h"
#include <vector>

/** Separates a received message into substrings

  Splits a given string into substrings upon each occurance of a
  semicolon (";")

  @param message the received message

  @returns a vector (list) filled with the substrings
*/
std::vector<String> separateMessage(String& message);

/** Separates a four character string into four integers

  Splits the given string into a vector of unsigned bytes
  (values [0 .. 3] so really just 2 bit integers) and also indicates the
  value of the of the bytes on the LED array

  @param str a four character string formed from numbers

  @returns a vector (list) with the integer directions
*/
std::vector<uint8_t> getDirs(String &str);

/** Mathematical absolute

  Transforms the given value to its mathemtical absolute ( x->|x| )

  @param &num a number of any type

  @returns |&num|
*/
template<class T>
inline T& abs(T &num) {
  if (num < 0) num *= -1;

  return num;
};

/** Fast average (median)

  Calculates the median value of a dataset (somewhat quickly)

  @param numArray an array of numbers

  @returns average
*/
template<class T>
inline T fastAvg(T *numArray) {
  float average = 0;
  size_t arrayLen = sizeof(numArray) / sizeof(T);

  for (uint16_t i = 0; i < arrayLen; i++)
    average += ((float) numArray[i]) / arrayLen;

  return (T) average;
}

template<class T>
inline T min(T &a, T &b) {
  return a < b ? a : b;
}

template<class T>
inline T max(T &a, T &b) {
  return a > b ? a : b;
}

#endif //_SOFT_UTILS_H
