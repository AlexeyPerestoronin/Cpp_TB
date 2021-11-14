// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../http.hpp"

namespace TB_NS::CurlAdapter_NS::Http_NS {
    void PostRequest::prepare(StrToStr::CR i_headers) {
        for (const auto& [key, value] : i_headers)
            addValue(Str{ key } += ": "s += value);
    }
} // namespace TB_NS::CurlAdapter_NS::Http_NS
