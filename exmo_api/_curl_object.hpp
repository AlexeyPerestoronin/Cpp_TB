#pragma once

#include <curl/curl.h>

class curl_object {
	static CURL* ch;
public:
	curl_object() = delete;

	~curl_object();

	static CURL* get_instance();
};
