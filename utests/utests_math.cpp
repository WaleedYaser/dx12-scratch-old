#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <kuro_math.h>

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
        CHECK(v.x == 0);
        CHECK(v.y == 0);

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

        {
            kuro::vec2 b = a * f;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
        }

        {
            kuro::vec2 b = f * a;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
        }

        {
            kuro::vec2 b = a;
            b *= f;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
        }
    }

    SUBCASE("divide")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::f32 f = 5.0f;

        {
            kuro::vec2 b = a / f;
            CHECK(b.x == a.x / f);
            CHECK(b.y == a.y / f);
        }

        {
            kuro::vec2 b = a;
            b /= f;
            CHECK(b.x == a.x / f);
            CHECK(b.y == a.y / f);
        }
    }

    SUBCASE("norm")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(a_norm == 5.0f);
        CHECK(b_norm == 265.0f);
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

    SUBCASE("dot")
    {
        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = {11.0f, 12.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(kuro::dot(a, a) == doctest::Approx(a_norm));
        CHECK(kuro::dot(b, b) == doctest::Approx(b_norm));
        CHECK(kuro::dot(a, b) == doctest::Approx(35.0f));

        kuro::vec2 a_normalized = kuro::normalize(a);
        kuro::vec2 b_normalized = kuro::normalize(b);

        CHECK(kuro::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(a_normalized, b_normalized) == doctest::Approx(0.962f).epsilon(0.001f));
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

        {
            kuro::vec3 b = a * f;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
            CHECK(b.z == a.z * f);
        }

        {
            kuro::vec3 b = f * a;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
            CHECK(b.z == a.z * f);
        }

        {
            kuro::vec3 b = a;
            b *= f;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
            CHECK(b.z == a.z * f);
        }
    }

    SUBCASE("divide")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::f32 f = 5.0f;

        {
            kuro::vec3 b = a / f;
            CHECK(b.x == a.x / f);
            CHECK(b.y == a.y / f);
            CHECK(b.z == a.z / f);
        }

        {
            kuro::vec3 b = a;
            b /= f;
            CHECK(b.x == a.x / f);
            CHECK(b.y == a.y / f);
            CHECK(b.z == a.z / f);
        }
    }

    SUBCASE("norm")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(a_norm == 14.0f);
        CHECK(b_norm == 434.0f);
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

    SUBCASE("dot")
    {
        kuro::vec3 a = {1.0f, 2.0f, 3.0f};
        kuro::vec3 b = {11.0f, 12.0f, 13.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(kuro::dot(a, a) == doctest::Approx(a_norm));
        CHECK(kuro::dot(b, b) == doctest::Approx(b_norm));
        CHECK(kuro::dot(a, b) == doctest::Approx(74.0f));

        kuro::vec3 a_normalized = kuro::normalize(a);
        kuro::vec3 b_normalized = kuro::normalize(b);

        CHECK(kuro::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(a_normalized, b_normalized) == doctest::Approx(0.949f).epsilon(0.001f));
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

        {
            kuro::vec4 b = a * f;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
            CHECK(b.z == a.z * f);
            CHECK(b.w == a.w * f);
        }

        {
            kuro::vec4 b = f * a;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
            CHECK(b.z == a.z * f);
            CHECK(b.w == a.w * f);
        }

        {
            kuro::vec4 b = a;
            b *= f;
            CHECK(b.x == a.x * f);
            CHECK(b.y == a.y * f);
            CHECK(b.z == a.z * f);
            CHECK(b.w == a.w * f);
        }
    }

    SUBCASE("divide")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::f32 f = 5.0f;

        {
            kuro::vec4 b = a / f;
            CHECK(b.x == a.x / f);
            CHECK(b.y == a.y / f);
            CHECK(b.z == a.z / f);
            CHECK(b.w == a.w / f);
        }

        {
            kuro::vec4 b = a;
            b /= f;
            CHECK(b.x == a.x / f);
            CHECK(b.y == a.y / f);
            CHECK(b.z == a.z / f);
            CHECK(b.w == a.w / f);
        }
    }

    SUBCASE("norm")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(a_norm == 30.0f);
        CHECK(b_norm == 630.0f);
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

    SUBCASE("dot")
    {
        kuro::vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        kuro::vec4 b = {11.0f, 12.0f, 13.0f, 14.0f};

        kuro::f32 a_norm = kuro::norm(a);
        kuro::f32 b_norm = kuro::norm(b);

        CHECK(kuro::dot(a, a) == doctest::Approx(a_norm));
        CHECK(kuro::dot(b, b) == doctest::Approx(b_norm));
        CHECK(kuro::dot(a, b) == doctest::Approx(130.0f));

        kuro::vec4 a_normalized = kuro::normalize(a);
        kuro::vec4 b_normalized = kuro::normalize(b);

        CHECK(kuro::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
        CHECK(kuro::dot(a_normalized, b_normalized) == doctest::Approx(0.946f).epsilon(0.001f));
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

        CHECK(M.m00 == 0.0f);
        CHECK(M.m01 == 0.0f);
        CHECK(M.m10 == 0.0f);
        CHECK(M.m11 == 0.0f);

        M = {
             1.0f,  2.0f,
            11.0f, 12.0f
        };

        CHECK(M.m00 == 1.0f);
        CHECK(M.m01 == 2.0f);
        CHECK(M.m10 == 11.0f);
        CHECK(M.m11 == 12.0f);
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
        CHECK(C.m00 == A.m00 + B.m00);
        CHECK(C.m01 == A.m01 + B.m01);
        CHECK(C.m10 == A.m10 + B.m10);
        CHECK(C.m11 == A.m11 + B.m11);

        A += B;
        CHECK(A.m00 == C.m00);
        CHECK(A.m01 == C.m01);
        CHECK(A.m10 == C.m10);
        CHECK(A.m11 == C.m11);
    }

    SUBCASE("unary minus")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = -A;
        CHECK(B.m00 == -A.m00);
        CHECK(B.m01 == -A.m01);
        CHECK(B.m10 == -A.m10);
        CHECK(B.m11 == -A.m11);
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
        CHECK(C.m00 == A.m00 - B.m00);
        CHECK(C.m01 == A.m01 - B.m01);
        CHECK(C.m10 == A.m10 - B.m10);
        CHECK(C.m11 == A.m11 - B.m11);

        A -= B;
        CHECK(A.m00 == C.m00);
        CHECK(A.m01 == C.m01);
        CHECK(A.m10 == C.m10);
        CHECK(A.m11 == C.m11);
    }

    SUBCASE("multiply scalar")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat2 B = A * f;
        CHECK(B.m00 == f * A.m00);
        CHECK(B.m01 == f * A.m01);
        CHECK(B.m10 == f * A.m10);
        CHECK(B.m11 == f * A.m11);

        B = f * A;
        CHECK(B.m00 == f * A.m00);
        CHECK(B.m01 == f * A.m01);
        CHECK(B.m10 == f * A.m10);
        CHECK(B.m11 == f * A.m11);

        A *= f;
        CHECK(A.m00 == B.m00);
        CHECK(A.m01 == B.m01);
        CHECK(A.m10 == B.m10);
        CHECK(A.m11 == B.m11);
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
        CHECK(C.m00 == 37.0f);
        CHECK(C.m01 == 40.0f);
        CHECK(C.m10 == 85.0f);
        CHECK(C.m11 == 92.0f);
    }

    SUBCASE("divide")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::f32 f = 5.0f;

        kuro::mat2 C = A / f;
        CHECK(C.m00 == A.m00 / f);
        CHECK(C.m01 == A.m01 / f);
        CHECK(C.m10 == A.m10 / f);
        CHECK(C.m11 == A.m11 / f);

        A /= f;
        CHECK(A.m00 == C.m00);
        CHECK(A.m01 == C.m01);
        CHECK(A.m10 == C.m10);
        CHECK(A.m11 == C.m11);
    }

    SUBCASE("identity")
    {
        kuro::mat2 I = kuro::mat2_identity();
        CHECK(I.m00 == 1.0f);
        CHECK(I.m01 == 0.0f);
        CHECK(I.m10 == 0.0f);
        CHECK(I.m11 == 1.0f);

        kuro::vec2 a = {1.0f, 2.0f};
        kuro::vec2 b = a * I;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);

        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = I * A;
        CHECK(B.m00 == A.m00);
        CHECK(B.m01 == A.m01);
        CHECK(B.m10 == A.m10);
        CHECK(B.m11 == A.m11);

        B = A * I;
        CHECK(B.m00 == A.m00);
        CHECK(B.m01 == A.m01);
        CHECK(B.m10 == A.m10);
        CHECK(B.m11 == A.m11);
    }

    SUBCASE("transpose")
    {
        kuro::mat2 A = {
            1.0f, 2.0f,
            3.0f, 4.0f
        };

        kuro::mat2 B = kuro::mat2_transpose(A);
        CHECK(B.m00 == A.m00);
        CHECK(B.m01 == A.m10);
        CHECK(B.m10 == A.m01);
        CHECK(B.m11 == A.m11);
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
        CHECK(C.m00 == 1.0f);
        CHECK(C.m01 == 0.0f);
        CHECK(C.m10 == 0.0f);
        CHECK(C.m11 == 1.0f);

        C = B * A;
        CHECK(C.m00 == 1.0f);
        CHECK(C.m01 == 0.0f);
        CHECK(C.m10 == 0.0f);
        CHECK(C.m11 == 1.0f);

        kuro::vec2 a = {11.0f, 12.0f};
        kuro::vec2 b = a * A * B;
        CHECK(b.x == a.x);
        CHECK(b.y == a.y);
    }

    SUBCASE("rotation")
    {
        kuro::mat2 R = kuro::mat2_rotation((kuro::f32)kuro::PI_OVER_2);
        kuro::vec2 a = kuro::vec2{1.0f, 1.0f};

        kuro::vec2 b = a * R;
        CHECK(b.x == doctest::Approx(-a.x));
        CHECK(b.y == doctest::Approx(a.y));

        R = kuro::mat2_rotation(-(kuro::f32)kuro::PI_OVER_2);
        b = b * R;
        CHECK(b.x == doctest::Approx(a.x));
        CHECK(b.y == doctest::Approx(a.y));
    }

    SUBCASE("scaling")
    {
        kuro::vec2 s = {10.0f, 20.f};

        kuro::mat2 S = kuro::mat2_scaling(s.x, s.y);
        kuro::vec2 a = kuro::vec2{1.0f, 2.0f};

        kuro::vec2 b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);

        S = kuro::mat2_scaling(s);

        b = a * S;
        CHECK(b.x == a.x * s.x);
        CHECK(b.y == a.y * s.y);
    }

    SUBCASE("transform")
    {

        kuro::mat2 T = kuro::mat2_transform({10.0f, 10.0f}, (kuro::f32)kuro::PI_OVER_2);
        kuro::vec2 a = {1.0f, 1.0f};

        kuro::vec2 b = a * T;
        CHECK(b.x == doctest::Approx(-10.0f));
        CHECK(b.y == doctest::Approx(10.0f));

        T = kuro::mat2_transform({10.0f, 20.0f}, (kuro::f32)kuro::PI_OVER_2);

        b = a * T;
        CHECK(b.x == doctest::Approx(-20.0f));
        CHECK(b.y == doctest::Approx(10.0f));
    }
}

// =================================================================================================
// == MAT3 =========================================================================================
// =================================================================================================

// =================================================================================================
// == MAT4 =========================================================================================
// =================================================================================================

// =================================================================================================
// == INTERSECTIONS ================================================================================
// =================================================================================================