// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <common/base_exception.hpp>
#include <common/rai.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS::Error_NS {
    namespace {
        // brief: list of all Exceptions of the instance of the program
        std::list<Exception::SP> _Exceptions{};
    }; // namespace

#pragma region Exception

    Exception::Exception(Str i_id, Str i_key, Values i_values) noexcept
        : d_id(std::move(i_id))
        , d_key(std::move(i_key))
        , d_values(std::move(i_values)) {}

    bool Exception::isSatisfy(Str::CR i_IdOrKey) const noexcept {
        return d_id == i_IdOrKey || d_key == i_IdOrKey;
    }

    const char* Exception::what() const {
        using namespace boost;

        std::stringstream message;

        auto printer = [&message](size_t offset, auto... i_message) {
            message << Str(offset, ' ');
            ((message << i_message), ...);
            message << std::endl;
        };

        for (auto [suberror, offcet] = std::make_tuple(this, size_t{ 0 }); suberror; suberror = get_error_info<Suberror>(*suberror), offcet+=3) {
            auto print = [&printer, &offcet](auto... i_message) { printer(offcet, i_message...); };
            if (auto* locationP = get_error_info<Location>(*suberror); locationP)
                print("location: ", *locationP);
            print("id: ", suberror->d_id);
            print("key: ", suberror->d_key);
            for (size_t counter{ 0 }; const auto& [key, value] : suberror->d_values)
                print(++counter, ") ", key, ": ", value);
            print("suberror:");
        }
        d_errorMessage = std::make_shared<Str>(message.str());
        return d_errorMessage->data();
    }

    Exception::CR Exception::operator[](Str::CR i_IdOrKey) const noexcept {
        const auto finder = [&i_IdOrKey](const auto& exceptions) -> auto {
            const auto comparator = [&i_IdOrKey](auto exceptionP) -> bool { return exceptionP->isSatisfy(i_IdOrKey); };
            if (const auto exception = std::ranges::find_if(exceptions, comparator); exception != exceptions.end())
                return *exception;
            else
                return decltype(*exception){};
        };

        // try to search through  the sub exceptions list
        if (auto exceptionP = finder(d_subException); exceptionP) {
            exceptionP->d_rootException = nullptr;
            return *exceptionP;
        }

        // try to search through  the sub exceptions list
        if (auto exceptionP = finder(Exceptions::d_allExceptions); exceptionP) {
            exceptionP->d_rootException = this;
            return *exceptionP;
        }

        // if no one known exceptions wasn't be founded then creates new exception, links it with current root-exception and returns
        auto& newException = Exceptions::RegistUnknowException(i_IdOrKey, i_IdOrKey);
        newException.d_rootException = this;
        return newException;
    }

#pragma endregion

#pragma region Exceptions

    std::list<Exception::SP> Exceptions::d_unregistedExceptions{};
    std::list<Exception::SP> Exceptions::d_allExceptions{ std::make_shared<Exception>() /* <--- this is the d_rootException */ };
    Exception::PC Exceptions::d_rootException = Exceptions::d_allExceptions.begin()->get();
    Exceptions::C Exceptions::Ins{};

    Exceptions::~Exceptions() {
        if (!d_unregistedExceptions.empty()) {
            std::function<void(Str::CR)> logger = [](auto message) { std::cerr << message; };

            RAI<std::fstream> fileLogger("unregisted-error.txt", std::ios::app | std::ios::in);
            if (fileLogger.is_open()) {
                logger = [&fileLogger](const auto& message) {
                    fileLogger << message;
                    std::cerr << message;
                };
            }

            logger("\n[UNREGISTED EXCEPTINOS][BEGIN]\n");
            for (const auto& exceptionSP : d_unregistedExceptions)
                logger(exceptionSP->what());
            logger("\n[UNREGISTED EXCEPTINOS][END]\n");
        }
    }

    Exception::R Exceptions::RegistUnknowException(Str::CR i_id, Str::CR i_key, Exception::Values::CRO i_values) {
        auto& exceptions = Exceptions::d_unregistedExceptions;
        exceptions.push_back(std::make_shared<Exception>(
            i_id, i_key,
            i_values.value_or(Exception::Values{
                { "unknown exception", "this exceptions cannot be recognized as registered" },
                { "possible action", "try to add exception by presented id/key inside target errof-settings-file" },
            })));
        return *exceptions.back().get();
    }

    void Exceptions::LoadSettings(Path::CR i_settigsFilePath) {
        using namespace nlohmann;

        json errors;
        RAI<std::ifstream>(i_settigsFilePath) >> errors;

        struct {
            Exception::PC createNewException(json& i_error) {
                Exception::P result{};
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

            void operator()(json& i_errors, Exception::PC i_rootException) {
                for (auto error : i_errors["exceptions"]) {
                    if (auto* exceptionPC = createNewException(error); exceptionPC) {
                        i_rootException->d_subException.push_back(exceptionPC);
                        exceptionPC->d_parentException = i_rootException;
                        if (error.find("exceptions") != error.end())
                            this->operator()(error, exceptionPC);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << error.dump(4) << "\n***\n";
                    }
                }
            }
        } parser;
        parser(errors, d_rootException);
    }

    const Exception& Exceptions::operator[](Str::CR i_IdOrKey) const noexcept {
        const auto& exceptions = d_rootException->d_subException;
        const auto comparator = std::bind(&Exception::isSatisfy, std::placeholders::_1, i_IdOrKey);
        if (const auto exception = std::ranges::find_if(exceptions, std::move(comparator)); exception != exceptions.end())
            return *(*exception);
        return Exceptions::RegistUnknowException(i_IdOrKey, i_IdOrKey);
    }

#pragma endregion
} // namespace TB_NS::Error_NS
