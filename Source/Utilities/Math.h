#ifndef MATH_H
#define MATH_H


//--------------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

/* Period parameters */
#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   /* constant vector a */
#define CMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
#define CMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

#define RADIANS_TO_DEGREES(x) ((x) * 180.0f / GCC_PI)
#define DEGREES_TO_RADIANS(x) ((x) * GCC_PI / 180.0f)




class Math
{
private:
    static const unsigned short angle_to_sin[90];

public:
    Math();
};

#endif // MATH_H
