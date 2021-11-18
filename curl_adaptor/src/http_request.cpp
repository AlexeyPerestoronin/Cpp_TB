// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../http.hpp"

namespace TB_NS::CurlAdapter_NS::Http_NS {
    void RequestI::addValue(Str::CR i_value) noexcept {
        m_list = curl_slist_append(m_list, i_value.c_str());
        m_value.append(i_value);
    }

    RequestI::~RequestI() {
        if (m_list)
            curl_slist_free_all(m_list);
    }

    TB_NODISCARD curl_slist* RequestI::get() noexcept {
        return m_list;
    }

    void RequestI::reset() noexcept {
        m_value.clear();
        if (m_list)
            curl_slist_free_all(m_list);
        m_list = nullptr;
    }

    Str::CR RequestI::value() noexcept {
        return m_value;
    }
} // namespace TB_NS::CurlAdapter_NS::Http_NS
