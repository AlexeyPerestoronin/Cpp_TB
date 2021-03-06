#pragma once

#include <cppcoro/generator.hpp>

namespace TB_NS {
    namespace {
        template<typename T>
        concept ContainerType = requires(T t) {
            T::value_type;
            t.begin();
            t.end();
        };

        template<ContainerType Container>
        using index_value = std::pair<size_t, typename Container::value_type&>;

        template<ContainerType Container>
        using cindex_value = std::pair<size_t, const typename Container::value_type&>;
    } // namespace

    // brief: enumerates the all elements of the target container in like-python style
    // param: i_container - target container for enumerating of its elements
    template<ContainerType Container>
    cppcoro::generator<index_value<Container>> enumerator(Container& i_container) {
        for (size_t index{ 0 }; auto& value : i_container)
            co_yield { index++, value };
    }

    // brief: enumerates the all elements of the target container in like-python style
    // note1: working with constant data only
    // param: i_container - target container for enumerating of its elements
    template<ContainerType Container>
    cppcoro::generator<cindex_value<Container>> cenumerator(const Container& i_container) {
        for (size_t index{ 0 }; const auto& value : i_container)
            co_yield { index++, value };
    }
} // namespace TB_NS
