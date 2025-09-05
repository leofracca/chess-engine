#pragma once

#include <print>

namespace chess_engine::debug
{
#ifdef DEBUG_BUILD
constexpr bool is_debug = true;
#else
constexpr bool is_debug = false;
#endif

template<typename... Args>
inline void debug_log(std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr (is_debug)
    {
        std::print(fmt, std::forward<Args>(args)...);
    }
}
} // namespace chess_engine::debug