// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

namespace TB_NS {
    // brief: the Singleton-pattern implementation
    struct Singleton {
        Singleton() noexcept = default;
        Singleton(Singleton&&) noexcept = default;
        Singleton& operator=(Singleton&&) noexcept = default;

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
    };
} // namespace TB_NS
