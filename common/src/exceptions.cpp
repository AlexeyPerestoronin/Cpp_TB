// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <common/base_exception.hpp>
#include <common/rai.hpp>

#include <nlohmann/json.hpp>

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
        using namespace nlohmann;

        json errors;
        if (auto settingsFile = RAI<std::ifstream>(i_settigsFilePath); settingsFile.is_open())
            settingsFile >> errors;
        else
            throw Exception(PredefinedError_NS::SettingsFileNotLoading)({ "{file_name:}", i_settigsFilePath.string() });

        class Parser {
            Error::Values m_reductinos{};
            Exceptions& m_exceptions;
            json& m_errors;

            public:
            Parser(Exceptions& i_exceptions, json& i_errors)
                : m_exceptions(i_exceptions)
                , m_errors(i_errors) {
                for (auto reduction : m_errors["reductions"]) {
                    Str::O key{}, value{};
                    if (auto k = reduction.find("key"); k != reduction.end())
                        key = static_cast<Str::BaseType>(*k);
                    if (auto v = reduction.find("value"); v != reduction.end())
                        value = static_cast<Str::BaseType>(*v);
                    if (key && value)
                        m_reductinos[std::move(key.value())] = std::move(value.value());
                }
            }

            Error* createNewError(json& i_error) {
                try {
                    Error* newError = new Error{ .id = i_error["id"], .key = i_error["key"] };
                    if (auto values = i_error.find("values"); values != i_error.end()) {
                        Error::Values val{};
                        for (auto [key, value] : values->items())
                            val[static_cast<Str::BaseType>(key)] = static_cast<Str::BaseType>(value);
                        newError->values = std::move(val);
                    }
                    return newError;
                } catch (...) {
                    return nullptr;
                }
            }

            void parse(Error* i_rootErrorPtr = nullptr) {
                for (auto error : m_errors["exceptions"]) {
                    if (auto* errorPtr = createNewError(error); errorPtr) {
                        i_rootErrorPtr ? i_rootErrorPtr->subErrors.push_back(errorPtr) : m_exceptions.m_rootErrors.push_back(errorPtr);
                        if (error.find("exceptions") != error.end())
                            parse(errorPtr);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << error.dump(4) << "\n***\n";
                    }
                }
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
