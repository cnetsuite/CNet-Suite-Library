#pragma once

#ifndef CNET_MATH_H
#define CNET_MATH_H

#define MATH_PI 3.14159265358979323846 /**< The constant pi. */
#define MATH_E  2.71828182845904523536 /**< The constant e (Euler's number). */

#include "CNetApi.h"

/**
 * Absolute value of a double.
 *
 * @param value  Input value.
 * @return value if non-negative, otherwise -value.
 */
CNET_API double mathAbs(double value);

/**
 * Absolute value of an int.
 *
 * @param value  Input value.
 * @return value if non-negative, otherwise -value.
 */
CNET_API int mathAbsInt(int value);

/**
 * Smaller of two doubles.
 *
 * @param a  First value.
 * @param b  Second value.
 * @return a if a < b, otherwise b.
 */
CNET_API double mathMin(double a, double b);

/**
 * Larger of two doubles.
 *
 * @param a  First value.
 * @param b  Second value.
 * @return a if a > b, otherwise b.
 */
CNET_API double mathMax(double a, double b);

/**
 * Smaller of two ints.
 *
 * @param a  First value.
 * @param b  Second value.
 * @return a if a < b, otherwise b.
 */
CNET_API int mathMinInt(int a, int b);

/**
 * Larger of two ints.
 *
 * @param a  First value.
 * @param b  Second value.
 * @return a if a > b, otherwise b.
 */
CNET_API int mathMaxInt(int a, int b);

/**
 * Raise a value to a power.
 *
 * @param base      Base value.
 * @param exponent  Exponent to raise the base to.
 * @return base raised to exponent, per the standard pow() semantics.
 */
CNET_API double mathPow(double base, double exponent);

/**
 * Square root of a value.
 *
 * @param value  Input value; negative values yield NaN (per sqrt()).
 * @return Non-negative square root of value.
 */
CNET_API double mathSqrt(double value);

/**
 * Cube root of a value.
 *
 * @param value  Input value; negative values give a negative result.
 * @return Cube root of value.
 */
CNET_API double mathCbrt(double value);

/**
 * Natural exponential function.
 *
 * @param value  Exponent.
 * @return e raised to value.
 */
CNET_API double mathExp(double value);

/**
 * Natural logarithm.
 *
 * @param value  Input value; must be positive (0 or negative per log()).
 * @return Natural log of value.
 */
CNET_API double mathLog(double value);

/**
 * Base-10 logarithm.
 *
 * @param value  Input value; must be positive (0 or negative per log10()).
 * @return Base-10 log of value.
 */
CNET_API double mathLog10(double value);

/**
 * Round a value down to the nearest integer.
 *
 * @param value  Input value.
 * @return Largest integral value not greater than value.
 */
CNET_API double mathFloor(double value);

/**
 * Round a value up to the nearest integer.
 *
 * @param value  Input value.
 * @return Smallest integral value not less than value.
 */
CNET_API double mathCeil(double value);

/**
 * Round a value to the nearest integer, half-up (ties round toward
 * positive infinity, e.g. mathRound(-2.5) == -2).
 *
 * @param value  Input value.
 * @return value rounded to the nearest long.
 */
CNET_API long mathRound(double value);

/**
 * Sign of a value.
 *
 * @param value  Input value.
 * @return 1.0 if positive, -1.0 if negative, 0.0 if zero.
 */
CNET_API double mathSignum(double value);

/**
 * Euclidean distance / hypotenuse, computed without intermediate
 * overflow/underflow.
 *
 * @param x  First leg length.
 * @param y  Second leg length.
 * @return sqrt(x*x + y*y).
 */
CNET_API double mathHypot(double x, double y);

/**
 * Convert an angle from degrees to radians.
 *
 * @param degrees  Angle in degrees.
 * @return Angle in radians.
 */
CNET_API double mathToRadians(double degrees);

/**
 * Convert an angle from radians to degrees.
 *
 * @param radians  Angle in radians.
 * @return Angle in degrees.
 */
CNET_API double mathToDegrees(double radians);

/**
 * Sine of an angle.
 *
 * @param value  Angle in radians.
 * @return Sine of value.
 */
CNET_API double mathSin(double value);

/**
 * Cosine of an angle.
 *
 * @param value  Angle in radians.
 * @return Cosine of value.
 */
CNET_API double mathCos(double value);

/**
 * Tangent of an angle.
 *
 * @param value  Angle in radians.
 * @return Tangent of value.
 */
CNET_API double mathTan(double value);

/**
 * Arc sine.
 *
 * @param value  Input value; domain [-1, 1], out-of-range yields NaN.
 * @return Angle in radians in [-pi/2, pi/2].
 */
CNET_API double mathAsin(double value);

/**
 * Arc cosine.
 *
 * @param value  Input value; domain [-1, 1], out-of-range yields NaN.
 * @return Angle in radians in [0, pi].
 */
CNET_API double mathAcos(double value);

/**
 * Arc tangent.
 *
 * @param value  Input value.
 * @return Angle in radians in [-pi/2, pi/2].
 */
CNET_API double mathAtan(double value);

/**
 * Two-argument arc tangent, using the signs of both arguments to pick the
 * correct quadrant.
 *
 * @param y  Y coordinate.
 * @param x  X coordinate.
 * @return Angle in radians in [-pi, pi].
 */
CNET_API double mathAtan2(double y, double x);

/**
 * Integer division that rounds toward negative infinity (as opposed to
 * C's truncating division).
 *
 * @param x  Dividend.
 * @param y  Divisor; must not be 0.
 * @return Floor of x / y.
 */
CNET_API int mathFloorDiv(int x, int y);

/**
 * Integer modulo that follows floor division, so the result has the same
 * sign as the divisor (as opposed to C's truncating % operator).
 *
 * @param x  Dividend.
 * @param y  Divisor; must not be 0.
 * @return x modulo y, with the sign of y.
 */
CNET_API int mathFloorMod(int x, int y);

/**
 * Pseudo-random double. Backed by the C library rand(), seeded once from
 * the current time on first call (not thread-safe).
 *
 * @return Value in [0.0, 1.0).
 */
CNET_API double mathRandom(void);

#endif
