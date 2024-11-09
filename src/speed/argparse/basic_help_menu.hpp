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
 *  @file       speed/argparse/basic_help_menu.hpp
 *  @brief      basic_help_menu class header.
 *  @author     Killian Valverde
 *  @date       2024/04/16
 */

#ifndef SPEED_ARGPARSE_BASIC_HELP_MENU_HPP
#define SPEED_ARGPARSE_BASIC_HELP_MENU_HPP

#include <memory>
#include <string>
#include <vector>

#include "../containers/containers.hpp"
#include "basic_arg_constraint.hpp"
#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_keyless_arg.hpp"
#include "basic_value_arg.hpp"
#include "basic_version_arg.hpp"
#include "forward_declarations.hpp"
#include "help_menu_flags.hpp"


namespace speed::argparse {


/**
 * @brief       Class used to represent a help menu.
 */
template<typename TpAllocator>
class basic_help_menu
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

    /** Flags type used in the class. */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;
    
    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Type that represents the argument to get the version information. */
    using version_arg_type = basic_version_arg<TpAllocator>;

    /** Type that represents arguments without keys. */
    using keyless_arg_type = basic_keyless_arg<TpAllocator>;
    
    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<TpAllocator>;

    /** Type that represents the argument to get the help information. */
    using help_arg_type = basic_help_arg<TpAllocator>;
    
    /** Type that represents a constraint for a set of arguments. */
    using arg_dependency_type = basic_arg_constraint<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    explicit basic_help_menu(arg_parser_type* arg_parsr)
            : desc_()
            , epilg_()
            , ky_args_()
            , kyless_args_()
            , arg_parsr_(arg_parsr)
            , args_indent_(2)
            , max_line_len_(80)
            , new_line_indent_(2)
            , max_short_kys_len_(0)
            , max_long_kys_len_(0)
            , flgs_(help_menu_flags::DEFAULT)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_menu(const basic_help_menu& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_menu(basic_help_menu&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_menu() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_menu& operator =(const basic_help_menu& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_menu& operator =(basic_help_menu&& rhs) noexcept = default;

    /**
     * @brief       Add an argument into the help menu.
     * @param       bse_arg : Argument to add.
     */
    void add_entry(base_arg_type* bse_arg)
    {
        auto* kyless_arg = dynamic_cast<keyless_arg_type*>(bse_arg);
        if (kyless_arg != nullptr)
        {
            kyless_args_.push_back(kyless_arg);
        }
        else
        {
            ky_args_.push_back(dynamic_cast<key_arg_type*>(bse_arg));
        }
    }

    /**
     * @brief       Remove the specified argument from the help menu.
     * @param       bse_arg : Argument to remove.
     */
    void remove_entry(base_arg_type* bse_arg)
    {
        auto* kyless_arg = dynamic_cast<keyless_arg_type*>(bse_arg);
        if (kyless_arg != nullptr)
        {
            for (auto it = kyless_args_.begin(); it != kyless_args_.end(); ++it)
            {
                if (kyless_arg == *it)
                {
                    kyless_args_.erase(it);
                    break;
                }
            }
        }
        else
        {
            for (auto it = ky_args_.begin(); it != ky_args_.end(); ++it)
            {
                if (dynamic_cast<key_arg_type*>(bse_arg) == *it)
                {
                    ky_args_.erase(it);
                    break;
                }
            }
        }
    }

    /**
     * @brief       Update the maximum length needed to print the keys in the help menu.
     */
    void update_max_keys_length() noexcept
    {
        for (auto& ky_arg : ky_args_)
        {
            if ((ky_arg->is_option() && flgs_.is_set(help_menu_flags::PRINT_OPTIONS)) ||
                (!ky_arg->is_option() && flgs_.is_set(help_menu_flags::PRINT_COMMANDS)) &&
                !ky_arg->is_help_text_empty())
            {
                update_max_keys_length_from_key_arg(ky_arg);
            }
        }

        for (auto& kyless_arg : kyless_args_)
        {
            if (flgs_.is_set(help_menu_flags::PRINT_VALUES) && !kyless_arg->is_help_text_empty())
            {
                update_max_keys_length_from_keyless_arg(kyless_arg);
            }
        }
    }

    /**
     * @brief       Update the maximum keys length fron a key argument.
     * @param       ky_arg : Key argument to check.
     */
    void update_max_keys_length_from_key_arg(key_arg_type* ky_arg) noexcept
    {
        if (max_short_kys_len_ < ky_arg->get_short_keys_length())
        {
            max_short_kys_len_ = ky_arg->get_short_keys_length();
        }
        if (max_long_kys_len_ < ky_arg->get_long_keys_length())
        {
            max_long_kys_len_ = ky_arg->get_long_keys_length();
        }
    }

    /**
     * @brief       Update the maximum keys length fron a keyless argument.
     * @param       kyless_arg : Keyless argument to check.
     */
    void update_max_keys_length_from_keyless_arg(keyless_arg_type* kyless_arg) noexcept
    {
        std::size_t totl = max_short_kys_len_ + max_long_kys_len_;

        if (kyless_arg->get_short_keys_length() > totl)
        {
            max_long_kys_len_ =  kyless_arg->get_short_keys_length() - max_short_kys_len_;
        }
    }

    /**
     * @brief       Set the arguments indentation during the print.
     * @param       arg_desc_indentation : Arguments indentation during the print.
     */
    void set_args_indentation(std::size_t arg_desc_indentation)
    {
        args_indent_ = arg_desc_indentation;
    }

    /**
     * @brief       Set an epilog.
     * @param       epilg : The epilog to set.
     */
    template<typename TpString_>
    void set_epilog(TpString_&& epilg)
    {
        epilg_ = std::forward<TpString_>(epilg);
    }

    /**
     * @brief       Set a description.
     * @param       desc :  Description to set.
     */
    template<typename TpString_>
    void set_description(TpString_&& desc)
    {
        desc_ = std::forward<TpString_>(desc);
    }

    /**
     * @brief       Set the indentation after printing new lines.
     * @param       desc_new_line_indentation : The indentation after printing new lines.
     */
    void set_new_line_indentation(std::size_t desc_new_line_indentation)
    {
        new_line_indent_ = desc_new_line_indentation;
    }

    /**
     * @brief       Set a flad in the help menu.
     * @param       flg : Flag to set.
     */
    inline void set_flag(help_menu_flags flg) noexcept
    {
        flgs_.set(flg);
    }

    /**
     * @brief       Set the maximum lenght for a line.
     * @param       max_desc_line_length : The maximum length for a line.
     */
    void set_maximum_line_length(std::size_t max_desc_line_length)
    {
        max_line_len_ = max_desc_line_length;
    }

    /**
     * @brief       Set a flad in the help menu.
     * @param       flg : Flag to set.
     */
    inline basic_help_menu& unset_flag(help_menu_flags flg) noexcept
    {
        flgs_.unset(flg);
        return *this;
    }

    /**
     * @brief       Allows knowing whether an argument flag is set.
     * @param       flg : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_flag_set(help_menu_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }

    // TODO: Apply wrapping to the usage message.
    /**
     * @brief       Print the help menu.
     */
    void print()
    {
        update_max_keys_length();

        print_usage();
        print_description();
        print_options();
        print_commands();
        print_values();
        print_epilog();

        std::flush(std::cout);
    }

    /**
     * @brief       Print the usage.
     */
    void print_usage()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_USAGE))
        {
            return;
        }
        arg_parsr_->print_usage();
    }

    /**
     * @brief       Print the description.
     */
    inline void print_description()
    {
        if (flgs_.is_set(help_menu_flags::PRINT_DESCRIPTION))
        {
            if (!desc_.empty())
            {
                speed::iostream::print_wrapped(std::cout, desc_, max_line_len_, 0);
                std::cout << "\n\n";
            }
        }
    }

    /**
     * @brief       Print the options.
     */
    void print_options()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_OPTIONS))
        {
            return;
        }

        bool fnd = false;

        for (auto& ky_arg : ky_args_)
        {
            if (!ky_arg->is_help_text_empty() && ky_arg->is_option())
            {
                fnd = true;
                break;
            }
        }
        if (!fnd)
        {
            return;
        }

        std::cout << "Options:\n";

        for (auto& ky_arg : ky_args_)
        {
            if (ky_arg->is_option())
            {
                print_arg(ky_arg);
            }
        }

        std::cout << '\n';
    }

    /**
     * @brief       Print the commands.
     */
    void print_commands()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_COMMANDS))
        {
            return;
        }

        bool fnd = false;

        for (auto& ky_arg : ky_args_)
        {
            if (!ky_arg->is_help_text_empty() && !ky_arg->is_option())
            {
                fnd = true;
            }
        }
        if (!fnd)
        {
            return;
        }

        std::cout << "Commands:\n";

        for (auto& ky_arg : ky_args_)
        {
            if (!ky_arg->is_option())
            {
                print_arg(ky_arg);
            }
        }

        std::cout << '\n';
    }

    /**
     * @brief       Print the values.
     */
    void print_values()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_VALUES))
        {
            return;
        }

        bool fnd = false;

        for (auto& kyless_arg : kyless_args_)
        {
            if (!kyless_arg->is_help_text_empty())
            {
                fnd = true;
            }
        }
        if (!fnd)
        {
            return;
        }

        std::cout << "Values:\n";

        for (auto& kyless_arg : kyless_args_)
        {
            print_arg(kyless_arg);
        }

        std::cout << '\n';
    }

    /**
     * @brief       Print the epilog.
     */
    void print_epilog()
    {
        if (flgs_.is_set(help_menu_flags::PRINT_EPILOGUE))
        {
            if (!epilg_.empty())
            {
                speed::iostream::print_wrapped(std::cout, epilg_, max_line_len_, 0);
                std::cout << "\n\n";
            }
        }
    }

    /**
     * @brief       Print the argument.
     * @param       bse_arg : Print the base arguement.
     */
    inline void print_arg(base_arg_type* bse_arg)
    {
        if (bse_arg->is_help_text_empty())
        {
            return;
        }

        if (flgs_.is_not_set(help_menu_flags::PRINT_ARGS_KEYS))
        {
            bse_arg->print_help_text(0, max_line_len_, new_line_indent_);
        }
        else
        {
            bse_arg->print_help_text(args_indent_, max_line_len_, new_line_indent_,
                                     max_short_kys_len_, max_long_kys_len_);
        }
    }

private:
    /** The description. */
    string_type desc_;

    /** The epilogue. */
    string_type epilg_;

    /** They key args of the help menu. */
    vector_type<key_arg_type*> ky_args_;

    /** The keyless args of the help menu. */
    vector_type<keyless_arg_type*> kyless_args_;

    /** Reference to the argument parser that holds this object. */
    arg_parser_type* arg_parsr_;

    /** Indentation of the arugments keys in the help menu. */
    std::size_t args_indent_;

    /** The maximum line length that will be printed. */
    std::size_t max_line_len_;

    /** The indentation used after printing a new line. */
    std::size_t new_line_indent_;

    /** The maximum lenght needed to print the short keys. */
    std::size_t max_short_kys_len_;

    /** The maximum lenght needed to print the long keys. */
    std::size_t max_long_kys_len_;

    /** Flags that dictates the help menu behavior. */
    flags_type<help_menu_flags> flgs_;
};


}


#endif
