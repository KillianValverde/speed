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
 * @file        speed/argparse/basic_key_arg.hpp
 * @brief       basic_key_arg class header.
 * @author      Killian Valverde
 * @date        2016/03/10
 */

#ifndef SPEED_ARGPARSE_BASIC_KEY_ARG_HPP
#define SPEED_ARGPARSE_BASIC_KEY_ARG_HPP

#include <string>
#include <utility>
#include <vector>

#include "arg_flags.hpp"
#include "basic_arg_key.hpp"
#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents arguments that have keys.
 */
template<typename TpAllocator>
class basic_key_arg : public virtual basic_base_arg<TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;

    /** Type that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       kys : Argument keys.
     */
    template<typename... Ts_>
    explicit basic_key_arg(arg_parser_type* arg_parsr, Ts_&&... kys)
            : base_arg_type(arg_parsr)
            , kys_()
            , sub_arg_parsr_(nullptr)
            , short_kys_len_(0)
            , long_kys_len_(0)
    {
        base_arg_type::set_flags(arg_flags::DEFAULT_KEY_ARG_FLAGS);

        int foreach[sizeof...(Ts_) + 1] = { (
                kys_.push_back({std::forward<Ts_>(kys), arg_parsr}), 0)... };

        if (kys_.empty())
        {
            throw no_key_specified_exception();
        }

        for (auto& ky : kys_)
        {
            if (ky.is_empty())
            {
                kys_.clear();
                throw no_key_specified_exception();
            }

            if (ky.is_prefix_long())
            {
                speed::safety::try_addml(&long_kys_len_, ky.get_string_length(), 2);
            }
            else
            {
                speed::safety::try_addml(&short_kys_len_, ky.get_string_length(), 2);
            }
        }

        base_arg_type::set_error_name(get_front_key());
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_arg(const basic_key_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_arg(basic_key_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_arg() override = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_arg& operator =(const basic_key_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_arg& operator =(basic_key_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            kys_ = std::move(rhs.kys_);
            std::swap(short_kys_len_, rhs.short_kys_len_);
            std::swap(long_kys_len_, rhs.long_kys_len_);
        }
    
        return *this;
    }

    /**
     * @brief       Parse the arg key arg sub parser.
     * @param       argc : Reference to the number of arguments found in the program call.
     * @param       argv : Arguments found in the program call.
     * @param       cur_idx : The current index checked in argv.
     * @param       pos_increment : How much the index will be increased afther the parsing.
     */
    template<typename TpArgc_, typename TpArgv_>
    void parse_sub_arg_parser(
            const TpArgc_& argc,
            const TpArgv_& argv,
            std::size_t cur_idx,
            std::size_t* pos_increment
    )
    {
        if (sub_arg_parsr_ != nullptr)
        {
            sub_arg_parsr_->parse_args(argc - cur_idx, &argv[cur_idx]);
            *pos_increment = argc - cur_idx;
        }
    }

    /**
     * @brief       Function to call when prefixes change in the argument parser in order to update
     *              the short and long keys total length.
     */
    void update_prefixes()
    {
        short_kys_len_ = 0;
        long_kys_len_ = 0;

        for (auto& ky : kys_)
        {
            ky.update_prefix_type();

            if (ky.is_prefix_long())
            {
                speed::safety::try_addml(&long_kys_len_, ky.get_string_length(), 2);
            }
            else
            {
                speed::safety::try_addml(&short_kys_len_, ky.get_string_length(), 2);
            }
        }
    }
    
    /**
     * @brief       Get first argument key.
     * @return      The first specified key is returned.
     */
    [[nodiscard]] const arg_key_type& get_front_key() const
    {
        if (kys_.empty())
        {
            throw key_not_found_exception();
        }
        
        return kys_.front();
    }

    /**
     * @brief       Get the number of keys.
     * @return      The number of keys.
     */
    [[nodiscard]] inline std::size_t get_keys_size() const noexcept
    {
        return kys_.size();
    }

    /**
     * @brief       Get the necessary length to print long arguments keys.
     * @return      The necessary length to print long arguments keys.
     */
    [[nodiscard]] inline std::size_t get_long_keys_length() noexcept override
    {
        if (base_arg_type::is_help_text_empty())
        {
            return 0;
        }

        return long_kys_len_;
    }

    /**
     * @brief       Get the necessary length to print short arguments keys.
     * @return      The necessary length to print short arguments keys.
     */
    [[nodiscard]] inline std::size_t get_short_keys_length() noexcept override
    {
        if (base_arg_type::is_help_text_empty())
        {
            return 0;
        }

        return short_kys_len_;
    }

    /**
     * @brief       Set a sub argument parser.
     * @param       sub_arg_parsr : Sub arguement parser.
     */
    inline void set_sub_arg_parser(arg_parser_type* sub_arg_parsr) noexcept
    {
        sub_arg_parsr_ = sub_arg_parsr;
    }

    /**
     * @brief       Allows knowing if the argument has any key with a long prefix.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline std::size_t has_long_prefix_keys() const noexcept
    {
        return long_kys_len_ > 0;
    }

    /**
     * @brief       Prin the usage.
     */
    void print_name() override
    {
        if (kys_.empty())
        {
            throw key_not_found_exception();
        }
        
        std::cout << kys_.front().get_string();
    }

    /**
     * @brief       Prin the usage.
     */
    void print_usage() override
    {
        if (kys_.empty())
        {
            throw key_not_found_exception();
        }
        
        std::cout << kys_.front().get_string();
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
        
        std::size_t current_ky_len = 0;
        std::size_t n_args_printd = 0;
        std::size_t i;
    
        for (i = args_indent; i > 0; --i)
        {
            std::cout << ' ';
        }

        print_keys(&n_args_printd, &current_ky_len, true);
        
        if (n_args_printd < kys_.size() && n_args_printd > 0)
        {
            std::cout << ", ";
            speed::safety::try_addm(&current_ky_len, 2);
        }
        
        if (current_ky_len < short_kys_len)
        {
            for (i = short_kys_len - current_ky_len; i > 0; --i)
            {
                std::cout << ' ';
            }
        }
        
        n_args_printd = 0;
        current_ky_len = 0;

        print_keys(&n_args_printd, &current_ky_len, false);
        
        if (current_ky_len < long_kys_len)
        {
            for (i = long_kys_len - current_ky_len; i > 0; --i)
            {
                std::cout << ' ';
            }
        }
    
        speed::safety::try_addml(&args_indent, short_kys_len, long_kys_len);
        speed::safety::try_addm(&new_line_indent, args_indent);
        
        base_arg_type::print_help_text(args_indent, max_line_len, new_line_indent);
    }

    /**
     * @brief       Print the keys selected.
     * @param       nr_args_printd :  Will hold the number of printed keys.
     * @param       cur_ky_len : Will hold the total length of the printed keys.
     * @param       just_print_short_kys : If it is true just short keys will be printed, otherwise
     *              just long keys will be printed.
     */
    void print_keys(
            std::size_t* nr_args_printd,
            std::size_t* cur_ky_len,
            bool just_print_short_kys
    )
    {
        for (auto& ky : kys_)
        {
            if (ky.is_prefix_long() ^ just_print_short_kys)
            {
                if (*nr_args_printd > 0)
                {
                    std::cout << ", " << ky.get_string();
                    speed::safety::try_addml(cur_ky_len, ky.get_string_length(), 2);
                }
                else
                {
                    std::cout << ky.get_string();
                    speed::safety::try_addm(cur_ky_len, ky.get_string_length());
                }
    
                speed::safety::try_addm(nr_args_printd, 1);
            }
        }
    }

private:
    /** Argument keys collection. */
    vector_type<arg_key_type> kys_;

    /** Reference to the argument parser that holds this object. */
    arg_parser_type* sub_arg_parsr_;

    /** The total short keys length. */
    std::size_t short_kys_len_;

    /** The total long keys length. */
    std::size_t long_kys_len_;
};


}


#endif
