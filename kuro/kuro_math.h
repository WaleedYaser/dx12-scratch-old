//
// kuro_math.h - C++ single header mathematics library for graphics programming
//
// Copyright (c) 2020-2021 Waleed Yaser
//
// TODO[Waleed]:
// * add equal and not equal operator
// * implement sqrt, sin, cos, tan

#pragma once

extern "C" {
    // forward declaration (cmath)
    double __cdecl sqrt(double);
    double __cdecl sin(double);
    double __cdecl cos(double);
    double __cdecl tan(double);
}

namespace kuro
{
    // =================================================================================================
    // == TYPEDEFS =====================================================================================
    // =================================================================================================

    using i8  = signed char;
    using i16 = short;
    using i32 = int;
    using i64 = long long;
    using u8  = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;
    using f32 = float;
    using f64 = double;

    // =================================================================================================
    // == LIMITS =======================================================================================
    // =================================================================================================

    static constexpr i8  I8_MIN  = -128;
    static constexpr i16 I16_MIN = -32'767-1;
    static constexpr i32 I32_MIN = -2'147'483'647-1;
    static constexpr i64 I64_MIN = -9'223'372'036'854'775'807ll-1;
    static constexpr u8  U8_MIN  = 0u;
    static constexpr u16 U16_MIN = 0u;
    static constexpr u32 U32_MIN = 0u;
    static constexpr u64 U64_MIN = 0ull;
    static constexpr f32 F32_MIN = 1.175494351e-38f;          // min normalized positive value
    static constexpr f64 F64_MIN = 2.2250738585072014e-308;   // min normalized positive value

    static constexpr i8  I8_MAX  = 127;
    static constexpr i16 I16_MAX = 32'767;
    static constexpr i32 I32_MAX = 2'147'483'647;
    static constexpr i64 I64_MAX = 9'223'372'036'854'775'807ll;
    static constexpr u8  U8_MAX  = 255u;
    static constexpr u16 U16_MAX = 65'535u;
    static constexpr u32 U32_MAX = 4'294'967'295u;
    static constexpr u64 U64_MAX = 18'446'744'073'709'551'615ull;
    static constexpr f32 F32_MAX = 3.402823466e+38f;          // max value
    static constexpr f64 F64_MAX = 1.7976931348623158e+308;   // max value

    // =================================================================================================
    // == CONSTS =======================================================================================
    // =================================================================================================

    static constexpr f64 PI        = 3.14159265358979323846;
    static constexpr f64 PI_OVER_2 = PI * 0.5;
    static constexpr f64 TAU       = 2.0 * PI;
    static constexpr f64 TO_DEGREE = 360.0 / TAU;
    static constexpr f64 TO_RADIAN = TAU / 360.0;

    // =================================================================================================
    // == TYPES ========================================================================================
    // =================================================================================================

    struct vec2
    {
        f32 x, y;
    };

    struct vec3
    {
        f32 x, y, z;
    };

    struct vec4
    {
        f32 x, y, z, w;
    };

    struct mat2
    {
        f32 m00, m01;
        f32 m10, m11;
    };

    struct mat3
    {
        f32 m00, m01, m02;
        f32 m10, m11, m12;
        f32 m20, m21, m22;
    };

    struct mat4
    {
        f32 m00, m01, m02, m03;
        f32 m10, m11, m12, m13;
        f32 m20, m21, m22, m23;
        f32 m30, m31, m32, m33;
    };

    // =================================================================================================
    // == MATH =========================================================================================
    // =================================================================================================

    inline static f32
    sqrt(f32 f)
    {
        return (kuro::f32)::sqrt(f);
    }

    inline static f64
    sqrt(f64 f)
    {
        return ::sqrt(f);
    }

    inline static f32
    sin(f32 f)
    {
        return (kuro::f32)::sin(f);
    }

    inline static f64
    sin(f64 f)
    {
        return ::sin(f);
    }

    inline static f32
    cos(f32 f)
    {
        return (kuro::f32)::cos(f);
    }

    inline static f64
    cos(f64 f)
    {
        return ::cos(f);
    }

    inline static f32
    tan(f32 f)
    {
        return (kuro::f32)::tan(f);
    }

    inline static f64
    tan(f64 f)
    {
        return ::tan(f);
    }

    // =================================================================================================
    // == VEC2 =========================================================================================
    // =================================================================================================

    inline static vec2
    operator+(const vec2 &a, const vec2 &b)
    {
        return vec2{a.x + b.x, a.y + b.y};
    }

    inline static vec2 &
    operator+=(vec2 &a, const vec2 &b)
    {
        a = a + b;
        return a;
    }

    inline static vec2
    operator-(const vec2 &v)
    {
        return vec2{-v.x, -v.y};
    }

    inline static vec2
    operator-(const vec2 &a, const vec2 &b)
    {
        return vec2{a.x - b.x, a.y - b.y};
    }

    inline static vec2 &
    operator-=(vec2 &a, const vec2 &b)
    {
        a = a - b;
        return a;
    }

    inline static vec2
    operator*(const vec2 &v, f32 f)
    {
        return vec2{v.x * f, v.y * f};
    }

    inline static vec2
    operator*(f32 f, const vec2 &v)
    {
        return v * f;
    }

    inline static vec2 &
    operator*=(vec2 &v, f32 f)
    {
        v = v * f;
        return v;
    }

    inline static vec2
    operator/(const vec2 &v, f32 f)
    {
        return v * (1.0f / f);
    }

    inline static vec2 &
    operator/=(vec2 &v, f32 f)
    {
        v = v / f;
        return v;
    }

    inline static f32
    dot(const vec2 &a, const vec2 &b)
    {
        return (a.x * b.x + a.y * b.y);
    }

    inline static f32
    norm(const vec2 &v)
    {
        return sqrt(dot(v, v));
    }

    inline static f32
    length(const vec2 &v)
    {
        return norm(v);
    }

    inline static vec2
    normalize(const vec2 &v)
    {
        return v / length(v);
    }

    inline static f32
    cross(const vec2 &a, const vec2 &b)
    {
        return (a.x * b.y - a.y * b.x);
    }

    // =================================================================================================
    // == VEC3 =========================================================================================
    // =================================================================================================

    inline static vec3
    operator+(const vec3 &a, const vec3 &b)
    {
        return vec3{a.x + b.x, a.y + b.y, a.z + b.z};
    }

    inline static vec3 &
    operator+=(vec3 &a, const vec3 &b)
    {
        a = a + b;
        return a;
    }

    inline static vec3
    operator-(const vec3 &v)
    {
        return vec3{-v.x, -v.y, -v.z};
    }

    inline static vec3
    operator-(const vec3 &a, const vec3 &b)
    {
        return vec3{a.x - b.x, a.y - b.y, a.z - b.z};
    }

    inline static vec3 &
    operator-=(vec3 &a, const vec3 &b)
    {
        a = a - b;
        return a;
    }

    inline static vec3
    operator*(const vec3 &v, f32 f)
    {
        return vec3{v.x * f, v.y * f, v.z * f};
    }

    inline static vec3
    operator*(f32 f, const vec3 &v)
    {
        return v * f;
    }

    inline static vec3 &
    operator*=(vec3 &v, f32 f)
    {
        v = v * f;
        return v;
    }

    inline static vec3
    operator/(const vec3 &v, f32 f)
    {
        return v * (1.0f / f);
    }

    inline static vec3 &
    operator/=(vec3 &v, f32 f)
    {
        v = v / f;
        return v;
    }

    inline static f32
    dot(const vec3 &a, const vec3 &b)
    {
        return (a.x * b.x + a.y * b.y + a.z * b.z);
    }

    inline static f32
    norm(const vec3 &v)
    {
        return sqrt(dot(v, v));
    }

    inline static f32
    length(const vec3 &v)
    {
        return norm(v);
    }

    inline static vec3
    normalize(const vec3 &v)
    {
        return v / length(v);
    }

    inline static vec3
    cross(const vec3 &a, const vec3 &b)
    {
        return vec3{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
    }

    // =================================================================================================
    // == VEC4 =========================================================================================
    // =================================================================================================

    inline static vec4
    operator+(const vec4 &a, const vec4 &b)
    {
        return vec4{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }

    inline static vec4 &
    operator+=(vec4 &a, const vec4 &b)
    {
        a = a + b;
        return a;
    }

    inline static vec4
    operator-(const vec4 &v)
    {
        return vec4{-v.x, -v.y, -v.z, -v.w};
    }

    inline static vec4
    operator-(const vec4 &a, const vec4 &b)
    {
        return vec4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }

    inline static vec4 &
    operator-=(vec4 &a, const vec4 &b)
    {
        a = a - b;
        return a;
    }

    inline static vec4
    operator*(const vec4 &v, f32 f)
    {
        return vec4{v.x * f, v.y * f, v.z * f, v.w * f};
    }

    inline static vec4
    operator*(f32 f, const vec4 &v)
    {
        return v * f;
    }

    inline static vec4 &
    operator*=(vec4 &v, f32 f)
    {
        v = v * f;
        return v;
    }

    inline static vec4
    operator/(const vec4 &v, f32 f)
    {
        return v * (1.0f / f);
    }

    inline static vec4 &
    operator/=(vec4 &v, f32 f)
    {
        v = v / f;
        return v;
    }

    inline static f32
    dot(const vec4 &a, const vec4 &b)
    {
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    }

    inline static f32
    norm(const vec4 &v)
    {
        return sqrt(dot(v, v));
    }

    inline static f32
    length(const vec4 &v)
    {
        return norm(v);
    }

    inline static vec4
    normalize(const vec4 &v)
    {
        return v / length(v);
    }

    // =================================================================================================
    // == MAT2 =========================================================================================
    // =================================================================================================

    inline static mat2
    operator+(const mat2 &A, const mat2 &B)
    {
        return mat2{
            A.m00 + B.m00, A.m01 + B.m01,
            A.m10 + B.m10, A.m11 + B.m11
        };
    }

    inline static mat2 &
    operator+=(mat2 &A, const mat2 &B)
    {
        A = A + B;
        return A;
    }

    inline static mat2
    operator-(const mat2 &M)
    {
        return mat2{
            -M.m00, -M.m01,
            -M.m10, -M.m11
        };
    }

    inline static mat2
    operator-(const mat2 &A, const mat2 &B)
    {
        return mat2{
            A.m00 - B.m00, A.m01 - B.m01,
            A.m10 - B.m10, A.m11 - B.m11
        };
    }

    inline static mat2 &
    operator-=(mat2 &A, const mat2 &B)
    {
        A = A - B;
        return A;
    }

    inline static mat2
    operator*(const mat2 &M, f32 f)
    {
        return mat2{
            M.m00 * f, M.m01 * f,
            M.m10 * f, M.m11 * f
        };
    }

    inline static mat2
    operator*(f32 f, const mat2 &M)
    {
        return M * f;
    }

    inline static mat2 &
    operator*=(mat2 &M, f32 f)
    {
        M = M * f;
        return M;
    }

    inline static vec2
    operator*(const vec2 &v, const mat2 &M)
    {
        return vec2{
            v.x * M.m00 + v.y * M.m10,
            v.x * M.m01 + v.y * M.m11
        };
    }

    inline static mat2
    operator*(const mat2 &A, const mat2 &B)
    {
        mat2 C;

        C.m00 = A.m00 * B.m00 + A.m01 * B.m10;
        C.m01 = A.m00 * B.m01 + A.m01 * B.m11;

        C.m10 = A.m10 * B.m00 + A.m11 * B.m10;
        C.m11 = A.m10 * B.m01 + A.m11 * B.m11;

        return C;
    }

    inline static mat2
    operator/(const mat2 &M, f32 f)
    {
        return mat2{
            M.m00 / f, M.m01 / f,
            M.m10 / f, M.m11 / f
        };
    }

    inline static mat2
    operator/(f32 f, const mat2 &M)
    {
        return M / f;
    }

    inline static mat2 &
    operator/=(mat2 &M, f32 f)
    {
        M = M / f;
        return M;
    }

    inline static mat2
    mat2_identity()
    {
        return mat2{
            1, 0,
            0, 1
        };
    }

    inline static mat2
    mat2_transpose(const mat2 &M)
    {
        return mat2{
            M.m00, M.m10,
            M.m01, M.m11
        };
    }

    inline static f32
    mat2_trace(const mat2 &M)
    {
        return M.m00 + M.m11;
    }

    inline static f32
    mat2_det(const mat2 &M)
    {
        return M.m00 * M.m11 - M.m01 * M.m10;
    }

    inline static bool
    mat2_invertible(const mat2 &M)
    {
        return mat2_det(M) != 0.0f;
    }

    inline static mat2
    mat2_inverse(const mat2 &M)
    {
        f32 d = mat2_det(M);
        if (d == 0.0f)
            return mat2{};

        return (1.0f / d) * mat2{
             M.m11, -M.m01,
            -M.m10,  M.m00};
    }

    inline static mat2
    mat2_rotation(f32 theta)
    {
        f32 c = cos(theta);
        f32 s = sin(theta);

        return mat2{
             c, s,
            -s, c};
    }

    inline static mat2
    mat2_scaling(f32 sx, f32 sy)
    {
        return mat2{
            sx,  0,
             0, sy};
    }

    inline static mat2
    mat2_scaling(const vec2 &s)
    {
        return mat2_scaling(s.x, s.y);
    }

    inline static mat2
    mat2_shearing_x(f32 s)
    {
        return mat2{
            1, 0,
            s, 1
        };
    }

    inline static mat2
    mat2_shearing_y(f32 s)
    {
        return mat2{
            1, s,
            0, 1
        };
    }

    // =================================================================================================
    // == MAT3 =========================================================================================
    // =================================================================================================

    inline static mat3
    operator+(const mat3 &A, const mat3 &B)
    {
        return mat3{
            A.m00 + B.m00, A.m01 + B.m01, A.m02 + B.m02,
            A.m10 + B.m10, A.m11 + B.m11, A.m12 + B.m12,
            A.m20 + B.m20, A.m21 + B.m21, A.m22 + B.m22
        };
    }

    inline static mat3 &
    operator+=(mat3 &A, const mat3 &B)
    {
        A = A + B;
        return A;
    }

    inline static mat3
    operator-(const mat3 &M)
    {
        return mat3{
            -M.m00, -M.m01, -M.m02,
            -M.m10, -M.m11, -M.m12,
            -M.m20, -M.m21, -M.m22
        };
    }

    inline static mat3
    operator-(const mat3 &A, const mat3 &B)
    {
        return mat3{
            A.m00 - B.m00, A.m01 - B.m01, A.m02 - B.m02,
            A.m10 - B.m10, A.m11 - B.m11, A.m12 - B.m12,
            A.m20 - B.m20, A.m21 - B.m21, A.m22 - B.m22
        };
    }

    inline static mat3 &
    operator-=(mat3 &A, const mat3 &B)
    {
        A = A - B;
        return A;
    }

    inline static mat3
    operator*(const mat3 &M, f32 f)
    {
        return mat3{
            M.m00 * f, M.m01 * f, M.m02 * f,
            M.m10 * f, M.m11 * f, M.m12 * f,
            M.m20 * f, M.m21 * f, M.m22 * f
        };
    }

    inline static mat3
    operator*(f32 f, const mat3 &M)
    {
        return M * f;
    }

    inline static mat3 &
    operator*=(mat3 &M, f32 f)
    {
        M = M * f;
        return M;
    }

    inline static vec3
    operator*(const vec3 &v, const mat3 &M)
    {
        return vec3{
            v.x * M.m00 + v.y * M.m10 + v.z * M.m20,
            v.x * M.m01 + v.y * M.m11 + v.z * M.m21,
            v.x * M.m02 + v.y * M.m12 + v.z * M.m22
        };
    }

    inline static mat3
    operator*(const mat3 &A, const mat3 &B)
    {
        mat3 C;

        C.m00 = A.m00 * B.m00 + A.m01 * B.m10 + A.m02 * B.m20;
        C.m01 = A.m00 * B.m01 + A.m01 * B.m11 + A.m02 * B.m21;
        C.m02 = A.m00 * B.m02 + A.m01 * B.m12 + A.m02 * B.m22;

        C.m10 = A.m10 * B.m00 + A.m11 * B.m10 + A.m12 * B.m20;
        C.m11 = A.m10 * B.m01 + A.m11 * B.m11 + A.m12 * B.m21;
        C.m12 = A.m10 * B.m02 + A.m11 * B.m12 + A.m12 * B.m22;

        C.m20 = A.m20 * B.m00 + A.m21 * B.m10 + A.m22 * B.m20;
        C.m21 = A.m20 * B.m01 + A.m21 * B.m11 + A.m22 * B.m21;
        C.m22 = A.m20 * B.m02 + A.m21 * B.m12 + A.m22 * B.m22;

        return C;
    }

    inline static mat3
    operator/(const mat3 &M, f32 f)
    {
        return mat3{
            M.m00 / f, M.m01 / f, M.m02 / f,
            M.m10 / f, M.m11 / f, M.m12 / f,
            M.m20 / f, M.m21 / f, M.m22 / f
        };
    }

    inline static mat3
    operator/(f32 f, const mat3 &M)
    {
        return M / f;
    }

    inline static mat3 &
    operator/=(mat3 &M, f32 f)
    {
        M = M / f;
        return M;
    }

    inline static mat3
    mat3_identity()
    {
        return mat3{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
    }

    inline static mat3
    mat3_transpose(const mat3 &M)
    {
        return mat3{
            M.m00, M.m10, M.m20,
            M.m01, M.m11, M.m21,
            M.m02, M.m12, M.m22
        };
    }

    inline static f32
    mat3_trace(const mat3 &M)
    {
        return M.m00 + M.m11 + M.m22;
    }

    inline static f32
    mat3_det(const mat3 &M)
    {
        return
            + M.m00 * (M.m11 * M.m22 - M.m12 * M.m21)
            - M.m01 * (M.m10 * M.m22 - M.m12 * M.m20)
            + M.m02 * (M.m10 * M.m21 - M.m11 * M.m20);
    }

    inline static mat3
    mat3_adj(const mat3 &M)
    {
        return mat3{
            // m00
            + (M.m11 * M.m22 - M.m12 * M.m21),
            // m10
            - (M.m01 * M.m22 - M.m02 * M.m21),
            // m20
            + (M.m01 * M.m12 - M.m02 * M.m11),

            // m01
            - (M.m10 * M.m22 - M.m12 * M.m20),
            // m11
            + (M.m00 * M.m22 - M.m02 * M.m20),
            // m21
            - (M.m00 * M.m12 - M.m02 * M.m10),

            // m02
            + (M.m10 * M.m21 - M.m11 * M.m20),
            // m12
            - (M.m00 * M.m21 - M.m01 * M.m20),
            // m22
            + (M.m00 * M.m11 - M.m01 * M.m10)
        };
    }

    inline static bool
    mat3_invertible(const mat3 &M)
    {
        return mat3_det(M) != 0.0f;
    }

    inline static mat3
    mat3_inverse(const mat3 &M)
    {
        f32 d = mat3_det(M);
        if (d == 0)
            return mat3{};

        return (1.0f / d) * mat3_adj(M);
    }

    inline static mat3
    mat3_translation_2d(f32 dx, f32 dy)
    {
        return mat3{
             1,  0, 0,
             0,  1, 0,
            dx, dy, 1
        };
    }

    inline static mat3
    mat3_translation_2d(const vec2 &translation)
    {
        return mat3_translation_2d(translation.x, translation.y);
    }

    inline static mat3
    mat3_rotation_2d(f32 theta)
    {
        f32 c = cos(theta);
        f32 s = sin(theta);

        return mat3{
             c, s, 0,
            -s, c, 0,
             0, 0, 1
        };
    }

    inline static mat3
    mat3_scaling_2d(f32 sx, f32 sy)
    {
        return mat3{
            sx,  0, 0,
             0, sy, 0,
             0,  0, 1
        };
    }

    inline static mat3
    mat3_scaling_2d(vec2 s)
    {
        return mat3_scaling_2d(s.x, s.y);
    }

    inline static mat3
    mat3_rotation_x(f32 pitch)
    {
        f32 c = cos(pitch);
        f32 s = sin(pitch);

        return mat3{
            1,  0, 0,
            0,  c, s,
            0, -s, c};
    }

    inline static mat3
    mat3_rotation_y(f32 yaw)
    {
        f32 c = cos(yaw);
        f32 s = sin(yaw);

        return mat3{
            c, 0, -s,
            0, 1,  0,
            s, 0,  c};
    }

    inline static mat3
    mat3_rotation_z(f32 roll)
    {
        f32 c = cos(roll);
        f32 s = sin(roll);

        return mat3{
             c, s, 0,
            -s, c, 0,
             0, 0, 1};
    }

    inline static mat3
    mat3_scaling(f32 sx, f32 sy, f32 sz)
    {
        return mat3{
            sx,  0,  0,
             0, sy,  0,
             0,  0, sz};
    }

    inline static mat3
    mat3_scaling(const vec3 &scaling)
    {
        return mat3_scaling(scaling.x, scaling.y, scaling.z);
    }

    inline mat3
    mat3_shearing_xy(f32 s)
    {
        return mat3{
            1, 0, 0,
            s, 1, 0,
            0, 0, 1
        };
    }

    inline mat3
    mat3_shearing_xz(f32 s)
    {
        return mat3{
            1, 0, 0,
            0, 1, 0,
            s, 0, 1
        };
    }

    inline mat3
    mat3_shearing_yx(f32 s)
    {
        return mat3{
            1, s, 0,
            0, 1, 0,
            0, 0, 1
        };
    }

    inline mat3
    mat3_shearing_yz(f32 s)
    {
        return mat3{
            1, 0, 0,
            0, 1, 0,
            0, s, 1
        };
    }

    inline mat3
    mat3_shearing_zx(f32 s)
    {
        return mat3{
            1, 0, s,
            0, 1, 0,
            0, 0, 1
        };
    }

    inline mat3
    mat3_shearing_zy(f32 s)
    {
        return mat3{
            1, 0, 0,
            0, 1, s,
            0, 0, 1
        };
    }

    // =================================================================================================
    // == MAT4 =========================================================================================
    // =================================================================================================

    inline mat4
    operator+(const mat4 &A, const mat4 &B)
    {
        return mat4{
            A.m00 + B.m00, A.m01 + B.m01, A.m02 + B.m02, A.m03 + B.m03,
            A.m10 + B.m10, A.m11 + B.m11, A.m12 + B.m12, A.m13 + B.m13,
            A.m20 + B.m20, A.m21 + B.m21, A.m22 + B.m22, A.m23 + B.m23,
            A.m30 + B.m30, A.m31 + B.m31, A.m32 + B.m32, A.m33 + B.m33
        };
    }

    inline mat4 &
    operator+=(mat4 &A, const mat4 &B)
    {
        A = A + B;
        return A;
    }

    inline mat4
    operator-(const mat4 &M)
    {
        return mat4{
            -M.m00, -M.m01, -M.m02, -M.m03,
            -M.m10, -M.m11, -M.m12, -M.m13,
            -M.m20, -M.m21, -M.m22, -M.m23,
            -M.m30, -M.m31, -M.m32, -M.m33
        };
    }

    inline mat4
    operator-(const mat4 &A, const mat4 &B)
    {
        return mat4{
            A.m00 - B.m00, A.m01 - B.m01, A.m02 - B.m02, A.m03 - B.m03,
            A.m10 - B.m10, A.m11 - B.m11, A.m12 - B.m12, A.m13 - B.m13,
            A.m20 - B.m20, A.m21 - B.m21, A.m22 - B.m22, A.m23 - B.m23,
            A.m30 - B.m30, A.m31 - B.m31, A.m32 - B.m32, A.m33 - B.m33
        };
    }

    inline mat4 &
    operator-=(mat4 &A, const mat4 &B)
    {
        A = A - B;
        return A;
    }

    inline mat4
    operator*(const mat4 &M, f32 f)
    {
        return mat4{
            M.m00 * f, M.m01 * f, M.m02 * f, M.m03 * f,
            M.m10 * f, M.m11 * f, M.m12 * f, M.m13 * f,
            M.m20 * f, M.m21 * f, M.m22 * f, M.m23 * f,
            M.m30 * f, M.m31 * f, M.m32 * f, M.m33 * f
        };
    }

    inline mat4
    operator*(f32 f, const mat4 &M)
    {
        return M * f;
    }

    inline mat4 &
    operator*=(mat4 &M, f32 f)
    {
        M = M * f;
        return M;
    }

    inline vec4
    operator*(const vec4 &v, const mat4 &M)
    {
        return vec4{
            v.x * M.m00 + v.y * M.m10 + v.z * M.m20 + v.w * M.m30,
            v.x * M.m01 + v.y * M.m11 + v.z * M.m21 + v.w * M.m31,
            v.x * M.m02 + v.y * M.m12 + v.z * M.m22 + v.w * M.m32,
            v.x * M.m03 + v.y * M.m13 + v.z * M.m23 + v.w * M.m33
        };
    }

    inline mat4
    operator*(const mat4 &A, const mat4 &B)
    {
        mat4 C;

        C.m00 = A.m00 * B.m00 + A.m01 * B.m10 + A.m02 * B.m20 + A.m03 * B.m30;
        C.m01 = A.m00 * B.m01 + A.m01 * B.m11 + A.m02 * B.m21 + A.m03 * B.m31;
        C.m02 = A.m00 * B.m02 + A.m01 * B.m12 + A.m02 * B.m22 + A.m03 * B.m32;
        C.m03 = A.m00 * B.m03 + A.m01 * B.m13 + A.m02 * B.m23 + A.m03 * B.m33;

        C.m10 = A.m10 * B.m00 + A.m11 * B.m10 + A.m12 * B.m20 + A.m13 * B.m30;
        C.m11 = A.m10 * B.m01 + A.m11 * B.m11 + A.m12 * B.m21 + A.m13 * B.m31;
        C.m12 = A.m10 * B.m02 + A.m11 * B.m12 + A.m12 * B.m22 + A.m13 * B.m32;
        C.m13 = A.m10 * B.m03 + A.m11 * B.m13 + A.m12 * B.m23 + A.m13 * B.m33;

        C.m20 = A.m20 * B.m00 + A.m21 * B.m10 + A.m22 * B.m20 + A.m23 * B.m30;
        C.m21 = A.m20 * B.m01 + A.m21 * B.m11 + A.m22 * B.m21 + A.m23 * B.m31;
        C.m22 = A.m20 * B.m02 + A.m21 * B.m12 + A.m22 * B.m22 + A.m23 * B.m32;
        C.m23 = A.m20 * B.m03 + A.m21 * B.m13 + A.m22 * B.m23 + A.m23 * B.m33;

        C.m30 = A.m30 * B.m00 + A.m31 * B.m10 + A.m32 * B.m20 + A.m33 * B.m30;
        C.m31 = A.m30 * B.m01 + A.m31 * B.m11 + A.m32 * B.m21 + A.m33 * B.m31;
        C.m32 = A.m30 * B.m02 + A.m31 * B.m12 + A.m32 * B.m22 + A.m33 * B.m32;
        C.m33 = A.m30 * B.m03 + A.m31 * B.m13 + A.m32 * B.m23 + A.m33 * B.m33;

        return C;
    }


    inline mat4
    operator/(const mat4 &M, f32 f)
    {
        return mat4{
            M.m00 / f, M.m01 / f, M.m02 / f, M.m03 / f,
            M.m10 / f, M.m11 / f, M.m12 / f, M.m13 / f,
            M.m20 / f, M.m21 / f, M.m22 / f, M.m23 / f,
            M.m30 / f, M.m31 / f, M.m32 / f, M.m33 / f
        };
    }

    inline mat4
    operator/(f32 f, const mat4 &M)
    {
        return M / f;
    }

    inline mat4 &
    operator/=(mat4 &M, f32 f)
    {
        M = M / f;
        return M;
    }

    inline mat4
    mat4_identity()
    {
        return mat4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    inline mat4
    mat4_transpose(const mat4 &M)
    {
        return mat4{
            M.m00, M.m10, M.m20, M.m30,
            M.m01, M.m11, M.m21, M.m31,
            M.m02, M.m12, M.m22, M.m32,
            M.m03, M.m13, M.m23, M.m33
        };
    }

    inline static f32
    mat4_trace(const mat4 &M)
    {
        return M.m00 + M.m11 + M.m22 + M.m33;
    }

    inline f32
    mat4_det(const mat4 &M)
    {
        /*
        * before optimization:
            + M.m00 * (
                + M.m11 * (M.m22 * M.m33 - M.m23 * M.m32)
                - M.m12 * (M.m21 * M.m33 - M.m23 * M.m31)
                + M.m13 * (M.m21 * M.m32 - M.m22 * M.m31))
            - M.m01 * (
                + M.m10 * (M.m22 * M.m33 - M.m23 * M.m32)
                - M.m12 * (M.m20 * M.m33 - M.m23 * M.m30)
                + M.m13 * (M.m20 * M.m32 - M.m22 * M.m30))
            + M.m02 * (
                + M.m10 * (M.m21 * M.m33 - M.m23 * M.m31)
                - M.m11 * (M.m20 * M.m33 - M.m23 * M.m30)
                + M.m13 * (M.m20 * M.m31 - M.m21 * M.m30))
            - M.m03 * (
                + M.m10 * (M.m21 * M.m32 - M.m22 * M.m31)
                - M.m11 * (M.m20 * M.m32 - M.m22 * M.m30)
                + M.m12 * (M.m20 * M.m31 - M.m21 * M.m30));
        */
        return
            (M.m00 * M.m11 - M.m01 * M.m10) * (M.m22 * M.m33 - M.m23 * M.m32) -
            (M.m00 * M.m12 - M.m02 * M.m10) * (M.m21 * M.m33 - M.m23 * M.m31) +
            (M.m00 * M.m13 - M.m03 * M.m10) * (M.m21 * M.m32 - M.m22 * M.m31) +
            (M.m01 * M.m12 - M.m02 * M.m11) * (M.m20 * M.m33 - M.m23 * M.m30) -
            (M.m01 * M.m13 - M.m03 * M.m11) * (M.m20 * M.m32 - M.m22 * M.m30) +
            (M.m02 * M.m13 - M.m03 * M.m12) * (M.m20 * M.m31 - M.m21 * M.m30);
    }

    inline mat4
    mat4_adj(const mat4 &M)
    {
        return mat4{
            // m00
            + M.m11 * (M.m22 * M.m33 - M.m23 * M.m32)
            - M.m12 * (M.m21 * M.m33 - M.m23 * M.m31)
            + M.m13 * (M.m21 * M.m32 - M.m22 * M.m31),
            // m10
            - M.m01 * (M.m22 * M.m33 - M.m23 * M.m32)
            + M.m02 * (M.m21 * M.m33 - M.m23 * M.m31)
            - M.m03 * (M.m21 * M.m32 - M.m22 * M.m31),
            // m20
            + M.m01 * (M.m12 * M.m33 - M.m13 * M.m32)
            - M.m02 * (M.m11 * M.m33 - M.m13 * M.m31)
            + M.m03 * (M.m11 * M.m32 - M.m12 * M.m31),
            // m30
            - M.m01 * (M.m12 * M.m23 - M.m13 * M.m22)
            + M.m02 * (M.m11 * M.m23 - M.m13 * M.m21)
            - M.m03 * (M.m11 * M.m22 - M.m12 * M.m21),

            // m01
            - M.m10 * (M.m22 * M.m33 - M.m23 * M.m32)
            + M.m12 * (M.m20 * M.m33 - M.m23 * M.m30)
            - M.m13 * (M.m20 * M.m32 - M.m22 * M.m30),
            // m11
            + M.m00 * (M.m22 * M.m33 - M.m23 * M.m32)
            - M.m02 * (M.m20 * M.m33 - M.m23 * M.m30)
            + M.m03 * (M.m20 * M.m32 - M.m22 * M.m30),
            // m21
            - M.m00 * (M.m12 * M.m33 - M.m13 * M.m32)
            + M.m02 * (M.m10 * M.m33 - M.m13 * M.m30)
            - M.m03 * (M.m10 * M.m32 - M.m12 * M.m30),
            // m31
            + M.m00 * (M.m12 * M.m23 - M.m13 * M.m22)
            - M.m02 * (M.m10 * M.m23 - M.m13 * M.m20)
            + M.m03 * (M.m10 * M.m22 - M.m12 * M.m20),

            // m02
            + M.m10 * (M.m21 * M.m33 - M.m23 * M.m31)
            - M.m11 * (M.m20 * M.m33 - M.m23 * M.m30)
            + M.m13 * (M.m20 * M.m31 - M.m21 * M.m30),
            // m12
            - M.m00 * (M.m21 * M.m33 - M.m23 * M.m31)
            + M.m01 * (M.m20 * M.m33 - M.m23 * M.m30)
            - M.m03 * (M.m20 * M.m31 - M.m21 * M.m30),
            // m22
            + M.m00 * (M.m11 * M.m33 - M.m13 * M.m31)
            - M.m01 * (M.m10 * M.m33 - M.m13 * M.m30)
            + M.m03 * (M.m10 * M.m31 - M.m11 * M.m30),
            // m32
            - M.m00 * (M.m11 * M.m23 - M.m13 * M.m21)
            + M.m01 * (M.m10 * M.m23 - M.m13 * M.m20)
            - M.m03 * (M.m10 * M.m21 - M.m11 * M.m20),

            // m03
            - M.m10 * (M.m21 * M.m32 - M.m22 * M.m31)
            + M.m11 * (M.m20 * M.m32 - M.m22 * M.m30)
            - M.m12 * (M.m20 * M.m31 - M.m21 * M.m30),
            // m13
            + M.m00 * (M.m21 * M.m32 - M.m22 * M.m31)
            - M.m01 * (M.m20 * M.m32 - M.m22 * M.m30)
            + M.m02 * (M.m20 * M.m31 - M.m21 * M.m30),
            // m23
            - M.m00 * (M.m11 * M.m32 - M.m12 * M.m31)
            + M.m01 * (M.m10 * M.m32 - M.m12 * M.m30)
            - M.m02 * (M.m10 * M.m31 - M.m11 * M.m30),
            // m33
            + M.m00 * (M.m11 * M.m22 - M.m12 * M.m21)
            - M.m01 * (M.m10 * M.m22 - M.m12 * M.m20)
            + M.m02 * (M.m10 * M.m21 - M.m11 * M.m20)
        };
    }

    inline bool
    mat4_invertible(const mat4 &M)
    {
        return mat4_det(M) != 0.0f;
    }

    inline mat4
    mat4_inverse(const mat4 &M)
    {
        f32 d = mat4_det(M);
        if (d == 0)
            return mat4{};

        return (1.0f / d) * mat4_adj(M);
    }

    inline mat4
    mat4_translation(f32 tx, f32 ty, f32 tz)
    {
        return mat4{
            1,  0,  0, 0,
            0,  1,  0, 0,
            0,  0,  1, 0,
            tx, ty, tz, 1
        };
    }

    inline mat4
    mat4_translation(const vec3 &translation)
    {
        return mat4_translation(translation.x, translation.y, translation.z);
    }

    inline mat4
    mat4_rotation_x(f32 pitch)
    {
        f32 c = cos(pitch);
        f32 s = sin(pitch);

        return mat4{
            1,  0, 0, 0,
            0,  c, s, 0,
            0, -s, c, 0,
            0,  0, 0, 1
        };
    }

    inline mat4
    mat4_rotation_y(f32 yaw)
    {
        f32 c = cos(yaw);
        f32 s = sin(yaw);

        return mat4{
            c, 0, -s, 0,
            0, 1,  0, 0,
            s, 0,  c, 0,
            0, 0,  0, 1
        };
    }

    inline mat4
    mat4_rotation_z(f32 roll)
    {
        f32 c = cos(roll);
        f32 s = sin(roll);

        return mat4{
             c, s, 0, 0,
            -s, c, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1
        };
    }

    inline mat4
    mat4_scaling(f32 sx, f32 sy, f32 sz)
    {
        return mat4{
            sx,  0,  0, 0,
             0, sy,  0, 0,
             0,  0, sz, 0,
             0,  0,  0, 1
        };
    }

    inline mat4
    mat4_scaling(const vec3 &scaling)
    {
        return mat4_scaling(scaling.x, scaling.y, scaling.z);
    }

    inline mat4
    mat4_shearing_xy(f32 s)
    {
        return mat4{
            1, 0, 0, 0,
            s, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    inline mat4
    mat4_shearing_xz(f32 s)
    {
        return mat4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            s, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    inline mat4
    mat4_shearing_yx(f32 s)
    {
        return mat4{
            1, s, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    inline mat4
    mat4_shearing_yz(f32 s)
    {
        return mat4{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, s, 1, 0,
            0, 0, 0, 1
        };
    }

    inline mat4
    mat4_shearing_zx(f32 s)
    {
        return mat4{
            1, 0, s, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    inline mat4
    mat4_shearing_zy(f32 s)
    {
        return mat4{
            1, 0, 0, 0,
            0, 1, s, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    // TODO[Waleed]: add unittests
    inline mat4
    mat4_look_at(vec3 eye, vec3 target, vec3 up)
    {
        vec3 v = normalize(eye - target);
        vec3 r = normalize(cross(up, v));
        vec3 u = cross(v, r);

        // TODO: documentation
        // TODO: translation?!
        return mat4{
               r.x,    u.x,    v.x, 0.0f,
               r.y,    u.y,    v.y, 0.0f,
               r.z,    u.z,    v.z, 0.0f,
            -eye.x, -eye.y, -eye.z, 1.0f
        };
    }

    // TODO[Waleed]: add unittests
    inline mat4
    mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar)
    {
        mat4 M{};

        M.m00 = 2.0f / (right - left);
        M.m30 = -(right + left) / (right - left);

        M.m11 = 2.0f / (top - bottom);
        M.m31 = -(top + bottom) / (top - bottom);

        // to map z to [-1, 1] use
        // M.m22 = -2.0f / (zfar - znear)
        // M.m32 = -(zfar + znear) / (zfar - znear)
        M.m22 = -1.0f / (zfar - znear);
        M.m32 = -znear / (zfar - znear);

        M.m33 = 1.0f;

        return M;
    }

    // TODO[Waleed]: add unittests
    inline mat4
    mat4_prespective(f32 fovy, f32 aspect, f32 znear, f32 zfar)
    {
        mat4 M{};

        f32 h = tan(fovy / 2.0f);
        f32 w = aspect * h;

        M.m00 = 1.0f / w;
        M.m11 = 1.0f / h;

        // to map z to [-1, 1] use
        // M.m22 = -(zfar + znear) / (zfar - znear)
        // M.m32 = -(2.0f * zfar * znear) / (zfar - znear)
        M.m22 = -zfar / (zfar - znear);
        M.m23 = -1.0f;
        M.m32 = -(zfar * znear) / (zfar - znear);

        return M;
    }

    // =================================================================================================
    // == INTERSECTIONS ================================================================================
    // =================================================================================================
}