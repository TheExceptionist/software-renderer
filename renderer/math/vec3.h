/*
 * vector3.h
 *
 *  Created on: Mar 10, 2012
 *      Author: flamingo
 */

#ifndef vector3_H
#define vector3_H

#include "comm_pch.h"

#include "common_math.h"
#include <smmintrin.h>

namespace math
{

//! Three-component row vector.
/*!
  * v = (x, y, z)
  */
struct vec3
{
    //! X Y Z coordinates of this vector
    union
    {
        struct { float x, y, z, w; };      // w unused
        __attribute__((aligned(16))) __m128 v;
    };

    //! Default ctor.
    /*! Default zero vector. */
    vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
        : x(_x), y(_y), z(_z), w(1.0f) { }

    //! Addition assignment.
    vec3 &operator+= (const vec3 &other);
    //! Subtraction assignment.
    vec3 &operator-= (const vec3 &other);
    //! Scalar multiplication assignment.
    vec3 &operator*= (float s);
    //! 1/scalar multiplication.
    vec3 &operator/= (float s);

    //! Returns flipped vector.
    vec3 operator-() const;

    //! Equality check.
    bool operator== (const vec3 &other) const;
    //! Non-equality check.
    bool operator!= (const vec3 &other) const;

    //! Convertion to pointer to float. Returns address of `this'.
    operator const float* () const;

    //! Vector's magnitude.
    float length() const;
    //! Normalizes this vector and returns it.
    vec3 &normalize();
    //! Some type of ctor.
    void set(float x, float y, float z);
    //! Constructor from another vector.
    void set(const vec3 &other);
    //! Resets vector.
    void zero();
    //! Check all components for zero equality.
    bool isZero() const;
    //! Scalar product.
    float dotProduct(const vec3 &other) const;
    //! Cross product.
    vec3 crossProduct(const vec3 &other) const;

    //! Addition of two vectors.
    friend vec3 operator+ (const vec3 &a, const vec3 &b);
    //! Subtraction of two vectors.
    friend vec3 operator- (const vec3 &a, const vec3 &b);
    //! Multiplication of vector and scalar.
    friend vec3 operator* (const vec3 &a, float b);
    //! Multiplication of vector and scalar.
    friend vec3 operator* (float a, const vec3 &b);
    //! Vector and 1/scalar multiplication.
    friend vec3 operator/ (const vec3 &a, float b);

    //! Logger helper. Writes `[x = ?, y = ?, z = ?]' in the stream
    friend std::ostream &operator<< (std::ostream &os, const vec3 &v);

    //! Vector comparator.
    /*! \returns true if a.x < b.x. */
    friend bool comparex(const vec3 &a, const vec3 &b);

    //! Vector comparator.
    /*! \returns true if a.y < b.y. */
    friend bool comparey(const vec3 &a, const vec3 &b);

    //! Vector comparator.
    /*! \returns true if a.z < b.z. */
    friend bool comparez(const vec3 &a, const vec3 &b);
};

inline vec3 &vec3::operator+= (const vec3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

inline vec3 &vec3::operator-= (const vec3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

inline vec3 &vec3::operator*= (float s)
{
    x *= s;
    y *= s;
    z *= s;

    return *this;
}

inline vec3 &vec3::operator/= (float s)
{
    assert(!DCMP(s, 0.0f));
    x /= s;
    y /= s;
    z /= s;

    return *this;
}

inline vec3 vec3::operator- () const
{
    return vec3(-x, -y, -z);
}

inline bool vec3::operator== (const vec3 &other) const
{
    if (fabs(x - other.x) > EPSILON_E3)
        return false;
    if (fabs(y - other.y) > EPSILON_E3)
        return false;
    if (fabs(z - other.z) > EPSILON_E3)
        return false;
    return true;
}

inline bool vec3::operator!= (const vec3 &other) const
{
    if (fabs(x - other.x) > EPSILON_E3)
        return true;
    if (fabs(y - other.y) > EPSILON_E3)
        return true;
    if (fabs(z - other.z) > EPSILON_E3)
        return true;
    return false;
}

inline vec3::operator const float* () const
{
    return &x;
}

inline float vec3::length() const
{
    float t = x * x + y * y + z * z;
    assert(!DCMP(t, 0.0f));

    return sqrt(t);
}

inline vec3 &vec3::normalize()
{
    if ((fabs(x - 0.0f) < EPSILON_E6) &&
        (fabs(y - 0.0f) < EPSILON_E6) &&
        (fabs(z - 0.0f) < EPSILON_E6))
        return (*this);

    return (*this) /= length();
}

inline void vec3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

inline void vec3::set(const vec3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

inline void vec3::zero()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

inline bool vec3::isZero() const
{
    return (DCMP(x, 0.0f)) && (DCMP(y, 0.0f)) && (DCMP(z, 0.0f));
}

inline float vec3::dotProduct(const vec3 &other) const
{
    return (x * other.x + y * other.y + z * other.z);
}

inline vec3 vec3::crossProduct(const vec3 &other) const
{
    return vec3(y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
}

inline vec3 operator+ (const vec3 &a, const vec3 &b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator- (const vec3 &a, const vec3 &b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator* (const vec3 &a, float b)
{
    return vec3(a.x * b, a.y * b, a.z * b);
}

inline vec3 operator* (float a, const vec3 &b)
{
    return vec3(b.x * a, b.y * a, b.z * a);
}

inline vec3 operator/ (const vec3 &a, float b)
{
    assert(!DCMP(b, 0.0));
    return vec3(a.x / b, a.y / b, a.z / b);
}

inline std::ostream &operator<< (std::ostream &os, const vec3 &v)
{
    os << "[x: " << v.x << " y: " << v.y << " z: " << v.z << "]";
    return os;
}

inline bool comparex(const vec3 &a, const vec3 &b)
{
    return a.x < b.x;
}

inline bool comparey(const vec3 &a, const vec3 &b)
{
    return a.y < b.y;
}

inline bool comparez(const vec3 &a, const vec3 &b)
{
    return a.z < b.z;
}

inline vec3 lerp(const vec3 &a, const vec3 &b, float t)
{
    return a + t * (b - a);
}

}

#endif // vector3_H
