#include <common/base_exception.hpp>
#include <common/rai.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS::Error_NS {
    namespace {
        // brief: list of all exceptions of the instance of the program
        std::list<Exception::ShaPtr> Exceptions{};
    }; // namespace

    std::list<Exception::Ptr> Exception::d_rootExceptions{};

    Exception::Exception(Str i_id, Str i_key) noexcept
        : d_id(std::move(i_id))
        , d_key(std::move(i_key)) {}

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
            message << "[id]" << d_id << std::endl;
            message << "[key]" << d_key << std::endl;
            extractor("[location] ", get_error_info<Location>(*suberror));
            extractor("[description] ", get_error_info<Description>(*suberror));
            message << "***" << std::endl;
        }
        d_errorMessage = std::make_shared<std::string>(message.str());
        return d_errorMessage->data();
    }

    void Exception::LoadSettings(const fs::path& i_settigsFilePath) {
        using namespace nlohmann;

        json errors;
        RAI<std::ifstream>(i_settigsFilePath) >> errors;

        struct {
            Exception::PtrC createNewException(json& error) {
                Exception::Ptr result{};
                try {
                    auto exception = std::make_shared<Exception>(std::move(error["id"]), std::move(error["key"]));
                    result = exception.get();
                    Exceptions.push_back(std::move(exception));
                    if (auto values = error.find("values"); values != error.end())
                        for (auto [key, value] : values->items())
                            result->d_values.emplace(key, value);
                } catch (...) {
                }
                return result;
            }

            void operator()(json& errors, Exception::PtrC rootException = nullptr) {
                for (auto error : errors["exceptions"]) {
                    if (auto* exceptionPtrC = createNewException(error); exceptionPtrC) {
                        if (!rootException)
                            Exception::d_rootExceptions.push_back(rootException);
                        else
                            rootException->d_subException.push_back(exceptionPtrC);
                        if (error.find("exceptions") != error.end())
                            this->operator()(error, exceptionPtrC);
                    } else {
                        std::cerr << "some error in the json: " << error.dump(4);
                    }
                }
            }
        } parser;
        parser(errors);
    }
} // namespace TB_NS::Error_NS
