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
 *  @file       speed/argparse/basic_mutually_exclusive_constraint.hpp
 *  @brief      basic_mutually_exclusive_constraint class header.
 *  @author     Killian Valverde
 *  @date       2024/05/16
 */

#ifndef SPEED_ARGPARSE_BASIC_MUTUALLY_EXCLUSIVE_CONSTRAINT_HPP
#define SPEED_ARGPARSE_BASIC_MUTUALLY_EXCLUSIVE_CONSTRAINT_HPP

#include <memory>
#include <string>
#include <unordered_set>

#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_keyless_arg.hpp"
#include "basic_value_arg.hpp"
#include "basic_version_arg.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class used to represent a constraint that verifies that exclusivelly one of the
 *              specified arguments is found in the program call.
 */
template<typename TpBase, typename TpAllocator>
class basic_mutually_exclusive_constraint : public TpBase
{
public:
    /** Base class type. */
    using base_type = TpBase;

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

    /** Type that represents an argument key. */
    using arg_key_type = basic_arg_key<TpAllocator>;

    /** Type that represents an argument value. */
    using arg_value_type = basic_arg_value<TpAllocator>;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;

    /** Type that represents the option to get the version information. */
    using version_arg_type = basic_version_arg<TpAllocator>;

    /** Type that represents arguments without keys. */
    using keyless_arg_type = basic_keyless_arg<TpAllocator>;

    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<TpAllocator>;

    /** Type that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this constraint.
     * @param       kys : The arguments keys in which apply the constraint.
     */
    template<typename... Ts_>
    explicit basic_mutually_exclusive_constraint(arg_parser_type* arg_parsr, const Ts_&... kys)
            : base_type(arg_parsr, kys...)
    {
        if (base_type::get_base_args_size() <= 1)
        {
            throw wrong_dependency_exception();
        }

        for (auto& bse_arg : base_type::get_base_args())
        {
            if (bse_arg->is_flag_set(arg_flags::MANDATORY))
            {
                throw wrong_dependency_exception();
            }
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_mutually_exclusive_constraint(const basic_mutually_exclusive_constraint& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_mutually_exclusive_constraint(basic_mutually_exclusive_constraint&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_mutually_exclusive_constraint() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_mutually_exclusive_constraint& operator =(const basic_mutually_exclusive_constraint& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_mutually_exclusive_constraint& operator =(basic_mutually_exclusive_constraint&& rhs) noexcept = default;
    
    /**
     * @brief       Allows knowing whether a relational constraint is violed.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool violed() override
    {
        bool fnd = false;

        for (auto& bse_arg : base_type::get_base_args())
        {
            if (bse_arg->was_found())
            {
                if (fnd)
                {
                    return true;
                }

                fnd = true;
            }
        }

        return base_type::violed();
    }

    /**
     * @brief       Print the constraint usage.
     */
    void print_usage() override
    {
        std::size_t cnt = 0;
        auto& bse_args = base_type::get_base_args();
        auto bse_arg_it = bse_args.begin();

        std::cout << "{";
        (*bse_arg_it)->print_usage();
        ++bse_arg_it;

        for (; bse_arg_it != bse_args.end(); ++bse_arg_it)
        {
            std::cout << " ⊕ ";
            (*bse_arg_it)->print_usage();
        }

        std::cout << "}";

        base_type::print_usage();
    }
    
    /**
     * @brief       Print the constraints errors.
     */
    void print_errors() override
    {
        const string_type* err_name;

        if (violed())
        {
            auto& bse_args = base_type::get_base_args();

            std::cout << base_type::get_program_name() << ": ";
            
            for (auto it = bse_args.cbegin(); it != bse_args.cend(); )
            {
                err_name = &(*it)->get_error_name();
                
                if (!err_name->empty())
                {
                    if (base_type::colors_enabled())
                    {
                        std::cout << speed::iostream::set_light_red_text;
                    }
                    std::cout << *err_name;
    
                    if (++it != bse_args.cend())
                    {
                        std::cout << ", ";
                    }
                    else
                    {
                        std::cout << ": ";
                    }
                    
                    if (base_type::colors_enabled())
                    {
                        std::cout << speed::iostream::set_default_text;
                    }
                }
            }
            
            std::cout << "The arguments are mutually exclusive.\n";
        }

        base_type::print_errors();
    }

protected:
    /**
     * @brief       Allows knowing whether or not the colors are enabled.
     * @return      If fucntion is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool colors_enabled() const noexcept override
    {
        return base_type::colors_enabled();
    }

    /**
     * @brief       Get the base args in which the constraint is applied.
     * @return      The base args in which the constraint is applied.
     */
    [[nodiscard]] inline unordered_set_type<base_arg_type*>& get_base_args() noexcept override
    {
        return base_type::get_base_args();
    }

    /**
     * @brief       Get the number of arguments concerned by the constraint.
     * @return      The number of arguments concerned by the constraint.
     */
    [[nodiscard]] inline std::size_t get_base_args_size() const noexcept override
    {
        return base_type::get_base_args_size();
    }

    /**
     * @brief       Get the program name.
     * @return      The program name.
     */
    [[nodiscard]] inline const string_type& get_program_name() const noexcept override
    {
        return base_type::get_program_name();
    }
};


}


#endif
