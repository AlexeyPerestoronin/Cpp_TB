// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../http.hpp"

namespace TB_NS::CurlAdapter_NS::Http_NS {
    PostRequest::PostRequest() {}

    PostRequest ::~PostRequest() {}

    curl_slist* PostRequest::prepare(const StrToStr& headers) {
        for (auto h : headers) {
            std::string s{ std::string(h.first + ": " + h.second) };
            m_list = curl_slist_append(m_list, s.c_str());
        }
        return m_list;
    }
} // namespace TB_NS::CurlAdapter_NS::Http_NS
