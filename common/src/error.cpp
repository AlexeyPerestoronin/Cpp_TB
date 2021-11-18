// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../base_exception.hpp"

namespace TB_NS::Error_NS {
    Error::~Error() {
        for (const Error::Vp& errors : std::initializer_list<Error::Vp>{ subErrors, unregSubErrors })
            for (Error* error : errors)
                if (error)
                    delete error;
    }
    
    TB_NODISCARD bool Error::isEquivalent(Str::CR i_idOrKey) noexcept {
        return id == i_idOrKey || key == i_idOrKey;
    }

    TB_NODISCARD bool Error::operator=(Error::CR i_error) const noexcept {
        return id == i_error.id && key == i_error.key;
    }
} // namespace TB_NS::Error_NS
