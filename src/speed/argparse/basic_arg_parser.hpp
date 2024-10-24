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
 *  @file       speed/argparse/basic_arg_parser.hpp
 *  @brief      basic_arg_parser class header.
 *  @author     Killian Valverde
 *  @date       2015/12/31
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_PARSER_HPP
#define SPEED_ARGPARSE_BASIC_ARG_PARSER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../containers/containers.hpp"
#include "../memory/memory.hpp"
#include "../stringutils/stringutils.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"
#include "arg_parser_error_flags.hpp"
#include "arg_parser_flags.hpp"
#include "arg_value_error_flags.hpp"
#include "basic_arg_constraint.hpp"
#include "basic_arg_parser_setter.hpp"
#include "basic_one_or_more_constraint.hpp"
#include "basic_help_arg.hpp"
#include "basic_help_arg_setter.hpp"
#include "basic_help_menu.hpp"
#include "basic_help_menu_setter.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_arg_setter.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_key_value_arg_setter.hpp"
#include "basic_keyless_arg.hpp"
#include "basic_keyless_arg_setter.hpp"
#include "basic_mutually_exclusive.hpp"
#include "basic_version_arg.hpp"
#include "basic_version_arg_setter.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class used to parse arguments.
 */
template<typename TpAllocator>
class basic_arg_parser
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

    /** Unordered map type used in the class. */
    template<typename TpKey_, typename TpValue_>
    using unordered_map_type = std::unordered_map<
            TpKey_, TpValue_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<std::pair<const TpKey_, TpValue_>>>;

    /** Unordered set type used in the class. */
    template<typename TpKey_>
    using unordered_set_type = std::unordered_set<
            TpKey_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<TpKey_>>;

    /** Class that represents a bit field */
    template<typename T>
    using flags_type = speed::containers::flags<T>;

    /** Type that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;

    /** Type that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
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

    /** Type that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<TpAllocator>;

    /** Type that represents a constraint for a set of arguments. */
    using arg_constraint_type = basic_arg_constraint<TpAllocator>;

    /** Type that represents an 'at least one found' constraint for a set of arguments. */
    using one_or_more_constraint_type = basic_one_or_more_constraint<
            arg_constraint_type, TpAllocator>;

    /** Type that represents a mutually exclusive constraint for a set of arguments. */
    using mutually_exclusive_type = basic_mutually_exclusive<arg_constraint_type, TpAllocator>;

    /** Type that represents a help menu. */
    using help_menu_type = basic_help_menu<TpAllocator>;

    /** Type used to configure an added key arg. */
    using key_arg_setter_type = basic_key_arg_setter<TpAllocator>;

    /** Type used to configure an added version arg. */
    using version_arg_setter_type = basic_version_arg_setter<TpAllocator>;

    /** Type used to configure an added keyless arg. */
    using keyless_arg_setter_type = basic_keyless_arg_setter<TpAllocator>;

    /** Type used to configure an added key value arg. */
    using key_value_arg_setter_type = basic_key_value_arg_setter<TpAllocator>;

    /** Type used to configure an added help arg. */
    using help_arg_setter_type = basic_help_arg_setter<TpAllocator>;

    /** Type used to configure an added help menu. */
    using help_menu_setter_type = basic_help_menu_setter<TpAllocator>;

    /** Type used to configure the argument parser. */
    using arg_parser_setter_type = basic_arg_parser_setter<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       prog_name : The program name. If you don't specify any, the program name will
     *              be obtained from argv.
     */
    template<
            typename TpString_ = string_type,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_parser, std::decay_t<TpString_>>::value
            >
    >
    explicit basic_arg_parser(TpString_&& prog_name = string_type())
            : max_unrecog_args_(1)
            , default_hlp_arg_(nullptr)
            , current_vers_arg_(nullptr)
            , prog_name_(std::forward<TpString_>(prog_name))
            , err_id_("error")
            , bse_arg_list_()
            , kyless_arg_list_()
            , constrnts_()
            , unrecog_args_()
            , short_prefxs_({"-"})
            , long_prefxs_({"--"})
            , bse_arg_map_()
            , hlp_menu_map_()
            , flgs_(arg_parser_flags::DEFAULT_ARG_PARSER_FLAGS)
            , err_flgs_(arg_parser_error_flags::NIL)
            , key_arg_type_alloc_()
            , help_arg_type_alloc_()
            , version_arg_type_alloc_()
            , key_value_arg_type_alloc_()
            , keyless_arg_type_alloc_()
            , at_least_one_found_type_alloc_()
            , mutually_exclusive_type_alloc_()
            , parsd_(false)
    {
    }
    
    /**
     * @brief       Copy constructor [deleted].
     * @param       rhs : Object to copy.
     */
    basic_arg_parser(const basic_arg_parser& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_parser(basic_arg_parser&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_parser()
    {
        for (auto& x : bse_arg_list_)
        {
            delete_base_arg(x);
        }

        for (auto& x : constrnts_)
        {
            delete_arg_constraint(x);
        }
    
        current_vers_arg_ = nullptr;
    }
    
    /**
     * @brief       Copy assignment operator [deleted].
     * @param       rhs : Object to copy.
     */
    basic_arg_parser& operator =(const basic_arg_parser& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     */
    basic_arg_parser& operator =(basic_arg_parser&& rhs) noexcept = delete;

    /**
     * @brief       Allows to get access to an object to configure the argument parser behaviour.
     * @return      An object that will allow to configure the argument parser behaviour.
     */
    arg_parser_setter_type configure() noexcept
    {
        return arg_parser_setter_type(this);
    }
    
    /**
     * @brief       Add an argument that has keys.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... Ts_>
    key_arg_setter_type add_key_arg(Ts_&&... kys)
    {
        assert_valid_keys(kys...);
        key_arg_type* ky_arg;
        speed::memory::allocate_and_construct(key_arg_type_alloc_, ky_arg, this, kys...);
        register_key_arg(ky_arg, std::forward<Ts_>(kys)...);

        return key_arg_setter_type(ky_arg);
    }
    
    /**
     * @brief       Add an argument that has keys and a set of values associated.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... Ts_>
    key_value_arg_setter_type add_key_value_arg(Ts_&&... kys)
    {
        assert_valid_keys(kys...);
        key_value_arg_type* ky_val_arg;
        speed::memory::allocate_and_construct(key_value_arg_type_alloc_, ky_val_arg, this, kys...);
        register_key_value_arg(ky_val_arg, std::forward<Ts_>(kys)...);

        return key_value_arg_setter_type(ky_val_arg);
    }

    /**
     * @brief       Add an argument that just has values and doesn't have keys.
     * @param       usage_ky : The ID that will be used to make refence to this argument in the
     *              parser as well as being used during the printing of the usage message.
     */
    template<typename TpString_>
    keyless_arg_setter_type add_keyless_arg(TpString_&& usage_ky)
    {
        assert_valid_key(usage_ky);
        keyless_arg_type* kyless_arg;
        speed::memory::allocate_and_construct(keyless_arg_type_alloc_, kyless_arg, this, usage_ky);
        register_keyless_arg(kyless_arg, std::forward<TpString_>(usage_ky));

        return keyless_arg_setter_type(kyless_arg);
    }
    
    /**
     * @brief       Add an argument used to print a help menu.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... Ts_>
    help_arg_setter_type add_help_arg(Ts_&&... kys)
    {
        assert_valid_keys(kys...);
        help_arg_type* hlp_arg;
        speed::memory::allocate_and_construct(help_arg_type_alloc_, hlp_arg, this, kys...);
        register_help_arg(hlp_arg, std::forward<Ts_>(kys)...);

        return help_arg_setter_type(hlp_arg);
    }
    
    /**
     * @brief       Add an argument used to print the version information.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... Ts_>
    version_arg_setter_type add_version_arg(Ts_&&... kys)
    {
        assert_valid_keys(kys...);
        assert_valid_version_add();
        version_arg_type* vers_arg;
        speed::memory::allocate_and_construct(version_arg_type_alloc_, vers_arg, this, kys...);
        register_version_arg(vers_arg, std::forward<Ts_>(kys)...);

        return version_arg_setter_type(vers_arg);
    }
    
    /**
     * @brief       Add a constraint that verifies that at least one of the specified arguments has
     *              been found in the program call.
     * @param       kys : Variadic number of strings to speficy every key in which the constraint
     *              applies.
     */
    template<typename... Ts_>
    void add_constraint_one_or_more(const Ts_&... kys)
    {
        one_or_more_constraint_type* at_least_one_fnd;
        speed::memory::allocate_and_construct(at_least_one_found_type_alloc_, at_least_one_fnd,
                                              this, kys...);
        constrnts_.push_back(at_least_one_fnd);
    }

    /**
     * @brief       Add a constraint that verifies that exclusivelly one of the specified arguments
     *              is found in the program call.
     * @param       kys : Variadic number of strings to speficy every key in which the constraint
     *              applies.
     */
    template<typename... Ts_>
    void add_mutually_exclusive_constraint(const Ts_&... kys)
    {
        mutually_exclusive_type* mutually_excl;
        speed::memory::allocate_and_construct(mutually_exclusive_type_alloc_, mutually_excl,
                                              this, kys...);
        constrnts_.push_back(mutually_excl);
    }

    /**
     * @brief       Add a help menu to the arguement parser.
     * @param       hlp_menu_id : The ID that the help menu will have and that it will be refered
     *              by from the arguements. If the ID already exists nothing will happened. If the
     *              ID is empty the default help menu will be refered.
     * @return      Object that allows configurint the help menu added.
     */
    template<typename TpString_ = string_type>
    help_menu_setter_type add_help_menu(TpString_&& hlp_menu_id = string_type())
    {
        return help_menu_setter_type(&get_help_menu(std::forward<TpString_>(hlp_menu_id)));
    }
    
    /**
     * @brief       Perform the parsing of the arguements.
     * @param       argc : Contains the number of arguments gotten through the program call.
     * @param       argv : Contains the arguments gotten through the program call.
     */
    template<typename TpArgc_, typename TpArgv_>
    void parse_args(const TpArgc_& argc, const TpArgv_& argv)
    {
        enum class dfa_t : std::uint8_t
        {
            START, READ_ARG, PARSE_KEY, PARSE_KEY_ARG, PARSE_EQ_OPERATOR, PARSE_GROUPING_ARGS,
            PARSE_KEYLESS_ARG, PARSE_UNRECOGNIZED_ARG, QUIT, FINISH
        };

        dfa_t cur_state;
        string_type cur_argv;
        string_type ky_builder;
        string_type eq_val;
        std::size_t cur_idx;
        std::size_t pos_increment;
        std::size_t eq_pos;
        key_arg_type *ky_arg = nullptr;
        key_value_arg_type *ky_val_arg = nullptr;
        keyless_arg_type *kyless_arg = nullptr;
        base_arg_type *prev_arg = nullptr;
        vector_type<key_arg_type*> chaind_args;
        bool insertd;
        bool prefix_err;
        auto cur_keyless_arg_it = kyless_arg_list_.begin();

        for (cur_state = dfa_t::START; cur_state != dfa_t::FINISH; )
        {
            switch (cur_state)
            {
            case dfa_t::START:
                reset_args_parse();
                if (argc > 0 && prog_name_.empty())
                {
                    prog_name_ = argv[0];
                }
                cur_idx = 1;
                cur_state = dfa_t::READ_ARG;
                continue;

            case dfa_t::READ_ARG:
                if (cur_idx >= argc)
                {
                    cur_state = dfa_t::QUIT;
                    continue;
                }
                cur_argv = argv[cur_idx];
                cur_state = dfa_t::PARSE_KEY;
                continue;

            case dfa_t::PARSE_KEY:
                if ((ky_arg = get_key_arg(cur_argv)) != nullptr)
                {
                    cur_state = dfa_t::PARSE_KEY_ARG;
                    continue;
                }
                cur_state = dfa_t::PARSE_EQ_OPERATOR;
                continue;

            case dfa_t::PARSE_KEY_ARG:
                parse_key_arg(argc, argv, ky_arg, cur_idx, &pos_increment);
                cur_idx += pos_increment + 1;
                prev_arg = ky_arg;
                cur_state = dfa_t::READ_ARG;
                continue;

            case dfa_t::PARSE_EQ_OPERATOR:
                if (cur_argv.size() > 2 &&
                    (eq_pos = cur_argv.find('=', 1)) != string_type::npos)
                {
                    ky_builder = cur_argv.substr(0, eq_pos);
                    eq_val = cur_argv.substr(eq_pos + 1);

                    if (!ky_builder.empty() && 
                        !eq_val.empty() && 
                        (ky_val_arg = get_key_value_arg(ky_builder)) != nullptr &&
                            ky_val_arg->is_flag_set(arg_flags::ASSIGNMENT_OPERATOR))
                    {
                        ky_val_arg->add_value(std::move(eq_val));
                        ky_arg = ky_val_arg;
                        cur_state = dfa_t::PARSE_KEY_ARG;
                        continue;
                    }
                }
                cur_state = dfa_t::PARSE_GROUPING_ARGS;
                continue;

            case dfa_t::PARSE_GROUPING_ARGS:
                chaind_args.clear();
                ky_builder = get_key_prefix(cur_argv);
                ky_builder += ' ';
                for (auto i = ky_builder.size() - 1; i < cur_argv.size(); ++i)
                {
                    ky_builder[ky_builder.size() - 1] = cur_argv[i];
                    ky_arg = get_key_arg(ky_builder);
                    if (ky_arg != nullptr && ky_arg->is_flag_set(arg_flags::GROUPING))
                    {
                        chaind_args.push_back(ky_arg);
                    }
                    else
                    {
                        chaind_args.clear();
                        break;
                    }
                }
                if (!chaind_args.empty())
                {
                    for (auto& chaind_arg : chaind_args)
                    {
                        parse_key_arg(argc, argv, chaind_arg, cur_idx, &pos_increment);
                        cur_idx += pos_increment;
                    }
                    ++cur_idx;
                    cur_state = dfa_t::READ_ARG;
                    prev_arg = static_cast<base_arg_type*>(chaind_args.back());
                    continue;
                }
                cur_state = dfa_t::PARSE_KEYLESS_ARG;
                continue;

            case dfa_t::PARSE_KEYLESS_ARG:
                insertd = false;
                while (cur_keyless_arg_it != kyless_arg_list_.end() &&
                       (*cur_keyless_arg_it)->max_values_reached())
                {
                    ++cur_keyless_arg_it;
                }
                while (cur_keyless_arg_it != kyless_arg_list_.end())
                {
                    kyless_arg = *cur_keyless_arg_it;
                    prefix_err = has_value_with_prefix_error(kyless_arg, cur_argv);
                    if (prefix_err || !kyless_arg->try_add_value(cur_argv))
                    {
                        if (kyless_arg->min_values_reached() ||
                            (kyless_arg->get_number_of_values() > 0 && prefix_err))
                        {
                            ++cur_keyless_arg_it;
                            continue;
                        }
                        else if (!prefix_err)
                        {
                            kyless_arg->add_value(std::move(cur_argv));
                            insertd = true;
                        }
                        break;
                    }
                    insertd = true;
                    break;
                }
                if (insertd)
                {
                    if (static_cast<base_arg_type*>(kyless_arg) !=  prev_arg)
                    {
                        kyless_arg->execute_action();
                        kyless_arg->set_found(true);
                    }
                    ++cur_idx;
                    cur_state = dfa_t::READ_ARG;
                    prev_arg = kyless_arg;
                    continue;
                }
                cur_state = dfa_t::PARSE_UNRECOGNIZED_ARG;
                continue;

            case dfa_t::PARSE_UNRECOGNIZED_ARG:
                if (unrecog_args_.size() < max_unrecog_args_)
                {
                    unrecog_args_.push_back(std::move(cur_argv));
                }
                ++cur_idx;
                cur_state = dfa_t::READ_ARG;
                prev_arg = nullptr;
                continue;

            case dfa_t::QUIT:
                parsd_ = true;
                update_error_flags();
                trigger_prints();
                if (flgs_.is_set(arg_parser_flags::PRINT_ERRORS))
                {
                    print_errors();
                }
                cur_state = dfa_t::FINISH;
                continue;

            case dfa_t::FINISH:
                break;
            }
        }
    }
    
    /**
     * @brief       Allows knowing whether an argument has been found in the program call.
     * @param       ky : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool was_found(const string_type& ky) const
    {
        base_arg_type* bse_arg = get_base_arg(ky);

        if (bse_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return bse_arg->was_found();
    }

    /**
     * @brief       Allows knowing the number of values that has been found for a specified
     *              arguement that have values associated.
     * @param       ky : The arguement key to check.
     * @return      The number of values that has been found for a specified argument that have
     *              values associated.
     */
    [[nodiscard]] std::size_t count_values_found(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg(ky);

        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->get_number_of_values();
    }
    
    /**
     * @brief       Get the key argument front value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @return      The key argument front value converted to target
     */
    template<typename TpTarget_>
    [[nodiscard]] TpTarget_ get_front_as(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_front_value_as<TpTarget_>();
    }
    
    /**
     * @brief       Get the key argument front value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the key argument front value converted to target 
     *              is returned, otherwise the default value is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    [[nodiscard]] TpTarget_ get_front_as(
            const string_type& ky,
            TpDefaultValue_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_front_value_as<TpTarget_>(
                std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the key argument front value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    [[nodiscard]] TpTarget_ try_get_front_as(
            const string_type& ky,
            TpTarget_* res
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template try_get_front_value_as<TpTarget_>(res);
    }
    
    /**
     * @brief       Get the key argument index value converted to target.
     * @param       ky : Key of the argument to get the index value.
     * @param       indx : Index of the value to get.
     * @return      The key argument index value converted to target.
     */
    template<typename TpTarget_>
    [[nodiscard]] TpTarget_ get_at_as(const string_type& ky, std::size_t indx) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_value_at_as<TpTarget_>(indx);
    }
    
    /**
     * @brief       Get the key argument index value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @param       indx : Index of the value to get.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the key argument index value converted to target is
     *              returned, otherwise the default value is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    [[nodiscard]] TpTarget_ get_at_as(
            const string_type& ky,
            std::size_t indx,
            TpDefaultValue_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_value_at_as<TpTarget_>(
                indx, std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the key argument index value converted to target.
     * @param       ky : Key of the argument to get the index value.
     * @param       indx : Index of the value to get.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    [[nodiscard]] TpTarget_ try_get_at_as(
            const string_type& ky,
            std::size_t indx,
            TpTarget_* res
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template try_get_value_at_as<TpTarget_>(indx, res);
    }
    
    /**
     * @brief       Get the key argument values converted to target.
     * @param       ky : Key of the argument to get the values.
     * @return      The key argument values converted to target.
     */
    template<typename TpTarget_>
    [[nodiscard]] vector_type<TpTarget_> get_all_as(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->template get_values_as<TpTarget_>();
    }
    
    /**
     * @brief       Get the key argument values converted to target.
     * @param       ky : Key of the argument to get the values.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the key argument values converted to target are 
     *              returned, otherwise the default value is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_ = vector_type<TpTarget_>>
    [[nodiscard]] vector_type<TpTarget_> get_all_as(
            const string_type& ky,
            TpDefaultValue_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->template get_values_as<TpTarget_>(
                std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the key argument values converted to target.
     * @param       ky : Key of the argument to get the values.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    [[nodiscard]] bool try_get_all_as(
            const string_type& ky,
            vector_type<TpTarget_>* res
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->template try_get_values_as<TpTarget_>(res);
    }
    
    /**
     * @brief       Allows knowing whether there are errors in the argument parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool has_errors() const noexcept
    {
        return err_flgs_.is_not_empty();
    }
    
    /**
     * @brief       Allows knowing whether there are errors associated with a specific argument in
     *              the argument parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool arg_has_errors(const string_type& ky) const
    {
        base_arg_type* bse_arg = get_base_arg(ky);

        if (bse_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return bse_arg->has_errors();
    }
    
    /**
     * @brief       Print a help menu.
     * @param       hlp_menu_id : The ID of the help menu to print. If nothing is pecified the
     *              default help menu will be printed.
     */
    void print_help(const string_type& hlp_menu_id = string_type())
    {
        get_help_menu(hlp_menu_id).print();
    }
    
    /**
     * @brief       Print the version information.
     */
    void print_version()
    {
        if (current_vers_arg_ == nullptr)
        {
            throw version_not_found_exception();
        }

        current_vers_arg_->print_version_information();
    }
    
    /**
     * @brief       Print the argument parser errors.
     */
    void print_errors()
    {
        if (err_flgs_.is_empty())
        {
            return;
        }
        
        if (err_flgs_.is_set(arg_parser_error_flags::ARGS_ERROR))
        {
            for (auto& bse_arg : bse_arg_list_)
            {
                bse_arg->print_errors();
            }
        }
        
        if (err_flgs_.is_set(arg_parser_error_flags::ARGS_CONSTRAINTS_ERROR))
        {
            for (auto& constrnt : constrnts_)
            {
                constrnt->print_errors();
            }
        }
        
        if (err_flgs_.is_set(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR))
        {
            for (auto& unrecog_arg : unrecog_args_)
            {
                std::cout << prog_name_ << ": ";
                if (!err_id_.empty())
                {
                    if (flgs_.is_set(arg_parser_flags::USE_COLORS))
                    {
                        std::cout << speed::iostream::set_light_red_text
                                    << err_id_ << ": "
                                    << speed::iostream::set_default_text;
                    }
                    else
                    {
                        std::cout << err_id_ << ": ";
                    }

                }
                std::cout << "Unrecognized option '" << unrecog_arg << "'\n";
            }
        }

        if (!prog_name_.empty() && default_hlp_arg_ != nullptr)
        {
            std::cout << "Try '" << prog_name_ << ' ' << default_hlp_arg_->get_front_key()
                      << "' for more information.\n";
        }
        
        if (flgs_.is_set(arg_parser_flags::PRINT_HELP_AFTER_PRINTING_ERRORS))
        {
            print_help();
        }
        
        if (flgs_.is_set(arg_parser_flags::PKILL_AFTER_PRINTING_ERRORS))
        {
            exit_program(-1);
        }
    }

private:
    /**
     * @brief       Assert the validity of the specified keys.
     * @param       kys : Keys to check.
     */
    template<typename... Ts_>
    inline void assert_valid_keys(const Ts_&... kys) const
    {
        int foreach[sizeof...(Ts_) + 1] = { (assert_valid_key(kys), 0)... };
    }

    /**
     * @brief       Assert the validity of the specified key.
     * @param       ky : Key to check.
     */
    template<typename TpString_>
    inline void assert_valid_key(const TpString_& ky) const
    {
        if (arg_key_exists(ky))
        {
            throw key_already_exists_exception();
        }
    }

    /**
     * @brief       Assert the validity of adding a version arg.
     */
    inline void assert_valid_version_add() const
    {
        if (current_vers_arg_ != nullptr)
        {
            throw version_already_exists_exception();
        }
    }

    /**
     * @brief       Register in the data stuctures a specified key arg and its keys.
     * @param       ky_arg : Key arg to register.
     * @param       kys : Keys of the key arg.
     */
    template<typename... Ts_>
    void register_key_arg(key_arg_type* ky_arg, Ts_&&... kys)
    {
        bse_arg_list_.push_back(static_cast<base_arg_type*>(ky_arg));

        int foreach[sizeof...(Ts_) + 1] = { (
                bse_arg_map_.emplace(std::forward<Ts_>(kys), ky_arg), 0)... };

        register_into_help_menus(ky_arg);
    }

    /**
     * @brief       Register in the data stuctures a specified key value arg and its keys.
     * @param       ky_val_arg : Key value arg to register.
     * @param       kys : Keys of the key value arg.
     */
    template<typename... Ts_>
    inline void register_key_value_arg(key_value_arg_type* ky_val_arg, Ts_&&... kys)
    {
        register_key_arg(ky_val_arg, std::forward<Ts_>(kys)...);
    }

    /**
     * @brief       Register in the data stuctures a specified keyless arg and its usage key.
     * @param       kyless_arg : Keyless arg to register.
     * @param       usage_ky : Usage key of the keyless arg.
     */
    template<typename TpString_>
    inline void register_keyless_arg(keyless_arg_type* kyless_arg, TpString_&& usage_ky)
    {
        bse_arg_list_.push_back(static_cast<base_arg_type*>(kyless_arg));
        kyless_arg_list_.push_back(kyless_arg);
        bse_arg_map_.emplace(std::forward<TpString_>(usage_ky), kyless_arg);
        register_into_help_menus(kyless_arg);
    }

    /**
     * @brief       Register in the data stuctures a specified help arg and its keys.
     * @param       hlp_arg : Help arg to register.
     * @param       kys : Keys of the help arg.
     */
    template<typename... Ts_>
    inline void register_help_arg(help_arg_type* hlp_arg, Ts_&&... kys)
    {
        if (default_hlp_arg_ == nullptr)
        {
            default_hlp_arg_ = hlp_arg;
        }
        register_key_arg(hlp_arg, std::forward<Ts_>(kys)...);
    }

    /**
     * @brief       Register in the data stuctures a specified version arg and its keys.
     * @param       vers_arg : Version arg to register.
     * @param       kys : Keys of the version arg.
     */
    template<typename... Ts_>
    inline void register_version_arg(version_arg_type* vers_arg, Ts_&&... kys)
    {
        if (current_vers_arg_ == nullptr)
        {
            current_vers_arg_ = vers_arg;
        }
        register_key_arg(vers_arg, std::forward<Ts_>(kys)...);
    }

    /**
     * @brief       Register the specified arguement into the default help menu.
     * @param       bse_arg : Specified argument to register into the default help menu.
     */
    void register_into_help_menus(base_arg_type* bse_arg)
    {
        get_help_menu().add_entry(bse_arg);
    }

    /**
     * @brief       Register the specified arguement into the help menus specified by their IDs.
     * @param       bse_arg : Argument to register.
     * @param       hlp_menus_ids : IDs of the help menu in which register the argument.
     */
    template<typename... Ts_>
    void register_into_help_menus(base_arg_type* bse_arg, Ts_&&... hlp_menus_ids)
    {
        int foreach[sizeof...(Ts_) + 1] = { (
                get_help_menu(std::forward<Ts_>(hlp_menus_ids)).add_entry(bse_arg), 0)... };
    }

    /**
     * @brief       Remove an arguement to every help menu specified by their IDs.
     * @param       bse_arg : Argument to remove.
     * @param       hlp_menus_ids : IDs of the help menus in which remove.
     */
    void remove_from_help_menus(
            base_arg_type* bse_arg,
            const unordered_set_type<string_type>& hlp_menus_ids
    )
    {
        if (hlp_menus_ids.empty())
        {
            get_help_menu().remove_entry(bse_arg);
        }
        else
        {
            for (auto &hlp_menu_id: hlp_menus_ids)
            {
                get_help_menu(hlp_menu_id).remove_entry(bse_arg);
            }
        }
    }

    /**
     * @brief       Destroy and deallocate the specified argument.
     * @param       arg : The specified argument.
     */
    void delete_base_arg(base_arg_type*& arg) noexcept
    {
        key_arg_type* ky_arg;
        help_arg_type* hlp_arg;
        version_arg_type* vers_arg;
        key_value_arg_type* ky_val_arg;
        keyless_arg_type* kyless_arg;

        if ((hlp_arg = dynamic_cast<help_arg_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(help_arg_type_alloc_, hlp_arg);
        }
        else if ((ky_val_arg = dynamic_cast<key_value_arg_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(key_value_arg_type_alloc_, ky_val_arg);
        }
        else if ((kyless_arg = dynamic_cast<keyless_arg_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(keyless_arg_type_alloc_, kyless_arg);
        }
        else if ((vers_arg = dynamic_cast<version_arg_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(version_arg_type_alloc_, vers_arg);
        }
        else if ((ky_arg = dynamic_cast<key_arg_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(key_arg_type_alloc_, ky_arg);
        }

        arg = nullptr;
    }

    /**
     * @brief       Destroy and deallocate the specified argument.
     * @param       arg : The specified argument.
     */
    void delete_arg_constraint(arg_constraint_type*& arg) noexcept
    {
        one_or_more_constraint_type* at_least_one_fnd;
        mutually_exclusive_type* mutually_excl;

        if ((at_least_one_fnd = dynamic_cast<one_or_more_constraint_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(at_least_one_found_type_alloc_,
                                                   at_least_one_fnd);
        }
        else if ((mutually_excl = dynamic_cast<mutually_exclusive_type*>(arg)) != nullptr)
        {
            speed::memory::destruct_and_deallocate(mutually_exclusive_type_alloc_,
                                                   mutually_excl);
        }

        arg = nullptr;
    }

    /**
     * @brief       Exit the program with the value specified.
     * @param       val : Value that the program will return.
     */
    inline void exit_program(int val) const noexcept
    {
        this->~basic_arg_parser();
        exit(val);
    }

    /**
     * @brief       Parse the specified key arg.
     * @param       argc : Reference to the number of arguments found in the program call.
     * @param       argv : Arguments found in the program call.
     * @param       ky_arg : The key arg to parse.
     * @param       cur_idx : The current index checked in argv.
     * @param       pos_increment : How much the index will be increased afther the parsing.
     */
    template<typename TpArgc_, typename TpArgv_>
    void parse_key_arg(
            const TpArgc_& argc,
            const TpArgv_& argv,
            key_arg_type *ky_arg,
            std::size_t cur_idx,
            std::size_t* pos_increment
    )
    {
        *pos_increment = 0;
        auto* ky_val_arg = dynamic_cast<key_value_arg_type*>(ky_arg);

        if (ky_val_arg != nullptr)
        {
            for (*pos_increment = 1;
                 !ky_val_arg->max_values_reached() &&
                         cur_idx + *pos_increment < argc &&
                         string_can_be_value(ky_val_arg, argv[cur_idx + *pos_increment]);
                 ++*pos_increment)
            {
                if (ky_val_arg->min_values_reached())
                {
                    if (!ky_val_arg->try_add_value(argv[cur_idx + *pos_increment]))
                    {
                        break;
                    }
                }
                else
                {
                    ky_val_arg->add_value(argv[cur_idx + *pos_increment]);
                }
            }

            --*pos_increment;
        }

        ky_arg->execute_action();
        ky_arg->set_found(true);
        ky_arg->parse_sub_arg_parser(argc, argv, cur_idx, pos_increment);
    }

    /**
     * @brief       Reset the arguments parse.
     */
    void reset_args_parse() noexcept
    {
        value_arg_type* value_arg;

        parsd_ = false;

        for (auto& bse_arg : bse_arg_list_)
        {
            bse_arg->set_found(false);

            if ((value_arg = dynamic_cast<value_arg_type*>(bse_arg)) != nullptr)
            {
                value_arg->clear_values();
            }
        }

        unrecog_args_.clear();

        update_error_flags();
    }

    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept
    {
        bool err_fnd = false;

        for (auto& bse_arg : bse_arg_list_)
        {
            bse_arg->update_error_flags();
            if (bse_arg->has_errors() && !err_fnd)
            {
                err_fnd = true;
                err_flgs_.set(arg_parser_error_flags::ARGS_ERROR);
            }
        }
        if (!err_fnd)
        {
            err_flgs_.unset(arg_parser_error_flags::ARGS_ERROR);
        }

        if (!unrecog_args_.empty())
        {
            err_flgs_.set(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }
        else
        {
            err_flgs_.unset(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }

        err_fnd = false;
        for (auto& constrnt : constrnts_)
        {
            if (constrnt->violed())
            {
                err_fnd = true;
                err_flgs_.set(arg_parser_error_flags::ARGS_CONSTRAINTS_ERROR);
                break;
            }
        }
        if (!err_fnd)
        {
            err_flgs_.unset(arg_parser_error_flags::ARGS_CONSTRAINTS_ERROR);
        }
    }

    /**
     * @brief       Update the key arguments after the prifixes changed.
     */
    void update_arg_keys_prefixes()
    {
        key_arg_type* ky_arg;

        for (auto& bse_arg : bse_arg_list_)
        {
            if ((ky_arg = dynamic_cast<key_arg_type*>(bse_arg)) != nullptr)
            {
                ky_arg->update_prefixes();
            }
        }
    }

    /**
     * @brief       Get a reference to a base argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned,
     *              otherwise a null pointer is returned.
     */
    [[nodiscard]] base_arg_type* get_base_arg(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return it->second;
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a key argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    [[nodiscard]] key_arg_type* get_key_arg(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<key_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a value argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    [[nodiscard]] value_arg_type* get_value_arg(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a key value argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    [[nodiscard]] key_value_arg_type* get_key_value_arg(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<key_value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a keyless argument.
     * @param       usage_ky : Usage key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    [[nodiscard]] keyless_arg_type* get_keyless_arg(const string_type& usage_ky) const noexcept
    {
        auto it = bse_arg_map_.find(usage_ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<keyless_arg_type*>(it->second);
        }
        
        return nullptr;
    }

    /**
     * @brief       Get a reference to a help argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    [[nodiscard]] help_arg_type* get_help_arg(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<help_arg_type*>(it->second);
        }
        
        return nullptr;
    }

    /**
     * @brief       Get the help menu specified an ID.
     * @param       hlp_menu_id : ID of the help menu to get.
     * @return      The help menu related witht he specified ID.
     */
    template<typename TpString_ = string_type>
    [[nodiscard]] help_menu_type& get_help_menu(TpString_&& hlp_menu_id = string_type()) noexcept
    {
        auto it = hlp_menu_map_.find(hlp_menu_id);
        
        if (it == hlp_menu_map_.end())
        {
            it = hlp_menu_map_.emplace(std::forward<TpString_>(hlp_menu_id),
                    help_menu_type(this)).first;
        }

        return it->second;
    }

    /**
     * @brief       Get the number of arguments that are options.
     * @return      The number of arguments that are options.
     */
    [[nodiscard]] inline std::size_t get_nr_options() const noexcept
    {
        key_arg_type* ky_arg;
        std::size_t nr_options_bldr = 0;
        std::size_t nr_term_not_always_requird = 0;

        for (auto& bse_arg : bse_arg_list_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bse_arg);
            if (ky_arg != nullptr)
            {
                if (!ky_arg->is_flag_set(arg_flags::MANDATORY))
                {
                    if (!ky_arg->is_flag_set(arg_flags::TERMINAL) &&
                        !ky_arg->is_flag_set(arg_flags::PKILL_AFTER_TRIGGERING))
                    {
                        speed::safety::try_addm(&nr_options_bldr, 1);
                    }
                    else
                    {
                        speed::safety::try_addm(&nr_term_not_always_requird, 1);
                    }
                }
            }
        }

        return (nr_options_bldr == 0 && nr_term_not_always_requird > 0) ? 1 : nr_options_bldr;
    }

    /**
     * @brief       Get the program name.
     * @return      The program name.
     */
    [[nodiscard]] inline const string_type& get_program_name() const noexcept
    {
        return prog_name_;
    }

    /**
     * @brief       Get the prefix of the specified key.
     * @param       ky : String that contains the prefix.
     * @return      If function was successful the prefix is returned, if not an empty string is
     *              returned.
     */
    [[nodiscard]] string_type get_key_prefix(const string_type& ky) const
    {
        auto long_prefx_it = long_prefxs_.begin();
        auto short_prefx_it = short_prefxs_.begin();
        auto res_long_prefx_it = long_prefxs_.end();
        auto res_short_prefx_it = short_prefxs_.end();
        std::size_t cur_len;

        for (; long_prefx_it != long_prefxs_.end(); ++long_prefx_it)
        {
            cur_len = long_prefx_it->length();
            if (ky.compare(0, cur_len, *long_prefx_it) == 0)
            {
                if (res_long_prefx_it == long_prefxs_.end() ||
                    res_long_prefx_it->length() < cur_len)
                {
                    res_long_prefx_it = long_prefx_it;
                }
            }
        }

        for (; short_prefx_it != short_prefxs_.end(); ++short_prefx_it)
        {
            cur_len = short_prefx_it->length();
            if (ky.compare(0, cur_len, *short_prefx_it) == 0)
            {
                if (res_short_prefx_it == short_prefxs_.end() ||
                    res_short_prefx_it->length() < cur_len)
                {
                    res_short_prefx_it = short_prefx_it;
                }
            }
        }

        if (res_long_prefx_it != long_prefxs_.end() && res_short_prefx_it != short_prefxs_.end())
        {
            return (res_long_prefx_it->length() >= res_short_prefx_it->length()) ?
                   *res_long_prefx_it : *res_short_prefx_it;
        }

        return res_long_prefx_it != long_prefxs_.end() ? *res_long_prefx_it :
               res_short_prefx_it != short_prefxs_.end() ? *res_short_prefx_it :
               string_type();
    }

    /**
     * @brief       Set the error id used by the parser for generic errors.
     * @param       err_id : Error id used by the parser for generic errors.
     */
    template<typename TpString_>
    inline void set_error_id(TpString_&& err_id) noexcept
    {
        err_id_ = std::forward<TpString_>(err_id);
    }

    /**
     * @brief       Set an argument parser flag.
     * @param       flg : Flag to set.
     */
    inline void set_flag(arg_parser_flags flg) noexcept
    {
        flgs_.set(flg);
    }

    /**
     * @brief       Set the long prefixes.
     * @param       prefxs : The long prefixes.
     */
    template<typename... Ts_>
    void set_long_prefixes(Ts_&&... prefxs)
    {
        long_prefxs_.clear();
        int foreach[sizeof...(Ts_) + 1] = { (long_prefxs_.emplace(prefxs), 0)... };
        update_arg_keys_prefixes();
    }

    /**
     * @brief       Set the maximum amount of unrecognized arguments.
     * @param       max : The maximum amount of unrecognized arguments.
     */
    inline void set_maximum_unrecognized_args(std::size_t max) noexcept
    {
        max_unrecog_args_ = max;
    }

    /**
     * @brief       Set the short prefixes.
     * @param       prefxs : The short prefixes.
     */
    template<typename... Ts_>
    void set_short_prefixes(Ts_&&... prefxs)
    {
        short_prefxs_.clear();
        int foreach[sizeof...(Ts_) + 1] = { (short_prefxs_.emplace(prefxs), 0)... };
        update_arg_keys_prefixes();
    }

    /**
     * @brief       Set the program name.
     * @param       prog_name : The program name.
     */
    template<typename TpString_>
    inline void set_program_name(TpString_&& prog_name)
    {
        prog_name_ = std::forward<TpString_>(prog_name);
    }

    /**
     * @brief       Unset an argument parser flag.
     * @param       flg : Flag to unset.
     */
    inline void unset_flag(arg_parser_flags flg) noexcept
    {
        flgs_.unset(flg);
    }

    /**
     * @brief       Allows knowing whether a string contains an eq operator.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool arg_has_eq_operator(const string_type& str) const
    {
        key_arg_type* ky_arg;
        std::size_t eq_pos;

        if (str.size() > 2)
        {
            eq_pos = str.find('=', 1);
            if (eq_pos != string_type::npos)
            {
                ky_arg = get_key_arg(str.substr(0, eq_pos));
                return ky_arg != nullptr && ky_arg->is_flag_set(arg_flags::ASSIGNMENT_OPERATOR);
            }
        }

        return false;
    }

    /**
     * @brief       Allows knowing whether an argument key has been already added.
     * @param       ky : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool arg_key_exists(const string_type& ky) const noexcept
    {
        return bse_arg_map_.count(ky) > 0;
    }

    /**
     * @brief       Allows knowing whether a string are composed by chained arguments.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool chained_args_exists(const string_type& str) const
    {
        string_type ky_builder = get_key_prefix(str);
        key_arg_type* ky_arg = nullptr;

        ky_builder += ' ';
        for (auto i = ky_builder.size() - 1; i < str.size(); ++i)
        {
            ky_builder[ky_builder.size() - 1] = str[i];
            ky_arg = get_key_arg(ky_builder);
            if (ky_arg == nullptr || !ky_arg->is_flag_set(arg_flags::GROUPING))
            {
                return false;
            }
        }

        return ky_arg != nullptr;
    }

    /**
     * @brief       Allows knowing whether colors are enalbled.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool colors_enabled() const noexcept
    {
        return flgs_.is_set(arg_parser_flags::USE_COLORS);
    }

    /**
     * @brief       Allows knowing whether a string can't be an argument value due to the
     *              presece of a prefix while the value argument doesn't allow it.
     * @param       val_arg : The value arg to consider for the checking.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool has_value_with_prefix_error(
            value_arg_type* val_arg,
            const string_type& str
    ) const
    {
        return value_has_prefix(str) && !val_arg->is_flag_set(arg_flags::VALUES_WITH_PREFIX);
    }

    /**
     * @brief       Allows knowing whether the parse of the arguements have been done.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool has_parsed() const noexcept
    {
        return parsd_;
    }

    /**
     * @brief       Allows knowing whether an error flag is set.
     * @param       flg : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_error_flag_set(arg_parser_error_flags flg) const noexcept
    {
        return err_flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether a flag is set.
     * @param       flg : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_flag_set(arg_parser_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether a key prefix is long.
     * @param       ky : String that contains the prefix
     * @return      If function was successful true is returned, if not false is returned.
     */
    [[nodiscard]] bool is_key_prefix_long(const string_type& ky) const
    {
        std::size_t long_prefx_len = 0;
        std::size_t short_prefx_len = 0;
        std::size_t cur_len;

        for (auto& long_prefx : long_prefxs_)
        {
            cur_len = long_prefx.length();

            if (ky.compare(0, cur_len, long_prefx) == 0)
            {
                if (long_prefx_len < cur_len)
                {
                    long_prefx_len = cur_len;
                }
            }
        }

        for (auto& short_prefx : short_prefxs_)
        {
            cur_len = short_prefx.length();

            if (ky.compare(0, cur_len, short_prefx) == 0)
            {
                if (short_prefx_len < cur_len)
                {
                    short_prefx_len = cur_len;
                }
            }
        }

        if (long_prefx_len > short_prefx_len)
        {
            return true;
        }

        return false;
    }

    /**
     * @brief       Allows knowing whether a string can be interpreted as an argument value.
     * @param       val_arg : The value arg that the value is currecntly related.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool string_can_be_value(value_arg_type* val_arg, const string_type& str) const
    {
        return !has_value_with_prefix_error(val_arg, str) &&
               (!arg_key_exists(str) || val_arg->is_flag_set(arg_flags::KEYS_AS_VALUES)) &&
               !arg_has_eq_operator(str) && !chained_args_exists(str);
    }

    /**
     * @brief       Print the help menu or version information if it is necessary.
     */
    void trigger_prints()
    {
        help_arg_type* hlp_arg;

        for (auto& bse_arg : bse_arg_list_)
        {
            hlp_arg = dynamic_cast<help_arg_type*>(bse_arg);
            if (hlp_arg != nullptr &&
                hlp_arg->was_found() &&
                    hlp_arg->is_flag_set(arg_flags::TRIGGER_HELP_PRINTING))
            {
                print_help(hlp_arg->get_help_menu_id_assigned());
                if (hlp_arg->is_flag_set(arg_flags::PKILL_AFTER_TRIGGERING))
                {
                    exit_program(0);
                }
            }
        }

        if (current_vers_arg_ != nullptr &&
            current_vers_arg_->was_found() &&
                current_vers_arg_->is_flag_set(arg_flags::TRIGGER_VERSION_PRINTING))
        {
            print_version();
            if (current_vers_arg_->is_flag_set(arg_flags::PKILL_AFTER_TRIGGERING))
            {
                exit_program(0);
            }
        }
    }

    /**
     * @brief       Allows knowing whether a specified value has a prefix.
     * @param       val : Value to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool value_has_prefix(const string_type& val) const
    {
        for (auto& long_prefx : long_prefxs_)
        {
            if (val.compare(0, long_prefx.length(), long_prefx) == 0)
            {
                return true;
            }
        }

        for (auto& short_prefx : short_prefxs_)
        {
            if (val.compare(0, short_prefx.length(), short_prefx) == 0)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief       Print the usage message.
     */
    void print_usage()
    {
        std::size_t nr_options = get_nr_options();

        std::cout << "Usage: ";

        if (!prog_name_.empty())
        {
            std::cout << prog_name_ << ' ';
        }
        else
        {
            std::cout << "???" << ' ';
        }

        if (nr_options > 0)
        {
            std::cout << "[OPTION]";

            if (nr_options > 1)
            {
                std::cout << "...";
            }
        }

        print_commands_usage();
        print_constraints_usage();
        print_values_usage();

        std::cout << "\n\n";
    }

    /**
     * @brief       Print the commands usage.
     */
    inline void print_commands_usage()
    {
        key_arg_type* ky_arg;

        for (auto& bse_arg : bse_arg_list_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bse_arg);
            if (ky_arg != nullptr && !ky_arg->is_option())
            {
                std::cout << ' ';
                ky_arg->print_usage();
            }
        }
    }

    /**
     * @brief       Pring the values usage.
     */
    inline void print_values_usage()
    {
        keyless_arg_type * kyless_arg;

        for (auto& bse_arg : bse_arg_list_)
        {
            kyless_arg = dynamic_cast<keyless_arg_type*>(bse_arg);
            if (kyless_arg != nullptr)
            {
                std::cout << ' ';
                kyless_arg->print_usage();
            }
        }
    }

    /**
     * @brief       Print the constraints usage.
     */
    inline void print_constraints_usage()
    {
        if (constrnts_.empty())
        {
            return;
        }

        std::cout << " ";

        for (auto it = constrnts_.begin(); ; )
        {
            (*it)->print_usage();

            if (++it != constrnts_.end())
            {
                std::cout << " ";
            }
            else
            {
                break;
            }
        }
    }

private:
    /** Map allowing access to a base arg from one of its keys. */
    unordered_map_type<string_type, base_arg_type*> bse_arg_map_;

    /** Map allowing access to a help menu from its key. */
    unordered_map_type<string_type, help_menu_type> hlp_menu_map_;

    /** Contains the short prefixes for arguments. */
    unordered_set_type<string_type> short_prefxs_;

    /** Contains the long prefixes for arguments. */
    unordered_set_type<string_type> long_prefxs_;

    /** Contains the program name. */
    string_type prog_name_;

    /** Error id that will be used for global errors. */
    string_type err_id_;

    /** Contains all the arguments. */
    vector_type<base_arg_type*> bse_arg_list_;

    /** Contains all the keyless arguments. */
    vector_type<keyless_arg_type*> kyless_arg_list_;

    /** Collection of arguments constraints. */
    vector_type<arg_constraint_type*> constrnts_;

    /** Contains the unrecognized arguments if an error happen. */
    vector_type<string_type> unrecog_args_;

    /** Reference to the first added help argument. */
    help_arg_type* default_hlp_arg_;

    /** Contains the current version argument. */
    version_arg_type* current_vers_arg_;
    
    /** Contains the maximum number of unrecognized args to be catched. */
    std::size_t max_unrecog_args_;
    
    /** Flags that dictates the argument parser behavior. */
    flags_type<arg_parser_flags> flgs_;
    
    /** Argument parser error flags. */
    flags_type<arg_parser_error_flags> err_flgs_;

    /** Indicates whether the parsing has been done. */
    bool parsd_;

    /** Allocator of the key_arg_type. */
    allocator_type<key_arg_type> key_arg_type_alloc_;

    /** Allocator of the help_arg_type. */
    allocator_type<help_arg_type> help_arg_type_alloc_;

    /** Allocator of the version_arg_type. */
    allocator_type<version_arg_type> version_arg_type_alloc_;

    /** Allocator of the key_value_arg_type. */
    allocator_type<key_value_arg_type> key_value_arg_type_alloc_;

    /** Allocator of the keyless_arg_type. */
    allocator_type<keyless_arg_type> keyless_arg_type_alloc_;

    /** Allocator of the at_least_one_found_type. */
    allocator_type<one_or_more_constraint_type> at_least_one_found_type_alloc_;

    /** Allocator of the mutually_exclusive_type. */
    allocator_type<mutually_exclusive_type> mutually_exclusive_type_alloc_;
    
    friend class basic_arg_key<TpAllocator>;
    friend class basic_arg_value<TpAllocator>;
    friend class basic_base_arg<TpAllocator>;
    friend class basic_key_arg<TpAllocator>;
    friend class basic_value_arg<TpAllocator>;
    friend class basic_version_arg<TpAllocator>;
    friend class basic_keyless_arg<TpAllocator>;
    friend class basic_key_value_arg<TpAllocator>;
    friend class basic_help_arg<TpAllocator>;
    friend class basic_arg_constraint<TpAllocator>;
    friend class basic_help_menu<TpAllocator>;
    friend class basic_arg_parser_setter<TpAllocator>;
};


/** Class used to parse arguments. */
using arg_parser = basic_arg_parser<std::allocator<int>>;


}


#endif
