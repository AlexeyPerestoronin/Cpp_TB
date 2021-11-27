// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <gtest/gtest.h>
#include <common/common.hpp>

namespace TB_NS::UnitTests_NS {
    // brief: auxiliary class for more agile testings constructing
    class Error {
        bool m_isSuccess{ true };

        public:
        template<class... Args>
        TB_NODISCARD Str operator()(Args&&... i_args) {
            m_isSuccess = false;
            std::stringstream stream;
            ((stream << i_args), ...);
            return stream.str();
        }

        TB_NODISCARD bool result() const noexcept {
            return m_isSuccess;
        }
    };
} // namespace TB_NS::UnitTests_NS
