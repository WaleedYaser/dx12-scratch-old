#include <kuro_math.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// =================================================================================================
// == TYPEDEFS =====================================================================================
// =================================================================================================
TEST_CASE("[kuro_math]: typedefs")
{
    SUBCASE("size")
    {
        CHECK(sizeof(kuro::i8)  == 1);
        CHECK(sizeof(kuro::i16) == 2);
        CHECK(sizeof(kuro::i32) == 4);
        CHECK(sizeof(kuro::i64) == 8);
        CHECK(sizeof(kuro::u8)  == 1);
        CHECK(sizeof(kuro::u16) == 2);
        CHECK(sizeof(kuro::u32) == 4);
        CHECK(sizeof(kuro::u64) == 8);
        CHECK(sizeof(kuro::f32) == 4);
        CHECK(sizeof(kuro::f64) == 8);
    }

    SUBCASE("type")
    {
        CHECK(typeid(kuro::i8)  == typeid(signed char));
        CHECK(typeid(kuro::i16) == typeid(short));
        CHECK(typeid(kuro::i32) == typeid(int));
        CHECK(typeid(kuro::i64) == typeid(long long));
        CHECK(typeid(kuro::u8)  == typeid(unsigned char));
        CHECK(typeid(kuro::u16) == typeid(unsigned short));
        CHECK(typeid(kuro::u32) == typeid(unsigned int));
        CHECK(typeid(kuro::u64) == typeid(unsigned long long));
        CHECK(typeid(kuro::f32) == typeid(float));
        CHECK(typeid(kuro::f64) == typeid(double));
    }
}

// =================================================================================================
// == LIMITS =======================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: limits")
{
    SUBCASE("min")
    {
        CHECK(kuro::I8_MIN  == SCHAR_MIN);
        CHECK(kuro::I16_MIN == SHRT_MIN);
        CHECK(kuro::I32_MIN == INT_MIN);
        CHECK(kuro::I64_MIN == LLONG_MIN);
        CHECK(kuro::U8_MIN  == 0u);
        CHECK(kuro::U16_MIN == 0u);
        CHECK(kuro::U32_MIN == 0u);
        CHECK(kuro::U64_MIN == 0ull);
        CHECK(kuro::F32_MIN == FLT_MIN);
        CHECK(kuro::F64_MIN == DBL_MIN);
    }

    SUBCASE("max")
    {
        CHECK(kuro::I8_MAX  == SCHAR_MAX);
        CHECK(kuro::I16_MAX == SHRT_MAX);
        CHECK(kuro::I32_MAX == INT_MAX);
        CHECK(kuro::I64_MAX == LLONG_MAX);
        CHECK(kuro::U8_MAX  == UCHAR_MAX);
        CHECK(kuro::U16_MAX == USHRT_MAX);
        CHECK(kuro::U32_MAX == UINT_MAX);
        CHECK(kuro::U64_MAX == ULLONG_MAX);
        CHECK(kuro::F32_MAX == FLT_MAX);
        CHECK(kuro::F64_MAX == DBL_MAX);
    }
}

// =================================================================================================
// == CONSTS =======================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: consts")
{
    CHECK(doctest::Approx(kuro::PI) == 3.14159265358979323846);
    CHECK(doctest::Approx(kuro::PI_OVER_2) == (3.14159265358979323846 / 2.0));
    CHECK(doctest::Approx(kuro::TAU) == (3.14159265358979323846 * 2.0));

    CHECK(doctest::Approx(kuro::TO_DEGREE * kuro::PI) == 180.0);
    CHECK(doctest::Approx(kuro::TO_RADIAN * 360.0) == kuro::TAU);
}

// =================================================================================================
// == MATH =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: math")
{
    SUBCASE("sqrt")
    {
        for (kuro::f64 i = 0.0; i < 100.0; i += 0.1)
        {
            CHECK(doctest::Approx(kuro::sqrt(i*i)) == i);
            CHECK(doctest::Approx(kuro::sqrt(kuro::f32(i*i))) == kuro::f32(i));
        }
    }

    SUBCASE("sin")
    {
        for (kuro::f64 theta = -2.0 * kuro::TAU; theta < 2 * kuro::TAU; theta += 0.1)
        {
            INFO("theta: ", theta);
            CHECK(doctest::Approx(kuro::sin(theta)) == ::sin(theta));
            CHECK(doctest::Approx(kuro::sin(kuro::f32(theta))) == ::sinf(kuro::f32(theta)));
        }
    }

    SUBCASE("cos")
    {
        for (kuro::f64 theta = -2.0 * kuro::TAU; theta < 2 * kuro::TAU; theta += 0.1)
        {
            INFO("theta: ", theta);
            CHECK(doctest::Approx(kuro::cos(theta)) == ::cos(theta));
            CHECK(doctest::Approx(kuro::cos(kuro::f32(theta))) == ::cosf(kuro::f32(theta)));
        }
    }

    SUBCASE("tan")
    {
        for (kuro::f64 theta = -2.0 * kuro::TAU; theta < 2 * kuro::TAU; theta += 0.1)
        {
            INFO("theta: ", theta);
            CHECK(doctest::Approx(kuro::tan(theta)) == ::tan(theta));
            CHECK(doctest::Approx(kuro::tan(kuro::f32(theta))) == ::tanf(kuro::f32(theta)));
        }
    }
}

// =================================================================================================
// == VEC2 =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: vec2")
{
    SUBCASE("init")
    {
        kuro::vec2 v = {};
        CHECK(v.x == 0.0f);
        CHECK(v.y == 0.0f);

        v = {1.0f, 2.0f};
        CHECK(v.x == 1.0f);
        CHECK(v.y == 2.0f);
    }

    SUBCASE("add")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::vec2 c = a + b;
        CHECK(c.x == a.x + b.x);
        CHECK(c.y == a.y + b.y);

        a += b;
        CHECK(a.x == c.x);
        CHECK(a.y == c.y);
    }

    SUBCASE("unary minus")
    {
        kuro::vec2 a = {1.0f, 2.0f};

        kuro::vec2 b = -a;
        CHECK(b.x == -a.x);
        CHECK(b.y == -a.y);
    }

    SUBCASE("subtract")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::vec2 c = a - b;
        CHECK(c.x == a.x - b.x);
        CHECK(c.y == a.y - b.y);

        a -= b;
        CHECK(a.x == c.x);
        CHECK(a.y == c.y);
    }

    SUBCASE("multiply")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::f32 f = 5.0f;

        kuro::vec2 b = a * f;
        CHECK(b.x == a.x * f);
        CHECK(b.y == a.y * f);

        b = f * a;
        CHECK(b.x == a.x * f);
        CHECK(b.y == a.y * f);

        a *= f;
        CHECK(b.x == b.x);
        CHECK(a.y == b.y);
    }

    SUBCASE("divide")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::f32 f = 5.0f;

        kuro::vec2 b = a / f;
        CHECK(b.x == a.x / f);
        CHECK(b.y == a.y / f);

        a /= f;
        CHECK(b.x == b.x);
        CHECK(a.y == b.y);
    }

    SUBCASE("dot")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        CHECK(kuro::dot(a, a) == 5.0f);
        CHECK(kuro::dot(b, b) == 265.0f);
        CHECK(kuro::dot(a, b) == 35.0f);

        kuro::vec2 a_normalized = kuro::normalize(a);
        kuro::vec2 b_normalized = kuro::normalize(b);

        CHECK(kuro::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(a_normalized, b_normalized) == doctest::Approx(0.962f).epsilon(0.001f));
    }

    SUBCASE("norm")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(a_norm == doctest::Approx(2.236f).epsilon(0.001f));
        CHECK(b_norm == doctest::Approx(16.279f).epsilon(0.001f));
    }

    SUBCASE("length")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::f32 a_length = kuro::length(a);
        kuro::f32 b_length = kuro::length(b);

        CHECK(a_length == doctest::Approx(2.236f).epsilon(0.001f));
        CHECK(b_length == doctest::Approx(16.279f).epsilon(0.001f));
    }

    SUBCASE("normalize")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::vec2 a_normalized = kuro::normalize(a);
        kuro::vec2 b_normalized = kuro::normalize(b);

        kuro::f32 a_length = kuro::length(a_normalized);
        kuro::f32 b_length = kuro::length(b_normalized);

        CHECK(a_length == doctest::Approx(1.0f));
        CHECK(b_length == doctest::Approx(1.0f));
    }

    SUBCASE("cross")
    {
        kuro::vec2 x = {1.0f, 0.0f};
        kuro::vec2 y = {0.0f, 1.0f};

        kuro::f32 xy = kuro::cross(x, y);
        CHECK(xy == doctest::Approx(1.0f));

        kuro::f32 yx = kuro::cross(y, x);
        CHECK(yx == doctest::Approx(-1.0f));
    }
}

// =================================================================================================
// == VEC3 =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: vec3")
{
    SUBCASE("init")
    {
        kuro::vec3 v = {};
        CHECK(v.x == 0);
        CHECK(v.y == 0);
        CHECK(v.z == 0);

        v = {1.0f, 2.0f, 3.0f};
        CHECK(v.x == 1.0f);
        CHECK(v.y == 2.0f);
        CHECK(v.z == 3.0f);
    }

    SUBCASE("add")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::vec3 c = a + b;
        CHECK(c.x == a.x + b.x);
        CHECK(c.y == a.y + b.y);
        CHECK(c.z == a.z + b.z);

        a += b;
        CHECK(a.x == c.x);
        CHECK(a.y == c.y);
        CHECK(a.z == c.z);
    }

    SUBCASE("unary minus")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = -a;
        CHECK(b.x == -a.x);
        CHECK(b.y == -a.y);
        CHECK(b.z == -a.z);
    }

    SUBCASE("subtract")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::vec3 c = a - b;
        CHECK(c.x == a.x - b.x);
        CHECK(c.y == a.y - b.y);
        CHECK(c.z == a.z - b.z);

        a -= b;
        CHECK(a.x == c.x);
        CHECK(a.y == c.y);
        CHECK(a.z == c.z);
    }

    SUBCASE("multiply")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::f32 f = 5.0f;

        kuro::vec3 b = a * f;
        CHECK(b.x == a.x * f);
        CHECK(b.y == a.y * f);
        CHECK(b.z == a.z * f);

        b = f * a;
        CHECK(b.x == a.x * f);
        CHECK(b.y == a.y * f);
        CHECK(b.z == a.z * f);

        a *= f;
        CHECK(a.x == b.x);
        CHECK(a.y == b.y);
        CHECK(a.z == b.z);
    }

    SUBCASE("divide")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::f32 f = 5.0f;

        kuro::vec3 b = a / f;
        CHECK(b.x == a.x / f);
        CHECK(b.y == a.y / f);
        CHECK(b.z == a.z / f);

        a /= f;
        CHECK(a.x == b.x);
        CHECK(a.y == b.y);
        CHECK(a.z == a.z);
    }

    SUBCASE("dot")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        CHECK(kuro::dot(a, a) == 14.0f);
        CHECK(kuro::dot(b, b) == 434.0f);
        CHECK(kuro::dot(a, b) == 74.0f);

        kuro::vec3 a_normalized = kuro::normalize(a);
        kuro::vec3 b_normalized = kuro::normalize(b);

        CHECK(kuro::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(a_normalized, b_normalized) == doctest::Approx(0.949f).epsilon(0.001f));
    }

    SUBCASE("norm")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(a_norm == doctest::Approx(3.742f).epsilon(0.001f));
        CHECK(b_norm == doctest::Approx(20.833f).epsilon(0.001f));
    }

    SUBCASE("length")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::f32 a_length = kuro::length(a);
        kuro::f32 b_length = kuro::length(b);

        CHECK(a_length == doctest::Approx(3.742f).epsilon(0.001f));
        CHECK(b_length == doctest::Approx(20.833f).epsilon(0.001f));
    }

    SUBCASE("normalize")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::vec3 a_normalized = kuro::normalize(a);
        kuro::vec3 b_normalized = kuro::normalize(b);

        kuro::f32 a_length = kuro::length(a_normalized);
        kuro::f32 b_length = kuro::length(b_normalized);

        CHECK(a_length == doctest::Approx(1.0f));
        CHECK(b_length == doctest::Approx(1.0f));
    }

    SUBCASE("cross")
    {
        kuro::vec3 x = {1.0f, 0.0f, 0.0f};
        kuro::vec3 y = {0.0f, 1.0f, 0.0f};
        kuro::vec3 z = {0.0f, 0.0f, 1.0f};

        kuro::vec3 xy = kuro::cross(x, y);
        CHECK(xy.x == doctest::Approx(z.x));
        CHECK(xy.y == doctest::Approx(z.y));
        CHECK(xy.z == doctest::Approx(z.z));

        kuro::vec3 yx = kuro::cross(y, x);
        CHECK(yx.x == doctest::Approx(-z.x));
        CHECK(yx.y == doctest::Approx(-z.y));
        CHECK(yx.z == doctest::Approx(-z.z));

        kuro::vec3 yz = kuro::cross(y, z);
        CHECK(yz.x == doctest::Approx(x.x));
        CHECK(yz.y == doctest::Approx(x.y));
        CHECK(yz.z == doctest::Approx(x.z));

        kuro::vec3 zy = kuro::cross(z, y);
        CHECK(zy.x == doctest::Approx(-x.x));
        CHECK(zy.y == doctest::Approx(-x.y));
        CHECK(zy.z == doctest::Approx(-x.z));

        kuro::vec3 zx = kuro::cross(z, x);
        CHECK(zx.x == doctest::Approx(y.x));
        CHECK(zx.y == doctest::Approx(y.y));
        CHECK(zx.z == doctest::Approx(y.z));

        kuro::vec3 xz = kuro::cross(x, z);
        CHECK(xz.x == doctest::Approx(-y.x));
        CHECK(xz.y == doctest::Approx(-y.y));
        CHECK(xz.z == doctest::Approx(-y.z));
    }
}

// =================================================================================================
// == VEC4 =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: vec4")
{
    SUBCASE("init")
    {
        kuro::vec4 v = {};
        CHECK(v.x == 0);
        CHECK(v.y == 0);
        CHECK(v.z == 0);
        CHECK(v.w == 0);

        v = {1.0f, 2.0f, 3.0f, 4.0f};
        CHECK(v.x == 1.0f);
        CHECK(v.y == 2.0f);
        CHECK(v.z == 3.0f);
        CHECK(v.w == 4.0f);
    }

    SUBCASE("add")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::vec4 c = a + b;
        CHECK(c.x == a.x + b.x);
        CHECK(c.y == a.y + b.y);
        CHECK(c.z == a.z + b.z);
        CHECK(c.w == a.w + b.w);

        a += b;
        CHECK(a.x == c.x);
        CHECK(a.y == c.y);
        CHECK(a.z == c.z);
        CHECK(a.w == c.w);
    }

    SUBCASE("unary minus")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};

        kuro::vec4 b = -a;
        CHECK(b.x == -a.x);
        CHECK(b.y == -a.y);
        CHECK(b.z == -a.z);
        CHECK(b.w == -a.w);
    }

    SUBCASE("subtract")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::vec4 c = a - b;
        CHECK(c.x == a.x - b.x);
        CHECK(c.y == a.y - b.y);
        CHECK(c.z == a.z - b.z);
        CHECK(c.w == a.w - b.w);

        a -= b;
        CHECK(a.x == c.x);
        CHECK(a.y == c.y);
        CHECK(a.z == c.z);
        CHECK(a.w == c.w);
    }

    SUBCASE("multiply")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::f32 f = 5.0f;

        kuro::vec4 b = a * f;
        CHECK(b.x == a.x * f);
        CHECK(b.y == a.y * f);
        CHECK(b.z == a.z * f);
        CHECK(b.w == a.w * f);

        b = f * a;
        CHECK(b.x == a.x * f);
        CHECK(b.y == a.y * f);
        CHECK(b.z == a.z * f);
        CHECK(b.w == a.w * f);

        a *= f;
        CHECK(a.x == b.x);
        CHECK(a.y == b.y);
        CHECK(a.z == b.z);
        CHECK(a.w == b.w);
    }

    SUBCASE("divide")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::f32 f = 5.0f;

        kuro::vec4 b = a / f;
        CHECK(b.x == a.x / f);
        CHECK(b.y == a.y / f);
        CHECK(b.z == a.z / f);
        CHECK(b.w == a.w / f);

        a /= f;
        CHECK(a.x == b.x);
        CHECK(a.y == b.y);
        CHECK(a.z == b.z);
        CHECK(a.w == b.w);
    }

    SUBCASE("dot")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        CHECK(kuro::dot(a, a) == 30.0f);
        CHECK(kuro::dot(b, b) == 630.0f);
        CHECK(kuro::dot(a, b) == 130.0f);

        kuro::vec4 a_normalized = kuro::normalize(a);
        kuro::vec4 b_normalized = kuro::normalize(b);

        CHECK(kuro::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(a_normalized, b_normalized) == doctest::Approx(0.946f).epsilon(0.001f));
    }

    SUBCASE("norm")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(a_norm == doctest::Approx(5.477f).epsilon(0.001f));
        CHECK(b_norm == doctest::Approx(25.1f).epsilon(0.001f));
    }

    SUBCASE("length")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::f32 a_length = kuro::length(a);
        kuro::f32 b_length = kuro::length(b);

        CHECK(a_length == doctest::Approx(5.477f).epsilon(0.001f));
        CHECK(b_length == doctest::Approx(25.1f).epsilon(0.001f));
    }

    SUBCASE("normalize")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::vec4 a_normalized = kuro::normalize(a);
        kuro::vec4 b_normalized = kuro::normalize(b);

        kuro::f32 a_length = kuro::length(a_normalized);
        kuro::f32 b_length = kuro::length(b_normalized);

        CHECK(a_length == doctest::Approx(1.0f));
        CHECK(b_length == doctest::Approx(1.0f));
    }
}

// =================================================================================================
// == MAT2 =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: mat2")
{
    SUBCASE("init")
    {
        kuro::mat2 M = {};

        CHECK(M.m00 == 0.0f); CHECK(M.m01 == 0.0f);
        CHECK(M.m10 == 0.0f); CHECK(M.m11 == 0.0f);

        M = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        CHECK(M.m00 == 1.0f); CHECK(M.m01 == 2.0f);
        CHECK(M.m10 == 3.0f); CHECK(M.m11 == 4.0f);
    }

    SUBCASE("add")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = {
            11.0f, 12.0f,
            13.0f, 14.0f
        };

        kuro::mat2 C = A + B;
        CHECK(C.m00 == A.m00 + B.m00); CHECK(C.m01 == A.m01 + B.m01);
        CHECK(C.m10 == A.m10 + B.m10); CHECK(C.m11 == A.m11 + B.m11);

        A += B;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11);
    }

    SUBCASE("unary minus")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = -A;
        CHECK(B.m00 == -A.m00); CHECK(B.m01 == -A.m01);
        CHECK(B.m10 == -A.m10); CHECK(B.m11 == -A.m11);
    }

    SUBCASE("subtract")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = {
            11.0f, 12.0f,
            13.0f, 14.0f
        };

        kuro::mat2 C = A - B;
        CHECK(C.m00 == A.m00 - B.m00); CHECK(C.m01 == A.m01 - B.m01);
        CHECK(C.m10 == A.m10 - B.m10); CHECK(C.m11 == A.m11 - B.m11);

        A -= B;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11);
    }

    SUBCASE("multiply scalar")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat2 B = A * f;
        CHECK(B.m00 == f * A.m00); CHECK(B.m01 == f * A.m01);
        CHECK(B.m10 == f * A.m10); CHECK(B.m11 == f * A.m11);

        B = f * A;
        CHECK(B.m00 == f * A.m00); CHECK(B.m01 == f * A.m01);
        CHECK(B.m10 == f * A.m10); CHECK(B.m11 == f * A.m11);

        A *= f;
        CHECK(A.m00 == B.m00); CHECK(A.m01 == B.m01);
        CHECK(A.m10 == B.m10); CHECK(A.m11 == B.m11);
    }

    SUBCASE("multiply vector")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::mat2 M = {
            11.0f, 12.0f,
            13.0f, 14.0f
        };

        kuro::vec2 b = a * M;
        CHECK(b.x == 37.0f);
        CHECK(b.y == 40.0f);
    }

    SUBCASE("multiply matrix")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = {
            11.0f, 12.0f,
            13.0f, 14.0f
        };

        kuro::mat2 C = A * B;
        CHECK(C.m00 == 37.0f); CHECK(C.m01 == 40.0f);
        CHECK(C.m10 == 85.0f); CHECK(C.m11 == 92.0f);
    }

    SUBCASE("divide")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat2 C = A / f;
        CHECK(C.m00 == A.m00 / f); CHECK(C.m01 == A.m01 / f);
        CHECK(C.m10 == A.m10 / f); CHECK(C.m11 == A.m11 / f);

        A /= f;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11);
    }

    SUBCASE("identity")
    {
        kuro::mat2 I = kuro::mat2_identity();
        CHECK(I.m00 == 1.0f); CHECK(I.m01 == 0.0f);
        CHECK(I.m10 == 0.0f); CHECK(I.m11 == 1.0f);

        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = a * I;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);

        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = I * A;
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m01);
        CHECK(B.m10 == A.m10); CHECK(B.m11 == A.m11);

        B = A * I;
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m01);
        CHECK(B.m10 == A.m10); CHECK(B.m11 == A.m11);
    }

    SUBCASE("transpose")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = kuro::mat2_transpose(A);
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m10);
        CHECK(B.m10 == A.m01); CHECK(B.m11 == A.m11);
    }

    SUBCASE("trace")
    {
        kuro::mat2 M = {
            0.0f, 2.0f,
            3.0f, 4.0f
        };

        CHECK(kuro::mat2_trace(M) == 4.0f);
    }

    SUBCASE("determinant")
    {
        kuro::mat2 I = kuro::mat2_identity();
        CHECK(kuro::mat2_det(I) == 1.0f);

        kuro::mat2 M = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        CHECK(kuro::mat2_det(M) == -2.0f);
    }

    SUBCASE("invertible")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        CHECK(kuro::mat2_invertible(A) == true);

        kuro::mat2 B = {
            1.0f, 1.0f,
            1.0f, 1.0f
        };

        CHECK(kuro::mat2_invertible(B) == false);
    }

    SUBCASE("inverse")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = kuro::mat2_inverse(A);

        kuro::mat2 C = A * B;
        CHECK(C.m00 == 1.0f); CHECK(C.m01 == 0.0f);
        CHECK(C.m10 == 0.0f); CHECK(C.m11 == 1.0f);

        C = B * A;
        CHECK(C.m00 == 1.0f); CHECK(C.m01 == 0.0f);
        CHECK(C.m10 == 0.0f); CHECK(C.m11 == 1.0f);

        kuro::vec2 a = {11.0f, 12.0f};
        kuro::vec2 b = a * A * B;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
    }

    SUBCASE("rotation")
    {
        kuro::mat2 R = kuro::mat2_rotation((kuro::f32)kuro::PI_OVER_2);
        kuro::vec2 a = {1.0f, 1.0f};

        kuro::vec2 b = a * R;
        CHECK(b.x == doctest::Approx(-a.x));
        CHECK(b.y == doctest::Approx(a.y));

        kuro::vec2 c = b * kuro::mat2_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
    }

    SUBCASE("scaling")
    {
        kuro::vec2 s = {10.0f, 20.0f};

        kuro::mat2 S = kuro::mat2_scaling(s.x, s.y);
        kuro::vec2 a = {1.0f, 2.0f};

        kuro::vec2 b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);

        S = kuro::mat2_scaling(s);

        b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);

        kuro::vec2 c = b * kuro::mat2_inverse(S);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
    }

    SUBCASE("shearing x")
    {
        kuro::f32 s = 10.0f;

        kuro::mat2 H = kuro::mat2_shearing_x(s);
        kuro::vec2 a = {1.0f, 2.0f};

        kuro::vec2 b = a * H;
        CHECK(b.x == a.x + a.y*s);
        CHECK(b.y == a.y);

        kuro::vec2 c = b * kuro::mat2_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
    }

    SUBCASE("shearing y")
    {
        kuro::f32 s = 10.0f;

        kuro::mat2 H = kuro::mat2_shearing_y(s);
        kuro::vec2 a = {1.0f, 2.0f};

        kuro::vec2 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y + a.x*s);

        kuro::vec2 c = b * kuro::mat2_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
    }
}

// =================================================================================================
// == MAT3 =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: mat3")
{
    SUBCASE("init")
    {
        kuro::mat3 M = {};

        CHECK(M.m00 == 0.0f); CHECK(M.m01 == 0.0f); CHECK(M.m02 == 0.0f);
        CHECK(M.m10 == 0.0f); CHECK(M.m11 == 0.0f); CHECK(M.m12 == 0.0f);
        CHECK(M.m20 == 0.0f); CHECK(M.m21 == 0.0f); CHECK(M.m22 == 0.0f);

        M = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        CHECK(M.m00 == 1.0f); CHECK(M.m01 == 2.0f); CHECK(M.m02 == 3.0f);
        CHECK(M.m10 == 4.0f); CHECK(M.m11 == 5.0f); CHECK(M.m12 == 6.0f);
        CHECK(M.m20 == 7.0f); CHECK(M.m21 == 8.0f); CHECK(M.m22 == 9.0f);
    }

    SUBCASE("add")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::mat3 B = {
            11.0f, 12.0f, 13.0f,
            14.0f, 15.0f, 16.0f,
            17.0f, 18.0f, 19.0f
        };

        kuro::mat3 C = A + B;
        CHECK(C.m00 == A.m00 + B.m00); CHECK(C.m01 == A.m01 + B.m01); CHECK(C.m02 == A.m02 + B.m02);
        CHECK(C.m10 == A.m10 + B.m10); CHECK(C.m11 == A.m11 + B.m11); CHECK(C.m12 == A.m12 + B.m12);
        CHECK(C.m20 == A.m20 + B.m20); CHECK(C.m21 == A.m21 + B.m21); CHECK(C.m22 == A.m22 + B.m22);

        A += B;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01); CHECK(A.m02 == C.m02);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11); CHECK(A.m12 == C.m12);
        CHECK(A.m20 == C.m20); CHECK(A.m21 == C.m21); CHECK(A.m22 == C.m22);
    }

    SUBCASE("unary minus")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::mat3 B = -A;
        CHECK(B.m00 == -A.m00); CHECK(B.m01 == -A.m01); CHECK(B.m02 == -A.m02);
        CHECK(B.m10 == -A.m10); CHECK(B.m11 == -A.m11); CHECK(B.m12 == -A.m12);
        CHECK(B.m20 == -A.m20); CHECK(B.m21 == -A.m21); CHECK(B.m22 == -A.m22);
    }

    SUBCASE("subtract")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::mat3 B = {
            11.0f, 12.0f, 13.0f,
            14.0f, 15.0f, 16.0f,
            17.0f, 18.0f, 19.0f
        };

        kuro::mat3 C = A - B;
        CHECK(C.m00 == A.m00 - B.m00); CHECK(C.m01 == A.m01 - B.m01); CHECK(C.m02 == A.m02 - B.m02);
        CHECK(C.m10 == A.m10 - B.m10); CHECK(C.m11 == A.m11 - B.m11); CHECK(C.m12 == A.m12 - B.m12);
        CHECK(C.m20 == A.m20 - B.m20); CHECK(C.m21 == A.m21 - B.m21); CHECK(C.m22 == A.m22 - B.m22);

        A -= B;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01); CHECK(A.m02 == C.m02);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11); CHECK(A.m12 == C.m12);
        CHECK(A.m20 == C.m20); CHECK(A.m21 == C.m21); CHECK(A.m22 == C.m22);
    }

    SUBCASE("multiply scalar")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat3 B = A * f;
        CHECK(B.m00 == f * A.m00); CHECK(B.m01 == f * A.m01); CHECK(B.m02 == f * A.m02);
        CHECK(B.m10 == f * A.m10); CHECK(B.m11 == f * A.m11); CHECK(B.m12 == f * A.m12);
        CHECK(B.m20 == f * A.m20); CHECK(B.m21 == f * A.m21); CHECK(B.m22 == f * A.m22);

        B = f * A;
        CHECK(B.m00 == f * A.m00); CHECK(B.m01 == f * A.m01); CHECK(B.m02 == f * A.m02);
        CHECK(B.m10 == f * A.m10); CHECK(B.m11 == f * A.m11); CHECK(B.m12 == f * A.m12);
        CHECK(B.m20 == f * A.m20); CHECK(B.m21 == f * A.m21); CHECK(B.m22 == f * A.m22);

        A *= f;
        CHECK(A.m00 == B.m00); CHECK(A.m01 == B.m01); CHECK(A.m02 == B.m02);
        CHECK(A.m10 == B.m10); CHECK(A.m11 == B.m11); CHECK(A.m12 == B.m12);
        CHECK(A.m20 == B.m20); CHECK(A.m21 == B.m21); CHECK(A.m22 == B.m22);
    }

    SUBCASE("multiply vector")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::mat3 M = {
            11.0f, 12.0f, 13.0f,
            14.0f, 15.0f, 16.0f,
            17.0f, 18.0f, 19.0f
        };

        kuro::vec3 b = a * M;
        CHECK(b.x == 90.0f);
        CHECK(b.y == 96.0f);
        CHECK(b.z == 102.0f);
    }

    SUBCASE("multiply matrix")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::mat3 B = {
            11.0f, 12.0f, 13.0f,
            14.0f, 15.0f, 16.0f,
            17.0f, 18.0f, 19.0f
        };

        kuro::mat3 C = A * B;
        CHECK(C.m00 ==  90.0f); CHECK(C.m01 ==  96.0f); CHECK(C.m02 == 102.0f);
        CHECK(C.m10 == 216.0f); CHECK(C.m11 == 231.0f); CHECK(C.m12 == 246.0f);
        CHECK(C.m20 == 342.0f); CHECK(C.m21 == 366.0f); CHECK(C.m22 == 390.0f);
    }

    SUBCASE("divide")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat3 B = A / f;
        CHECK(B.m00 == A.m00 / f); CHECK(B.m01 == A.m01 / f); CHECK(B.m02 == A.m02 / f);
        CHECK(B.m10 == A.m10 / f); CHECK(B.m11 == A.m11 / f); CHECK(B.m12 == A.m12 / f);
        CHECK(B.m20 == A.m20 / f); CHECK(B.m21 == A.m21 / f); CHECK(B.m22 == A.m22 / f);

        A /= f;
        CHECK(A.m00 == B.m00); CHECK(A.m01 == B.m01); CHECK(A.m02 == B.m02);
        CHECK(A.m10 == B.m10); CHECK(A.m11 == B.m11); CHECK(A.m12 == B.m12);
        CHECK(A.m20 == B.m20); CHECK(A.m21 == B.m21); CHECK(A.m22 == B.m22);
    }

    SUBCASE("identity")
    {
        kuro::mat3 I = kuro::mat3_identity();
        CHECK(I.m00 == 1.0f); CHECK(I.m01 == 0.0f); CHECK(I.m02 == 0.0f);
        CHECK(I.m10 == 0.0f); CHECK(I.m11 == 1.0f); CHECK(I.m12 == 0.0f);
        CHECK(I.m20 == 0.0f); CHECK(I.m21 == 0.0f); CHECK(I.m22 == 1.0f);

        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = a * I;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);

        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::mat3 B = I * A;
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m01); CHECK(B.m02 == A.m02);
        CHECK(B.m10 == A.m10); CHECK(B.m11 == A.m11); CHECK(B.m12 == A.m12);
        CHECK(B.m20 == A.m20); CHECK(B.m21 == A.m21); CHECK(B.m22 == A.m22);

        B = A * I;
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m01); CHECK(B.m02 == A.m02);
        CHECK(B.m10 == A.m10); CHECK(B.m11 == A.m11); CHECK(B.m12 == A.m12);
        CHECK(B.m20 == A.m20); CHECK(B.m21 == A.m21); CHECK(B.m22 == A.m22);
    }

    SUBCASE("transpose")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        kuro::mat3 B = kuro::mat3_transpose(A);
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m10); CHECK(B.m02 == A.m20);
        CHECK(B.m10 == A.m01); CHECK(B.m11 == A.m11); CHECK(B.m12 == A.m21);
        CHECK(B.m20 == A.m02); CHECK(B.m21 == A.m12); CHECK(B.m22 == A.m22);
    }

    SUBCASE("trace")
    {
        kuro::mat3 M = {
            0.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        CHECK(kuro::mat3_trace(M) == 14.0f);
    }

    SUBCASE("determinant")
    {
        kuro::mat3 I = kuro::mat3_identity();
        CHECK(kuro::mat3_det(I) == 1.0f);

        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        CHECK(kuro::mat3_det(A) == 0.0f);

        kuro::mat3 B = {
            3.0f, 2.0f, -1.0f,
            2.0f, 1.0f,  5.0f,
            0.0f, 5.0f,  2.0f
        };

        CHECK(kuro::mat3_det(B) == -87.0f);
    }

    SUBCASE("adjoint")
    {
        kuro::mat3 A = {
             3.0f, 2.0f, -1.0f,
             2.0f, 1.0f,  5.0f,
             0.0f, 5.0f,  2.0f
        };

        kuro::mat3 B = kuro::mat3_adj(A);
        CHECK(B.m00 == -23.0f); CHECK(B.m01 ==  -9.0f); CHECK(B.m02 ==  11.0f);
        CHECK(B.m10 ==  -4.0f); CHECK(B.m11 ==   6.0f); CHECK(B.m12 == -17.0f);
        CHECK(B.m20 ==  10.0f); CHECK(B.m21 == -15.0f); CHECK(B.m22 ==  -1.0f);
    }


    SUBCASE("invertible")
    {
        kuro::mat3 A = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
        };

        CHECK(kuro::mat3_invertible(A) == false);

        kuro::mat3 B = {
            3.0f, 2.0f, -1.0f,
            2.0f, 1.0f,  5.0f,
            0.0f, 5.0f,  2.0f
        };

        CHECK(kuro::mat3_invertible(B) == true);
    }

    SUBCASE("inverse")
    {
        kuro::mat3 A = {
            3.0f, 2.0f, -1.0f,
            2.0f, 1.0f,  5.0f,
            0.0f, 5.0f,  2.0f
        };

        kuro::mat3 B = kuro::mat3_inverse(A);

        kuro::mat3 C = A * B;
        CHECK(C.m00 == doctest::Approx(1.0f));
        CHECK(C.m01 == doctest::Approx(0.0f));
        CHECK(C.m02 == doctest::Approx(0.0f));

        CHECK(C.m10 == doctest::Approx(0.0f));
        CHECK(C.m11 == doctest::Approx(1.0f));
        CHECK(C.m12 == doctest::Approx(0.0f));

        CHECK(C.m20 == doctest::Approx(0.0f));
        CHECK(C.m21 == doctest::Approx(0.0f));
        CHECK(C.m22 == doctest::Approx(1.0f));

        C = B * A;
        CHECK(C.m00 == doctest::Approx(1.0f));
        CHECK(C.m01 == doctest::Approx(0.0f));
        CHECK(C.m02 == doctest::Approx(0.0f));

        CHECK(C.m10 == doctest::Approx(0.0f));
        CHECK(C.m11 == doctest::Approx(1.0f));
        CHECK(C.m12 == doctest::Approx(0.0f));

        CHECK(C.m20 == doctest::Approx(0.0f));
        CHECK(C.m21 == doctest::Approx(0.0f));
        CHECK(C.m22 == doctest::Approx(1.0f));

        kuro::vec3 a = {11.0f, 12.0f, 13.0f};
        kuro::vec3 b = a * A * B;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);
    }

    SUBCASE("translation 2d")
    {
        kuro::vec2 p = {10.0f, 20.0f};

        kuro::mat3 P = kuro::mat3_translation_2d(p.x, p.y);
        kuro::vec3 a = {1.0f, 1.0f, 1.0f};

        kuro::vec3 b = a * P;
        CHECK(b.x == a.x + p.x);
        CHECK(b.y == a.y + p.y);
        CHECK(b.z == 1.0f);

        P = kuro::mat3_translation_2d(p);

        b = a * P;
        CHECK(b.x == a.x + p.x);
        CHECK(b.y == a.y + p.y);
        CHECK(b.z == 1.0f);

        kuro::vec3 c = b * kuro::mat3_inverse(P);
        CHECK(c.x == a.x);
        CHECK(c.y == a.y);
        CHECK(c.z == 1.0f);
    }

    SUBCASE("rotation 2d")
    {
        kuro::mat3 R = kuro::mat3_rotation_2d((kuro::f32)kuro::PI_OVER_2);
        kuro::vec3 a = {1.0f, 1.0f, 1.0f};

        kuro::vec3 b = a * R;
        CHECK(b.x == doctest::Approx(-a.x));
        CHECK(b.y == doctest::Approx(a.y));
        CHECK(b.z == doctest::Approx(1.0f));

        kuro::vec3 c = b * kuro::mat3_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.y == doctest::Approx(1.0f));
    }

    SUBCASE("scaling 2d")
    {
        kuro::vec2 s = {10.0f, 20.0f};

        kuro::mat3 S = kuro::mat3_scaling_2d(s.x, s.y);
        kuro::vec3 a = {1.0f, 2.0f, 1.0f};

        kuro::vec3 b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);

        S = kuro::mat3_scaling_2d(s);

        b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);

        kuro::vec3 c = b * kuro::mat3_inverse(S);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(1.0f));
    }

    SUBCASE("rotation x")
    {
        kuro::mat3 R = kuro::mat3_rotation_x((kuro::f32)kuro::PI_OVER_2);
        kuro::vec3 a = {1.0f, 1.0f, 1.0f};

        kuro::vec3 b = a * R;
        CHECK(b.x == doctest::Approx(a.x));
        CHECK(b.y == doctest::Approx(-a.y));
        CHECK(b.z == doctest::Approx(a.z));

        kuro::vec3 c = b * kuro::mat3_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("rotation y")
    {
        kuro::mat3 R = kuro::mat3_rotation_y((kuro::f32)kuro::PI_OVER_2);
        kuro::vec3 a = {1.0f, 1.0f, 1.0f};

        kuro::vec3 b = a * R;
        CHECK(b.x == doctest::Approx(a.x));
        CHECK(b.y == doctest::Approx(a.y));
        CHECK(b.z == doctest::Approx(-a.z));

        kuro::vec3 c = b * kuro::mat3_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("rotation z")
    {
        kuro::mat3 R = kuro::mat3_rotation_z((kuro::f32)kuro::PI_OVER_2);
        kuro::vec3 a = {1.0f, 1.0f, 1.0f};

        kuro::vec3 b = a * R;
        CHECK(b.x == doctest::Approx(-a.x));
        CHECK(b.y == doctest::Approx(a.y));
        CHECK(b.z == doctest::Approx(a.z));

        kuro::vec3 c = b * kuro::mat3_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("scaling")
    {
        kuro::vec3 s = {10.0f, 20.0f, 30.0f};

        kuro::mat3 S = kuro::mat3_scaling(s.x, s.y, s.z);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);
        CHECK(b.z == a.z * s.z);

        S = kuro::mat3_scaling(s);

        b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);
        CHECK(b.z == a.z * s.z);

        kuro::vec3 c = b * kuro::mat3_inverse(S);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("shearing xy")
    {
        kuro::f32 s = 10.0f;

        kuro::mat3 H = kuro::mat3_shearing_xy(s);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * H;
        CHECK(b.x == a.x + a.y*s);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);

        kuro::vec3 c = b * kuro::mat3_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("shearing xz")
    {
        kuro::f32 s = 10.0f;

        kuro::mat3 H = kuro::mat3_shearing_xz(s);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * H;
        CHECK(b.x == a.x + a.z*s);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);

        kuro::vec3 c = b * kuro::mat3_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("shearing yx")
    {
        kuro::f32 s = 10.0f;

        kuro::mat3 H = kuro::mat3_shearing_yx(s);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y + a.x*s);
        CHECK(b.z == a.z);

        kuro::vec3 c = b * kuro::mat3_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("shearing yz")
    {
        kuro::f32 s = 10.0f;

        kuro::mat3 H = kuro::mat3_shearing_yz(s);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y + a.z*s);
        CHECK(b.z == a.z);

        kuro::vec3 c = b * kuro::mat3_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("shearing zx")
    {
        kuro::f32 s = 10.0f;

        kuro::mat3 H = kuro::mat3_shearing_zx(s);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z + a.x*s);

        kuro::vec3 c = b * kuro::mat3_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }

    SUBCASE("shearing zy")
    {
        kuro::f32 s = 10.0f;

        kuro::mat3 H = kuro::mat3_shearing_zy(s);
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};

        kuro::vec3 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z + a.y*s);

        kuro::vec3 c = b * kuro::mat3_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
    }
}

// =================================================================================================
// == MAT4 =========================================================================================
// =================================================================================================

TEST_CASE("[kuro_math]: mat4")
{
    SUBCASE("init")
    {
        kuro::mat4 M = {};

        CHECK(M.m00 == 0.0f); CHECK(M.m01 == 0.0f); CHECK(M.m02 == 0.0f); CHECK(M.m03 == 0.0f);
        CHECK(M.m10 == 0.0f); CHECK(M.m11 == 0.0f); CHECK(M.m12 == 0.0f); CHECK(M.m13 == 0.0f);
        CHECK(M.m20 == 0.0f); CHECK(M.m21 == 0.0f); CHECK(M.m22 == 0.0f); CHECK(M.m23 == 0.0f);
        CHECK(M.m30 == 0.0f); CHECK(M.m31 == 0.0f); CHECK(M.m32 == 0.0f); CHECK(M.m33 == 0.0f);

        M = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        CHECK(M.m00 ==  1.0f); CHECK(M.m01 ==  2.0f); CHECK(M.m02 ==  3.0f); CHECK(M.m03 ==  4.0f);
        CHECK(M.m10 ==  5.0f); CHECK(M.m11 ==  6.0f); CHECK(M.m12 ==  7.0f); CHECK(M.m13 ==  8.0f);
        CHECK(M.m20 ==  9.0f); CHECK(M.m21 == 10.0f); CHECK(M.m22 == 11.0f); CHECK(M.m23 == 12.0f);
        CHECK(M.m30 == 13.0f); CHECK(M.m31 == 14.0f); CHECK(M.m32 == 15.0f); CHECK(M.m33 == 16.0f);
    }

    SUBCASE("add")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::mat4 B = {
            21.0f, 22.0f, 23.0f, 24.0f,
            25.0f, 26.0f, 27.0f, 28.0f,
            29.0f, 30.0f, 31.0f, 32.0f,
            33.0f, 34.0f, 35.0f, 36.0f
        };

        kuro::mat4 C = A + B;
        CHECK(C.m00 == A.m00 + B.m00);
        CHECK(C.m01 == A.m01 + B.m01);
        CHECK(C.m02 == A.m02 + B.m02);
        CHECK(C.m03 == A.m03 + B.m03);

        CHECK(C.m10 == A.m10 + B.m10);
        CHECK(C.m11 == A.m11 + B.m11);
        CHECK(C.m12 == A.m12 + B.m12);
        CHECK(C.m13 == A.m13 + B.m13);

        CHECK(C.m20 == A.m20 + B.m20);
        CHECK(C.m21 == A.m21 + B.m21);
        CHECK(C.m22 == A.m22 + B.m22);
        CHECK(C.m23 == A.m23 + B.m23);

        CHECK(C.m30 == A.m30 + B.m30);
        CHECK(C.m31 == A.m31 + B.m31);
        CHECK(C.m32 == A.m32 + B.m32);
        CHECK(C.m33 == A.m33 + B.m33);

        A += B;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01); CHECK(A.m02 == C.m02); CHECK(A.m03 == C.m03);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11); CHECK(A.m12 == C.m12); CHECK(A.m13 == C.m13);
        CHECK(A.m20 == C.m20); CHECK(A.m21 == C.m21); CHECK(A.m22 == C.m22); CHECK(A.m23 == C.m23);
        CHECK(A.m30 == C.m30); CHECK(A.m31 == C.m31); CHECK(A.m32 == C.m32); CHECK(A.m33 == C.m33);
    }

    SUBCASE("unary minus")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::mat4 B = -A;
        CHECK(B.m00 == -A.m00); CHECK(B.m01 == -A.m01); CHECK(B.m02 == -A.m02); CHECK(B.m03 == -A.m03);
        CHECK(B.m10 == -A.m10); CHECK(B.m11 == -A.m11); CHECK(B.m12 == -A.m12); CHECK(B.m13 == -A.m13);
        CHECK(B.m20 == -A.m20); CHECK(B.m21 == -A.m21); CHECK(B.m22 == -A.m22); CHECK(B.m23 == -A.m23);
        CHECK(B.m30 == -A.m30); CHECK(B.m31 == -A.m31); CHECK(B.m32 == -A.m32); CHECK(B.m33 == -A.m33);
    }

    SUBCASE("subtract")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::mat4 B = {
            21.0f, 22.0f, 23.0f, 24.0f,
            25.0f, 26.0f, 27.0f, 28.0f,
            29.0f, 30.0f, 31.0f, 32.0f,
            33.0f, 34.0f, 35.0f, 36.0f
        };

        kuro::mat4 C = A - B;
        CHECK(C.m00 == A.m00 - B.m00);
        CHECK(C.m01 == A.m01 - B.m01);
        CHECK(C.m02 == A.m02 - B.m02);
        CHECK(C.m03 == A.m03 - B.m03);

        CHECK(C.m10 == A.m10 - B.m10);
        CHECK(C.m11 == A.m11 - B.m11);
        CHECK(C.m12 == A.m12 - B.m12);
        CHECK(C.m13 == A.m13 - B.m13);

        CHECK(C.m20 == A.m20 - B.m20);
        CHECK(C.m21 == A.m21 - B.m21);
        CHECK(C.m22 == A.m22 - B.m22);
        CHECK(C.m23 == A.m23 - B.m23);

        CHECK(C.m30 == A.m30 - B.m30);
        CHECK(C.m31 == A.m31 - B.m31);
        CHECK(C.m32 == A.m32 - B.m32);
        CHECK(C.m33 == A.m33 - B.m33);

        A -= B;
        CHECK(A.m00 == C.m00); CHECK(A.m01 == C.m01); CHECK(A.m02 == C.m02); CHECK(A.m03 == C.m03);
        CHECK(A.m10 == C.m10); CHECK(A.m11 == C.m11); CHECK(A.m12 == C.m12); CHECK(A.m13 == C.m13);
        CHECK(A.m20 == C.m20); CHECK(A.m21 == C.m21); CHECK(A.m22 == C.m22); CHECK(A.m23 == C.m23);
        CHECK(A.m30 == C.m30); CHECK(A.m31 == C.m31); CHECK(A.m32 == C.m32); CHECK(A.m33 == C.m33);
    }

    SUBCASE("multiply scalar")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat4 B = A * f;
        CHECK(B.m00 == f * A.m00);
        CHECK(B.m01 == f * A.m01);
        CHECK(B.m02 == f * A.m02);
        CHECK(B.m03 == f * A.m03);

        CHECK(B.m10 == f * A.m10);
        CHECK(B.m11 == f * A.m11);
        CHECK(B.m12 == f * A.m12);
        CHECK(B.m13 == f * A.m13);

        CHECK(B.m20 == f * A.m20);
        CHECK(B.m21 == f * A.m21);
        CHECK(B.m22 == f * A.m22);
        CHECK(B.m23 == f * A.m23);

        CHECK(B.m30 == f * A.m30);
        CHECK(B.m31 == f * A.m31);
        CHECK(B.m32 == f * A.m32);
        CHECK(B.m33 == f * A.m33);

        B = f * A;
        CHECK(B.m00 == f * A.m00);
        CHECK(B.m01 == f * A.m01);
        CHECK(B.m02 == f * A.m02);
        CHECK(B.m03 == f * A.m03);

        CHECK(B.m10 == f * A.m10);
        CHECK(B.m11 == f * A.m11);
        CHECK(B.m12 == f * A.m12);
        CHECK(B.m13 == f * A.m13);

        CHECK(B.m20 == f * A.m20);
        CHECK(B.m21 == f * A.m21);
        CHECK(B.m22 == f * A.m22);
        CHECK(B.m23 == f * A.m23);

        CHECK(B.m30 == f * A.m30);
        CHECK(B.m31 == f * A.m31);
        CHECK(B.m32 == f * A.m32);
        CHECK(B.m33 == f * A.m33);

        A *= f;
        CHECK(A.m00 == B.m00); CHECK(A.m01 == B.m01); CHECK(A.m02 == B.m02); CHECK(A.m03 == B.m03);
        CHECK(A.m10 == B.m10); CHECK(A.m11 == B.m11); CHECK(A.m12 == B.m12); CHECK(A.m13 == B.m13);
        CHECK(A.m20 == B.m20); CHECK(A.m21 == B.m21); CHECK(A.m22 == B.m22); CHECK(A.m23 == B.m23);
        CHECK(A.m30 == B.m30); CHECK(A.m31 == B.m31); CHECK(A.m32 == B.m32); CHECK(A.m33 == B.m33);
    }

    SUBCASE("multiply vector")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::mat4 M = {
            11.0f, 12.0f, 13.0f, 14.0f,
            15.0f, 16.0f, 17.0f, 18.0f,
            19.0f, 20.0f, 21.0f, 22.0f,
            23.0f, 24.0f, 25.0f, 26.0f
        };

        kuro::vec4 b = a * M;
        CHECK(b.x == 190.0f);
        CHECK(b.y == 200.0f);
        CHECK(b.z == 210.0f);
        CHECK(b.w == 220.0f);
    }

    SUBCASE("multiply matrix")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::mat4 B = {
            21.0f, 22.0f, 23.0f, 24.0f,
            25.0f, 26.0f, 27.0f, 28.0f,
            29.0f, 30.0f, 31.0f, 32.0f,
            33.0f, 34.0f, 35.0f, 36.0f
        };

        kuro::mat4 C = A * B;
        CHECK(C.m00 ==  290.0f); CHECK(C.m01 ==  300.0f); CHECK(C.m02 ==  310.0f); CHECK(C.m03 ==  320.0f);
        CHECK(C.m10 ==  722.0f); CHECK(C.m11 ==  748.0f); CHECK(C.m12 ==  774.0f); CHECK(C.m13 ==  800.0f);
        CHECK(C.m20 == 1154.0f); CHECK(C.m21 == 1196.0f); CHECK(C.m22 == 1238.0f); CHECK(C.m23 == 1280.0f);
        CHECK(C.m30 == 1586.0f); CHECK(C.m31 == 1644.0f); CHECK(C.m32 == 1702.0f); CHECK(C.m33 == 1760.0f);
    }

    SUBCASE("divide")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat4 B = A / f;
        CHECK(B.m00 == A.m00 / f);
        CHECK(B.m01 == A.m01 / f);
        CHECK(B.m02 == A.m02 / f);
        CHECK(B.m03 == A.m03 / f);

        CHECK(B.m10 == A.m10 / f);
        CHECK(B.m11 == A.m11 / f);
        CHECK(B.m12 == A.m12 / f);
        CHECK(B.m13 == A.m13 / f);

        CHECK(B.m20 == A.m20 / f);
        CHECK(B.m21 == A.m21 / f);
        CHECK(B.m22 == A.m22 / f);
        CHECK(B.m23 == A.m23 / f);

        CHECK(B.m30 == A.m30 / f);
        CHECK(B.m31 == A.m31 / f);
        CHECK(B.m32 == A.m32 / f);
        CHECK(B.m33 == A.m33 / f);

        A /= f;
        CHECK(A.m00 == B.m00); CHECK(A.m01 == B.m01); CHECK(A.m02 == B.m02); CHECK(A.m03 == B.m03);
        CHECK(A.m10 == B.m10); CHECK(A.m11 == B.m11); CHECK(A.m12 == B.m12); CHECK(A.m13 == B.m13);
        CHECK(A.m20 == B.m20); CHECK(A.m21 == B.m21); CHECK(A.m22 == B.m22); CHECK(A.m23 == B.m23);
        CHECK(A.m30 == B.m30); CHECK(A.m31 == B.m31); CHECK(A.m32 == B.m32); CHECK(A.m33 == B.m33);
    }

    SUBCASE("identity")
    {
        kuro::mat4 I = kuro::mat4_identity();
        CHECK(I.m00 == 1.0f); CHECK(I.m01 == 0.0f); CHECK(I.m02 == 0.0f); CHECK(I.m03 == 0.0f);
        CHECK(I.m10 == 0.0f); CHECK(I.m11 == 1.0f); CHECK(I.m12 == 0.0f); CHECK(I.m13 == 0.0f);
        CHECK(I.m20 == 0.0f); CHECK(I.m21 == 0.0f); CHECK(I.m22 == 1.0f); CHECK(I.m23 == 0.0f);
        CHECK(I.m30 == 0.0f); CHECK(I.m31 == 0.0f); CHECK(I.m32 == 0.0f); CHECK(I.m33 == 1.0f);

        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = a * I;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);
        CHECK(b.w == a.w);

        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::mat4 B = I * A;
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m01); CHECK(B.m02 == A.m02); CHECK(B.m03 == A.m03);
        CHECK(B.m10 == A.m10); CHECK(B.m11 == A.m11); CHECK(B.m12 == A.m12); CHECK(B.m13 == A.m13);
        CHECK(B.m20 == A.m20); CHECK(B.m21 == A.m21); CHECK(B.m22 == A.m22); CHECK(B.m23 == A.m23);
        CHECK(B.m30 == A.m30); CHECK(B.m31 == A.m31); CHECK(B.m32 == A.m32); CHECK(B.m33 == A.m33);

        B = A * I;
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m01); CHECK(B.m02 == A.m02); CHECK(B.m03 == A.m03);
        CHECK(B.m10 == A.m10); CHECK(B.m11 == A.m11); CHECK(B.m12 == A.m12); CHECK(B.m13 == A.m13);
        CHECK(B.m20 == A.m20); CHECK(B.m21 == A.m21); CHECK(B.m22 == A.m22); CHECK(B.m23 == A.m23);
        CHECK(B.m30 == A.m30); CHECK(B.m31 == A.m31); CHECK(B.m32 == A.m32); CHECK(B.m33 == A.m33);
    }

    SUBCASE("transpose")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        kuro::mat4 B = kuro::mat4_transpose(A);
        CHECK(B.m00 == A.m00); CHECK(B.m01 == A.m10); CHECK(B.m02 == A.m20); CHECK(B.m03 == A.m30);
        CHECK(B.m10 == A.m01); CHECK(B.m11 == A.m11); CHECK(B.m12 == A.m21); CHECK(B.m13 == A.m31);
        CHECK(B.m20 == A.m02); CHECK(B.m21 == A.m12); CHECK(B.m22 == A.m22); CHECK(B.m23 == A.m32);
        CHECK(B.m30 == A.m03); CHECK(B.m31 == A.m13); CHECK(B.m32 == A.m23); CHECK(B.m33 == A.m33);
    }

    SUBCASE("trace")
    {
        kuro::mat4 M = {
             0.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        CHECK(kuro::mat4_trace(M) == 33.0f);
    }

    SUBCASE("determinant")
    {
        kuro::mat4 I = kuro::mat4_identity();
        CHECK(kuro::mat4_det(I) == 1.0f);

        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        CHECK(kuro::mat4_det(A) == 0.0f);

        kuro::mat4 B = {
             3.0f, 2.0f, -1.0f,  4.0f,
             2.0f, 1.0f,  5.0f,  7.0f,
             0.0f, 5.0f,  2.0f, -6.0f,
            -1.0f, 2.0f,  1.0f,  0.0f
        };

        CHECK(kuro::mat4_det(B) == -418.0f);
    }

    SUBCASE("adjoint")
    {
        kuro::mat4 A = {
             3.0f, 2.0f, -1.0f,  4.0f,
             2.0f, 1.0f,  5.0f,  7.0f,
             0.0f, 5.0f,  2.0f, -6.0f,
            -1.0f, 2.0f,  1.0f,  0.0f
        };

        kuro::mat4 B = kuro::mat4_adj(A);
        CHECK(B.m00 == -47.0f); CHECK(B.m01 == -28.0f); CHECK(B.m02 == -64.0f); CHECK(B.m03 ==  221.0f);
        CHECK(B.m10 == -56.0f); CHECK(B.m11 ==  20.0f); CHECK(B.m12 == -14.0f); CHECK(B.m13 == -128.0f);
        CHECK(B.m20 ==  65.0f); CHECK(B.m21 == -68.0f); CHECK(B.m22 == -36.0f); CHECK(B.m23 ==   59.0f);
        CHECK(B.m30 == -25.0f); CHECK(B.m31 ==  -6.0f); CHECK(B.m32 ==  46.0f); CHECK(B.m33 ==  -87.0f);
    }

    SUBCASE("invertible")
    {
        kuro::mat4 A = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };

        CHECK(kuro::mat4_invertible(A) == false);

        kuro::mat4 B = {
             3.0f, 2.0f, -1.0f,  4.0f,
             2.0f, 1.0f,  5.0f,  7.0f,
             0.0f, 5.0f,  2.0f, -6.0f,
            -1.0f, 2.0f,  1.0f,  0.0f
        };

        CHECK(kuro::mat4_invertible(B) == true);
    }

    SUBCASE("inverse")
    {
        kuro::mat4 A = {
             3.0f, 2.0f, -1.0f,  4.0f,
             2.0f, 1.0f,  5.0f,  7.0f,
             0.0f, 5.0f,  2.0f, -6.0f,
            -1.0f, 2.0f,  1.0f,  0.0f
        };

        kuro::mat4 B = kuro::mat4_inverse(A);

        kuro::mat4 C = A * B;
        CHECK(C.m00 == doctest::Approx(1.0f));
        CHECK(C.m01 == doctest::Approx(0.0f));
        CHECK(C.m02 == doctest::Approx(0.0f));
        CHECK(C.m03 == doctest::Approx(0.0f));

        CHECK(C.m10 == doctest::Approx(0.0f));
        CHECK(C.m11 == doctest::Approx(1.0f));
        CHECK(C.m12 == doctest::Approx(0.0f));
        CHECK(C.m13 == doctest::Approx(0.0f));

        CHECK(C.m20 == doctest::Approx(0.0f));
        CHECK(C.m21 == doctest::Approx(0.0f));
        CHECK(C.m22 == doctest::Approx(1.0f));
        CHECK(C.m23 == doctest::Approx(0.0f));

        CHECK(C.m30 == doctest::Approx(0.0f));
        CHECK(C.m31 == doctest::Approx(0.0f));
        CHECK(C.m32 == doctest::Approx(0.0f));
        CHECK(C.m33 == doctest::Approx(1.0f));

        C = B * A;
        CHECK(C.m00 == doctest::Approx(1.0f));
        CHECK(C.m01 == doctest::Approx(0.0f));
        CHECK(C.m02 == doctest::Approx(0.0f));
        CHECK(C.m03 == doctest::Approx(0.0f));

        CHECK(C.m10 == doctest::Approx(0.0f));
        CHECK(C.m11 == doctest::Approx(1.0f));
        CHECK(C.m12 == doctest::Approx(0.0f));
        CHECK(C.m13 == doctest::Approx(0.0f));

        CHECK(C.m20 == doctest::Approx(0.0f));
        CHECK(C.m21 == doctest::Approx(0.0f));
        CHECK(C.m22 == doctest::Approx(1.0f));
        CHECK(C.m23 == doctest::Approx(0.0f));

        CHECK(C.m30 == doctest::Approx(0.0f));
        CHECK(C.m31 == doctest::Approx(0.0f));
        CHECK(C.m32 == doctest::Approx(0.0f));
        CHECK(C.m33 == doctest::Approx(1.0f));

        kuro::vec4 a = {11.0f, 12.0f, 13.0f, 14.0f};
        kuro::vec4 b = a * A * B;
        CHECK(b.x == doctest::Approx(a.x));
        CHECK(b.y == doctest::Approx(a.y));
        CHECK(b.z == doctest::Approx(a.z));
        CHECK(b.w == doctest::Approx(a.w));
    }

    SUBCASE("translation")
    {
        kuro::vec3 t = {10.0f, 20.0f, 30.0f};

        kuro::mat4 T = kuro::mat4_translation(t.x, t.y, t.z);
        kuro::vec4 a = {1.0f, 1.0f, 1.0f, 1.0f};

        kuro::vec4 b = a * T;
        CHECK(b.x == a.x + t.x);
        CHECK(b.y == a.y + t.y);
        CHECK(b.z == a.z + t.z);
        CHECK(b.w == 1.0f);

        T = kuro::mat4_translation(t);

        b = a * T;
        CHECK(b.x == a.x + t.x);
        CHECK(b.y == a.y + t.y);
        CHECK(b.z == a.z + t.z);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(T);
        CHECK(c.x == a.x);
        CHECK(c.y == a.y);
        CHECK(c.z == a.z);
        CHECK(c.w == 1.0f);
    }

    SUBCASE("rotation x")
    {
        kuro::mat4 R = kuro::mat4_rotation_x((kuro::f32)kuro::PI_OVER_2);
        kuro::vec4 a = {1.0f, 1.0f, 1.0f, 1.0f};

        kuro::vec4 b = a * R;
        CHECK(b.x == doctest::Approx(a.x));
        CHECK(b.y == doctest::Approx(-a.y));
        CHECK(b.z == doctest::Approx(a.z));
        CHECK(b.w == doctest::Approx(1.0f));

        kuro::vec4 c = b * kuro::mat4_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("rotation y")
    {
        kuro::mat4 R = kuro::mat4_rotation_y((kuro::f32)kuro::PI_OVER_2);
        kuro::vec4 a = {1.0f, 1.0f, 1.0f, 1.0f};

        kuro::vec4 b = a * R;
        CHECK(b.x == doctest::Approx(a.x));
        CHECK(b.y == doctest::Approx(a.y));
        CHECK(b.z == doctest::Approx(-a.z));
        CHECK(b.w == doctest::Approx(1.0f));

        kuro::vec4 c = b * kuro::mat4_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("rotation z")
    {
        kuro::mat4 R = kuro::mat4_rotation_z((kuro::f32)kuro::PI_OVER_2);
        kuro::vec4 a = {1.0f, 1.0f, 1.0f, 1.0f};

        kuro::vec4 b = a * R;
        CHECK(b.x == doctest::Approx(-a.x));
        CHECK(b.y == doctest::Approx(a.y));
        CHECK(b.z == doctest::Approx(a.z));
        CHECK(b.w == doctest::Approx(1.0f));

        kuro::vec4 c = b * kuro::mat4_inverse(R);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    // TODO[Waleed]: invistigate euler test case
    SUBCASE("euler")
    {
        // kuro::vec3 e = kuro::f32(kuro::TO_RADIAN) * kuro::vec3{30.0f, 45.0f, 0.0f};

        // kuro::mat4 E = kuro::mat4_euler(e.x, e.y, e.z);
        // kuro::vec4 a = {0.0f, 0.0f, 1.0f, 1.0f};

        // kuro::vec4 b = a * E;
        // CHECK(b.x == 0.0f);
        // CHECK(b.y == doctest::Approx(-0.5f));
        // CHECK(b.z == 0.0f);
        // CHECK(b.w == 1.0f);
    }

    SUBCASE("scaling")
    {
        kuro::vec3 s = {10.0f, 20.0f, 30.0f};

        kuro::mat4 S = kuro::mat4_scaling(s.x, s.y, s.z);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);
        CHECK(b.z == a.z * s.z);
        CHECK(b.w == 1.0f);

        S = kuro::mat4_scaling(s);

        b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);
        CHECK(b.z == a.z * s.z);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(S);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("shearing xy")
    {
        kuro::f32 s = 10.0f;

        kuro::mat4 H = kuro::mat4_shearing_xy(s);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * H;
        CHECK(b.x == a.x + a.y*s);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("shearing xz")
    {
        kuro::f32 s = 10.0f;

        kuro::mat4 H = kuro::mat4_shearing_xz(s);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * H;
        CHECK(b.x == a.x + a.z*s);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("shearing yx")
    {
        kuro::f32 s = 10.0f;

        kuro::mat4 H = kuro::mat4_shearing_yx(s);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y + a.x*s);
        CHECK(b.z == a.z);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("shearing yz")
    {
        kuro::f32 s = 10.0f;

        kuro::mat4 H = kuro::mat4_shearing_yz(s);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y + a.z*s);
        CHECK(b.z == a.z);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("shearing zx")
    {
        kuro::f32 s = 10.0f;

        kuro::mat4 H = kuro::mat4_shearing_zx(s);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z + a.x*s);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("shearing zy")
    {
        kuro::f32 s = 10.0f;

        kuro::mat4 H = kuro::mat4_shearing_zy(s);
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};

        kuro::vec4 b = a * H;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
        CHECK(b.z == a.z + a.y*s);
        CHECK(b.w == 1.0f);

        kuro::vec4 c = b * kuro::mat4_inverse(H);
        CHECK(c.x == doctest::Approx(a.x));
        CHECK(c.y == doctest::Approx(a.y));
        CHECK(c.z == doctest::Approx(a.z));
        CHECK(c.w == doctest::Approx(1.0f));
    }

    SUBCASE("look at")
    {
        kuro::mat4 M = kuro::mat4_look_at({0.0f, 0.0f, 10.0f}, {}, {0.0f, 1.0f, 0.0f});
        kuro::vec4 a = {0.0f, 0.0f, 0.0f, 1.0f};

        kuro::vec4 b = a * M;
        CHECK(b.x == doctest::Approx(0.0f));
        CHECK(b.y == doctest::Approx(0.0f));
        CHECK(b.z == doctest::Approx(-10.0f));
        CHECK(b.w == doctest::Approx(1.0f));

        M = kuro::mat4_look_at({0.0f, 10.0f, 0.0f}, {}, {1.0f, 0.0f, 0.0f});

        b = a * M;
        CHECK(b.x == doctest::Approx(0.0f));
        CHECK(b.y == doctest::Approx(0.0f));
        CHECK(b.z == doctest::Approx(-10.0f));
        CHECK(b.w == doctest::Approx(1.0f));

        M = kuro::mat4_look_at({10.0f, 0.0f, 0.0f}, {}, {0.0f, 1.0f, 0.0f});

        b = a * M;
        CHECK(b.x == doctest::Approx(0.0f));
        CHECK(b.y == doctest::Approx(0.0f));
        CHECK(b.z == doctest::Approx(-10.0f));
        CHECK(b.w == doctest::Approx(1.0f));
    }

    SUBCASE("orthographic projection")
    {
        // TODO[Waleed]: implement
    }

    SUBCASE("perspective projection")
    {
        // TODO[Waleed]: implement
    }
}

// =================================================================================================
// == INTERSECTIONS ================================================================================
// =================================================================================================