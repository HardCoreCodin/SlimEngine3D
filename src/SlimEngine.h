#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif

#if defined(__clang__)
#define COMPILER_CLANG 1
#define COMPILER_CLANG_OR_GCC 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC 1
    #define COMPILER_CLANG_OR_GCC 1
#elif defined(_MSC_VER)
    #define COMPILER_MSVC 1
#endif

#ifndef NDEBUG
#define INLINE
#elif defined(COMPILER_MSVC)
#define INLINE inline __forceinline
#elif defined(COMPILER_CLANG_OR_GCC)
    #define INLINE inline __attribute__((always_inline))
#else
    #define INLINE inline
#endif

#ifdef COMPILER_CLANG
#define ENABLE_FP_CONTRACT \
        _Pragma("clang diagnostic push") \
        _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"") \
        _Pragma("STDC FP_CONTRACT ON") \
        _Pragma("clang diagnostic pop")
#else
#define ENABLE_FP_CONTRACT
#endif

#ifdef FP_FAST_FMAF
#define fast_mul_add(a, b, c) fmaf(a, b, c)
#else
ENABLE_FP_CONTRACT
#define fast_mul_add(a, b, c) ((a) * (b) + (c))
#endif

#define TAU 6.28f
#define EPS 0.0001f
#define HALF_SQRT2 0.70710678118f
#define SQRT2 1.41421356237f
#define SQRT3 1.73205080757f

typedef struct vec3 { f32 x, y, z;     } vec3;
typedef struct vec4 { f32 x, y, z, w;  } vec4;
typedef struct mat3 { vec3 X, Y, Z;    } mat3;
typedef struct mat4 { vec4 X, Y, Z, W; } mat4;
typedef struct AABB { vec3 min, max;   } AABB;
typedef struct quat { vec3 axis; f32 amount; } quat;

enum RenderMode {
    RenderMode_Normals,
    RenderMode_Beauty,
    RenderMode_Depth,
    RenderMode_UVs
};

INLINE mat3 getMat3Identity() {
    mat3 out;

    out.X.x = 1; out.X.y = 0; out.X.z = 0;
    out.Y.x = 0; out.Y.y = 1; out.Y.z = 0;
    out.Z.x = 0; out.Z.y = 0; out.Z.z = 1;

    return out;
}

INLINE vec3 getVec3Of(f32 value) {
    vec3 out;

    out.x = out.y = out.z = value;

    return out;
}

INLINE vec3 invertedVec3(vec3 in) {
    vec3 out;

    out.x = -in.x;
    out.y = -in.y;
    out.z = -in.z;

    return out;
}

INLINE vec3 oneOverVec3(vec3 v) {
    vec3 out;

    out.x = 1.0f / v.x;
    out.y = 1.0f / v.y;
    out.z = 1.0f / v.z;

    return out;
}

INLINE f32 approach(f32 src, f32 trg, f32 diff) {
    f32 out;

    out = src + diff; if (trg > out) return out;
    out = src - diff; if (trg < out) return out;

    return trg;
}

INLINE vec3 approachVec3(vec3 src, vec3 trg, f32 diff) {
    vec3 out;

    out.x = approach(src.x, trg.x, diff);
    out.y = approach(src.y, trg.y, diff);
    out.z = approach(src.z, trg.z, diff);

    return out;
}

INLINE bool nonZeroVec3(vec3 v) {
    return v.x != 0 ||
           v.y != 0 ||
           v.z != 0;
}

INLINE vec3 minVec3(vec3 a, vec3 b) {
    vec3 out;

    out.x = a.x < b.x ? a.x : b.x;
    out.y = a.y < b.y ? a.y : b.y;
    out.z = a.z < b.z ? a.z : b.z;

    return out;
}

INLINE vec3 maxVec3(vec3 a, vec3 b) {
    vec3 out;

    out.x = a.x > b.x ? a.x : b.x;
    out.y = a.y > b.y ? a.y : b.y;
    out.z = a.z > b.z ? a.z : b.z;

    return out;
}

INLINE f32 minCoordVec3(vec3 v) {
    f32 out = v.x;
    if (v.y < out) out = v.y;
    if (v.z < out) out = v.z;
    return out;
}

INLINE f32 maxCoordVec3(vec3 v) {
    f32 out = v.x;
    if (v.y > out) out = v.y;
    if (v.z > out) out = v.z;
    return out;
}

INLINE vec2 getPointOnUnitCircle(f32 t) {
    f32 t_squared = t * t;
    f32 factor = 1 / (1 + t_squared);

    vec2 xy = {factor - factor * t_squared, factor * 2 * t};

    return xy;
}

INLINE vec3 setPointOnUnitSphere(f32 s, f32 t) {
    vec3 out;

    f32 t_squared = t * t;
    f32 s_squared = s * s;
    f32 factor = 1 / ( t_squared + s_squared + 1);

    out.x = 2*s * factor;
    out.y = 2*t * factor;
    out.z = (t_squared + s_squared - 1) * t_squared;

    return out;
}

INLINE vec3 subVec3(vec3 a, vec3 b) {
    vec3 out;

    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;

    return out;
}

INLINE vec3 addVec3(vec3 a, vec3 b) {
    vec3 out;

    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;

    return out;
}

INLINE vec3 mulVec3(vec3 a, vec3 b) {
    vec3 out;

    out.x = a.x * b.x;
    out.y = a.y * b.y;
    out.z = a.z * b.z;

    return out;
}

INLINE vec3 mulAddVec3(vec3 v, vec3 factors, vec3 to_be_added) {
    vec3 out;

    out.x = fast_mul_add(v.x, factors.x, to_be_added.x);
    out.y = fast_mul_add(v.y, factors.y, to_be_added.y);
    out.z = fast_mul_add(v.z, factors.z, to_be_added.z);

    return out;
}

INLINE vec3 scaleAddVec3(vec3 v, f32 factor, vec3 to_be_added) {
    vec3 out;

    out.x = fast_mul_add(v.x, factor, to_be_added.x);
    out.y = fast_mul_add(v.y, factor, to_be_added.y);
    out.z = fast_mul_add(v.z, factor, to_be_added.z);

    return out;
}

INLINE vec3 scaleVec3(vec3 a, f32 factor) {
    vec3 out;

    out.x = a.x * factor;
    out.y = a.y * factor;
    out.z = a.z * factor;

    return out;
}

INLINE vec3 mulVec3Mat3(vec3 in, mat3 m) {
    vec3 out;

    out.x = in.x * m.X.x + in.y * m.Y.x + in.z * m.Z.x;
    out.y = in.x * m.X.y + in.y * m.Y.y + in.z * m.Z.y;
    out.z = in.x * m.X.z + in.y * m.Y.z + in.z * m.Z.z;

    return out;
}

INLINE f32 dotVec3(vec3 a, vec3 b) {
    return (
            (a.x * b.x) +
            (a.y * b.y) +
            (a.z * b.z)
    );
}

INLINE vec3 crossVec3(vec3 a, vec3 b) {
    vec3 out;

    out.x = (a.y * b.z) - (a.z * b.y);
    out.y = (a.z * b.x) - (a.x * b.z);
    out.z = (a.x * b.y) - (a.y * b.x);

    return out;
}

INLINE f32 squaredLengthVec3(vec3 v) {
    return (
            (v.x * v.x) +
            (v.y * v.y) +
            (v.z * v.z)
    );
}

INLINE f32 lengthVec3(vec3 v) {
    return sqrtf(squaredLengthVec3(v));
}

INLINE vec3 normVec3(vec3 v) {
    return scaleVec3(v, 1.0f / length(v));
}

INLINE f32 clampValue(f32 value) {
    f32 mn = value < 1.0f ? value : 1.0f;
    return mn > 0.0f ? mn : 0.0f;
}
INLINE f32 DotVec3(vec3 a, vec3 b) { return clampValue(dot(a, b)); }

INLINE mat3 outerVec3(vec3 a, vec3 b) {
    mat3 out;

    out.X = scaleVec3(a, b.x);
    out.Y = scaleVec3(a, b.y);
    out.Z = scaleVec3(a, b.z);

    return out;
}

INLINE vec3 reflectVec3(vec3 V, vec3 N) {
    vec3 out = scaleVec3(N, -2 * dot(N, V));
    out = addVec3(out, V);
    return out;
}

INLINE mat3 addMat3(mat3 a, mat3 b) {
    mat3 out;

    out.X = addVec3(a.X, b.X);
    out.Y = addVec3(a.Y, b.Y);
    out.Z = addVec3(a.Z, b.Z);

    return out;
}

INLINE mat3 scaleMat3(mat3 m, f32 factor) {
    mat3 out;

    out.X = scaleVec3(m.X, factor);
    out.Y = scaleVec3(m.Y, factor);
    out.Z = scaleVec3(m.Z, factor);

    return out;
}

INLINE mat3 transposedMat3(mat3 m) {
    mat3 out;

    out.X.x = m.X.x;  out.X.y = m.Y.x;  out.X.z = m.Z.x;
    out.Y.x = m.X.y;  out.Y.y = m.Y.y;  out.Y.z = m.Z.y;
    out.Z.x = m.X.z;  out.Z.y = m.Y.z;  out.Z.z = m.Z.z;

    return out;
}

INLINE mat3 mulMat3(mat3 a, mat3 b) {
    mat3 out;

    out.X.x = a.X.x*b.X.x + a.X.y*b.Y.x + a.X.z*b.Z.x; // Row 1 | Column 1
    out.X.y = a.X.x*b.X.y + a.X.y*b.Y.y + a.X.z*b.Z.y; // Row 1 | Column 2
    out.X.z = a.X.x*b.X.z + a.X.y*b.Y.z + a.X.z*b.Z.z; // Row 1 | Column 3

    out.Y.x = a.Y.x*b.X.x + a.Y.y*b.Y.x + a.Y.z*b.Z.x; // Row 2 | Column 1
    out.Y.y = a.Y.x*b.X.y + a.Y.y*b.Y.y + a.Y.z*b.Z.y; // Row 2 | Column 2
    out.Y.z = a.Y.x*b.X.z + a.Y.y*b.Y.z + a.Y.z*b.Z.z; // Row 2 | Column 3

    out.Z.x = a.Z.x*b.X.x + a.Z.y*b.Y.x + a.Z.z*b.Z.x; // Row 3 | Column 1
    out.Z.y = a.Z.x*b.X.y + a.Z.y*b.Y.y + a.Z.z*b.Z.y; // Row 3 | Column 2
    out.Z.z = a.Z.x*b.X.z + a.Z.y*b.Y.z + a.Z.z*b.Z.z; // Row 3 | Column 3

    return out;
}

INLINE mat3 invMat3(mat3 m) {
    mat3 out;

    f32 one_over_determinant = 1.0f / (
            + m.X.x * (m.Y.y * m.Z.z - m.Z.y * m.Y.z)
            - m.Y.x * (m.X.y * m.Z.z - m.Z.y * m.X.z)
            + m.Z.x * (m.X.y * m.Y.z - m.Y.y * m.X.z)
    );

    out.X.x = + (m.Y.y * m.Z.z - m.Z.y * m.Y.z) * one_over_determinant;
    out.Y.x = - (m.Y.x * m.Z.z - m.Z.x * m.Y.z) * one_over_determinant;
    out.Z.x = + (m.Y.x * m.Z.y - m.Z.x * m.Y.y) * one_over_determinant;
    out.X.y = - (m.X.y * m.Z.z - m.Z.y * m.X.z) * one_over_determinant;
    out.Y.y = + (m.X.x * m.Z.z - m.Z.x * m.X.z) * one_over_determinant;
    out.Z.y = - (m.X.x * m.Z.y - m.Z.x * m.X.y) * one_over_determinant;
    out.X.z = + (m.X.y * m.Y.z - m.Y.y * m.X.z) * one_over_determinant;
    out.Y.z = - (m.X.x * m.Y.z - m.Y.x * m.X.z) * one_over_determinant;
    out.Z.z = + (m.X.x * m.Y.y - m.Y.x * m.X.y) * one_over_determinant;

    return out;
}

INLINE bool safeInvertMat3(mat3 *m) {
    f32 m11 = m->X.x,  m12 = m->X.y,  m13 = m->X.z,
            m21 = m->Y.x,  m22 = m->Y.y,  m23 = m->Y.z,
            m31 = m->Z.x,  m32 = m->Z.y,  m33 = m->Z.z,

            c11 = m22*m33 -
                  m23*m32,

            c12 = m13*m32 -
                  m12*m33,

            c13 = m12*m23 -
                  m13*m22,


            c21 = m23*m31 -
                  m21*m33,

            c22 = m11*m33 -
                  m13*m31,

            c23 = m13*m21 -
                  m11*m23,


            c31 = m21*m32 -
                  m22*m31,

            c32 = m12*m31 -
                  m11*m32,

            c33 = m11*m22 -
                  m12*m21,

            d = c11 + c12 + c13 +
                c21 + c22 + c23 +
                c31 + c32 + c33;

    if (!d) return false;

    d = 1 / d;

    m->X.x = d * c11;  m->X.y = d * c12;  m->X.z = d * c13;
    m->Y.x = d * c21;  m->Y.y = d * c22;  m->Y.z = d * c23;
    m->Z.x = d * c31;  m->Z.y = d * c32;  m->Z.z = d * c33;

    return true;
}

INLINE void yawMat3(f32 amount, mat3* out) {
    vec2 xy = getPointOnUnitCircle(amount);

    vec3 X = out->X;
    vec3 Y = out->Y;
    vec3 Z = out->Z;

    out->X.x = xy.x * X.x - xy.y * X.z;
    out->Y.x = xy.x * Y.x - xy.y * Y.z;
    out->Z.x = xy.x * Z.x - xy.y * Z.z;

    out->X.z = xy.x * X.z + xy.y * X.x;
    out->Y.z = xy.x * Y.z + xy.y * Y.x;
    out->Z.z = xy.x * Z.z + xy.y * Z.x;
}

INLINE void pitchMat3(f32 amount, mat3* out) {
    vec2 xy = getPointOnUnitCircle(amount);

    vec3 X = out->X;
    vec3 Y = out->Y;
    vec3 Z = out->Z;

    out->X.y = xy.x * X.y + xy.y * X.z;
    out->Y.y = xy.x * Y.y + xy.y * Y.z;
    out->Z.y = xy.x * Z.y + xy.y * Z.z;

    out->X.z = xy.x * X.z - xy.y * X.y;
    out->Y.z = xy.x * Y.z - xy.y * Y.y;
    out->Z.z = xy.x * Z.z - xy.y * Z.y;
}

INLINE void rollMat3(f32 amount, mat3* out) {
    vec2 xy = getPointOnUnitCircle(amount);

    vec3 X = out->X;
    vec3 Y = out->Y;
    vec3 Z = out->Z;

    out->X.x = xy.x * X.x + xy.y * X.y;
    out->Y.x = xy.x * Y.x + xy.y * Y.y;
    out->Z.x = xy.x * Z.x + xy.y * Z.y;

    out->X.y = xy.x * X.y - xy.y * X.x;
    out->Y.y = xy.x * Y.y - xy.y * Y.x;
    out->Z.y = xy.x * Z.y - xy.y * Z.x;
}

INLINE void reciprocateVec3(vec3 *orig, vec3 *rcp) {
    rcp->x = 1.0f / orig->x;
    rcp->y = 1.0f / orig->y;
    rcp->z = 1.0f / orig->z;
}

INLINE void setYawMat3(f32 yaw, mat3* yaw_matrix) {
    vec2 xy = getPointOnUnitCircle(yaw);

    yaw_matrix->X.x = yaw_matrix->Z.z = xy.x;
    yaw_matrix->X.z = +xy.y;
    yaw_matrix->Z.x = -xy.y;
}

INLINE void setPitchMat3(f32 pitch, mat3* pitch_matrix) {
    vec2 xy = getPointOnUnitCircle(pitch);

    pitch_matrix->Z.z = pitch_matrix->Y.y = xy.x;
    pitch_matrix->Y.z = -xy.y;
    pitch_matrix->Z.y = +xy.y;
}

INLINE void setRollMat3(f32 roll, mat3* roll_matrix) {
    vec2 xy = getPointOnUnitCircle(roll);

    roll_matrix->X.x = roll_matrix->Y.y = xy.x;
    roll_matrix->X.y = -xy.y;
    roll_matrix->Y.x = +xy.y;
}

INLINE mat4 getMat4Identity() {
    mat4 out;

    out.X.x = 1; out.X.y = 0; out.X.z = 0; out.X.w = 0;
    out.Y.x = 0; out.Y.y = 1; out.Y.z = 0; out.Y.w = 0;
    out.Z.x = 0; out.Z.y = 0; out.Z.z = 1; out.Z.w = 0;
    out.W.x = 0; out.W.y = 0; out.W.z = 0; out.W.w = 1;

    return out;
}

INLINE quat getIdentityQuaternion() {
    quat out;

    out.axis = getVec3Of(0);
    out.amount = 1;

    return out;
}

INLINE vec4 getVec4Of(f32 value) {
    vec4 out;

    out.x = out.y = out.z = out.w = value;

    return out;
}

INLINE vec4 invertedVec4(vec4 in) {
    vec4 out;

    out.x = -in.x;
    out.y = -in.y;
    out.z = -in.z;
    out.w = -in.w;

    return out;
}

INLINE vec4 approachVec4(vec4 src, vec4 trg, f32 diff) {
    vec4 out;

    out.x = approach(src.x, trg.x, diff);
    out.y = approach(src.y, trg.y, diff);
    out.z = approach(src.z, trg.z, diff);
    out.w = approach(src.w, trg.w, diff);

    return out;
}

INLINE bool nonZeroVec4(vec4 v) {
    return v.x != 0 ||
           v.y != 0 ||
           v.z != 0 ||
           v.w != 0;
}

INLINE vec4 subVec4(vec4 a, vec4 b) {
    vec4 out;

    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
    out.w = a.w - b.w;

    return out;
}

INLINE vec4 addVec4(vec4 a, vec4 b) {
    vec4 out;

    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;
    out.w = a.w + b.w;

    return out;
}

INLINE vec4 mulVec4(vec4 a, vec4 b) {
    vec4 out;

    out.x = a.x * b.x;
    out.y = a.y * b.y;
    out.z = a.z * b.z;
    out.w = a.w * b.w;

    return out;
}

INLINE vec4 scaleVec4(vec4 a, f32 factor) {
    vec4 out;

    out.x = a.x * factor;
    out.y = a.y * factor;
    out.z = a.z * factor;
    out.w = a.w * factor;

    return out;
}

INLINE vec4 mulVec4Mat4(vec4 in, mat4 m) {
    vec4 out;

    out.x = in.x * m.X.x + in.y * m.Y.x + in.z * m.Z.x + in.w * m.W.x;
    out.y = in.x * m.X.y + in.y * m.Y.y + in.z * m.Z.y + in.w * m.W.y;
    out.z = in.x * m.X.z + in.y * m.Y.z + in.z * m.Z.z + in.w * m.W.z;
    out.w = in.x * m.X.w + in.y * m.Y.w + in.z * m.Z.w + in.w * m.W.w;

    return out;
}

INLINE f32 dotVec4(vec4 a, vec4 b) {
    return (
            (a.x * b.x) +
            (a.y * b.y) +
            (a.z * b.z) +
            (a.w * b.w)
    );
}

INLINE f32 squaredLengthVec4(vec4 v) {
    return (
            (v.x * v.x) +
            (v.y * v.y) +
            (v.z * v.z) +
            (v.w * v.w)
    );
}

INLINE f32 lengthVec4(vec4 v) {
    return sqrtf(squaredLengthVec4(v));
}

INLINE vec4 norm4(vec4 v) {
    return scaleVec4(v, 1.0f / lengthVec4(v));
}

INLINE mat4 transposeMat4(mat4 m) {
    mat4 out;

    out.X.x = m.X.x;  out.X.y = m.Y.x;  out.X.z = m.Z.x;  out.X.w = m.W.x;
    out.Y.x = m.X.y;  out.Y.y = m.Y.y;  out.Y.z = m.Z.y;  out.Y.w = m.W.y;
    out.Z.x = m.X.z;  out.Z.y = m.Y.z;  out.Z.z = m.Z.z;  out.Z.w = m.W.z;
    out.W.x = m.X.w;  out.W.y = m.Y.w;  out.W.z = m.Z.w;  out.W.w = m.W.w;

    return out;
}

INLINE mat4 scaleMat4(mat4 m, f32 factor) {
    mat4 out;

    out.X = scaleVec4(m.X, factor);
    out.Y = scaleVec4(m.Y, factor);
    out.Z = scaleVec4(m.Z, factor);
    out.W = scaleVec4(m.W, factor);

    return out;
}

INLINE mat4 mulMat4(mat4 a, mat4 b) {
    mat4 out;

    out.X.x = a.X.x*b.X.x + a.X.y*b.Y.x + a.X.z*b.Z.x + a.X.w*b.W.x; // Row 1 | Column 1
    out.X.y = a.X.x*b.X.y + a.X.y*b.Y.y + a.X.z*b.Z.y + a.X.w*b.W.y; // Row 1 | Column 2
    out.X.z = a.X.x*b.X.z + a.X.y*b.Y.z + a.X.z*b.Z.z + a.X.w*b.W.z; // Row 1 | Column 3
    out.X.w = a.X.x*b.X.w + a.X.y*b.Y.w + a.X.z*b.Z.w + a.X.w*b.W.w; // Row 1 | Column 4

    out.Y.x = a.Y.x*b.X.x + a.Y.y*b.Y.x + a.Y.z*b.Z.x + a.Y.w*b.W.x; // Row 2 | Column 1
    out.Y.y = a.Y.x*b.X.y + a.Y.y*b.Y.y + a.Y.z*b.Z.y + a.Y.w*b.W.y; // Row 2 | Column 2
    out.Y.z = a.Y.x*b.X.z + a.Y.y*b.Y.z + a.Y.z*b.Z.z + a.Y.w*b.W.z; // Row 2 | Column 3
    out.Y.w = a.Y.x*b.X.w + a.Y.y*b.Y.w + a.Y.z*b.Z.w + a.Y.w*b.W.w; // Row 2 | Column 4

    out.Z.x = a.Z.x*b.X.x + a.Z.y*b.Y.x + a.Z.z*b.Z.x + a.Z.w*b.W.x; // Row 3 | Column 1
    out.Z.y = a.Z.x*b.X.y + a.Z.y*b.Y.y + a.Z.z*b.Z.y + a.Z.w*b.W.y; // Row 3 | Column 2
    out.Z.z = a.Z.x*b.X.z + a.Z.y*b.Y.z + a.Z.z*b.Z.z + a.Z.w*b.W.z; // Row 3 | Column 3
    out.Z.w = a.Z.x*b.X.w + a.Z.y*b.Y.w + a.Z.z*b.Z.w + a.Z.w*b.W.w; // Row 3 | Column 4

    out.W.x = a.W.x*b.X.x + a.W.y*b.Y.x + a.W.z*b.Z.x + a.W.w*b.W.x; // Row 4 | Column 1
    out.W.y = a.W.x*b.X.y + a.W.y*b.Y.y + a.W.z*b.Z.y + a.W.w*b.W.y; // Row 4 | Column 2
    out.W.z = a.W.x*b.X.z + a.W.y*b.Y.z + a.W.z*b.Z.z + a.W.w*b.W.z; // Row 4 | Column 3
    out.W.w = a.W.x*b.X.w + a.W.y*b.Y.w + a.W.z*b.Z.w + a.W.w*b.W.w; // Row 4 | Column 4

    return out;
}

INLINE mat4 invMat4(mat4 m) {
    mat4 out;

    f32 m11 = m.X.x,  m12 = m.X.y,  m13 = m.X.z, m14 = m.X.w,
            m21 = m.Y.x,  m22 = m.Y.y,  m23 = m.Y.z, m24 = m.Y.w,
            m31 = m.Z.x,  m32 = m.Z.y,  m33 = m.Z.z, m34 = m.Z.w,
            m41 = m.W.x,  m42 = m.W.y,  m43 = m.W.z, m44 = m.W.w;

    out.X.x = +m22*m33*m44 - m22*m34*m43 - m32*m23*m44 + m32*m24*m43 + m42*m23*m34 - m42*m24*m33;
    out.X.y = -m12*m33*m44 + m12*m34*m43 + m32*m13*m44 - m32*m14*m43 - m42*m13*m34 + m42*m14*m33;
    out.X.z = +m12*m23*m44 - m12*m24*m43 - m22*m13*m44 + m22*m14*m43 + m42*m13*m24 - m42*m14*m23;
    out.X.w = -m12*m23*m34 + m12*m24*m33 + m22*m13*m34 - m22*m14*m33 - m32*m13*m24 + m32*m14*m23;

    out.Y.x = -m21*m33*m44 + m21*m34*m43 + m31*m23*m44 - m31*m24*m43 - m41*m23*m34 + m41*m24*m33;
    out.Y.y = +m11*m33*m44 - m11*m34*m43 - m31*m13*m44 + m31*m14*m43 + m41*m13*m34 - m41*m14*m33;
    out.Y.z = -m11*m23*m44 + m11*m24*m43 + m21*m13*m44 - m21*m14*m43 - m41*m13*m24 + m41*m14*m23;
    out.Y.w = +m11*m23*m34 - m11*m24*m33 - m21*m13*m34 + m21*m14*m33 + m31*m13*m24 - m31*m14*m23;

    out.Z.x = +m21*m32*m44 - m21*m34*m42 - m31*m22*m44 + m31*m24*m42 + m41*m22*m34 - m41*m24*m32;
    out.Z.y = -m11*m32*m44 + m11*m34*m42 + m31*m12*m44 - m31*m14*m42 - m41*m12*m34 + m41*m14*m32;
    out.Z.z = +m11*m22*m44 - m11*m24*m42 - m21*m12*m44 + m21*m14*m42 + m41*m12*m24 - m41*m14*m22;
    out.Z.w = -m11*m22*m34 + m11*m24*m32 + m21*m12*m34 - m21*m14*m32 - m31*m12*m24 + m31*m14*m22;

    out.W.x = -m21*m32*m43 + m21*m33*m42 + m31*m22*m43 - m31*m23*m42 - m41*m22*m33 + m41*m23*m32;
    out.W.y = +m11*m32*m43 - m11*m33*m42 - m31*m12*m43 + m31*m13*m42 + m41*m12*m33 - m41*m13*m32;
    out.W.z = -m11*m22*m43 + m11*m23*m42 + m21*m12*m43 - m21*m13*m42 - m41*m12*m23 + m41*m13*m22;
    out.W.w = +m11*m22*m33 - m11*m23*m32 - m21*m12*m33 + m21*m13*m32 + m31*m12*m23 - m31*m13*m22;

    f32 det = m11*out.X.x + m12*out.Y.x + m13*out.Z.x + m14*out.W.x;
    if (!det) return m;

    out = scaleMat4(out, 1.0f / det);

    return out;
}

INLINE void yawMat4(f32 amount, mat4 *out) {
    vec2 xy = getPointOnUnitCircle(amount);

    vec4 X = out->X;
    vec4 Y = out->Y;
    vec4 Z = out->Z;

    out->X.x = xy.x * X.x - xy.y * X.z;
    out->Y.x = xy.x * Y.x - xy.y * Y.z;
    out->Z.x = xy.x * Z.x - xy.y * Z.z;

    out->X.z = xy.x * X.z + xy.y * X.x;
    out->Y.z = xy.x * Y.z + xy.y * Y.x;
    out->Z.z = xy.x * Z.z + xy.y * Z.x;
}

INLINE void pitchMat4(f32 amount, mat4 *out) {
    vec2 xy = getPointOnUnitCircle(amount);

    vec4 X = out->X;
    vec4 Y = out->Y;
    vec4 Z = out->Z;

    out->X.y = xy.x * X.y + xy.y * X.z;
    out->Y.y = xy.x * Y.y + xy.y * Y.z;
    out->Z.y = xy.x * Z.y + xy.y * Z.z;

    out->X.z = xy.x * X.z - xy.y * X.y;
    out->Y.z = xy.x * Y.z - xy.y * Y.y;
    out->Z.z = xy.x * Z.z - xy.y * Z.y;
}

INLINE void rollMat4(f32 amount, mat4 *out) {
    vec2 xy = getPointOnUnitCircle(amount);

    vec4 X = out->X;
    vec4 Y = out->Y;
    vec4 Z = out->Z;

    out->X.x = xy.x * X.x + xy.y * X.y;
    out->Y.x = xy.x * Y.x + xy.y * Y.y;
    out->Z.x = xy.x * Z.x + xy.y * Z.y;

    out->X.y = xy.x * X.y - xy.y * X.x;
    out->Y.y = xy.x * Y.y - xy.y * Y.x;
    out->Z.y = xy.x * Z.y - xy.y * Z.x;
}

INLINE void setYawMat4(f32 yaw, mat4 *yaw_matrix) {
    vec2 xy = getPointOnUnitCircle(yaw);

    yaw_matrix->X.x = yaw_matrix->Z.z = xy.x;
    yaw_matrix->X.z = +xy.y;
    yaw_matrix->Z.x = -xy.y;
}

INLINE void setPitchMat4(f32 pitch, mat4 *pitch_matrix) {
    vec2 xy = getPointOnUnitCircle(pitch);

    pitch_matrix->Z.z = pitch_matrix->Y.y = xy.x;
    pitch_matrix->Y.z = -xy.y;
    pitch_matrix->Z.y = +xy.y;
}

INLINE void setRollMat4(f32 roll, mat4 *roll_matrix) {
    vec2 xy = getPointOnUnitCircle(roll);

    roll_matrix->X.x = roll_matrix->Y.y = xy.x;
    roll_matrix->X.y = -xy.y;
    roll_matrix->Y.x = +xy.y;
}

INLINE quat normQuat(quat q) {
    quat out;

    f32 factor = 1.0f / sqrtf(q.axis.x * q.axis.x + q.axis.y * q.axis.y + q.axis.z * q.axis.z + q.amount * q.amount);
    out.axis = scaleVec3(q.axis, factor);
    out.amount = q.amount * factor;

    return out;
}

INLINE vec3 mulVec3Quat(const vec3 v, quat q) {
    vec3 out = cross(q.axis, v);
    vec3 qqv = cross(q.axis, out);
    out = scaleAddVec3(out, q.amount, qqv);
    out = scaleAddVec3(out, 2, v);
    return out;
}

INLINE quat mulQuat(quat a, quat b) {
    quat out;

    out.amount = a.amount * b.amount - a.axis.x * b.axis.x - a.axis.y * b.axis.y - a.axis.z * b.axis.z;
    out.axis.x = a.amount * b.axis.x + a.axis.x * b.amount + a.axis.y * b.axis.z - a.axis.z * b.axis.y;
    out.axis.y = a.amount * b.axis.y - a.axis.x * b.axis.z + a.axis.y * b.amount + a.axis.z * b.axis.x;
    out.axis.z = a.amount * b.axis.z + a.axis.x * b.axis.y - a.axis.y * b.axis.x + a.axis.z * b.amount;

    return out;
}


INLINE quat conjugate(quat q) {
    quat out;

    out.amount = q.amount;
    out.axis = invertedVec3(q.axis);

    return out;
}

INLINE quat convertRotationMatrixToQuaternion(mat3 rotation_matrix) {
    quat out;

    f32 fourXSquaredMinus1 = rotation_matrix.X.x - rotation_matrix.Y.y - rotation_matrix.Z.z;
    f32 fourYSquaredMinus1 = rotation_matrix.Y.y - rotation_matrix.X.x - rotation_matrix.Z.z;
    f32 fourZSquaredMinus1 = rotation_matrix.Z.z - rotation_matrix.X.x - rotation_matrix.Y.y;
    f32 fourWSquaredMinus1 = rotation_matrix.X.x + rotation_matrix.Y.y + rotation_matrix.Z.z;

    int biggestIndex = 0;
    f32 fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    f32 biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
    f32 mult = 0.25f / biggestVal;

    switch(biggestIndex) {
        case 0:
            out.amount = biggestVal;
            out.axis.x = (rotation_matrix.Y.z - rotation_matrix.Z.y) * mult;
            out.axis.y = (rotation_matrix.Z.x - rotation_matrix.X.z) * mult;
            out.axis.z = (rotation_matrix.X.y - rotation_matrix.Y.x) * mult;
            break;
        case 1:
            out.amount = (rotation_matrix.Y.z - rotation_matrix.Z.y) * mult;
            out.axis.x = biggestVal;
            out.axis.y = (rotation_matrix.X.y + rotation_matrix.Y.x) * mult;
            out.axis.z = (rotation_matrix.Z.x + rotation_matrix.X.z) * mult;
            break;
        case 2:
            out.amount = (rotation_matrix.Z.x - rotation_matrix.X.z) * mult;
            out.axis.x = (rotation_matrix.X.y + rotation_matrix.Y.x) * mult;
            out.axis.y = biggestVal;
            out.axis.z = (rotation_matrix.Y.z + rotation_matrix.Z.y) * mult;
            break;
        case 3:
            out.amount = (rotation_matrix.X.y - rotation_matrix.Y.x) * mult;
            out.axis.x = (rotation_matrix.Z.x + rotation_matrix.X.z) * mult;
            out.axis.y = (rotation_matrix.Y.z + rotation_matrix.Z.y) * mult;
            out.axis.z = biggestVal;
            break;
    }

    return out;
}

INLINE mat3 convertQuaternionToRotationMatrix(quat q) {
    mat3 out;

    f32 q0 = q.amount;
    f32 q1 = q.axis.x;
    f32 q2 = q.axis.y;
    f32 q3 = q.axis.z;

    out.X.x = 2 * (q0 * q0 + q1 * q1) - 1;
    out.X.y = 2 * (q1 * q2 - q0 * q3);
    out.X.z = 2 * (q1 * q3 + q0 * q2);

    out.Y.x = 2 * (q1 * q2 + q0 * q3);
    out.Y.y = 2 * (q0 * q0 + q2 * q2) - 1;
    out.Y.z = 2 * (q2 * q3 - q0 * q1);

    out.Z.x = 2 * (q1 * q3 - q0 * q2);
    out.Z.y = 2 * (q2 * q3 + q0 * q1);
    out.Z.z = 2 * (q0 * q0 + q3 * q3) - 1;

    return out;
}

INLINE quat getRotationAroundAxis(vec3 axis, f32 amount) {
    vec2 sin_cos = getPointOnUnitCircle(amount);
    quat out;
    out.axis = scaleVec3(axis, sin_cos.y);
    out.amount = sin_cos.x;

    return normQuat(out);
}

INLINE quat rotateAroundAxis(quat q, vec3 axis, f32 amount) {
    quat rotation = getRotationAroundAxis(axis, amount);
    return mulQuat(q, rotation);
}

typedef struct xform3 {
    mat3 matrix,
            yaw_matrix,
            pitch_matrix,
            roll_matrix,
            rotation_matrix,
            rotation_matrix_inverted;
    vec3 position,
            *up_direction,
            *right_direction,
            *forward_direction;
} xform3;

void initXform3(xform3 *xform) {
    mat3 I = getMat3Identity();
    xform->matrix = xform->yaw_matrix = xform->pitch_matrix = xform->roll_matrix = xform->rotation_matrix = xform->rotation_matrix_inverted = I;
    xform->right_direction   = &xform->rotation_matrix.X;
    xform->up_direction      = &xform->rotation_matrix.Y;
    xform->forward_direction = &xform->rotation_matrix.Z;
    xform->position = getVec3Of(0);
}

INLINE void rotateXform3(xform3 *xform, f32 yaw, f32 pitch, f32 roll) {
    if (yaw)   yawMat3(  yaw,   &xform->yaw_matrix);
    if (pitch) pitchMat3(pitch, &xform->pitch_matrix);
    if (roll)  rollMat3( roll,  &xform->roll_matrix);

    xform->rotation_matrix = mulMat3(mulMat3(xform->pitch_matrix, xform->yaw_matrix), xform->roll_matrix);
    xform->rotation_matrix_inverted = transposedMat3(xform->rotation_matrix);

    xform->matrix = mulMat3(xform->matrix, xform->rotation_matrix);
}

#define CAMERA_DEFAULT__FOCAL_LENGTH 2.0f
#define CAMERA_DEFAULT__TARGET_DISTANCE 10.0f

typedef struct Camera {
    f32 focal_length, zoom, dolly, target_distance;
    xform3 transform;
    vec3 current_velocity;
    bool moved, turned, zoomed;
} Camera;

typedef struct CameraSettings {
    f32 focal_length, target_distance;
} CameraSettings;

CameraSettings getDefaultCameraSettings() {
    CameraSettings default_camera_settings;

    default_camera_settings.focal_length    = CAMERA_DEFAULT__FOCAL_LENGTH;
    default_camera_settings.target_distance = CAMERA_DEFAULT__TARGET_DISTANCE;

    return default_camera_settings;
}

void initCamera(Camera* camera, CameraSettings camera_settings) {
    initXform3(&camera->transform);
    camera->zoom = camera_settings.focal_length;
    camera->focal_length = camera_settings.focal_length;
    camera->target_distance = camera_settings.target_distance;
    camera->dolly = 0;
    camera->turned = false;
    camera->moved = false;
    camera->zoomed = false;
    camera->current_velocity = getVec3Of(0);
}

void zoomCamera(Camera *camera, f32 zoom) {
    f32 new_zoom = camera->zoom + zoom;
    camera->focal_length = new_zoom > 1 ? new_zoom : (new_zoom < -1 ? (-1 / new_zoom) : 1);
    camera->zoom = new_zoom;
    camera->zoomed = true;
}

void dollyCamera(Camera *camera, f32 dolly, f32 max_distance) {
    vec3 target_position = scaleVec3(*camera->transform.forward_direction, camera->target_distance);
    target_position = addVec3(camera->transform.position, target_position);

    // Compute new target distance:
    camera->dolly += dolly;
    camera->target_distance = powf(2, camera->dolly / -200) * max_distance;

    // Back-track from target position to new current position:
    camera->transform.position = scaleVec3(*camera->transform.forward_direction, camera->target_distance);
    camera->transform.position = subVec3(target_position,camera->transform.position);
    camera->moved = true;
}

void turnCamera(Camera *camera, f32 yaw, f32 pitch) {
    rotateXform3(&camera->transform, yaw, pitch, 0);
    camera->turned = true;
}

void orbitCamera(Camera *camera, f32 azimuth, f32 altitude) {
    // Move the camera forward to the position of it's target:
    vec3 movement = scaleVec3(*camera->transform.forward_direction, camera->target_distance);
    camera->transform.position = addVec3(camera->transform.position, movement);

    // Reorient the camera while it is at the position of it's target:
    turnCamera(camera, azimuth, altitude);

    // Back the camera away from it's target position using the updated forward direction:
    movement = scaleVec3(*camera->transform.forward_direction, camera->target_distance);
    camera->transform.position = subVec3(camera->transform.position, movement);
    camera->moved = true;
}

void panCamera(Camera *camera, f32 right, f32 up) {
    vec3 right_movement = scaleVec3(*camera->transform.right_direction, right);
    vec3 up_movement    = scaleVec3(*camera->transform.up_direction, up);
    camera->transform.position = addVec3(camera->transform.position, up_movement);
    camera->transform.position = addVec3(camera->transform.position, right_movement);
    camera->moved = true;
}

#define NAVIGATION_DEFAULT__MAX_VELOCITY 8.0f
#define NAVIGATION_DEFAULT__ACCELERATION 30.0f
#define NAVIGATION_SPEED_DEFAULT__TURN 2.0f
#define NAVIGATION_SPEED_DEFAULT__ORIENT 0.002f
#define NAVIGATION_SPEED_DEFAULT__ORBIT 0.001f
#define NAVIGATION_SPEED_DEFAULT__ZOOM 0.002f
#define NAVIGATION_SPEED_DEFAULT__DOLLY_ 1.0f
#define NAVIGATION_SPEED_DEFAULT__PAN 0.02f

typedef struct NavigationSpeedSettings {
    f32 turn, zoom, dolly, pan, orbit, orient;
} NavigationSpeedSettings;

typedef struct NavigationSettings {
    NavigationSpeedSettings speeds;
    f32 max_velocity, acceleration;
} NavigationSettings;

NavigationSettings getDefaultNavigationSettings() {
    NavigationSettings navigation_settings;

    navigation_settings.max_velocity  = NAVIGATION_DEFAULT__MAX_VELOCITY;
    navigation_settings.acceleration  = NAVIGATION_DEFAULT__ACCELERATION;
    navigation_settings.speeds.turn   = NAVIGATION_SPEED_DEFAULT__TURN;
    navigation_settings.speeds.orient = NAVIGATION_SPEED_DEFAULT__ORIENT;
    navigation_settings.speeds.orbit  = NAVIGATION_SPEED_DEFAULT__ORBIT;
    navigation_settings.speeds.zoom   = NAVIGATION_SPEED_DEFAULT__ZOOM;
    navigation_settings.speeds.dolly  = NAVIGATION_SPEED_DEFAULT__DOLLY_;
    navigation_settings.speeds.pan    = NAVIGATION_SPEED_DEFAULT__PAN;

    return navigation_settings;
}

typedef struct NavigationTurn {
    bool right, left;
} NavigationTurn;

typedef struct NavigationMove {
    bool right, left, up, down, forward, backward;
} NavigationMove;

typedef struct Navigation {
    NavigationMove move;
    NavigationTurn turn;
    NavigationSettings settings;
} Navigation;

void initNavigation(Navigation *navigation, NavigationSettings navigation_settings) {
    navigation->settings = navigation_settings;

    navigation->move.right = false;
    navigation->move.left = false;
    navigation->move.up = false;
    navigation->move.down = false;
    navigation->move.forward = false;
    navigation->move.backward = false;

    navigation->turn.right = false;
    navigation->turn.left = false;
}

void navigateCamera(Camera *camera, Navigation *navigation, f32 delta_time) {
    vec3 target_velocity = getVec3Of(0);

    if (navigation->move.right)    target_velocity.x += navigation->settings.max_velocity;
    if (navigation->move.left)     target_velocity.x -= navigation->settings.max_velocity;
    if (navigation->move.up)       target_velocity.y += navigation->settings.max_velocity;
    if (navigation->move.down)     target_velocity.y -= navigation->settings.max_velocity;
    if (navigation->move.forward)  target_velocity.z += navigation->settings.max_velocity;
    if (navigation->move.backward) target_velocity.z -= navigation->settings.max_velocity;

    if (navigation->turn.right || navigation->turn.left) {
        f32 yaw = delta_time * navigation->settings.speeds.turn;
        rotateXform3(&camera->transform, navigation->turn.left ? yaw : -yaw, 0, 0);
        camera->turned = true;
    }

    // Update the current velocity:
    f32 velocity_difference = navigation->settings.acceleration * delta_time;
    camera->current_velocity = approachVec3(camera->current_velocity, target_velocity, velocity_difference);

    camera->moved = nonZeroVec3(camera->current_velocity);
    if (camera->moved) { // Update the current position:
        vec3 movement = scaleVec3(camera->current_velocity, delta_time);
        movement = mulVec3Mat3(movement, camera->transform.rotation_matrix);
        camera->transform.position = addVec3(camera->transform.position, movement);
    }
}

#define HUD_COLOR 0x0000FF00
#define HUD_LENGTH 140
#define HUD_WIDTH 12
#define HUD_RIGHT 100
#define HUD_TOP 10

typedef struct HUD {
    char text[HUD_LENGTH];
    char *width,
            *height,
            *mouseX,
            *mouseY,
            *run_on,
            *fps,
            *msf,
            *mode;
} HUD;

void setCountersInHUD(HUD *hud, Timer *timer) {
    NumberStringBuffer fps_number_string;
    NumberStringBuffer msf_number_string;

    printNumberIntoString(timer->average_frames_per_second,      &fps_number_string);
    printNumberIntoString(timer->average_microseconds_per_frame, &msf_number_string);

    for (u8 i = 0; i < 4; i++) {
        hud->fps[i] = fps_number_string.string[i];
        hud->msf[i] = msf_number_string.string[i];
    }
}

void setDimensionsInHUD(HUD *hud, u16 width, u16 height) {
    NumberStringBuffer width_number_string;
    NumberStringBuffer height_number_string;

    printNumberIntoString(width, &width_number_string);
    printNumberIntoString(height, &height_number_string);

    for (u8 i = 0; i < 4; i++) {
        hud->width[i] = width_number_string.string[i];
        hud->height[i] = height_number_string.string[i];
    }
}

void setMouseCoordinatesInHUD(HUD *hud, Mouse *mouse) {
    NumberStringBuffer mouse_pos_x_number_string;
    NumberStringBuffer mouse_pos_y_number_string;

    printNumberIntoString(mouse->pos.x, &mouse_pos_x_number_string);
    printNumberIntoString(mouse->pos.y, &mouse_pos_y_number_string);

    for (u8 i = 0; i < 4; i++) {
        hud->mouseX[i] = mouse_pos_x_number_string.string[i];
        hud->mouseY[i] = mouse_pos_y_number_string.string[i];
    }
}

void setRenderModeInHUD(HUD *hud, enum RenderMode render_mode) {
    char* mode = hud->mode;
    switch (render_mode) {
        case RenderMode_Normals: *mode++ = 'N'; *mode++ = 'o'; *mode++ = 'r'; *mode++ = 'm'; *mode++ = 'a'; *mode = 'l'; break;
        case RenderMode_Beauty : *mode++ = 'B'; *mode++ = 'e'; *mode++ = 'a'; *mode++ = 'u'; *mode++ = 't'; *mode = 'y'; break;
        case RenderMode_Depth  : *mode++ = ' '; *mode++ = 'D'; *mode++ = 'e'; *mode++ = 'p'; *mode++ = 't'; *mode = 'h'; break;
        case RenderMode_UVs    : *mode++ = 'T'; *mode++ = 'e'; *mode++ = 'x'; *mode++ = 'C'; *mode++ = 'o'; *mode = 'r'; break;
    }
}

void initHUD(HUD *hud, u16 width, u16 height) {
    char* str_template = (char*)"Width  : ___1\n"
                                "Height : ___2\n"
                                "Mouse X: ___3\n"
                                "Mouse Y: ___4\n"
                                "Using  :  5__\n"
                                "FPS    : ___6\n"
                                "mic-s/f: ___7\n"
                                "Mode : 8_____\n";

    char* HUD_char = str_template;
    char* HUD_text_char = hud->text;

    while (*HUD_char) {
        switch (*HUD_char) {
            case '1':  hud->width  = HUD_text_char; break;
            case '2':  hud->height = HUD_text_char; break;
            case '3':  hud->mouseX = HUD_text_char; break;
            case '4':  hud->mouseY = HUD_text_char; break;
            case '5':  hud->run_on = HUD_text_char; break;
            case '6':  hud->fps    = HUD_text_char; break;
            case '7':  hud->msf    = HUD_text_char; break;
            case '8':  hud->mode   = HUD_text_char; break;
        }

        *HUD_text_char++ = *HUD_char++;
    }
    *HUD_text_char = '\0';

    setDimensionsInHUD(hud, width, height);
}

#define VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE 0.1f
#define VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE 1000.0f

typedef struct ViewportSettings {
    f32 near_clipping_plane_distance,
            far_clipping_plane_distance;
    enum RenderMode render_mode;
    bool show_hud;
} ViewportSettings;

typedef struct Viewport {
    ViewportSettings settings;
    Navigation navigation;
    HUD hud;
    Camera *camera;
    PixelGrid *frame_buffer;
} Viewport;

void initViewport(Viewport *viewport, ViewportSettings viewport_settings, NavigationSettings navigation_settings) {
    viewport->camera = null;
    viewport->settings = viewport_settings;
    viewport->frame_buffer = &app->window_content;
    initHUD(&viewport->hud,
            viewport->frame_buffer->dimensions.width,
            viewport->frame_buffer->dimensions.height);
    initNavigation(&viewport->navigation, navigation_settings);
}

ViewportSettings getDefaultViewportSettings() {
    ViewportSettings default_viewport_settings;

    default_viewport_settings.near_clipping_plane_distance = VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE;
    default_viewport_settings.far_clipping_plane_distance  = VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE;

    default_viewport_settings.render_mode = RenderMode_Beauty;
    default_viewport_settings.show_hud = false;

    return default_viewport_settings;
}

#define IS_VISIBLE ((u8)1)
#define IS_TRANSLATED ((u8)2)
#define IS_ROTATED ((u8)4)
#define IS_SCALED ((u8)8)
#define IS_SCALED_NON_UNIFORMLY ((u8)16)

enum PrimitiveType {
    PrimitiveType_None = 0,
    PrimitiveType_Quad,
    PrimitiveType_Box,
    PrimitiveType_Tetrahedron
};

typedef struct Primitive {
    quat rotation;
    vec3 position, scale;
    u32 id;
    enum PrimitiveType type;
    u8 flags;
} Primitive;

void initPrimitive(Primitive *primitive) {
    primitive->type = PrimitiveType_None;
    primitive->flags = IS_VISIBLE;
    primitive->id = 0;

    primitive->scale    = getVec3Of(1);
    primitive->position = getVec3Of(0);
    primitive->rotation = getIdentityQuaternion();
}

INLINE void convertPositionAndDirectionToObjectSpace(
        vec3 position,
        vec3 dir,
        Primitive *primitive,
        vec3 *out_position,
        vec3 *out_direction
) {
    *out_position = primitive->flags & IS_TRANSLATED ?
                    subVec3(position, primitive->position) :
                    position;

    if (primitive->flags & IS_ROTATED) {
        quat inv_rotation = conjugate(primitive->rotation);
        *out_position = mulVec3Quat(*out_position, inv_rotation);
        *out_direction = mulVec3Quat(dir, inv_rotation);
    } else
        *out_direction = dir;

    if (primitive->flags & IS_SCALED) {
        vec3 inv_scale = oneOver(primitive->scale);
        *out_position = mulVec3(*out_position, inv_scale);
        if (primitive->flags & IS_SCALED_NON_UNIFORMLY)
            *out_direction = norm(mulVec3(*out_direction, inv_scale));
    }
}

INLINE vec3 convertPositionToWorldSpace(vec3 position, Primitive *primitive) {
    if (primitive->flags & IS_SCALED)     position = mulVec3(    position, primitive->scale);
    if (primitive->flags & IS_ROTATED)    position = mulVec3Quat(position, primitive->rotation);
    if (primitive->flags & IS_TRANSLATED) position = addVec3(    position, primitive->position);
    return position;
}
INLINE vec3 convertPositionToObjectSpace(vec3 position, Primitive *primitive) {
    if (primitive->flags & IS_TRANSLATED) position = subVec3(    position, primitive->position);
    if (primitive->flags & IS_ROTATED)    position = mulVec3Quat(position, conjugate(primitive->rotation));
    if (primitive->flags & IS_SCALED)     position = mulVec3(    position, oneOver(  primitive->scale));
    return position;
}

INLINE vec3 convertDirectionToWorldSpace(vec3 direction, Primitive *primitive) {
    if (primitive->flags & IS_SCALED_NON_UNIFORMLY) direction = mulVec3(    direction, oneOver(primitive->scale));
    if (primitive->flags & IS_ROTATED)              direction = mulVec3Quat(direction,         primitive->rotation);
    return direction;
}

AABB getPrimitiveAABB(Primitive *primitive) {
    AABB aabb;

    aabb.max = getVec3Of(primitive->type == PrimitiveType_Tetrahedron ? SQRT3 / 3 : 1);
    aabb.min = invertedVec3(aabb.max);

    return aabb;
}

void transformAABB(AABB *aabb, Primitive *primitive) {
    f32 x = aabb->min.x;
    f32 y = aabb->min.y;
    f32 z = aabb->min.z;
    f32 X = aabb->max.x;
    f32 Y = aabb->max.y;
    f32 Z = aabb->max.z;

    vec3 v[8] = {
            {x, y, z},
            {x, y, Z},
            {x, Y, z},
            {x, Y, Z},
            {X, y, z},
            {X, y, Z},
            {X, Y, z},
            {X, Y, Z}
    };

    x = y = z = +INFINITY;
    X = Y = Z = -INFINITY;

    vec3 position;
    for (u8 i = 0; i < 8; i++) {
        position = convertPositionToWorldSpace(v[i], primitive);

        if (position.x > X) X = position.x;
        if (position.y > Y) Y = position.y;
        if (position.z > Z) Z = position.z;
        if (position.x < x) x = position.x;
        if (position.y < y) y = position.y;
        if (position.z < z) z = position.z;
    }

    aabb->min.x = x;
    aabb->min.y = y;
    aabb->min.z = z;
    aabb->max.x = X;
    aabb->max.y = Y;
    aabb->max.z = Z;
}

typedef struct SceneCounts {
    u32 cameras, primitives;
} SceneCounts;

SceneCounts getDefaultSceneCounts() {
    SceneCounts default_scene_counts;

    default_scene_counts.cameras = 1;
    default_scene_counts.primitives = 0;

    return default_scene_counts;
}

typedef struct Scene {
    SceneCounts counts;
    Camera *cameras;
    Primitive *primitives;
} Scene;

void initScene(Scene *scene, SceneCounts scene_counts) {
    scene->counts = scene_counts;
    scene->primitives = null;
    scene->cameras    = null;

    if (scene_counts.primitives) {
        scene->primitives = allocateAppMemory(sizeof(Primitive) * scene->counts.primitives);
        if (scene->primitives)
            for (u32 i = 0; i < scene_counts.primitives; i++)
                initPrimitive(scene->primitives + i);
    }

    if (scene_counts.cameras) {
        scene->cameras =allocateAppMemory(sizeof(Camera) * scene->counts.cameras);
        if (scene->cameras)
            for (u32 i = 0; i < scene_counts.cameras; i++)
                initCamera(scene->cameras + i, getDefaultCameraSettings());
    }
}

typedef struct Edge {
    vec3 from, to;
} Edge;

void projectEdge(Edge *edge, Dimensions *dimensions, f32 focal_length, f32 near_clipping_plane_distance) {
    bool v1_is_out = edge->from.z < near_clipping_plane_distance;
    bool v2_is_out = edge->to.z   < near_clipping_plane_distance;

    // Cull:
    if (v1_is_out &&
        v2_is_out) {
        edge->from = getVec3Of(-1);
        edge->to   = getVec3Of(-1);
        return;
    }

    // Clip:
    if (v1_is_out ||
        v2_is_out) {
        vec3 v;
        if (v1_is_out) {
            v = subVec3(edge->from, edge->to);
            v = scaleVec3(v, (edge->to.z - near_clipping_plane_distance) / (edge->to.z - edge->from.z));
            edge->from = addVec3(edge->to, v);
        } else {
            v = subVec3(edge->to, edge->from);
            v = scaleVec3(v, (edge->from.z - near_clipping_plane_distance) / (edge->from.z - edge->to.z));
            edge->to = addVec3(edge->from, v);
        }
    }

    // Project:
    f32 fl_over_z_from = focal_length / edge->from.z;
    f32 fl_over_z_to   = focal_length / edge->to.z;
    edge->from.x *= fl_over_z_from;
    edge->from.y *= fl_over_z_from * dimensions->width_over_height;
    edge->to.x   *= fl_over_z_to;
    edge->to.y   *= fl_over_z_to   * dimensions->width_over_height;

    // NDC->screen:
    edge->from.x += 1; edge->from.x *= dimensions->h_width;
    edge->to.x   += 1; edge->to.x   *= dimensions->h_width;
    edge->from.y += 1; edge->from.y *= dimensions->h_height;
    edge->to.y   += 1; edge->to.y   *= dimensions->h_height;

    // Flip Y:
    edge->from.y = dimensions->f_height - edge->from.y;
    edge->to.y   = dimensions->f_height - edge->to.y;
}

void drawEdge(PixelGrid *canvas, RGBA color, Edge N, Camera *camera, f32 near_clipping_plane_distance) {
    N.from = mulVec3Mat3(subVec3(N.from, camera->transform.position), camera->transform.rotation_matrix_inverted);
    N.to   = mulVec3Mat3(subVec3(N.to  , camera->transform.position), camera->transform.rotation_matrix_inverted);
    projectEdge(&N, &canvas->dimensions, camera->focal_length, near_clipping_plane_distance);
    drawLine2D(canvas, color,
               (i32)N.from.x,
               (i32)N.from.y,
               (i32)N.to.x,
               (i32)N.to.y);
}

typedef struct Helix {
    vec3 position;
    f32 radius, thickness_radius;
    u32 revolution_count;
} Helix;

void drawHelix(PixelGrid *canvas, RGBA color, Helix *helix, Camera *camera, f32 near_clipping_plane_distance) {
    u32 step_count = (u32)3600;
    f32 orbit_angle_step = TAU / (f32)step_count,
            helix_angle_step = orbit_angle_step * (f32)helix->revolution_count;

    vec3 current_position, previous_position;
    vec3 *center_position = &helix->position;

    vec3 center_to_orbit;
    center_to_orbit.x = helix->radius;
    center_to_orbit.y = center_to_orbit.z = 0;

    vec3 orbit_to_helix;
    orbit_to_helix.x = helix->thickness_radius;
    orbit_to_helix.y = orbit_to_helix.z = 0;

    vec3 orbit_to_helix_in_world_space;

    mat3 orbit_rotation;
    orbit_rotation.X.x = orbit_rotation.Z.z = cosf(orbit_angle_step);
    orbit_rotation.X.z = sinf(orbit_angle_step);
    orbit_rotation.Z.x = -orbit_rotation.X.z;
    orbit_rotation.X.y = orbit_rotation.Z.y = orbit_rotation.Y.x = orbit_rotation.Y.z =  0;
    orbit_rotation.Y.y = 1;

    mat3 helix_rotation;
    helix_rotation.X.x = helix_rotation.Y.y = cosf(helix_angle_step);
    helix_rotation.X.y = sinf(helix_angle_step);
    helix_rotation.Y.x = -helix_rotation.X.y;
    helix_rotation.X.z = helix_rotation.Y.z = helix_rotation.Z.x = helix_rotation.Z.y =  0;
    helix_rotation.Z.z = 1;

    // Transform vertex positions of edges from view-space to screen-space (w/ culling and clipping):

    vec3 *cam_pos = &camera->transform.position;
    mat3 *cam_rot = &camera->transform.rotation_matrix_inverted;

    mat3 accumulated_orbit_rotation = orbit_rotation;
    Edge prev_to_curr_pos;

    for (u32 i = 0; i < step_count; i++) {
        center_to_orbit = mulVec3Mat3(center_to_orbit, orbit_rotation);
        orbit_to_helix  = mulVec3Mat3(orbit_to_helix,  helix_rotation);
        orbit_to_helix_in_world_space = mulVec3Mat3(orbit_to_helix, accumulated_orbit_rotation);

        current_position = addVec3(*center_position, center_to_orbit);
        current_position = addVec3(current_position, orbit_to_helix_in_world_space);

        if (i) {
            // Project line-segment previous_position->current_position from world-space to screen-space, and draw it.
            prev_to_curr_pos.to   = subVec3(current_position,  *cam_pos);
            prev_to_curr_pos.from = subVec3(previous_position, *cam_pos);

            prev_to_curr_pos.to   = mulVec3Mat3(prev_to_curr_pos.to, *cam_rot);
            prev_to_curr_pos.from = mulVec3Mat3(prev_to_curr_pos.from, *cam_rot);

            projectEdge(&prev_to_curr_pos, &canvas->dimensions, camera->focal_length, near_clipping_plane_distance);
            drawLine2D(canvas, color,
                       (i32)prev_to_curr_pos.from.x,
                       (i32)prev_to_curr_pos.from.y,
                       (i32)prev_to_curr_pos.to.x,
                       (i32)prev_to_curr_pos.to.y);
        }

        accumulated_orbit_rotation = mulMat3(accumulated_orbit_rotation, orbit_rotation);
        previous_position = current_position;
    }
}

typedef struct Coil {
    vec3 position;
    f32 radius, height;
    u32 revolution_count;
} Coil;

void drawCoil(PixelGrid *canvas, RGBA color, Coil *coil, Camera *camera, f32 near_clipping_plane_distance) {
    u32 step_count = (u32)3600;
    f32 angle_step = (TAU / (f32)step_count) * (f32)coil->revolution_count;
    f32 height_step = coil->height / (f32)step_count;

    vec3 current_position, previous_position;
    vec3 *center_position = &coil->position;

    vec3 center_to_coil;
    center_to_coil.x = coil->radius;
    center_to_coil.y = center_to_coil.z = 0;

    mat3 rotation;
    rotation.X.x = rotation.Z.z = cosf(angle_step);
    rotation.X.z = sinf(angle_step);
    rotation.Z.x = -rotation.X.z;
    rotation.X.y = rotation.Z.y = rotation.Y.x = rotation.Y.z =  0;
    rotation.Y.y = 1;

    vec3 *cam_pos = &camera->transform.position;
    mat3 *cam_rot = &camera->transform.rotation_matrix_inverted;

    Edge prev_to_curr_pos;

    for (u32 i = 0; i < step_count; i++) {
        center_to_coil   = mulVec3Mat3(center_to_coil, rotation);
        current_position = addVec3(*center_position, center_to_coil);

        if (i) {
            // Project line-segment previous_position->current_position from world-space to screen-space, and draw it.

            prev_to_curr_pos.to   = subVec3(current_position,  *cam_pos);
            prev_to_curr_pos.from = subVec3(previous_position, *cam_pos);

            prev_to_curr_pos.to   = mulVec3Mat3(prev_to_curr_pos.to, *cam_rot);
            prev_to_curr_pos.from = mulVec3Mat3(prev_to_curr_pos.from, *cam_rot);

            projectEdge(&prev_to_curr_pos, &canvas->dimensions, camera->focal_length, near_clipping_plane_distance);
            drawLine2D(canvas, color,
                       (i32)prev_to_curr_pos.from.x,
                       (i32)prev_to_curr_pos.from.y,
                       (i32)prev_to_curr_pos.to.x,
                       (i32)prev_to_curr_pos.to.y);
        }

        center_to_coil.y += height_step;
        previous_position = current_position;
    }
}

typedef enum BoxSide {
    NoSide = 0,
    Top    = 1,
    Bottom = 2,
    Left   = 4,
    Right  = 8,
    Front  = 16,
    Back   = 32
} BoxSide;

#define AllBoxSides (Top | Bottom | Left | Right | Front | Back)

#define BBOX_VERTEX_COUNT 8
#define BBOX_EDGE_COUNT 12

typedef struct BBoxCorners {
    vec3 front_top_left,
            front_top_right,
            front_bottom_left,
            front_bottom_right,
            back_top_left,
            back_top_right,
            back_bottom_left,
            back_bottom_right;
} BBoxCorners;

typedef union BBoxVertices {
    BBoxCorners corners;
    vec3 buffer[BBOX_VERTEX_COUNT];
} BBoxVertices;

typedef struct BBoxEdgeSides {
    Edge front_top,
            front_bottom,
            front_left,
            front_right,
            back_top,
            back_bottom,
            back_left,
            back_right,
            left_bottom,
            left_top,
            right_bottom,
            right_top;
} BBoxEdgeSides;

typedef union BBoxEdges {
    BBoxEdgeSides sides;
    Edge buffer[BBOX_EDGE_COUNT];
} BBoxEdges;

typedef struct BBox {
    BBoxVertices vertex;
    BBoxEdges edge;
} BBox;

void setBBoxEdges(BBox *bbox) {
    bbox->edge.sides.front_top.from    = bbox->vertex.corners.front_top_left;
    bbox->edge.sides.front_top.to      = bbox->vertex.corners.front_top_right;
    bbox->edge.sides.front_bottom.from = bbox->vertex.corners.front_bottom_left;
    bbox->edge.sides.front_bottom.to   = bbox->vertex.corners.front_bottom_right;
    bbox->edge.sides.front_left.from   = bbox->vertex.corners.front_bottom_left;
    bbox->edge.sides.front_left.to     = bbox->vertex.corners.front_top_left;
    bbox->edge.sides.front_right.from  = bbox->vertex.corners.front_bottom_right;
    bbox->edge.sides.front_right.to    = bbox->vertex.corners.front_top_right;

    bbox->edge.sides.back_top.from     = bbox->vertex.corners.back_top_left;
    bbox->edge.sides.back_top.to       = bbox->vertex.corners.back_top_right;
    bbox->edge.sides.back_bottom.from  = bbox->vertex.corners.back_bottom_left;
    bbox->edge.sides.back_bottom.to    = bbox->vertex.corners.back_bottom_right;
    bbox->edge.sides.back_left.from    = bbox->vertex.corners.back_bottom_left;
    bbox->edge.sides.back_left.to      = bbox->vertex.corners.back_top_left;
    bbox->edge.sides.back_right.from   = bbox->vertex.corners.back_bottom_right;
    bbox->edge.sides.back_right.to     = bbox->vertex.corners.back_top_right;

    bbox->edge.sides.left_bottom.from  = bbox->vertex.corners.front_bottom_left;
    bbox->edge.sides.left_bottom.to    = bbox->vertex.corners.back_bottom_left;
    bbox->edge.sides.left_top.from     = bbox->vertex.corners.front_top_left;
    bbox->edge.sides.left_top.to       = bbox->vertex.corners.back_top_left;
    bbox->edge.sides.right_bottom.from = bbox->vertex.corners.front_bottom_right;
    bbox->edge.sides.right_bottom.to   = bbox->vertex.corners.back_bottom_right;
    bbox->edge.sides.right_top.from    = bbox->vertex.corners.front_top_right;
    bbox->edge.sides.right_top.to      = bbox->vertex.corners.back_top_right;
}

void setBBoxFromAABB(AABB *aabb, BBox *bbox) {
    bbox->vertex.corners.front_top_left.x = aabb->min.x;
    bbox->vertex.corners.back_top_left.x = aabb->min.x;
    bbox->vertex.corners.front_bottom_left.x = aabb->min.x;
    bbox->vertex.corners.back_bottom_left.x = aabb->min.x;

    bbox->vertex.corners.front_top_right.x = aabb->max.x;
    bbox->vertex.corners.back_top_right.x = aabb->max.x;
    bbox->vertex.corners.front_bottom_right.x = aabb->max.x;
    bbox->vertex.corners.back_bottom_right.x = aabb->max.x;


    bbox->vertex.corners.front_bottom_left.y = aabb->min.y;
    bbox->vertex.corners.front_bottom_right.y = aabb->min.y;
    bbox->vertex.corners.back_bottom_left.y = aabb->min.y;
    bbox->vertex.corners.back_bottom_right.y = aabb->min.y;

    bbox->vertex.corners.front_top_left.y = aabb->max.y;
    bbox->vertex.corners.front_top_right.y = aabb->max.y;
    bbox->vertex.corners.back_top_left.y = aabb->max.y;
    bbox->vertex.corners.back_top_right.y = aabb->max.y;


    bbox->vertex.corners.front_top_left.z = aabb->max.z;
    bbox->vertex.corners.front_top_right.z = aabb->max.z;
    bbox->vertex.corners.front_bottom_left.z = aabb->max.z;
    bbox->vertex.corners.front_bottom_right.z = aabb->max.z;

    bbox->vertex.corners.back_top_left.z = aabb->min.z;
    bbox->vertex.corners.back_top_right.z = aabb->min.z;
    bbox->vertex.corners.back_bottom_left.z = aabb->min.z;
    bbox->vertex.corners.back_bottom_right.z = aabb->min.z;

    setBBoxEdges(bbox);
}

void projectBBox(BBox *bbox, Camera *camera, Dimensions *dimensions) {
    // Transform vertex positions from world-space to view-space:
    for (u8 i = 0; i < BBOX_VERTEX_COUNT; i++)
        bbox->vertex.buffer[i] = mulVec3Mat3(subVec3(bbox->vertex.buffer[i], camera->transform.position), camera->transform.rotation_matrix_inverted);

    // Distribute transformed vertex positions to edges:
    setBBoxEdges(bbox);

    // Transform vertex positions of edges from view-space to screen-space (w/ culling and clipping):
    for (u8 i = 0; i < BBOX_EDGE_COUNT; i++)
        projectEdge(bbox->edge.buffer + i, camera, dimensions);
}

void drawBBox(FrameBuffer *frame_buffer, BBox *bbox, Pixel pixel, u8 sides) {
    if (sides == AllBoxSides)
        for (u8 i = 0; i < BBOX_EDGE_COUNT; i++)
            drawLine2D((i32)bbox->edge.buffer[i].from.x,
                       (i32)bbox->edge.buffer[i].from.y,
                       (i32)bbox->edge.buffer[i].to.x,
                       (i32)bbox->edge.buffer[i].to.y,
                       pixel, frame_buffer);
    else {
        if (sides & Front |
            sides & Top)
            drawLine2D((i32)bbox->edge.sides.front_top.from.x,
                       (i32)bbox->edge.sides.front_top.from.y,
                       (i32)bbox->edge.sides.front_top.to.x,
                       (i32)bbox->edge.sides.front_top.to.y,
                       pixel, frame_buffer);

        if (sides & Front |
            sides & Bottom)
            drawLine2D((i32)bbox->edge.sides.front_bottom.from.x,
                       (i32)bbox->edge.sides.front_bottom.from.y,
                       (i32)bbox->edge.sides.front_bottom.to.x,
                       (i32)bbox->edge.sides.front_bottom.to.y,
                       pixel, frame_buffer);

        if (sides & Front |
            sides & Left)
            drawLine2D((i32)bbox->edge.sides.front_left.from.x,
                       (i32)bbox->edge.sides.front_left.from.y,
                       (i32)bbox->edge.sides.front_left.to.x,
                       (i32)bbox->edge.sides.front_left.to.y,
                       pixel, frame_buffer);

        if (sides & Front |
            sides & Right)
            drawLine2D((i32)bbox->edge.sides.front_right.from.x,
                       (i32)bbox->edge.sides.front_right.from.y,
                       (i32)bbox->edge.sides.front_right.to.x,
                       (i32)bbox->edge.sides.front_right.to.y,
                       pixel, frame_buffer);


        if (sides & Back |
            sides & Top)
            drawLine2D((i32)bbox->edge.sides.back_top.from.x,
                       (i32)bbox->edge.sides.back_top.from.y,
                       (i32)bbox->edge.sides.back_top.to.x,
                       (i32)bbox->edge.sides.back_top.to.y,
                       pixel, frame_buffer);

        if (sides & Back |
            sides & Bottom)
            drawLine2D((i32)bbox->edge.sides.back_bottom.from.x,
                       (i32)bbox->edge.sides.back_bottom.from.y,
                       (i32)bbox->edge.sides.back_bottom.to.x,
                       (i32)bbox->edge.sides.back_bottom.to.y,
                       pixel, frame_buffer);

        if (sides & Back |
            sides & Left)
            drawLine2D((i32)bbox->edge.sides.back_left.from.x,
                       (i32)bbox->edge.sides.back_left.from.y,
                       (i32)bbox->edge.sides.back_left.to.x,
                       (i32)bbox->edge.sides.back_left.to.y,
                       pixel, frame_buffer);

        if (sides & Back |
            sides & Right)
            drawLine2D((i32)bbox->edge.sides.back_right.from.x,
                       (i32)bbox->edge.sides.back_right.from.y,
                       (i32)bbox->edge.sides.back_right.to.x,
                       (i32)bbox->edge.sides.back_right.to.y,
                       pixel, frame_buffer);


        if (sides & Left |
            sides & Top)
            drawLine2D((i32)bbox->edge.sides.left_top.from.x,
                       (i32)bbox->edge.sides.left_top.from.y,
                       (i32)bbox->edge.sides.left_top.to.x,
                       (i32)bbox->edge.sides.left_top.to.y,
                       pixel, frame_buffer);

        if (sides & Left |
            sides & Bottom)
            drawLine2D((i32)bbox->edge.sides.left_bottom.from.x,
                       (i32)bbox->edge.sides.left_bottom.from.y,
                       (i32)bbox->edge.sides.left_bottom.to.x,
                       (i32)bbox->edge.sides.left_bottom.to.y,
                       pixel, frame_buffer);

        if (sides & Right |
            sides & Top)
            drawLine2D((i32)bbox->edge.sides.right_top.from.x,
                       (i32)bbox->edge.sides.right_top.from.y,
                       (i32)bbox->edge.sides.right_top.to.x,
                       (i32)bbox->edge.sides.right_top.to.y,
                       pixel, frame_buffer);

        if (sides & Right |
            sides & Bottom)
            drawLine2D((i32)bbox->edge.sides.right_bottom.from.x,
                       (i32)bbox->edge.sides.right_bottom.from.y,
                       (i32)bbox->edge.sides.right_bottom.to.x,
                       (i32)bbox->edge.sides.right_bottom.to.y,
                       pixel, frame_buffer);
    }
}

#ifdef __cplusplus
#define null nullptr
#else
#define null 0
typedef unsigned char      bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed   short     i16;
typedef signed   int       i32;

typedef float  f32;
typedef double f64;

typedef void* (*CallbackWithInt)(u64 size);
typedef void (*CallbackWithBool)(bool on);
typedef void (*CallbackWithCharPtr)(char* str);

#define MAX_COLOR_VALUE 0xFF
typedef struct vec2  { f32 x, y; } vec2;
typedef struct vec2i { i32 x, y; } vec2i;
typedef struct RGBA  { u8 B, G, R, A; } RGBA;
typedef struct Rect  { vec2i min, max; } Rect;
typedef union  Pixel { RGBA color; u32 value; } Pixel;

void swap(i32 *a, i32 *b) {
    i32 t = *a;
    *a = *b;
    *b = t;
}

void subRange(i32 from, i32 to, i32 end, i32 start, i32 *first, i32 *last) {
    *first = from;
    *last  = to;
    if (to < from) swap(first, last);
    *first = *first > start ? *first : start;
    *last  = (*last < end ? *last : end) - 1;
}

inline bool inRange(i32 value, i32 end, i32 start) {
    return value >= start && value < end;
}

enum ColorID {
    Black,
    White,
    Grey,

    Red,
    Green,
    Blue,

    Cyan,
    Magenta,
    Yellow
};

RGBA Color(enum ColorID color_id) {
    RGBA color;
    color.A = MAX_COLOR_VALUE;

    switch (color_id) {
        case Black:
            color.R = 0;
            color.G = 0;
            color.B = 0;
            break;
        case White:
            color.R = MAX_COLOR_VALUE;
            color.G = MAX_COLOR_VALUE;
            color.B = MAX_COLOR_VALUE;

            break;
        case Grey:
            color.R = MAX_COLOR_VALUE/2;
            color.G = MAX_COLOR_VALUE/2;
            color.B = MAX_COLOR_VALUE/2;
            break;

        case Red:
            color.R = MAX_COLOR_VALUE;
            color.G = 0;
            color.B = 0;
            break;
        case Green:
            color.R = 0;
            color.G = MAX_COLOR_VALUE;
            color.B = 0;
            break;
        case Blue:
            color.R = 0;
            color.G = 0;
            color.B = MAX_COLOR_VALUE;
            break;

        case Cyan:
            color.R = 0;
            color.G = MAX_COLOR_VALUE;
            color.B = MAX_COLOR_VALUE;
            break;
        case Magenta:
            color.R = MAX_COLOR_VALUE;
            color.G = 0;
            color.B = MAX_COLOR_VALUE;
            break;
        case Yellow:
            color.R = MAX_COLOR_VALUE;
            color.G = MAX_COLOR_VALUE;
            color.B = 0;
            break;
    }

    return color;
}


typedef struct NumberStringBuffer {
    char _buffer[12];
    char *string;
    u8 digit_count;
} NumberStringBuffer;

void printNumberIntoString(i32 number, NumberStringBuffer *number_string) {
    char *buffer = number_string->_buffer;
    buffer[11] = 0;
    for (u8 i = 0; i < 11; i++) buffer[i] = ' ';

    bool is_negative = number < 0;
    if (is_negative) number = -number;

    if (number) {
        u32 temp;
        buffer += 11;
        number_string->string = buffer;
        number_string->digit_count = 0;

        for (u8 i = 0; i < 11; i++) {
            temp = number;
            number /= 10;
            number_string->string--;
            number_string->digit_count++;
            *buffer-- = (char)('0' + temp - number * 10);
            if (!number) {
                if (is_negative) {
                    *buffer = '-';
                    number_string->string--;
                    number_string->digit_count++;
                }

                break;
            }
        }
    } else {
        buffer[11] = '0';
        number_string->digit_count = 1;
        number_string->string = buffer + 10;
    }
}


typedef u64 (*GetTicks)();

typedef struct PerTick {
    f64 seconds, milliseconds, microseconds, nanoseconds;
} PerTick;

typedef struct Ticks {
    PerTick per_tick;
    u64 per_second;
} Ticks;

typedef struct Timer {
    GetTicks getTicks;
    Ticks *ticks;

    f32 delta_time;
    u64 ticks_before,
            ticks_after,
            ticks_diff,
            accumulated_ticks,
            accumulated_frame_count,
            ticks_of_last_report,
            seconds,
            milliseconds,
            microseconds,
            nanoseconds;
    f64 average_frames_per_tick,
            average_ticks_per_frame;
    u16 average_frames_per_second,
            average_milliseconds_per_frame,
            average_microseconds_per_frame,
            average_nanoseconds_per_frame;
} Timer;

void initTimer(Timer *timer, GetTicks getTicks, Ticks *ticks) {
    timer->getTicks = getTicks;
    timer->ticks    = ticks;

    timer->delta_time = 0;
    timer->ticks_before = 0;
    timer->ticks_after = 0;
    timer->ticks_diff = 0;

    timer->accumulated_ticks = 0;
    timer->accumulated_frame_count = 0;

    timer->ticks_of_last_report = 0;

    timer->seconds = 0;
    timer->milliseconds = 0;
    timer->microseconds = 0;
    timer->nanoseconds = 0;

    timer->average_frames_per_tick = 0;
    timer->average_ticks_per_frame = 0;
    timer->average_frames_per_second = 0;
    timer->average_milliseconds_per_frame = 0;
    timer->average_microseconds_per_frame = 0;
    timer->average_nanoseconds_per_frame = 0;
}

typedef struct Timers {
    Timer update, render, aux;
} Timers;

typedef struct Time {
    Timers timers;
    Ticks ticks;
    GetTicks getTicks;
} Time;

void initTime(Time *time, GetTicks getTicks, u64 ticks_per_second) {
    time->getTicks = getTicks;
    time->ticks.per_second = ticks_per_second;

    time->ticks.per_tick.seconds      = 1          / (f64)(time->ticks.per_second);
    time->ticks.per_tick.milliseconds = 1000       / (f64)(time->ticks.per_second);
    time->ticks.per_tick.microseconds = 1000000    / (f64)(time->ticks.per_second);
    time->ticks.per_tick.nanoseconds  = 1000000000 / (f64)(time->ticks.per_second);

    initTimer(&time->timers.update, getTicks, &time->ticks);
    initTimer(&time->timers.render, getTicks, &time->ticks);
    initTimer(&time->timers.aux,    getTicks, &time->ticks);

    time->timers.update.ticks_before = time->timers.update.ticks_of_last_report = getTicks();
}

void accumulateTimer(Timer* timer) {
    timer->ticks_diff = timer->ticks_after - timer->ticks_before;
    timer->accumulated_ticks += timer->ticks_diff;
    timer->accumulated_frame_count++;

    timer->seconds      = (u64)(timer->ticks->per_tick.seconds      * (f64)(timer->ticks_diff));
    timer->milliseconds = (u64)(timer->ticks->per_tick.milliseconds * (f64)(timer->ticks_diff));
    timer->microseconds = (u64)(timer->ticks->per_tick.microseconds * (f64)(timer->ticks_diff));
    timer->nanoseconds  = (u64)(timer->ticks->per_tick.nanoseconds  * (f64)(timer->ticks_diff));
}

void averageTimer(Timer *timer) {
    timer->average_frames_per_tick = (f64)timer->accumulated_frame_count / timer->accumulated_ticks;
    timer->average_ticks_per_frame = (f64)timer->accumulated_ticks / timer->accumulated_frame_count;
    timer->average_frames_per_second = (u16)(timer->average_frames_per_tick      * timer->ticks->per_second);
    timer->average_milliseconds_per_frame = (u16)(timer->average_ticks_per_frame * timer->ticks->per_tick.milliseconds);
    timer->average_microseconds_per_frame = (u16)(timer->average_ticks_per_frame * timer->ticks->per_tick.microseconds);
    timer->average_nanoseconds_per_frame = (u16)(timer->average_ticks_per_frame  * timer->ticks->per_tick.nanoseconds);
    timer->accumulated_ticks = timer->accumulated_frame_count = 0;
}

inline void startFrameTimer(Timer *timer) {
    timer->ticks_after = timer->ticks_before;
    timer->ticks_before = timer->getTicks();
    timer->ticks_diff = timer->ticks_before - timer->ticks_after;
    timer->delta_time = (f32)(timer->ticks_diff * timer->ticks->per_tick.seconds);
}

inline void endFrameTimer(Timer *timer) {
    timer->ticks_after = timer->getTicks();
    accumulateTimer(timer);
    if (timer->accumulated_ticks >= timer->ticks->per_tick.seconds) averageTimer(timer);
}


#define MAX_WIDTH 3840
#define MAX_HEIGHT 2160

#define PIXEL_SIZE 4
#define RENDER_SIZE Megabytes(8 * PIXEL_SIZE)

typedef struct Dimensions {
    u16 width, height;
    u32 width_times_height;
    f32 height_over_width,
            width_over_height,
            f_height, f_width,
            h_height, h_width;
} Dimensions;

void updateDimensions(Dimensions *dimensions, u16 width, u16 height) {
    dimensions->width = width;
    dimensions->height = height;
    dimensions->width_times_height = dimensions->width * dimensions->height;
    dimensions->f_width  =      (f32)dimensions->width;
    dimensions->f_height =      (f32)dimensions->height;
    dimensions->h_width  =           dimensions->f_width  / 2;
    dimensions->h_height =           dimensions->f_height / 2;
    dimensions->width_over_height  = dimensions->f_width  / dimensions->f_height;
    dimensions->height_over_width  = dimensions->f_height / dimensions->f_width;
}

typedef struct PixelGrid {
    Dimensions dimensions;
    Pixel* pixels;
} PixelGrid;

void initPixelGrid(PixelGrid *pixel_grid, Pixel* pixels_memory) {
    pixel_grid->pixels = pixels_memory;
    updateDimensions(&pixel_grid->dimensions, MAX_WIDTH, MAX_HEIGHT);
}

void fillPixelGrid(PixelGrid *pixel_grid, RGBA color) {
    for (u32 i = 0; i < pixel_grid->dimensions.width_times_height; i++)
        pixel_grid->pixels[i].color = color;
}

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define MEMORY_SIZE Gigabytes(1)
#define MEMORY_BASE Terabytes(2)

typedef struct Memory {
    u8* address;
    u64 occupied, capacity;
} Memory;

void initMemory(Memory *memory, u8* address, u64 capacity) {
    memory->address = (u8*)address;
    memory->capacity = capacity;
    memory->occupied = 0;
}

void* allocateMemory(Memory *memory, u64 size) {
    if (!memory->address) return null;
    memory->occupied += size;
    if (memory->occupied > memory->capacity) return null;

    void* address = memory->address;
    memory->address += size;
    return address;
}

typedef struct MouseButton {
    vec2i down_pos, up_pos;
    bool is_pressed, is_handled;
} MouseButton;

typedef struct Mouse {
    MouseButton middle_button, right_button, left_button;
    vec2i pos, pos_raw_diff, movement;
    f32 wheel_scroll_amount;
    bool moved, is_captured, double_clicked, wheel_scrolled;
} Mouse;

void initMouse(Mouse *mouse) {
    mouse->moved = false;
    mouse->is_captured = false;
    mouse->double_clicked = false;
    mouse->wheel_scrolled= false;

    mouse->wheel_scroll_amount = 0;
    mouse->pos.x = 0;
    mouse->pos.y = 0;
    mouse->pos_raw_diff.x = 0;
    mouse->pos_raw_diff.y = 0;

    mouse->middle_button.is_pressed = false;
    mouse->middle_button.is_handled = false;
    mouse->middle_button.up_pos.x = 0;
    mouse->middle_button.down_pos.x = 0;

    mouse->right_button.is_pressed = false;
    mouse->right_button.is_handled = false;
    mouse->right_button.up_pos.x = 0;
    mouse->right_button.down_pos.x = 0;

    mouse->left_button.is_pressed = false;
    mouse->left_button.is_handled = false;
    mouse->left_button.up_pos.x = 0;
    mouse->left_button.down_pos.x = 0;
}

typedef struct KeyMap      { u8 ctrl, alt, shift, space, tab; } KeyMap;
typedef struct IsPressed { bool ctrl, alt, shift, space, tab; } IsPressed;
typedef struct Controls {
    IsPressed is_pressed;
    KeyMap key_map;
    Mouse mouse;
} Controls;

void initControls(Controls *controls) {
    initMouse(&controls->mouse);
}

void drawHLine2D(PixelGrid *canvas, RGBA color, i32 from, i32 to, i32 at) {
    if (!inRange(at, canvas->dimensions.height, 0)) return;

    i32 offset = at * (i32)canvas->dimensions.width;
    i32 first, last;
    subRange(from, to, canvas->dimensions.width, 0, &first, &last);
    first += offset;
    last += offset;
    for (i32 i = first; i <= last; i++) canvas->pixels[i].color = color;
}

void drawVLine2D(PixelGrid *canvas, RGBA color, i32 from, i32 to, i32 at) {
    if (!inRange(at, canvas->dimensions.width, 0)) return;
    i32 first, last;

    subRange(from, to, canvas->dimensions.height, 0, &first, &last);
    first *= canvas->dimensions.width; first += at;
    last  *= canvas->dimensions.width; last  += at;
    for (i32 i = first; i <= last; i += canvas->dimensions.width) canvas->pixels[i].color = color;
}

void drawLine2D(PixelGrid *canvas, RGBA color, i32 x0, i32 y0, i32 x1, i32 y1) {
    if (x0 < 0 &&
        y0 < 0 &&
        x1 < 0 &&
        y1 < 0)
        return;

    if (x0 == x1) {
        drawVLine2D(canvas, color, y0, y1, x1);
        return;
    }

    if (y0 == y1) {
        drawHLine2D(canvas, color, x0, x1, y1);
        return;
    }

    i32 width  = (i32)canvas->dimensions.width;
    i32 height = (i32)canvas->dimensions.height;

    i32 pitch = width;
    i32 index = x0 + y0 * pitch;

    i32 run  = x1 - x0;
    i32 rise = y1 - y0;

    i32 dx = 1;
    i32 dy = 1;
    if (run < 0) {
        dx = -dx;
        run = -run;
    }
    if (rise < 0) {
        dy = -dy;
        rise = -rise;
        pitch = -pitch;
    }

    // Configure for a shallow line:
    i32 end = x1 + dx;
    i32 start1 = x0;  i32 inc1 = dx;  i32 index_inc1 = dx;
    i32 start2 = y0;  i32 inc2 = dy;  i32 index_inc2 = pitch;
    i32 rise_twice = rise + rise;
    i32 run_twice = run + run;
    i32 threshold = run;
    i32 error_dec = run_twice;
    i32 error_inc = rise_twice;
    bool is_steap = rise > run;
    if (is_steap) { // Reconfigure for a steep line:
        swap(&inc1, &inc2);
        swap(&start1, &start2);
        swap(&index_inc1, &index_inc2);
        swap(&error_dec, &error_inc);
        end = y1 + dy;
        threshold = rise;
    }

    i32 error = 0;
    i32 current1 = start1;
    i32 current2 = start2;
    while (current1 != end) {
        current1 += inc1;

        if (inRange(index, canvas->dimensions.width_times_height, 0)) {
            if (is_steap) {
                if (inRange(current1, height, 0) &&
                    inRange(current2, width, 0))
                    canvas->pixels[index].color = color;
            } else {
                if (inRange(current2, height, 0) &&
                    inRange(current1, width, 0))
                    canvas->pixels[index].color = color;
            }
        }

        index += index_inc1;
        error += error_inc;
        if (error > threshold) {
            error -= error_dec;
            index += index_inc2;
            current2 += inc2;
        }
    }
}

void drawRect(PixelGrid *canvas, RGBA color, Rect *rect) {
    if (rect->max.x < 0 || rect->min.x >= canvas->dimensions.width ||
        rect->max.y < 0 || rect->min.y >= canvas->dimensions.height)
        return;

    drawHLine2D(canvas, color, rect->min.x, rect->max.x, rect->min.y);
    drawHLine2D(canvas, color, rect->min.x, rect->max.x, rect->max.y);
    drawVLine2D(canvas, color, rect->min.y, rect->max.y, rect->min.x);
    drawVLine2D(canvas, color, rect->min.y, rect->max.y, rect->max.x);
}

void fillRect(PixelGrid *canvas, RGBA color, Rect *rect) {
    if (rect->max.x < 0 || rect->min.x >= canvas->dimensions.width ||
        rect->max.y < 0 || rect->min.y >= canvas->dimensions.height)
        return;

    i32 min_x, min_y, max_x, max_y;
    subRange(rect->min.x, rect->max.x, canvas->dimensions.width,  0, &min_x, &max_x);
    subRange(rect->min.y, rect->max.y, canvas->dimensions.height, 0, &min_y, &max_y);
    for (u16 y = min_y; y <= max_y; y++)
        drawHLine2D(canvas, color, min_x, max_x, y);
}

void fillTriangle(PixelGrid *canvas, RGBA color, f32 *X, f32 *Y) {
    u16 W = canvas->dimensions.width;
    u16 H = canvas->dimensions.height;
    f32 dx1, x1, y1, xs,
            dx2, x2, y2, xe,
            dx3, x3, y3, dy;
    i32 offset,
            x, x1i, y1i, x2i, xsi, ysi = 0,
            y, y2i, x3i, y3i, xei, yei = 0;
    for (u8 i = 1; i <= 2; i++) {
        if (Y[i] < Y[ysi]) ysi = i;
        if (Y[i] > Y[yei]) yei = i;
    }
    u8* id = ysi ? (ysi == 1 ?
                    (u8[3]){1, 2, 0} :
                    (u8[3]){2, 0, 1}) :
             (u8[3]){0, 1, 2};
    x1 = X[id[0]]; y1 = Y[id[0]]; x1i = (i32)x1; y1i = (i32)y1;
    x2 = X[id[1]]; y2 = Y[id[1]]; x2i = (i32)x2; y2i = (i32)y2;
    x3 = X[id[2]]; y3 = Y[id[2]]; x3i = (i32)x3; y3i = (i32)y3;
    dx1 = x1i == x2i || y1i == y2i ? 0 : (x2 - x1) / (y2 - y1);
    dx2 = x2i == x3i || y2i == y3i ? 0 : (x3 - x2) / (y3 - y2);
    dx3 = x1i == x3i || y1i == y3i ? 0 : (x3 - x1) / (y3 - y1);
    dy = 1 - (y1 - (f32)y1);
    xs = dx3 ? x1 + dx3 * dy : x1; ysi = (i32)Y[ysi];
    xe = dx1 ? x1 + dx1 * dy : x1; yei = (i32)Y[yei];
    offset = W * y1i;
    for (y = ysi; y < yei; y++){
        if (y == y3i) xs = dx2 ? (x3 + dx2 * (1 - (y3 - (f32)y3i))) : x3;
        if (y == y2i) xe = dx2 ? (x2 + dx2 * (1 - (y2 - (f32)y2i))) : x2;
        xsi = (i32)xs;
        xei = (i32)xe;
        for (x = xsi; x < xei; x++)
            if (x > 0 && x < W && y > 0 && y < H)
                canvas->pixels[offset + x].color = color;
        offset += W;
        xs += y < y3i ? dx3 : dx2;
        xe += y < y2i ? dx1 : dx2;
    }
}

void drawCircle(PixelGrid *canvas, RGBA color, i32 center_x, i32 center_y, i32 radius) {
    if (radius <= 1) {
        if (inRange(0, center_x, canvas->dimensions.width - 1) &&
            inRange(0, center_y, canvas->dimensions.height - 1))
            canvas->pixels[canvas->dimensions.width * center_y + center_x].color = color;

        return;
    }

    i32 width = canvas->dimensions.width;
    i32 size  = canvas->dimensions.width_times_height;

    i32 x = radius, y = 0, y2 = 0;
    i32 r2 = radius * radius;
    i32 x2 = r2;

    i32 Sx1 = center_x - radius;
    i32 Ex1 = center_x + radius;
    i32 Sy1 = center_y * width;
    i32 Ey1 = Sy1;

    i32 Sx2 = center_x;
    i32 Ex2 = center_x;
    i32 Sy2 = (center_y - radius) * width;
    i32 Ey2 = (center_y + radius) * width;

    while (y <= x) {
        if (Sy1 >= 0 && Sy1 < size) {
            if (Sx1 >= 0 && Sx1 < width) canvas->pixels[Sy1 + Sx1].color = color;
            if (Ex1 >= 0 && Ex1 < width) canvas->pixels[Sy1 + Ex1].color = color;
        }
        if (Ey1 >= 0 && Ey1 < size) {
            if (Sx1 >= 0 && Sx1 < width) canvas->pixels[Ey1 + Sx1].color = color;
            if (Ex1 >= 0 && Ex1 < width) canvas->pixels[Ey1 + Ex1].color = color;
        }

        if (Sy2 >= 0 && Sy2 < size) {
            if (Sx2 >= 0 && Sx2 < width) canvas->pixels[Sy2 + Sx2].color = color;
            if (Ex2 >= 0 && Ex2 < width) canvas->pixels[Sy2 + Ex2].color = color;
        }
        if (Ey2 >= 0 && Ey2 < size) {
            if (Sx2 >= 0 && Sx2 < width) canvas->pixels[Ey2 + Sx2].color = color;
            if (Ex2 >= 0 && Ex2 < width) canvas->pixels[Ey2 + Ex2].color = color;
        }

        if ((x2 + y2) > r2) {
            x -= 1;
            x2 = x * x;

            Sx1 += 1;
            Ex1 -= 1;

            Sy2 += width;
            Ey2 -= width;
        }

        y += 1;
        y2 = y * y;

        Sy1 -= width;
        Ey1 += width;

        Sx2 -= 1;
        Ex2 += 1;
    }
}

void fillCircle(PixelGrid *canvas, RGBA color, i32 center_x, i32 center_y, i32 radius) {
    if (radius <= 1) {
        if (inRange(0, center_x, canvas->dimensions.width - 1) &&
            inRange(0, center_y, canvas->dimensions.height - 1))
            canvas->pixels[canvas->dimensions.width * center_y + center_x].color = color;

        return;
    }

    i32 width = canvas->dimensions.width;
    i32 size  = canvas->dimensions.width_times_height;

    i32 x = radius, y = 0, y2 = 0;
    i32 r2 = radius * radius;
    i32 x2 = r2;

    i32 Sx1 = center_x - radius;
    i32 Ex1 = center_x + radius;
    i32 Sy1 = center_y * width;
    i32 Ey1 = Sy1;

    i32 Sx2 = center_x;
    i32 Ex2 = center_x;
    i32 Sy2 = (center_y - radius) * width;
    i32 Ey2 = (center_y + radius) * width;

    i32 i, start, end;

    while (y <= x) {
        start = Sx1 > 0 ? Sx1 : 0;
        end   = Ex1 < (width - 1) ? Ex1 : (width - 1);
        if (Sy1 >= 0 && Sy1 < size) for (i = start; i <= end; i++) canvas->pixels[Sy1 + i].color = color;
        if (Ey1 >= 0 && Ey1 < size) for (i = start; i <= end; i++) canvas->pixels[Ey1 + i].color = color;

        start = Sx2 > 0 ? Sx2 : 0;
        end   = Ex2 < (width - 1) ? Ex2 : (width - 1);
        if (Sy2 >= 0 && Sy2 < size) for (i = start; i <= end; i++) canvas->pixels[Sy2 + i].color = color;
        if (Ey2 >= 0 && Ey2 < size) for (i = start; i <= end; i++) canvas->pixels[Ey2 + i].color = color;

        if ((x2 + y2) > r2) {
            x -= 1;
            x2 = x * x;

            Sx1 += 1;
            Ex1 -= 1;

            Sy2 += width;
            Ey2 -= width;
        }

        y += 1;
        y2 = y * y;

        Sy1 -= width;
        Ey1 += width;

        Sx2 -= 1;
        Ex2 += 1;
    }
}

#define FONT_HEIGHT 8
#define FONT_WIDTH 8

unsigned char Font[768] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18,
        0x18, 0x00, 0x18, 0x00, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00, 0x18, 0x3e, 0x60, 0x3c,
        0x06, 0x7c, 0x18, 0x00, 0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,
        0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00, 0x18, 0x18, 0x30, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00,
        0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x66, 0x3c, 0xff,
        0x3c, 0x66, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x7e,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
        0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x38, 0x6c, 0xc6, 0xd6,
        0xc6, 0x6c, 0x38, 0x00, 0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00,
        0x7c, 0xc6, 0x06, 0x1c, 0x30, 0x66, 0xfe, 0x00, 0x7c, 0xc6, 0x06, 0x3c,
        0x06, 0xc6, 0x7c, 0x00, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00,
        0xfe, 0xc0, 0xc0, 0xfc, 0x06, 0xc6, 0x7c, 0x00, 0x38, 0x60, 0xc0, 0xfc,
        0xc6, 0xc6, 0x7c, 0x00, 0xfe, 0xc6, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00,
        0x7c, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0x7c, 0x00, 0x7c, 0xc6, 0xc6, 0x7e,
        0x06, 0x0c, 0x78, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,
        0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 0x06, 0x0c, 0x18, 0x30,
        0x18, 0x0c, 0x06, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00,
        0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x7c, 0xc6, 0x0c, 0x18,
        0x18, 0x00, 0x18, 0x00, 0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00,
        0x38, 0x6c, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0xfc, 0x66, 0x66, 0x7c,
        0x66, 0x66, 0xfc, 0x00, 0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00,
        0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, 0xfe, 0x62, 0x68, 0x78,
        0x68, 0x62, 0xfe, 0x00, 0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,
        0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3a, 0x00, 0xc6, 0xc6, 0xc6, 0xfe,
        0xc6, 0xc6, 0xc6, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00,
        0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00, 0xe6, 0x66, 0x6c, 0x78,
        0x6c, 0x66, 0xe6, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00,
        0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00, 0xc6, 0xe6, 0xf6, 0xde,
        0xce, 0xc6, 0xc6, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00,
        0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00, 0x7c, 0xc6, 0xc6, 0xc6,
        0xc6, 0xce, 0x7c, 0x0e, 0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00,
        0x3c, 0x66, 0x30, 0x18, 0x0c, 0x66, 0x3c, 0x00, 0x7e, 0x7e, 0x5a, 0x18,
        0x18, 0x18, 0x3c, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00,
        0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 0xc6, 0xc6, 0xc6, 0xd6,
        0xd6, 0xfe, 0x6c, 0x00, 0xc6, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0xc6, 0x00,
        0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x3c, 0x00, 0xfe, 0xc6, 0x8c, 0x18,
        0x32, 0x66, 0xfe, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00,
        0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00, 0x3c, 0x0c, 0x0c, 0x0c,
        0x0c, 0x0c, 0x3c, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x30, 0x18, 0x0c, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
        0xe0, 0x60, 0x7c, 0x66, 0x66, 0x66, 0xdc, 0x00, 0x00, 0x00, 0x7c, 0xc6,
        0xc0, 0xc6, 0x7c, 0x00, 0x1c, 0x0c, 0x7c, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
        0x00, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 0x3c, 0x66, 0x60, 0xf8,
        0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
        0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00, 0x18, 0x00, 0x38, 0x18,
        0x18, 0x18, 0x3c, 0x00, 0x06, 0x00, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3c,
        0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00, 0x38, 0x18, 0x18, 0x18,
        0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0xec, 0xfe, 0xd6, 0xd6, 0xd6, 0x00,
        0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x7c, 0xc6,
        0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0,
        0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e, 0x00, 0x00, 0xdc, 0x76,
        0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x7e, 0xc0, 0x7c, 0x06, 0xfc, 0x00,
        0x30, 0x30, 0xfc, 0x30, 0x30, 0x36, 0x1c, 0x00, 0x00, 0x00, 0xcc, 0xcc,
        0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,
        0x00, 0x00, 0xc6, 0xd6, 0xd6, 0xfe, 0x6c, 0x00, 0x00, 0x00, 0xc6, 0x6c,
        0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0xfc,
        0x00, 0x00, 0x7e, 0x4c, 0x18, 0x32, 0x7e, 0x00, 0x0e, 0x18, 0x18, 0x70,
        0x18, 0x18, 0x0e, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
        0x70, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x70, 0x00, 0x76, 0xdc, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00
};

#define LINE_HEIGHT 12
#define FIRST_CHARACTER_CODE 32
#define LAST_CHARACTER_CODE 127

void drawText(PixelGrid *canvas, RGBA color, char *str, i32 x, i32 y) {
    if (x < 0 || x > canvas->dimensions.width - FONT_WIDTH ||
        y < 0 || y > canvas->dimensions.height - FONT_HEIGHT)
        return;

    u16 current_x = x;
    u16 current_y = y;
    u16 t_offset;
    u16 pixel_line_step = canvas->dimensions.width - FONT_WIDTH;
    u32 char_line_step  = canvas->dimensions.width * LINE_HEIGHT;
    Pixel* pixel = canvas->pixels + canvas->dimensions.width * y + x;
    Pixel* character_pixel;
    u8* byte;
    char character = *str;

    while (character) {
        if (character == '\n') {
            if (current_y + FONT_HEIGHT > canvas->dimensions.height)
                break;

            pixel += char_line_step - current_x + x;
            current_x = x;
            current_y += LINE_HEIGHT;
        } else if (character == '\t') {
            t_offset = FONT_WIDTH * (4 - ((current_x / FONT_WIDTH) & 3));
            current_x += t_offset;
            pixel += t_offset;
        } else if (character >= FIRST_CHARACTER_CODE &&
                   character <= LAST_CHARACTER_CODE) {
            byte = Font + FONT_WIDTH*(character - FIRST_CHARACTER_CODE);
            character_pixel = pixel;
            for (int h = 0; h < FONT_HEIGHT ; h++) {
                for (int w = 0; w < FONT_WIDTH; w++) {
                    /* skip background bits */
                    if (*byte & (0x80 >> w))
                        character_pixel->color = color;

                    character_pixel++;
                }
                byte++;
                character_pixel += pixel_line_step;
            }

            pixel += FONT_WIDTH;
            current_x += FONT_WIDTH;
            if (current_x + FONT_WIDTH > canvas->dimensions.width)
                return;
        }
        character = *++str;
    }
}

void drawNumber(PixelGrid *canvas, RGBA color, i32 number, i32 x, i32 y) {
    static NumberStringBuffer number_string;
    printNumberIntoString(number, &number_string);
    drawText(canvas, color, number_string.string, x - number_string.digit_count * FONT_WIDTH, y);
}

typedef struct AppCallbacks {
    void (*windowRedraw)();
    void (*windowResize)(u16 width, u16 height);
    void (*keyChanged)(  u8 key, bool pressed);
    void (*mouseButtonUp)(  MouseButton *mouse_button);
    void (*mouseButtonDown)(MouseButton *mouse_button);
    void (*mouseWheelScrolled)(f32 amount);
    void (*mousePositionSet)(i32 x, i32 y);
    void (*mouseMovementSet)(i32 x, i32 y);
    void (*mouseRawMovementSet)(i32 x, i32 y);
} AppCallbacks;

typedef void* (*CallbackForFileOpen)(const char* file_path);
typedef bool  (*CallbackForFileRW)(void *out, unsigned long, void *handle);
typedef void  (*CallbackForFileClose)(void *handle);

typedef struct Platform {
    GetTicks             getTicks;
    CallbackWithInt      getMemory;
    CallbackWithCharPtr  setWindowTitle;
    CallbackWithBool     setWindowCapture;
    CallbackWithBool     setCursorVisibility;
    CallbackForFileClose closeFile;
    CallbackForFileOpen  openFileForReading;
    CallbackForFileOpen  openFileForWriting;
    CallbackForFileRW    readFromFile;
    CallbackForFileRW    writeToFile;
    u64 ticks_per_second;
} Platform;

typedef struct Defaults {
    char* title;
    u16 width, height;
} Defaults;

typedef struct App {
    Memory memory;
    Platform platform;
    Controls controls;
    PixelGrid window_content;
    AppCallbacks on;
    Time time;
    bool is_running;
    void *user_data;
} App;

App *app;

void initApp(Defaults *defaults);

void _windowRedraw() {
    if (!app->is_running) return;
    if (app->on.windowRedraw) app->on.windowRedraw();
}

void _windowResize(u16 width, u16 height) {
    if (!app->is_running) return;
    updateDimensions(&app->window_content.dimensions, width, height);

    if (app->on.windowResize) app->on.windowResize(width, height);
    if (app->on.windowRedraw) app->on.windowRedraw();
}

void _keyChanged(u8 key, bool pressed) {
    if (key == app->controls.key_map.ctrl) app->controls.is_pressed.ctrl  = pressed;
    else if (key == app->controls.key_map.alt) app->controls.is_pressed.alt   = pressed;
    else if (key == app->controls.key_map.shift) app->controls.is_pressed.shift = pressed;
    else if (key == app->controls.key_map.space) app->controls.is_pressed.space = pressed;
    else if (key == app->controls.key_map.tab) app->controls.is_pressed.tab   = pressed;

    if (app->on.keyChanged) app->on.keyChanged(key, pressed);
}

void _mouseButtonDown(MouseButton *mouse_button, i32 x, i32 y) {
    mouse_button->is_pressed = true;
    mouse_button->is_handled = false;

    mouse_button->down_pos.x = x;
    mouse_button->down_pos.y = y;

    if (app->on.mouseButtonDown) app->on.mouseButtonDown(mouse_button);
}

void _mouseButtonUp(MouseButton *mouse_button, i32 x, i32 y) {
    mouse_button->is_pressed = false;
    mouse_button->is_handled = false;

    mouse_button->up_pos.x = x;
    mouse_button->up_pos.y = y;

    if (app->on.mouseButtonUp) app->on.mouseButtonUp(mouse_button);
}

void _mouseWheelScrolled(f32 amount) {
    app->controls.mouse.wheel_scroll_amount += amount * 100;
    app->controls.mouse.wheel_scrolled = true;

    if (app->on.mouseWheelScrolled) app->on.mouseWheelScrolled(amount);
}

void _mousePositionSet(i32 x, i32 y) {
    app->controls.mouse.pos.x = x;
    app->controls.mouse.pos.y = y;

    if (app->on.mousePositionSet) app->on.mousePositionSet(x, y);
}

void _mouseMovementSet(i32 x, i32 y) {
    app->controls.mouse.movement.x = x - app->controls.mouse.pos.x;
    app->controls.mouse.movement.y = y - app->controls.mouse.pos.y;
    app->controls.mouse.moved = true;

    if (app->on.mouseMovementSet) app->on.mouseMovementSet(x, y);
}

void _mouseRawMovementSet(i32 x, i32 y) {
    app->controls.mouse.pos_raw_diff.x += x;
    app->controls.mouse.pos_raw_diff.y += y;
    app->controls.mouse.moved = true;

    if (app->on.mouseRawMovementSet) app->on.mouseRawMovementSet(x, y);
}

bool initAppMemory(u64 size) {
    if (app->memory.address) return false;

    void* memory_address = app->platform.getMemory(size);
    if (!memory_address) {
        app->is_running = false;
        return false;
    }

    initMemory(&app->memory, (u8*)memory_address, size);
    return true;
}

void* allocateAppMemory(u64 size) {
    void *new_memory = allocateMemory(&app->memory, size);
    if (new_memory) return new_memory;

    app->is_running = false;
    return null;
}

void _initApp(Defaults *defaults, void* window_content_memory) {
    defaults->title = "";
    defaults->width = 480;
    defaults->height = 360;

    app->is_running = true;
    app->user_data = null;
    app->memory.address = null;

    app->on.windowRedraw = null;
    app->on.keyChanged = null;
    app->on.mouseButtonUp = null;
    app->on.mouseButtonDown = null;
    app->on.mouseWheelScrolled = null;
    app->on.mousePositionSet = null;
    app->on.mouseMovementSet = null;
    app->on.mouseRawMovementSet = null;

    initTime(&app->time, app->platform.getTicks, app->platform.ticks_per_second);
    initControls(&app->controls);
    initPixelGrid(&app->window_content, (Pixel*)window_content_memory);
    initApp(defaults);
}

#include "./SlimApp/platforms/win32.h"