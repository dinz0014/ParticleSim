#pragma once
#include <ranges>

template <typename Range>
auto non_null_view(Range&& range)
{
    return range
           | std::views::filter([](const auto& p) { return p != nullptr; });
}

template <typename Range>
auto deref_non_null_view(Range&& range)
{
    return non_null_view(range)
           | std::views::transform([](const auto& p) -> decltype(auto) { return *p; });
}