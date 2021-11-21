// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <common/base_exception.hpp>
#include <common/json.hpp>
#include <common/rai.hpp>

namespace TB_NS::Error_NS {
    Exceptions& Exceptions::GetIns() {
        static Exceptions::UP instance{ new Exceptions() };
        return *instance;
    }

    Exceptions::~Exceptions() {
        for (Error* errorPtr : m_rootErrors)
            if (errorPtr)
                delete errorPtr;
    }

    void Exceptions::LoadSettings(Path::CR i_settigsFilePath) {
        Json errors;
        if (auto settingsFile = RAI<std::ifstream>(i_settigsFilePath); settingsFile.is_open())
            try {
                settingsFile >> errors;
            } catch (std::exception& error) {
                throw Exception(PredefinedError_NS::SettingsFileCannotBeParsed)({ "{file_name:}", i_settigsFilePath.string() })("{error:}", error.what());
            }
        else
            throw Exception(PredefinedError_NS::SettingsFileNotLoading)({ "{file_name:}", i_settigsFilePath.string() });

        class Parser {
            Error::Values m_reductions{};
            Exceptions& m_exceptions;
            Json::CR m_errors;

            Error* createNewError(Json::CR i_error) {
                try {
                    Error* newError = new Error{ .id = i_error["id"], .key = i_error["key"] };
                    if (auto values = i_error.find("values"); values != i_error.end()) {
                        Error::Values val{};
                        for (const auto& [key, value] : values->items())
                            val[key] = static_cast<Str>(value).format(m_reductions);
                        newError->values = std::move(val);
                    }
                    return newError;
                } catch (...) {
                    return nullptr;
                }
            }

            void parse(Json::CR i_errors, Error* i_rootErrorPtr) {
                for (const auto& exceptions : i_errors["exceptions"]) {
                    if (auto* errorPtr = createNewError(exceptions); errorPtr) {
                        i_rootErrorPtr ? i_rootErrorPtr->subErrors.push_back(errorPtr) : m_exceptions.m_rootErrors.push_back(errorPtr);
                        if (exceptions.find("exceptions") != exceptions.end())
                            parse(exceptions, errorPtr);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << exceptions.dump(4) << "\n***\n";
                    }
                }
            }

            public:
            Parser(Exceptions& i_exceptions, Json::CR i_errors)
                : m_exceptions(i_exceptions)
                , m_errors(i_errors) {
                for (auto reduction : m_errors["reductions"]) {
                    Str::O key{}, value{};
                    if (auto k = reduction.find("key"); k != reduction.end())
                        key = static_cast<Str::BaseType>(*k);
                    if (auto v = reduction.find("value"); v != reduction.end())
                        value = static_cast<Str::BaseType>(*v);
                    if (key && value)
                        m_reductions[std::move(key.value())] = std::move(value.value());
                }
            }

            void parse() {
                parse(m_errors, nullptr);
            }
        };
        Parser parser{ *this, errors };
        parser.parse();
    }

    TB_NODISCARD Exception Exceptions::operator[](Str::CR i_IdOrKey) const noexcept {
        for (Error* errorPtr : m_rootErrors)
            if (errorPtr->isEquivalent(i_IdOrKey))
                return Exception(*errorPtr);
        return Exception(PredefinedError_NS::UnregException)({ "{keyOrValue:}", i_IdOrKey });
    }
} // namespace TB_NS::Error_NS
