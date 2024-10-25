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
 * @file        speed/argparse/basic_value_arg.hpp
 * @brief       basic_value_arg class header.
 * @author      Killian Valverde
 * @date        2016/03/10
 */

#ifndef SPEED_ARGPARSE_BASIC_VALUE_ARG_HPP
#define SPEED_ARGPARSE_BASIC_VALUE_ARG_HPP

#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "../type_casting/type_casting.hpp"
#include "arg_flags.hpp"
#include "basic_arg_parser.hpp"
#include "basic_arg_value.hpp"
#include "basic_base_arg.hpp"
#include "basic_type_caster.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents arguments that have values.
 */
template<typename TpAllocator>
class basic_value_arg : public virtual basic_base_arg<TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** Shared pointer type used in the class. */
    template<typename T>
    using shared_ptr_type = std::shared_ptr<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Regex type used in the class. */
    using regex_type = std::basic_regex<char, std::regex_traits<char>>;

    /** Pair type used in the class. */
    template<typename T1, typename T2>
    using pair_type = std::pair<T1, T2>;

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

    /** Type that represents the caster base type. */
    using caster_base_type = speed::type_casting::type_caster_base<string_type>;

    /** Type that represents the caster type. */
    template<typename T>
    using caster_type = speed::argparse::basic_type_caster<T, string_type>;

    /** Type that represents the array caster type. */
    template<typename T, std::size_t N>
    using array_caster_type = speed::argparse::basic_array_caster<
            T, string_type, allocator_type<T>, N>;

    /** Type that represents the vector caster type. */
    template<typename T>
    using vector_caster_type = speed::argparse::basic_vector_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the deque caster type. */
    template<typename T>
    using deque_caster_type = speed::argparse::basic_deque_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the queue caster type. */
    template<typename T>
    using queue_caster_type = speed::argparse::basic_queue_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the priority queue caster type. */
    template<typename T>
    using priority_queue_caster_type = speed::argparse::basic_priority_queue_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the stack caster type. */
    template<typename T>
    using stack_caster_type = speed::argparse::basic_stack_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the forward list type. */
    template<typename T>
    using forward_list_caster_type = speed::argparse::basic_forward_list_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the list type. */
    template<typename T>
    using list_caster_type = speed::argparse::basic_list_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the set type. */
    template<typename T>
    using set_caster_type = speed::argparse::basic_set_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the unordered set type. */
    template<typename T>
    using unordered_set_caster_type = speed::argparse::basic_unordered_set_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents the unordered multiset type. */
    template<typename T>
    using unordered_multiset_caster_type = speed::argparse::basic_unordered_multiset_caster<
            T, string_type, allocator_type<T>>;

    /** Type that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    explicit basic_value_arg(arg_parser_type* arg_parsr)
            : base_arg_type(arg_parsr)
            , vals_()
            , castrs_()
            , regxes_()
            , minmax_vals_(1, 1)
            , max_vals_auto_update_(true)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_value_arg(const basic_value_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_value_arg(basic_value_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_value_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_value_arg& operator =(const basic_value_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_value_arg& operator =(basic_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            minmax_vals_ = std::move(rhs.minmax_vals_);
            vals_ = std::move(rhs.vals_);
            castrs_ = std::move(rhs.castrs_);
            regxes_ = std::move(rhs.regxes_);
        }
        
        return *this;
    }

    /**
     * @brief       Add a value to the argument.
     * @param       val : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool add_value(TpString_&& val)
    {
        if (!max_values_reached())
        {
            vals_.emplace_back(get_next_regex(), std::forward<TpString_>(val),
                    get_next_caster(), base_arg_type::get_arg_parser(), this);

            return true;
        }

        base_arg_type::set_error_flag(arg_error_flags::MAX_VALUES_ERROR);
        return false;
    }

    /**
     * @brief       Try to add a value to the argument. The value will just be added to the
     *              argument if this process will not raise any error flag.
     * @param       val : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool try_add_value(TpString_&& val)
    {
        if (!max_values_reached())
        {
            arg_value_type arg_val(get_next_regex(), std::forward<TpString_>(val),
                    get_next_caster(), base_arg_type::get_arg_parser(), this);

            if (!arg_val.has_errors())
            {
                vals_.emplace_back(std::move(arg_val));
                return true;
            }
        }

        return false;
    }

    /**
     * @brief       Clear values collection.
     */
    inline void clear_values() noexcept
    {
        vals_.clear();
        base_arg_type::clear_error_flags();
    }

    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept override
    {
        bool in = false;

        base_arg_type::update_error_flags();

        if (!min_values_reached() && base_arg_type::was_found())
        {
            base_arg_type::set_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }
        else
        {
            base_arg_type::unset_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }

        if (vals_.size() > minmax_vals_.second)
        {
            base_arg_type::set_error_flag(arg_error_flags::MAX_VALUES_ERROR);
            while (vals_.size() > minmax_vals_.second)
            {
                vals_.pop_back();
            }
        }

        for (auto& val : vals_)
        {
            if (val.has_errors())
            {
                in = true;
                base_arg_type::set_error_flag(arg_error_flags::VALUES_ERROR);
                break;
            }
        }
        if (!in)
        {
            base_arg_type::unset_error_flag(arg_error_flags::VALUES_ERROR);
        }
    }

    /**
     * @brief       Get the front arg value converted to target.
     * @return      The front arg value converted to target.
     */
    template<typename TpTarget_>
    [[nodiscard]] inline TpTarget_ get_front_value_as() const
    {
        if (vals_.empty())
        {
            throw value_not_found_exception();
        }

        return vals_.front().template as<TpTarget_>();
    }
    
    /**
     * @brief       Get the front arg value converted to target.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the arg value converted to target is returned, 
     *              otherwise the specified default value is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    [[nodiscard]] inline TpTarget_ get_front_value_as(TpDefaultValue_&& default_val) const noexcept
    {
        return vals_.empty() ? std::forward<TpDefaultValue_>(default_val) : 
                vals_.front().template as<TpTarget_>(std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the front arg value converted to target.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    [[nodiscard]] inline bool try_get_front_value_as(TpTarget_* res) const noexcept
    {
        return !vals_.empty() && vals_.front().template try_as<TpTarget_>(res);
    }

    /**
     * @brief       Get the specified arg value converted to target.
     * @param       indx : Position of the element.
     * @return      The specified arg value converted to target.
     */
    template<typename TpTarget_>
    [[nodiscard]] inline TpTarget_ get_value_at_as(std::size_t indx) const
    {
        if (indx >= vals_.size())
        {
            throw value_not_found_exception();
        }

        return vals_.at(indx).template as<TpTarget_>();
    }
    
    /**
     * @brief       Get the specified arg value converted to target.
     * @param       indx : Position of the element.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the arg value converted to target is returned, 
     *              otherwise the specified default value is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    [[nodiscard]] inline TpTarget_ get_value_at_as(
            std::size_t indx, 
            TpDefaultValue_&& default_val
    ) const noexcept
    {
        return indx >= vals_.size() ? std::forward<TpDefaultValue_>(default_val) : 
                vals_.at(indx).template as<TpTarget_>(std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the specified arg value converted to target.
     * @param       indx : Position of the element.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    [[nodiscard]] inline bool try_get_value_at_as(std::size_t indx, TpTarget_* res) const noexcept
    {
        return indx < vals_.size() && vals_.at(indx).template try_as<TpTarget_>(res);
    }

    /**
     * @brief       Get all the arg values converted to target.
     * @return      All the arg values converted to target.
     */
    template<typename TpTarget_>
    [[nodiscard]] inline vector_type<TpTarget_> get_values_as() const
    {
        vector_type<TpTarget_> ret_vals;

        for (auto& x : vals_)
        {
            ret_vals.emplace_back(x.template as<TpTarget_>());
        }

        return ret_vals;
    }

    /**
     * @brief       Get all the arg values converted to target.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful all the arg values converted to target are returned, 
     *              otherwise the specified default value is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_ = vector_type<TpTarget_>>
    [[nodiscard]] inline vector_type<TpTarget_> get_values_as(
        TpDefaultValue_&& default_val
    ) const noexcept
    {
        vector_type<TpTarget_> res;
        TpTarget_ val_res;

        for (auto& val : vals_)
        {
            if (val.template try_as<TpTarget_>(&val_res))
            {
                res.emplace_back(std::move(val_res));
            }
            else
            {
                return std::forward<TpDefaultValue_>(default_val);
            }
        }

        return res.empty() ? std::forward<TpDefaultValue_>(default_val) : res;
    }

    /**
     * @brief       Try to get all the arg values converted to target.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    [[nodiscard]] inline vector_type<TpTarget_> try_get_values_as(
            vector_type<TpTarget_>* res
    ) const noexcept
    {
        TpTarget_ val_res;

        res->clear();

        for (auto& val : vals_)
        {
            if (val.template try_as<TpTarget_>(&val_res))
            {
                res->emplace_back(std::move(val_res));
            }
            else
            {
                res->clear();
                return false;
            }
        }

        return !res->empty();
    }

    /**
     * @brief       Get maximum number of values for an option.
     * @return      The maximum number of values for an option.
     */
    [[nodiscard]] inline std::size_t get_max_values() const noexcept
    {
        return minmax_vals_.second;
    }

    /**
     * @brief       Get minimum number of values for an option.
     * @return      The minimum number of values for an option.
     */
    [[nodiscard]] inline std::size_t get_min_values() const noexcept
    {
        return minmax_vals_.first;
    }

    /**
     * @brief       Get the caster associated witht the current value.
     * @return      The caster associated witht the current value.
     */
    [[nodiscard]] shared_ptr_type<caster_base_type> get_next_caster()
    {
        if (vals_.size() < castrs_.size())
        {
            return castrs_[vals_.size()];
        }
        else if (!castrs_.empty())
        {
            return castrs_.back();
        }

        return nullptr;
    }

    /**
     * @brief       Get the regex associated with the current value.
     * @return      The regex associated with the current value.
     */
    [[nodiscard]] regex_type get_next_regex()
    {
        if (vals_.size() < regxes_.size())
        {
            return regxes_[vals_.size()];
        }
        else if (!regxes_.empty())
        {
            return regxes_.back();
        }

        return regex_type("^.*$");
    }

    /**
     * @brief       Get the number of values of the arguement.
     * @return      The number of values of the argument.
     */
    [[nodiscard]] std::size_t get_number_of_values() const noexcept
    {
        return vals_.size();
    }

    /**
     * @brief       Get a string composed by all the values.
     * @return      A string composed by all the values.
     */
    [[nodiscard]] string_type get_values_as_string() const
    {
        string_type vals_str;
        std::size_t cnt = 0;

        for (auto& val : vals_)
        {
            vals_str += val.get_value();

            if (cnt > 0)
            {
                vals_str += " ";
            }
            else
            {
                ++cnt;
            }
        }

        return vals_str;
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_, std::size_t Nm>
    void set_holders(array_type<TpValue_, Nm>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<array_caster_type<TpValue_, Nm>>(
                allocator_type<array_caster_type<TpValue_, Nm>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(vector_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<vector_caster_type<TpValue_>>(
                allocator_type<vector_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(deque_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<deque_caster_type<TpValue_>>(
                allocator_type<deque_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(queue_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<queue_caster_type<TpValue_>>(
                allocator_type<queue_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(priority_queue_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<priority_queue_caster_type<TpValue_>>(
                allocator_type<priority_queue_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(stack_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<stack_caster_type<TpValue_>>(
                allocator_type<stack_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(forward_list_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<forward_list_caster_type<TpValue_>>(
                allocator_type<forward_list_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(list_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<list_caster_type<TpValue_>>(
                allocator_type<list_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(set_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<set_caster_type<TpValue_>>(
                allocator_type<set_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(unordered_set_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<unordered_set_caster_type<TpValue_>>(
                allocator_type<unordered_set_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the collection that will get the cast result of each value.
     * @param       holdr : The collection that will get the cast result of each value.
     */
    template<typename TpValue_>
    void set_holders(unordered_multiset_type<TpValue_>* holdr)
    {
        castrs_.clear();

        castrs_.emplace_back(std::allocate_shared<unordered_multiset_caster_type<TpValue_>>(
                allocator_type<unordered_multiset_caster_type<TpValue_>>(), holdr));

        update_max_values(1);
    }

    /**
     * @brief       Set the objects that will be holding the casted values.
     * @param       holdrs : The objects that will holding the casted values.
     */
    template<typename... Ts_>
    void set_holders(Ts_*... holdrs)
    {
        castrs_.clear();

        int foreach[sizeof...(Ts_) + 1] = { (
                castrs_.emplace_back(std::allocate_shared<caster_type<Ts_>>(
                        allocator_type<caster_type<Ts_>>(), holdrs)), 0)... };

        update_max_values(castrs_.size());
    }

    /**
     * @brief       Set whether the max values have to auto-update.
     * @param       auto_updat : Wheter the max values have to auto-update.
     */
    void set_max_values_auto_update(bool auto_updat)
    {
        max_vals_auto_update_ = auto_updat;
    }

    /**
     * @brief       Set the minimum and maximum values.
     * @param       min : The minimum values.
     * @param       max : The maxium values.
     */
    virtual void set_minmax_values(std::size_t min, std::size_t max)
    {
        if (min > max)
        {
            throw wrong_min_max_interval_exception();
        }

        minmax_vals_.first = min;
        minmax_vals_.second = max;
    }

    /**
     * @brief       Set the regexes that will be associated with the values.
     * @param       regxes : Regexes that will be associated with the values.
     */
    template<typename... Ts_>
    void set_regexes(Ts_&&... regxes)
    {
        regxes_.clear();

        int foreach[sizeof...(Ts_) + 1] = { (
                regxes_.emplace_back(std::forward<Ts_>(regxes)), 0)... };

        update_max_values(regxes_.size());
    }

    /**
     * @brief       Allows knowing whether the argument has rached the minimal number of values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool min_values_reached() const noexcept
    {
        return vals_.size() >= minmax_vals_.first;
    }

    /**
     * @brief       Allows knowing whether the argument can't get more values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool max_values_reached() const noexcept
    {
        return vals_.size() >= minmax_vals_.second;
    }
    
    /**
     * @brief       Print argument errors in standard output.
     */
    void print_errors() const override
    {
        if (!base_arg_type::has_errors())
        {
            return;
        }

        base_arg_type::print_errors();
        
        if (base_arg_type::is_error_flag_set(arg_error_flags::MIN_VALUES_ERROR))
        {
            base_arg_type::print_error_message();
            std::cout << base_arg_type::get_tittle() << " requires at least " << minmax_vals_.first
                      << (minmax_vals_.first > 1 ? " values" : " value") << '\n';
        }
        
        if (base_arg_type::is_error_flag_set(arg_error_flags::MAX_VALUES_ERROR))
        {
            base_arg_type::print_error_message();
            std::cout << base_arg_type::get_tittle() << " must have a maximum of "
                      << minmax_vals_.second
                      << (minmax_vals_.second > 1 ? " values" : " value") << '\n';
        }
        
        if (base_arg_type::is_error_flag_set(arg_error_flags::VALUES_ERROR) && !vals_.empty())
        {
            for (auto& val : vals_)
            {
                if (val.has_errors())
                {
                    val.print_errors();
                }
            }
        }
    }

protected:
    /**
     * @brief       Update the maximum amount of values.
     * @param       new_max : The new maximum amount of values.
     */
    void update_max_values(std::size_t new_max)
    {
        if (!max_vals_auto_update_)
        {
            return;
        }

        if (new_max > minmax_vals_.second)
        {
            set_minmax_values(minmax_vals_.first, new_max);
        }
    }

private:
    /** Collection that has the values gotten through the program call for an argument. */
    vector_type<arg_value_type> vals_;
    
    /** Type casters used to validate the values syntax. */
    vector_type<shared_ptr_type<caster_base_type>> castrs_;
    
    /** Regular expressions that the values has to match. */
    vector_type<regex_type> regxes_;

    /** Minimum and maximum number of values for an option. */
    pair_type<std::size_t, std::size_t> minmax_vals_;

    /** Dictates whether the max values number will auto-update. */
    bool max_vals_auto_update_;
};


}


#endif
