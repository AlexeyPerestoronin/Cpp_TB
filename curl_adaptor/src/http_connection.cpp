#pragma once

#include "../http.hpp"

#include <common/base_exception.hpp>

namespace TB_NS::CurlAdapter_NS::Http_NS {
    namespace {
        size_t WriteReceivedDataToString(char* ptr, size_t size, size_t nmemb, void* buffer) {
            if (buffer) {
                Str* tstr = reinterpret_cast<Str*>(buffer);
                tstr->append(ptr, size * nmemb);
            }
            return size * nmemb;
        };
    } // namespace

    void Connection::makeRequest(Str::CR i_url, RequestI& i_request, Str::CR i_params) {
        m_requestData.clear();
        m_requestHeader.clear();
        CURL* curlInsPtr = CurlAdapter_NS::EntryPoint::GetIns();

#define ERROR_AND_MESSAGE ("{:error_code}", ToStr(errorCode))("{:error_message}", curl_easy_strerror(errorCode))
        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_URL, i_url.data()); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["URL"]("{:url}", i_url) ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_WRITEDATA, &m_requestData); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["WRITEDATA"]("{:requested_data}", m_requestData) ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_HEADERDATA, &m_requestHeader); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["HEADERDATA"]("{:requested_header}", m_requestHeader) ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_WRITEFUNCTION, WriteReceivedDataToString); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["WRITEFUNCTION"] ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_POSTFIELDS, i_params.data()); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["POSTFIELDS"]("{:post_fields_params}", i_params) ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_POSTFIELDSIZE, i_params.size()); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["POSTFIELDSIZE"]("{:post_fields_size}", ToStr(i_params.size())) ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_setopt(curlInsPtr, CURLOPT_HTTPHEADER, i_request.get()); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_setopt"]["HTTPHEADER"]("{:value}", i_request.value()) ERROR_AND_MESSAGE << TB_LOCATION;

        if (CURLcode errorCode = curl_easy_perform(curlInsPtr); errorCode != CURLE_OK)
            throw TB_EXEPT["CURL"]["easy_perform"] ERROR_AND_MESSAGE << TB_LOCATION;
#undef ERROR_AND_MESSAGE
    }

    Str Connection::getResponse() {
        return m_requestData;
    }
} // namespace TB_NS::CurlAdapter_NS::Http_NS
