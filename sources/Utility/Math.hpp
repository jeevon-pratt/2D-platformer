#pragma once

#include <algorithm>        // std::max, std::min functions
#include <cmath>            // round function
#include <iosfwd>           // std::ostream
#include <type_traits>      // std::is_integral

struct b2Vec2;

class Sprite;


// ***************************
// b2Vec2 NON MEMBER FUNCTIONS
// ***************************

// Allows the multiplcation of a 2D vector by a scalar
b2Vec2 operator*(b2Vec2 vec, float scalar);

// Allows the division of a 2D vector by a scalar
b2Vec2 operator/(b2Vec2 vec, float scalar);

// Debugging function that allows for printing of vector components
std::ostream& operator<<(std::ostream& out, b2Vec2 vec);



// ********************
// CONVERSION FUNCTIONS
// ********************

// Converts meter coordinates into pixel coordinates
float ConvertToPixels(float x);

// Converts pixel coordinates into meter coordinates
float ConvertToMeters(float x);

// Converts Box2D world coordinates into SDL screen coordinates
b2Vec2 ConvertToScreenCoord(const Sprite& sprite, b2Vec2 worldPos, b2Vec2 camTransform);



// *************
// MISCELLANEOUS
// *************

// Accounts for precision errors when comparing two floats
bool ApproxEq(float num1, float num2);

// Returns the distance between two points
float GetDistance(b2Vec2 vec1, b2Vec2 vec2);

// Returns the midpoint between two points
b2Vec2 Midpoint(b2Vec2 p1, b2Vec2 p2);



// ******************
// TEMPLATE FUNCTIONS
// ******************

// Rounds a float value and converts it into an integer
//
// Note: Rounding to a non-integral type is not allowed.
template<typename T>
inline constexpr T RoundToInt(float val)
{
    static_assert(std::is_integral<T>::value, "'RoundToInt' only rounds to integral types");

    return static_cast<T>( round(val) );
}



// Rounds a double value and converts it into an integer
//
// Note: Rounding to a non-integral type is not allowed.
template<typename T>
inline constexpr T RoundToInt(double val)
{
    static_assert(std::is_integral<T>::value, "'RoundToInt' only rounds to integral types");

    return static_cast<T>( round(val) );
}



// Verifies the condition: lowerBound < value < upperBound
template<typename T>
inline constexpr bool InBounds(const T& value, const T& lowerBound, const T& upperBound)
{
    return ( (value > lowerBound) && (value < upperBound) );
}



// Verifies the condition: lowerBound <= value <= upperBound
template<typename T>
inline constexpr bool InBoundsEq(const T& value, const T& lowerBound, const T& upperBound)
{
    return ( (value >= lowerBound) && (value <= upperBound) );
}



// Restricts a value to a lower bound and an upper bound
template<typename T>
inline void Clamp(T& value, T lowerBound, T upperBound)
{
    value = std::max( lowerBound, std::min(value, upperBound) );
}
