// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

namespace TB_NS::States_NS {
    // brief: the states collection in which objects-type derived from Status<>-class could be
    enum class States : uint8_t {
        MANGLED,
        DEFAULT,
    };

    // brief: presents target state in which a derived object could be
    template<States state>
    class Status {
        bool m_isInStatus{ false };

        public:
        Status() = default;
        Status(States i_state) {
            if (i_state == state)
                m_isInStatus = true;
        }

        void reset() {
            m_isInStatus = false;
        }

        bool status() const {
            return m_isInStatus;
        }
    };

    using DefaultState = Status<States::DEFAULT>;
    using MangledState = Status<States::MANGLED>;
} // namespace TB_NS::States_NS