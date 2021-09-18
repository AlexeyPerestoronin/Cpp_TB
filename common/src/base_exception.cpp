#include "../base_exception.hpp"

namespace TB_NS::Error_NS {
    const char* Exception::what() const {
        using namespace boost;
        std::stringstream message;
        for (auto [suberror, extractor] = std::make_tuple(
                 this,
                 [&message](std::string i_info, auto*i_data, std::string i_defaultMessage = "no info") {
                     message << std::move(i_info);
                     i_data ? message << *i_data : message << i_defaultMessage;
                     message << std::endl;
                 });
             suberror; suberror = get_error_info<Suberror>(*suberror)) {
            message << "***" << std::endl;
            extractor("[location] ", get_error_info<Location>(*suberror));
            extractor("[description] ", get_error_info<Description>(*suberror));
            message << "***" << std::endl;
        }
        return d_errorMessage.assign(message.str()).data();
    }
} // namespace TB_NS::Error_NS
