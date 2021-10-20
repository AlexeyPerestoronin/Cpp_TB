#include <common/base_exception.hpp>
#include <common/rai.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS::Error_NS {
    namespace {
        // brief: list of all Exceptions of the instance of the program
        std::list<Exception::SPtr> _Exceptions{};
    }; // namespace

#pragma region Exception

    Exception::Exception(Str i_id, Str i_key, Values i_values) noexcept
        : d_id(std::move(i_id))
        , d_key(std::move(i_key))
        , d_values(std::move(i_values)) {}

    bool Exception::isSatisfy(StrView i_IdOrKey) const noexcept {
        return d_id == i_IdOrKey || d_key == i_IdOrKey;
    }

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
        d_errorMessage = std::make_shared<Str>(message.str());
        return d_errorMessage->data();
    }

    const Exception& Exception::operator[](StrView i_IdOrKey) const noexcept {
        const auto comparator = [&i_IdOrKey](auto exceptionPtr) -> bool { return exceptionPtr->isSatisfy(i_IdOrKey); };

        { // try to search through  the sub exceptions list
            const auto& exceptions = d_subException;
            if (const auto exception = std::ranges::find_if(exceptions, comparator); exception != exceptions.end()) {
                (*exception)->d_rootException = nullptr;
                return *(*exception);
            }
        }

        { // try to search through  the sub exceptions list
            const auto& exceptions = Exceptions::d_allExceptions;
            if (const auto exception = std::ranges::find_if(exceptions, comparator); exception != exceptions.end()) {
                (*exception)->d_rootException = this;
                return *(*exception);
            }
        }

        { // if no one known exceptions wasn't be founded then creates new exception, links it with current root-exception and returns
            auto& exceptions = Exceptions::d_allExceptions;
            exceptions.push_back(std::make_shared<Exception>(
                Str{ i_IdOrKey }, Str{ i_IdOrKey }, Exception::Values{ { "unknown exception", "this exceptions cannot be recognized as registered" } }));
            auto* newException = exceptions.back().get();
            newException->d_rootException = this;
            return *newException;
        }
    }

#pragma endregion

#pragma region Exceptions

    std::list<Exception::SPtr> Exceptions::d_allExceptions{ std::make_shared<Exception>() /*d_rootException*/ };
    Exception::PtrC Exceptions::d_rootException = Exceptions::d_allExceptions.begin()->get();
    Exceptions Exceptions::Ins{};

    void Exceptions::LoadSettings(const fs::path& i_settigsFilePath) {
        using namespace nlohmann;

        json errors;
        RAI<std::ifstream>(i_settigsFilePath) >> errors;

        struct {
            Exception::PtrC createNewException(json& i_error) {
                Exception::Ptr result{};
                try {
                    auto exception = std::make_shared<Exception>(Str::BaseType(i_error["id"]), Str::BaseType(i_error["key"]));
                    result = exception.get();
                    Exceptions::d_allExceptions.push_back(std::move(exception));
                    if (auto values = i_error.find("values"); values != i_error.end())
                        for (auto [key, value] : values->items())
                            result->d_values.emplace(key, value);
                } catch (...) {
                }
                return result;
            }

            void operator()(json& i_errors, Exception::PtrC i_rootException) {
                for (auto error : i_errors["_Exceptions"]) {
                    if (auto* exceptionPtrC = createNewException(error); exceptionPtrC) {
                        i_rootException->d_subException.push_back(exceptionPtrC);
                        exceptionPtrC->d_parentException = i_rootException;
                        if (error.find("_Exceptions") != error.end())
                            this->operator()(error, exceptionPtrC);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << error.dump(4) << "\n***\n";
                    }
                }
            }
        } parser;
        parser(errors, d_rootException);
    }

    const Exception& Exceptions::operator[](StrView i_IdOrKey) const noexcept {
        const auto& exceptions = d_rootException->d_subException;
        const auto comparator = std::bind(&Exception::isSatisfy, std::placeholders::_1, i_IdOrKey);
        if (const auto exception = std::ranges::find_if(exceptions, std::move(comparator)); exception != exceptions.end())
            return *(*exception);
        return *d_rootException;
    }

#pragma endregion
} // namespace TB_NS::Error_NS
