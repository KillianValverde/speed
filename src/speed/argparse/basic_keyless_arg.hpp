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
 * @file        speed/argparse/basic_keyless_arg.hpp
 * @brief       basic_keyless_arg class header.
 * @author      Killian Valverde
 * @date        2016/03/11
 */

#ifndef SPEED_ARGPARSE_BASIC_KEYLESS_ARG_HPP
#define SPEED_ARGPARSE_BASIC_KEYLESS_ARG_HPP

#include <memory>
#include <string>

#include "arg_flags.hpp"
#include "basic_arg_parser.hpp"
#include "basic_arg_value.hpp"
#include "basic_base_arg.hpp"
#include "basic_value_arg.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents arguments without keys.
 */
template<typename TpAllocator>
class basic_keyless_arg : public basic_value_arg<TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Class that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       usage_ky : Key used in the usage message.
     */
    template<typename TpString_>
    basic_keyless_arg(arg_parser_type* arg_parsr, TpString_&& usage_ky)
            : base_arg_type(arg_parsr)
            , value_arg_type(arg_parsr)
            , usage_ky_(std::forward<TpString_>(usage_ky))
    {
        base_arg_type::clear_flags();
        base_arg_type::set_flags(arg_flags::DEFAULT_KEYLESS_ARG_FLAGS);

        if (usage_ky_.empty())
        {
            throw no_key_specified_exception();
        }

        base_arg_type::set_error_name(usage_ky_);
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_keyless_arg(const basic_keyless_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_keyless_arg(basic_keyless_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_keyless_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_keyless_arg& operator =(const basic_keyless_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_keyless_arg& operator =(basic_keyless_arg&& rhs) noexcept = default;

    /**
     * @brief       Only used for polymorphic propose.
     * @return      0 is returned always.
     */
    [[nodiscard]] inline std::size_t get_long_keys_length() noexcept override
    {
        return 0;
    }

    /**
     * @brief       Get the necessary length to print the usage key.
     * @return      The necessary length to print the usage key.
     */
    [[nodiscard]] inline std::size_t get_short_keys_length() noexcept override
    {
        if (base_arg_type::is_help_text_empty())
        {
            return 0;
        }

        return speed::safety::addm(usage_ky_.length(), 2);
    }

    /**
     * @brief       Get a string that represents the kind of argument it is.
     * @return      A string that represents the kind of argument it is.
     */
    [[nodiscard]] inline string_type get_tittle() const override
    {
        return "Value";
    }

    /**
     * @brief       Prin the usage.
     */
    void print_name() override
    {
        std::cout << usage_ky_;
    }
    
    /**
     * @brief       Print the argument usage ID in standard output for usage sentence.
     */
    void print_usage() override
    {
        if (base_arg_type::is_flag_set(arg_flags::MANDATORY))
        {
            std::cout << usage_ky_;
        }
        else
        {
            std::cout << "[" << usage_ky_ << "]";
        }
        
        if (value_arg_type::get_max_values() > 1)
        {
            std::cout << "...";
        }
    }
    
    /**
     * @brief       Print the argument help text.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum line length that will be printed.
     * @param       new_line_indent : The indentation used after a newline is printed.
     * @param       short_kys_len : The maximum short keys length.
     * @param       long_kys_len : The maximum long keys length.
     */
    void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_kys_len,
            std::size_t long_kys_len
    ) override
    {
        if (base_arg_type::is_help_text_empty())
        {
            return;
        }
        
        std::size_t current_id_len = speed::safety::addm(usage_ky_.length(), 2);
        std::size_t total_id_len = speed::safety::addm(short_kys_len, long_kys_len);
        std::size_t i;
    
        for (i = args_indent; i > 0; i--)
        {
            std::cout << ' ';
        }
    
        std::cout << usage_ky_ << "  ";
        
        if (current_id_len < total_id_len)
        {
            for (i = total_id_len - current_id_len; i > 0; i--)
            {
                std::cout << ' ';
            }
        }
    
        speed::safety::try_addm(&args_indent, total_id_len);
        speed::safety::try_addm(&new_line_indent, args_indent);
        
        base_arg_type::print_help_text(args_indent, max_line_len, new_line_indent);
    }

private:
    /** The id used to reference a value argument in the help menu. */
    string_type usage_ky_;
};


}


#endif
