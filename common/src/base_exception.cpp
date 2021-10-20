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

    bool Exception::isSatisfy(Str::CRef i_IdOrKey) const noexcept {
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

        for (auto [suberror, offcet] = std::make_tuple(this, size_t{ 0 }); suberror; suberror = get_error_info<Suberror>(*suberror), ++offcet) {
            auto print = [&printer, &offcet](auto... i_message) { printer(offcet, i_message...); };
            print("----------");
            if (auto* locationPtr = get_error_info<Location>(*suberror); locationPtr)
                print("[location] ", *locationPtr);
            print("[id]", suberror->d_id);
            print("[key]", suberror->d_key);
            for (size_t counter{ 0 }; const auto& [key, value] : suberror->d_values)
                print(++counter, ") ", key, ": ", value);
            print("----------");
        }
        d_errorMessage = std::make_shared<Str>(message.str());
        return d_errorMessage->data();
    }

    Exception::CRef Exception::operator[](Str::CRef i_IdOrKey) const noexcept {
        const auto finder = [&i_IdOrKey](const auto& exceptions) -> auto {
            const auto comparator = [&i_IdOrKey](auto exceptionPtr) -> bool { return exceptionPtr->isSatisfy(i_IdOrKey); };
            if (const auto exception = std::ranges::find_if(exceptions, comparator); exception != exceptions.end())
                return *exception;
            else
                return decltype(*exception){};
        };

        // try to search through  the sub exceptions list
        if (auto exceptionPtr = finder(d_subException); exceptionPtr) {
            exceptionPtr->d_rootException = nullptr;
            return *exceptionPtr;
        }

        // try to search through  the sub exceptions list
        if (auto exceptionPtr = finder(Exceptions::d_allExceptions); exceptionPtr) {
            exceptionPtr->d_rootException = this;
            return *exceptionPtr;
        }

        // if no one known exceptions wasn't be founded then creates new exception, links it with current root-exception and returns
        auto& newException = Exceptions::RegistUnknowException(i_IdOrKey, i_IdOrKey);
        newException.d_rootException = this;
        return newException;
    }

#pragma endregion

#pragma region Exceptions

    std::list<Exception::SPtr> Exceptions::d_unregistedExceptions{};
    std::list<Exception::SPtr> Exceptions::d_allExceptions{ std::make_shared<Exception>() /* <--- this is the d_rootException */ };
    Exception::PtrC Exceptions::d_rootException = Exceptions::d_allExceptions.begin()->get();
    Exceptions::C Exceptions::Ins{};

    Exceptions::~Exceptions() {
        if (!d_unregistedExceptions.empty()) {
            std::cerr << "\n[UNREGISTED EXCEPTINOS][BEGIN]\n";
            for (const auto& exceptionSPtr : d_unregistedExceptions)
                std::cerr << exceptionSPtr->what();
            std::cerr << "\n[UNREGISTED EXCEPTINOS][END]\n";
        }
    }

    Exception::Ref Exceptions::RegistUnknowException(Str::CRef i_id, Str::CRef i_key, Exception::Values::CROpt i_values) {
        auto& exceptions = Exceptions::d_unregistedExceptions;
        exceptions.push_back(std::make_shared<Exception>(
            i_id, i_key,
            i_values.value_or(Exception::Values{
                { "unknown exception", "this exceptions cannot be recognized as registered" },
                { "possible action", "try to add exception by presented id/key inside target errof-settings-file" },
            })));
        return *exceptions.back().get();
    }

    void Exceptions::LoadSettings(Path::CRef i_settigsFilePath) {
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
                for (auto error : i_errors["exceptions"]) {
                    if (auto* exceptionPtrC = createNewException(error); exceptionPtrC) {
                        i_rootException->d_subException.push_back(exceptionPtrC);
                        exceptionPtrC->d_parentException = i_rootException;
                        if (error.find("exceptions") != error.end())
                            this->operator()(error, exceptionPtrC);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << error.dump(4) << "\n***\n";
                    }
                }
            }
        } parser;
        parser(errors, d_rootException);
    }

    const Exception& Exceptions::operator[](Str::CRef i_IdOrKey) const noexcept {
        const auto& exceptions = d_rootException->d_subException;
        const auto comparator = std::bind(&Exception::isSatisfy, std::placeholders::_1, i_IdOrKey);
        if (const auto exception = std::ranges::find_if(exceptions, std::move(comparator)); exception != exceptions.end())
            return *(*exception);
        return Exceptions::RegistUnknowException(i_IdOrKey, i_IdOrKey);
    }

#pragma endregion
} // namespace TB_NS::Error_NS
