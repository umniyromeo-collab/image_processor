// NOLINTBEGIN(readability-magic-numbers)
#include <catch.hpp>
#include <Image.h>
#include "../Main/filter_factory.h"
#include "../Exceptions/custom_exc_lib.h"

TEST_CASE("Pixel operations", "[pixel]") {
    Pixel p1{1, 2, 3};
    Pixel p2{4, 5, 6};

    SECTION("+") {
        Pixel add = p1 + p2;
        REQUIRE(add.r == 5);
        REQUIRE(add.g == 7);
        REQUIRE(add.b == 9);
    }

    SECTION("-") {
        Pixel sub = p1 - p2;
        REQUIRE(sub.r == -3);
        REQUIRE(sub.g == -3);
        REQUIRE(sub.b == -3);
    }

    SECTION("Clamp") {
        Pixel clamp{1.2, -0.1, 2};
        clamp.Clamp();
        REQUIRE(clamp.r == 1);
        REQUIRE(clamp.g == 0);
        REQUIRE(clamp.b == 1);
    }
}

TEST_CASE("Invalid filter type", "[StringToFilterType]") {
    REQUIRE_THROWS_WITH(StringToFilterType("-abc"), Catch::Contains("-abcis not a valid filter type"));
}

TEST_CASE("FabricTests", "[fabric]") {
    SECTION("No exc") {
        REQUIRE_NOTHROW(FilterFactory(FilterType::GRAYSCALE, {}));
        REQUIRE_NOTHROW(FilterFactory(FilterType::CROP, {"100", "200"}));
        REQUIRE_NOTHROW(FilterFactory(FilterType::EDGE, {"0.5"}));
    }

    SECTION("wrong args") {
        REQUIRE_THROWS_AS(FilterFactory(FilterType::CROP, {"100"}), InvalidFilterArgs);

        REQUIRE_THROWS_AS(FilterFactory(FilterType::CROP, {"100", "200", "300"}), InvalidFilterArgs);

        REQUIRE_THROWS_AS(FilterFactory(FilterType::GRAYSCALE, {"123"}), InvalidFilterArgs);

        REQUIRE_THROWS_AS(FilterFactory(FilterType::EDGE, {}), InvalidFilterArgs);

        REQUIRE_THROWS_AS(FilterFactory(FilterType::BLUR, {}), InvalidFilterArgs);
    }
}

TEST_CASE("StrCast tests", "[SafeStrCast]") {
    SECTION("no exc") {
        REQUIRE(SafeStrCast<uint64_t>("123") == 123);

        REQUIRE(SafeStrCast<double>("3.14") == 3.14);

        REQUIRE(SafeStrCast<long double>("2.5") == 2.5);
    }

    SECTION("invalid strings for cast") {
        REQUIRE_THROWS_AS(SafeStrCast<uint64_t>("abc"), InvalidFilterArgs);

        REQUIRE_THROWS_AS(SafeStrCast<double>("12.3abc"), InvalidFilterArgs);

        REQUIRE_THROWS_AS(SafeStrCast<uint64_t>(""), InvalidFilterArgs);
    }
}

// NOLINTEND(readability-magic-numbers)