# CURL Adaptor library
The library that is the custom adapter of the [CURL](https://curl.se/libcurl/) library for the TB's needs.

# Table of content
* [Main hierarchy](#Main-hierarchy)

# Main hierarchy
* namespace TB_NS
    * namespace CurlAdapter_NS
        * class EntryPoint - presents the entry point for the CURL library
        * namespace Http_NS
            * class RequestI - common interface of the HTTP-request for the CURL library
            * class PostRequest - HTTP-POST-request adapter
            * class Connection - the adapter of the CURL library HTTP-connection

