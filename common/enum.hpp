// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/stdafx.hpp>
#include <common/str.hpp>

namespace TB_NS {
    class Enum {
        StrToInt m_strToInt{};
        IntToStr m_intToStr{};

        public:
        Enum& add(int i_index, Str::CR i_view);
    };

    class ID {
        static constexpr int add(char i_view[]) {
            return 1;
        }
    };

    template<const char* message, size_t length>
    struct str {
        //static constexpr const char value[sizeof...(Chars) + 1] = { Chars..., '\0' };
        //static constexpr int size = sizeof...(Chars);
    };

    //template<char... Chars>
    //constexpr const char str<Chars...>::value[sizeof...(Chars) + 1];

    //constexpr auto operator"" _str(const char* i_message, size_t i_length) {
    //    constexpr const char m[] = "sadf";
    //    return str<m, 4>();
    //}

    //auto s = "hello"_str;

    struct ExchangeID {
        enum : uint8_t {
            UNSUPPORTED,
        };

        enum : uint8_t {
            EXMO,
        };
    };

    //auto s = "lkjh"_str;

    //struct ExchangeID : EnumLike<"ExchangeID"> {
    //    enum : uint8_t {
    //        UNSUPPORTED = EnumLike<"ExchangeID">::add<1, "UNSUPPORTED">,
    //        EXMO = EnumLike<"ExchangeID">::add<2, "EXMO">,
    //    };
    //};

} // namespace TB_NS
