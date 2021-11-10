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

        struct {
            Exceptions* exceptions;

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

            void operator()(json& i_errors, Error* i_rootErrorPtr = nullptr) {
                for (auto error : i_errors["exceptions"]) {
                    if (auto* errorPtr = createNewError(error); errorPtr) {
                        i_rootErrorPtr ? i_rootErrorPtr->subErrors.push_back(errorPtr) : exceptions->m_rootErrors.push_back(errorPtr);
                        if (error.find("exceptions") != error.end())
                            this->operator()(error, errorPtr);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << error.dump(4) << "\n***\n";
                    }
                }
            }
        } parser{ .exceptions = this };
        parser(errors);
    }

    TB_NODISCARD Exception Exceptions::operator[](Str::CR i_IdOrKey) const noexcept {
        for (Error* errorPtr : m_rootErrors)
            if (errorPtr->isEquivalent(i_IdOrKey))
                return Exception(*errorPtr);
        return Exception(PredefinedError_NS::UnregException)({ "{keyOrValue:}", i_IdOrKey });
    }
} // namespace TB_NS::Error_NS
