#ifndef RT_LOGGER_H
#define RT_LOGGER_H

#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

namespace powda
{

struct Logger
{
    enum Level
    {
        Debug,
        Info,
        Warn,
        Error
    };

#if POWDA_DEBUG
    static constexpr auto GLOBAL_DEFAULT = Level::Debug;
#else
    static constexpr auto GLOBAL_DEFAULT = Level::Info;
#endif // POWDA_DEBUG

    Logger(const std::string& category, Level lowest_enabled_level = GLOBAL_DEFAULT, std::ostream& ostream = std::cout);

    template <typename... Args> void log(Level level, std::string_view text, const Args&... args)
    {
        if (m_lowest_enabled_level > level)
        {
            return;
        }

        constexpr auto WHITE = "\033[0m";
        auto color = level_to_color(level);
        const auto level_text = level_to_text(level);
        std::string formatted_text = std::vformat(text, std::make_format_args(args...));
        namespace ch = std::chrono;
        auto current_time = ch::current_zone()->to_local(ch::floor<ch::milliseconds>(ch::system_clock::now()));
        m_stream << std::vformat("{:%T} {}[{}][{}]{} {}", std::make_format_args(current_time, color, level_text,
                                                                                m_category, WHITE, formatted_text))
        << std::endl;
    }

    template <typename... Args> void debug(std::string_view text, const Args... args)
    {
        return log(Level::Debug, text, args...);
    }
    template <typename... Args> void info(std::string_view text, const Args... args)
    {
        return log(Level::Info, text, args...);
    }
    template <typename... Args> void warn(std::string_view text, const Args... args)
    {
        return log(Level::Warn, text, args...);
    }
    template <typename... Args> void error(std::string_view text, const Args... args)
    {
        return log(Level::Error, text, args...);
    }

    void add_subcategory(const std::string& category) { m_category += "|" + category; }

private:
    Level m_lowest_enabled_level;
    static const char* level_to_text(Level level);
    static const char* level_to_color(Level level);

    std::string m_category;
    std::ostream& m_stream;
};

} // namespace powda

#endif // POWDA_LOGGER_H
