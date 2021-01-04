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

// =================================================================================================
// == VEC3 =========================================================================================
// =================================================================================================

// =================================================================================================
// == VEC4 =========================================================================================
// =================================================================================================

// =================================================================================================
// == MAT2 =========================================================================================
// =================================================================================================

// =================================================================================================
// == MAT3 =========================================================================================
// =================================================================================================

// =================================================================================================
// == MAT4 =========================================================================================
// =================================================================================================

// =================================================================================================
// == INTERSECTIONS ================================================================================
// =================================================================================================