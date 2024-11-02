/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file        speed/iostream/operations.hpp
 * @brief       operations functions header.
 * @author      Killian Valverde
 * @date        2016/08/24
 */

#ifndef SPEED_IOSTREAM_OPERATIONS_HPP
#define SPEED_IOSTREAM_OPERATIONS_HPP

#include <iostream>

#include "../safety/safety.hpp"
#include "../system/system.hpp"


namespace speed::iostream {


/**
 * @brief       Clears the buffers of the given stream. For output streams this discards any
 *              unwritten output. For input streams this discards any input read from the
 *              underlying object but not yet obtained via getc(3); this includes any text pushed
 *              back via ungetc(3).
 * @param       fp : Pointer to the FILE structure to purge.
 */
void fpurge(::FILE* fp) noexcept;


/**
 * @brief       Get the current 'std::basic_ostream' object used to print TpChar in standard output.
 * @return      The current 'std::basic_ostream' object used to print TpChar in standard output.
 */
template<typename TpChar>
inline std::basic_ostream<TpChar>& get_cout() noexcept;


/**
 * @brief       Get the current 'std::ostream' object used to print in standard output.
 * @return      The current 'std::ostream' object used to print in standard output.
 */
template<>
inline std::ostream& get_cout<char>() noexcept
{
    return std::cout;
}


/**
 * @brief       Get the current 'std::wostream' object used to print in standard output.
 * @return      The current 'std::wostream' object used to print in standard output.
 */
template<>
inline std::wostream& get_cout<wchar_t>() noexcept
{
    return std::wcout;
}


/**
 * @brief       Inserts a new-line character in the stream.
 * @param       os : Output stream object effected.
 * @return      Argument os.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& newl(std::basic_ostream<TpChar, TpTraits>& os)
{
    return os.put(os.widen('\n'));
}


/**
 * @brief       Writes the C string pointed by format to the standard output (stdout). If format
 *              includes format specifiers (subsequences beginning with %), the additional arguments
 *              following format are formatted and inserted in the resulting string replacing their
 *              respective specifiers.
 * @param       formt : C string that contains the text to be written to stdout. It can optionally
 *              contain embedded format specifiers that are replaced by the values specified in
 *              subsequent additional arguments and formatted as requested.
 * @return      On success, the total number of characters written is returned. If a writing error
 *              occurs, the error indicator (ferror) is set and a negative number is returned. If a
 *              multibyte character encoding error occurs while writing wide characters, errno is
 *              set to EILSEQ and a negative number is returned.
 */
int printf(const char* formt, ...) noexcept;


/**
 * @brief       Writes the C string pointed by format to the standard output (stdout). If format
 *              includes format specifiers (subsequences beginning with %), the additional arguments
 *              following format are formatted and inserted in the resulting string replacing their
 *              respective specifiers.
 * @param       formt : C string that contains the text to be written to stdout. It can optionally
 *              contain embedded format specifiers that are replaced by the values specified in
 *              subsequent additional arguments and formatted as requested.
 * @return      On success, the total number of characters written is returned. If a writing error
 *              occurs, the error indicator (ferror) is set and a negative number is returned. If a
 *              multibyte character encoding error occurs while writing wide characters, errno is
 *              set to EILSEQ and a negative number is returned.
 */
int printf(const wchar_t* formt, ...) noexcept;


/**
 * @brief       Print a string wrapping if necessary.
 * @param       os : Ostrem used to print.
 * @param       txt : Text to print.
 * @param       max_line_len : The maximum line length that will be printed.
 * @param       new_line_indent : The indentation used after a newline is printed.
 * @param       current_line_len : The current length of the current line.
 * @return
 */
template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
std::basic_ostream<TpChar, TpCharTraits>& print_wrapped(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& txt,
        std::size_t max_line_len,
        std::size_t new_line_indent,
        std::size_t current_line_len = 0
)
{
    using string_type = std::basic_string<TpChar, TpCharTraits, TpCharAlloc>;

    typename string_type::const_iterator str_it;
    typename string_type::const_iterator aux_str_it;
    std::size_t len_to_next;

    for (str_it = txt.cbegin(); str_it != txt.cend(); ++str_it)
    {
        if (*str_it == '\n')
        {
            os << '\n';
            for (std::size_t i = 0; i < new_line_indent; ++i)
            {
                os << ' ';
            }
            current_line_len = new_line_indent;
        }
        else if (*str_it == ' ')
        {
            aux_str_it = str_it;
            len_to_next = 0;

            do
            {
                ++aux_str_it;
                ++len_to_next;
            } while (aux_str_it != txt.cend() && *aux_str_it != ' ');

            speed::safety::try_addm(&len_to_next, current_line_len);
            if (len_to_next > max_line_len)
            {
                os << '\n';
                for (std::size_t i = 0; i < new_line_indent; ++i)
                {
                    os << ' ';
                }
                current_line_len = new_line_indent;
            }
            else
            {
                os << ' ';
                ++current_line_len;
            }
        }
        else
        {
            os << *str_it;
            ++current_line_len;
        }
    }

    return os;
}


/**
 * @brief       Set the default text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_default_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::DEFAULT);
    return os;
}


/**
 * @brief       Set black text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_black_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::BLACK);
    return os;
}


/**
 * @brief       Set red text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_red_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::RED);
    return os;
}


/**
 * @brief       Set green text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_green_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::GREEN);
    return os;
}


/**
 * @brief       Set brown text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_brown_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::BROWN);
    return os;
}


/**
 * @brief       Set blue text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_blue_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::BLUE);
    return os;
}


/**
 * @brief       Set purple text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_purple_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::PURPLE);
    return os;
}


/**
 * @brief       Set cyan text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_cyan_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::CYAN);
    return os;
}


/**
 * @brief       Set light gray text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_gray_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::LIGHT_GRAY);
    return os;
}


/**
 * @brief       Set dark gray text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_dark_gray_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::GRAY);
    return os;
}


/**
 * @brief       Set light red text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_red_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::LIGHT_RED);
    return os;
}


/**
 * @brief       Set light green text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_green_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::LIGHT_GREEN);
    return os;
}


/**
 * @brief       Set yellow text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_yellow_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::YELLOW);
    return os;
}


/**
 * @brief       Set light blue text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_blue_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::LIGHT_BLUE);
    return os;
}


/**
 * @brief       Set light purple text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_purple_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::LIGHT_PURPLE);
    return os;
}


/**
 * @brief       Set light cyan text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_cyan_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::LIGHT_CYAN);
    return os;
}


/**
 * @brief       Set white text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_white_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_foreground_text_attribute(
            stdout, speed::system::terminal::text_attribute::WHITE);
    return os;
}


}


#endif
