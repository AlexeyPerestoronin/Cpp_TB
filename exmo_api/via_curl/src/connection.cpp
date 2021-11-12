#pragma once

#include <exmo_api/via_curl/_connection.hpp>

namespace TB_NS::HTTP_NS {
    curl_slist* Post::prepare(const StrToStr& headers) const {
        curl_slist* list = nullptr;
        for (auto h : headers) {
            std::string s{ std::string(h.first + ": " + h.second) };
            list = curl_slist_append(list, s.c_str());
        }
        return list;
    }

    void Connection::request(std::string_view i_url, const Request& i_request, std::string_view i_params, const StrToStr& i_headers) {
        m_requestData.clear();
        m_requestHeader.clear();
        CURL* curlInsPtr = Curl_NS::EntryPoint::GetIns();
        curl_easy_setopt(curlInsPtr, CURLOPT_URL, i_url.data());
        curl_easy_setopt(curlInsPtr, CURLOPT_WRITEDATA, &m_requestData);
        curl_easy_setopt(curlInsPtr, CURLOPT_HEADERDATA, &m_requestHeader);
        curl_easy_setopt(curlInsPtr, CURLOPT_WRITEFUNCTION, write_received_data_to_string);

        curl_easy_setopt(curlInsPtr, CURLOPT_POSTFIELDS, i_params.data());
        curl_easy_setopt(curlInsPtr, CURLOPT_POSTFIELDSIZE, i_params.size());

        curl_slist* list = i_request.prepare(i_headers);
        curl_easy_setopt(curlInsPtr, CURLOPT_HTTPHEADER, list);

        CURLcode rescode = curl_easy_perform(curlInsPtr);
        if (rescode != CURLE_OK) {
            std::string msg{ "!> curl_easy_perform failed with error: " };
            msg += curl_easy_strerror(rescode);
            throw std::runtime_error(msg);
        }
    }

    JsonData Connection::get_response() {
        return m_requestData;
    }

    std::size_t Connection::write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer) {
        if (buffer) {
            std::string* tstr = reinterpret_cast<std::string*>(buffer);
            tstr->append(ptr, size * nmemb);
        }
        return size * nmemb;
    }
} // namespace TB_NS::HTTP_NS
