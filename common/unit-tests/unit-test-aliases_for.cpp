#include <gtest/gtest.h>
#include <common/stdafx.hpp>
#include <common/aliases.hpp>

namespace {
    using namespace TB_NS;
    TEST(Testing_AliasFor, IntFromToStr) {
        Int i{};
        ASSERT_STREQ(i.toStr().c_str(), "0");
        ASSERT_EQ(0, static_cast<int>(i));

        i += 100;
        ASSERT_STREQ(i.toStr().c_str(), "100");
        ASSERT_EQ(100, static_cast<int>(i));

        --i;
        ASSERT_STREQ(i.toStr().c_str(), "99");
        ASSERT_EQ(99, static_cast<int>(i));

        ASSERT_TRUE(i.fromStr("-123"));
        ASSERT_STREQ(i.toStr().c_str(), "-123");
        ASSERT_EQ(-123, static_cast<int>(i));

        ASSERT_TRUE(i.fromStr("+543"));
        ASSERT_STREQ(i.toStr().c_str(), "543");
        ASSERT_EQ(543, static_cast<int>(i));
    }

    TEST(Testing_AliasFor, IntFromToJson_1) {
        Int i{};
        Json json{ i.toJson() };
        ASSERT_STREQ(json.dump().c_str(), "0");
        ASSERT_TRUE(i.fromJson(json));
        ASSERT_EQ(0, static_cast<int>(i));
    }

    TEST(Testing_AliasFor, IntFromToJson_2) {
        Int i{ 111 };
        Json json = "{\"value\":0}"_json;
        json["value"] = i.toJson();
        ASSERT_STREQ(json.dump().c_str(), "{\"value\":111}");
        json["value"] = -123;
        ASSERT_TRUE(i.fromJson(json["value"]));
        ASSERT_EQ(-123, static_cast<int>(i));
    }

    TEST(Testing_AliasFor, IntFromToJson_3) {
        Json json = "{\"value\":\"123\"}"_json;
        Int::O i = FromJson<Int>(json);
        ASSERT_TRUE(i.has_value());
        ASSERT_EQ(123, static_cast<int>(i.value()));
    }

    TEST(Testing_AliasFor, DoubleFromToStr) {
        Double i{};
        ASSERT_STREQ(i.toStr().c_str(), "0");
        ASSERT_EQ(0., static_cast<double>(i));

        i += 100;
        ASSERT_STREQ(i.toStr().c_str(), "100");
        ASSERT_EQ(100., static_cast<double>(i));

        --i;
        ASSERT_STREQ(i.toStr().c_str(), "99");
        ASSERT_EQ(99., static_cast<double>(i));

        ASSERT_TRUE(i.fromStr("-123"));
        ASSERT_STREQ(i.toStr().c_str(), "-123");
        ASSERT_EQ(-123., static_cast<double>(i));

        ASSERT_TRUE(i.fromStr("+543"));
        ASSERT_STREQ(i.toStr().c_str(), "543");
        ASSERT_EQ(543., static_cast<double>(i));

        i = 5.5;
        ASSERT_STREQ(i.toStr().c_str(), "5.5");
        ASSERT_EQ(5.5, static_cast<double>(i));

        i = 0.5e-3;
        ASSERT_STREQ(i.toStr().c_str(), "0.0005");
        ASSERT_EQ(0.5e-3, static_cast<double>(i));

        ASSERT_TRUE(i.fromStr("123.123"));
        ASSERT_STREQ(i.toStr().c_str(), "123.123");
        ASSERT_EQ(123.123, static_cast<double>(i));

        ASSERT_TRUE(i.fromStr("-321.321"));
        ASSERT_STREQ(i.toStr().c_str(), "-321.321");
        ASSERT_EQ(-321.321, static_cast<double>(i));

        ASSERT_TRUE(i.fromStr("0.0000001"));
        ASSERT_STREQ(i.toStr().c_str(), "1e-07");
        ASSERT_EQ(0.0000001, static_cast<double>(i));

        i = 0.;
        ASSERT_EQ(0., static_cast<double>(i));
        ASSERT_EQ(0., static_cast<double>(i++));
        ASSERT_EQ(1., static_cast<double>(i));
        ASSERT_EQ(1., static_cast<double>(i++));
        ASSERT_EQ(2., static_cast<double>(i));
        ASSERT_EQ(2., static_cast<double>(i--));
        ASSERT_EQ(1., static_cast<double>(i--));
        ASSERT_EQ(0., static_cast<double>(i));
        ASSERT_EQ(5., static_cast<double>(i + 5.));
        ASSERT_EQ(5., static_cast<double>(i += 5));
        ASSERT_EQ(10., static_cast<double>(i += 5));
        ASSERT_EQ(50., static_cast<double>(i * 5.));
        ASSERT_EQ(50., static_cast<double>(i *= 5));
        ASSERT_EQ(50., static_cast<double>(i--));
        ASSERT_EQ(48., static_cast<double>(--i));
    }

    TEST(Testing_AliasFor, DoubleFromToJson_1) {
        Double i{};
        Json json{ i.toJson() };
        ASSERT_STREQ(json.dump().c_str(), "0.0");
        ASSERT_TRUE(i.fromJson(json));
        ASSERT_EQ(0., static_cast<double>(i));
    }

    TEST(Testing_AliasFor, DoubleFromToJson_2) {
        Double i{ 111. };
        Json json = "{\"value\":0.0}"_json;
        json["value"] = i.toJson();
        ASSERT_STREQ(json.dump().c_str(), "{\"value\":111.0}");
        json["value"] = -123.;
        ASSERT_TRUE(i.fromJson(json["value"]));
        ASSERT_EQ(-123., static_cast<double>(i));
        i = +345.543;
        json["value"] = i.toJson();
        ASSERT_STREQ(json.dump().c_str(), "{\"value\":345.543}");
    }

    TEST(Testing_AliasFor, For_Map) {
        using DoubltToString = std::map<double, std::string>;
        AliasFor<DoubltToString> aliase{};
        ASSERT_EQ(aliase.size(), 0);
        aliase.emplace(4., "4");
        ASSERT_EQ(aliase.size(), 1);
        ASSERT_EQ(aliase[4], "4");
        aliase = DoubltToString{};
        ASSERT_EQ(aliase.size(), 0);
        aliase = DoubltToString{ { 3., "3" } };
        ASSERT_EQ(aliase.size(), 1);
    }
} // namespace