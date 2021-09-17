#include "../base_exception.hpp"

namespace TB_NS {
    Exception::Exception(std::string i_message, boost::source_location&& i_location)
        : d_message{ std::move(i_message) }
        , d_location{ std::move(i_location) } {}

    Exception::Exception(std::string i_message, boost::source_location&& i_location, Exception& io_suberror)
        : d_message{ std::move(i_message) }
        , d_location{ std::move(i_location) }
        , d_suberror{ std::make_unique<Exception>(std::move(io_suberror)) } {}

    const char* Exception::what() const {
        return "";
    }

} // namespace TB_NS
