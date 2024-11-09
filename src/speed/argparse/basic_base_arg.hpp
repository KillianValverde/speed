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
 * @file        speed/argparse/basic_base_arg.hpp
 * @brief       basic_base_arg class header.
 * @author      Killian Valverde
 * @date        2015/12/28
 */

#ifndef SPEED_ARGPARSE_BASIC_BASE_ARG_HPP
#define SPEED_ARGPARSE_BASIC_BASE_ARG_HPP

#include <string>
#include <unordered_set>
#include <utility>

#include "../containers/containers.hpp"
#include "../iostream/iostream.hpp"
#include "../safety/safety.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"
#include "basic_arg_parser.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents the base of the arguments hierarchy.
 */
template<typename TpAllocator>
class basic_base_arg
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;
    
    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Unordered set type used in the class. */
    template<typename TpKey_>
    using unordered_set_type = std::unordered_set<
            TpKey_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<TpKey_>>;

    /** Flags type used in the class. */
    template<typename T>
    using flags_type = speed::containers::flags<T>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    explicit basic_base_arg(arg_parser_type* arg_parsr)
            : hlp_menus_ids_assignd_()
            , desc_()
            , err_name_()
            , actn_()
            , arg_parsr_(arg_parsr)
            , presence_holdr_(nullptr)
            , flgs_(arg_flags::NIL)
            , err_flgs_(arg_error_flags::NIL)
            , nr_fnd_(0)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_base_arg(const basic_base_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_base_arg(basic_base_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    virtual ~basic_base_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_base_arg& operator =(const basic_base_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_base_arg& operator =(basic_base_arg&& rhs) noexcept = default;

    /**
     * @brief       Execute the action if there is one.
     */
    void execute_action()
    {
        if (actn_)
        {
            actn_();
        }
    }

    /**
     * @brief       Set errors flags if required.
     */
    virtual inline void update_error_flags() noexcept
    {
        if (flgs_.is_set(arg_flags::MANDATORY) && nr_fnd_ == 0 && arg_parsr_->has_parsed())
        {
            err_flgs_.set(arg_error_flags::ALLWAYS_REQUIRED_ERROR);
        }
        else
        {
            err_flgs_.unset(arg_error_flags::ALLWAYS_REQUIRED_ERROR);
        }
    }

    /**
     * @brief       Get the composite object of this class.
     * @return      The composite object of this class.
     */
    [[nodiscard]] inline arg_parser_type* get_arg_parser() const noexcept
    {
        return arg_parsr_;
    }

    /**
     * @brief       Give access to the argument error id.
     * @return      The argument error id.
     */
    [[nodiscard]] inline const string_type& get_error_name() const noexcept
    {
        return err_name_;
    }

    /**
     * @brief       Get the necessary length to print long arguments keys.
     * @return      The necessary length to print long arguments keys.
     */
    [[nodiscard]] virtual std::size_t get_long_keys_length() noexcept = 0;

    /**
     * @brief       Get the program name.
     * @return      The program name.
     */
    [[nodiscard]] const string_type& get_program_name() const noexcept
    {
        return arg_parsr_->get_program_name();
    }

    /**
     * @brief       Get the necessary length to print short arguments keys.
     * @return      The necessary length to print short arguments keys.
     */
    [[nodiscard]] virtual std::size_t get_short_keys_length() noexcept = 0;

    /**
     * @brief       Get a string that represents the kind of argument it is.
     * @return      A string that represents the kind of argument it is.
     */
    [[nodiscard]] inline virtual string_type get_tittle() const
    {
        return is_option() ? "Option" : "Command";
    }

    /**
     * @brief       Set the action to execute when the arguement is found in the program call.
     * @param       callabl : Action to execute when the arguement is found in the program
     *              call.
     */
    template<typename TpCallable>
    inline void set_action(TpCallable&& callabl)
    {
        actn_ = std::forward<TpCallable>(callabl);
    }

    /**
     * @brief       Set the argument description.
     * @param       desc : The description to set.
     */
    template<typename TpString_>
    inline void set_description(TpString_&& desc)
    {
        desc_ = std::forward<TpString_>(desc);
    }

    /**
     * @brief       Set an argument error flag.
     * @param       flg : Error flag to set.
     */
    inline void set_error_flag(arg_error_flags flg) noexcept
    {
        err_flgs_.set(flg);
    }

    /**
     * @brief       Set the argument error name.
     * @param       err_name : The argument error name.
     */
    template<typename TpString_>
    inline void set_error_name(TpString_&& err_name) noexcept
    {
        err_name_ = std::forward<TpString_>(err_name);
    }

    /**
     * @brief       Set an argument flag.
     * @param       flg : Flag to set.
     */
    virtual inline void set_flag(arg_flags flg) noexcept
    {
        flgs_.set(flg);
    }

    /**
     * @brief       Set the argument flags.
     * @param       flgs : The argument flags.
     */
    virtual inline void set_flags(arg_flags flgs) noexcept
    {
        flgs_ = flgs;
    }

    /**
     * @brief       Specifies whether the argument has been found in the program call.
     * @param       fnd : The value that specifies whether the argument has been found in the
     *              program call.
     */
    inline void set_found(bool fnd) noexcept
    {
        if (fnd && nr_fnd_ != 0 && flgs_.is_set(arg_flags::UNIQUE_INSTANCE))
        {
            err_flgs_.set(arg_error_flags::APPEAR_JUST_ONCE_ERROR);
        }
        else
        {
            fnd ? speed::safety::try_addm(&nr_fnd_, 1) : nr_fnd_ = 0;

            if (presence_holdr_ != nullptr)
            {
                *presence_holdr_ = fnd;
            }

            err_flgs_.unset(arg_error_flags::APPEAR_JUST_ONCE_ERROR);
        }
    }

    /**
     * @brief       Set the help menus assigned with the arguement.
     * @param       hlp_menus_ids : The help menus ids of the help menus assigned.
     */
    template<typename... Ts_>
    void set_help_menus_assigned(Ts_&&... hlp_menus_ids)
    {
        arg_parsr_->remove_from_help_menus(this, hlp_menus_ids_assignd_);
        hlp_menus_ids_assignd_.clear();

        int foreach[sizeof...(Ts_) + 1] = { (
                hlp_menus_ids_assignd_.emplace(hlp_menus_ids), 0)... };

        arg_parsr_->register_into_help_menus(this, std::forward<Ts_>(hlp_menus_ids)...);
    }

    /**
     * @brief       Set the presence synchronizer.
     * @param       presence_holdr : The presence synchronizer.
     */
    inline void set_presence_holder(bool* presence_holdr) noexcept
    {
        presence_holdr_ = presence_holdr;

        if (presence_holdr_ != nullptr)
        {
            *presence_holdr_ = nr_fnd_ == 0;
        }
    }

    /**
     * @brief       Unset a argument error flag.
     * @param       flg : Error flag to unset.
     */
    inline void unset_error_flag(arg_error_flags flg) noexcept
    {
        err_flgs_.unset(flg);
    }

    /**
     * @brief       Unset an argument flag.
     * @param       flg : Flag to unset.
     */
    virtual inline void unset_flag(arg_flags flg) noexcept
    {
        flgs_.unset(flg);
    }

    /**
     * @brief       Erase all argument error flags.
     */
    inline void clear_error_flags() noexcept
    {
        err_flgs_.clear();
    }

    /**
     * @brief       Erase all argument flags.
     */
    inline void clear_flags() noexcept
    {
        flgs_.clear();
    }
    
    /**
     * @brief       Allows knowing whether there are errors.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool has_errors() const noexcept
    {
        return err_flgs_.is_not_empty();
    }

    /**
     * @brief       Allows knowing whether an argument error flag is set.
     * @param       flg : The error flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_error_flag_set(arg_error_flags flg) const noexcept
    {
        return err_flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether the argument error name is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_error_name_empty() const noexcept
    {
        return err_name_.empty();
    }

    /**
     * @brief       Allows knowing whether an argument flag is set.
     * @param       flg : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_flag_set(arg_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether the argument description is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_help_text_empty() const
    {
        return desc_.empty();
    }

    /**
     * @brief       Allows knowing if the argument in an option.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_option() const noexcept
    {
        return flgs_.is_not_set(arg_flags::MANDATORY);
    }

    /**
     * @brief       Allows knowing whether the argument has been found in the program call.
     * @return      The value that specifies whether the argument has been found in the program
     *              call.
     */
    [[nodiscard]] inline bool was_found() const noexcept
    {
        return nr_fnd_ != 0;
    }
    
    /**
     * @brief       Print argument errors in standard output.
     */
    virtual void print_errors() const
    {
        if (err_flgs_.is_set(arg_error_flags::ALLWAYS_REQUIRED_ERROR))
        {
            print_error_message();
            std::cout << get_tittle() << " is allways required\n";
        }
        if (err_flgs_.is_set(arg_error_flags::APPEAR_JUST_ONCE_ERROR))
        {
            print_error_message();
            std::cout << get_tittle() << " has appeared more than once\n";
        }
    }

    /**
     * @brief       Print an error message.
     * @param       err_message : The message to print.
     */
    void print_error_message(const string_type& err_message = string_type()) const
    {
        std::cout << arg_parsr_->get_program_name() << ": ";
        if (!err_name_.empty())
        {
            if (arg_parsr_->colors_enabled())
            {
                std::cout << speed::iostream::set_light_red_text
                          << err_name_ << ": "
                          << speed::iostream::set_default_text;
            }
            else
            {
                std::cout << err_name_ << ": ";
            }
        }

        if (!err_message.empty())
        {
            std::cout << err_message;
        }
    }

    /**
     * @brief       Print the argument help text.
     * @param       current_line_len : The current length of the current line.
     * @param       max_line_len : The maximum line length that will be printed.
     * @param       new_line_indent : The indentation used after a newline is printed.
     */
    virtual void print_help_text(
            std::size_t current_line_len,
            std::size_t max_line_len,
            std::size_t new_line_indent
    )
    {
        if (desc_.empty())
        {
            return;
        }

        speed::iostream::print_wrapped(std::cout, desc_, max_line_len, new_line_indent,
                                       current_line_len);
        std::cout << '\n';
    }

    /**
     * @brief       Print the argument help text.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum line length that will be printed.
     * @param       new_line_indent : The indentation used after a newline is printed.
     * @param       short_kys_len : The maximum short keys length.
     * @param       long_kys_len : The maximum long keys length.
     */
    virtual void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_kys_len,
            std::size_t long_kys_len
    ) = 0;

    /**
     * @brief       Print the name of the argument in the standard output.
    */
    virtual void print_name() = 0;

    /**
     * @brief       Print the usage in the standard output.
    */
    virtual void print_usage() = 0;

private:
    /** The help menus ids assigned. */
    unordered_set_type<string_type> hlp_menus_ids_assignd_;

    /** The argument description. */
    string_type desc_;
    
    /** The name used to reference an arguments during the error display. */
    string_type err_name_;

    /** Function to execute if the arguement is found during the program call. */
    std::function<void()> actn_;

    /** Reference to the argument parser that holds this object. */
    arg_parser_type* arg_parsr_;

    /** Reference to a boolen that will be set to indicate whether the argument has been found. */
    bool* presence_holdr_;

    /** Number of times the argument has been found. */
    std::size_t nr_fnd_;
    
    /** Flags that dictates the argument behavior. */
    flags_type<arg_flags> flgs_;
    
    /** Flags that allows knowing whether there are errors. */
    flags_type<arg_error_flags> err_flgs_;
};


}


#endif
