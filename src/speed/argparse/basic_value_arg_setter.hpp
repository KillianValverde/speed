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
 * @file        speed/argparse/basic_value_arg_setter.hpp
 * @brief       basic_value_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_BASIC_VALUE_ARG_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_VALUE_ARG_SETTER_HPP

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../type_traits/type_traits.hpp"
#include "arg_flags.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_value_arg.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring a value argument.
 */
template<typename TpAllocator, typename TpActual>
class basic_value_arg_setter : virtual public basic_base_arg_setter<TpAllocator, TpActual>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** Array type used in the class. */
    template<typename T, std::size_t N>
    using array_type = std::array<T, N>;

    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;

    /** Double ended queue type used in the class. */
    template<typename T>
    using deque_type = std::deque<T, allocator_type<T>>;

    /** Queue type used in the class. */
    template<typename T>
    using queue_type = std::queue<T, deque_type<T>>;

    /** Priority queue type used in the class. */
    template<typename T>
    using priority_queue_type = std::priority_queue<T, vector_type<T>>;

    /** Stack type used in the class. */
    template<typename T>
    using stack_type = std::stack<T, deque_type<T>>;

    /** Forward list type used in the class. */
    template<typename T>
    using forward_list_type = std::forward_list<T, allocator_type<T>>;

    /** Forward list type used in the class. */
    template<typename T>
    using list_type = std::list<T, allocator_type<T>>;

    /** Set type used in the class. */
    template<typename T>
    using set_type = std::set<T, std::less<T>, allocator_type<T>>;

    /** Unordered set type used in the class. */
    template<typename T>
    using unordered_set_type = std::unordered_set<
            T, std::hash<T>, std::equal_to<T>, allocator_type<T>>;

    /** Unordered multi set type used in the class. */
    template<typename T>
    using unordered_multiset_type = std::unordered_multiset<
            T, std::hash<T>, std::equal_to<T>, allocator_type<T>>;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = basic_base_arg_setter<TpAllocator, TpActual>;

    /** Type that represents the actual type of the object. */
    using self_type = TpActual;

    /**
     * @brief       Default constructor.
     */
    basic_value_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       val_arg : Value argument that this object will configure.
     */
    explicit basic_value_arg_setter(value_arg_type* val_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(val_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_value_arg_setter(const basic_value_arg_setter& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_value_arg_setter(basic_value_arg_setter&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_value_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_value_arg_setter& operator =(const basic_value_arg_setter& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_value_arg_setter& operator =(basic_value_arg_setter&& rhs) noexcept = default;

    /**
     * @brief       Allows to specify whether keys can be used as values. By default this feature
     *              is disabled for all kind of arguements.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& keys_as_values(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::KEYS_AS_VALUES);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::KEYS_AS_VALUES);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the minimum and maximum amount of values that the arguement
     *              can have. Key value arguements have both at 1 by default, and help arguments
     *              bot at 0 by default.
     * @param       min : The minimum amount of values.
     * @param       max : The maximum amount of values.
     * @return      The object who call the method.
     */
    self_type& minmax_values(std::size_t min, std::size_t max)
    {
        auto val_arg = dynamic_cast<value_arg_type*>(base_arg_setter_type::bse_arg_);
        val_arg->set_max_values_auto_update(false);
        val_arg->set_minmax_values(min, max);
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the regexes that will be assigned to each value. Each value
     *              will have assigned the regex that corresponds to their index. If there is more
     *              values than regexes, the remaining values will be asigned to the last specified
     *              regex. If there isn't any regex assigned, there will be no verification.
     * @param       regxes : Set of regexes that the values will have to match.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& regexes(Ts_&&... regxes)
    {
        dynamic_cast<value_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_regexes(std::forward<Ts_>(regxes)...);
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the types in which the values will be casted. Each value
     *              will have assigned the type that corresponds to their index. If there is more
     *              values than types, the remaining values will be asigned to the last specified
     *              type.
     * @tparam      Ts_ : The types in which cast the values.
     * @return      The object who call the method.
     */
    // template<typename... Ts_>
    // self_type& scan()
    // {
    //     dynamic_cast<value_arg_type*>(base_arg_setter_type::bse_arg_)
    //             ->template set_casters<Ts_...>();
    //     return dynamic_cast<self_type&>(*this);
    // }

    /**
     * @brief       Allows to specify the holders that will be assigned to each value. Each value
     *              will have assigned the holder that corresponds to their index. If there is more
     *              values than holders, the remaining values will not be assigned to any holder.
     *              Calling the store_into method will wipe the previous pecified holders of the
     *              argument.
     * @param       holdrs : Set of holders in which the values will store their cast results.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& store_into(Ts_*... holdrs)
    {
        dynamic_cast<value_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_holders(holdrs...);
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether values can contain prefixes. By default this feature
     *              is disabled for all kind of arguements. It major usage will be aimed towards
     *              being able to get negative numbers from the program call.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& values_with_prefix(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::VALUES_WITH_PREFIX);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::VALUES_WITH_PREFIX);
        }

        return dynamic_cast<self_type&>(*this);
    }
};


}


#endif
