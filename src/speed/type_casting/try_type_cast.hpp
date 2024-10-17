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
 * @file        speed/type_casting/type_cast.hpp
 * @brief       type_cast main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TRY_TYPE_CAST_HPP
#define SPEED_TYPE_CASTING_TRY_TYPE_CAST_HPP

#include <cctype>
#include <cmath>
#include <limits>
#include <system_error>
#include <type_traits>

#include "../filesystem/filesystem.hpp"
#include "../stringutils/stringutils.hpp"
#include "../type_traits/type_traits.hpp"
#include "error_category.hpp"
#include "error_codes.hpp"


namespace speed::type_casting {


/** @cond */
namespace __private {


/**
 * @brief       Tries to convert a c_string into an unsigned inegral.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_integral<TpTarget>::value &&
                std::is_unsigned<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using unsigned_type = TpTarget;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<TpSource>>>;
    using string_pointer_type = const char_type*;

    constexpr unsigned_type max_unsigned = std::numeric_limits<unsigned_type>::max();

    string_pointer_type str = arg;
    unsigned_type res_bldr = 0;
    char_type sgn;
    char_type digt;

    if (str == nullptr)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }

    while (isspace(*str))
    {
        ++str;
    }

    sgn = *str;
    if (sgn == '+')
    {
        ++str;
    }

    if (*str == '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_INVALID_SYNTAX),
            err_code);

        return false;
    }

    while ((digt = *str) != '\0')
    {
        if (!isdigit(digt))
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_INVALID_SYNTAX),
                err_code);

            return false;
        }

        digt -= '0';

        if ((res_bldr > max_unsigned / 10) ||
            (res_bldr == max_unsigned / 10 && digt > (max_unsigned % 10)))
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
                err_code);

            return false;
        }

        res_bldr = res_bldr * 10 + digt;
        ++str;
    }

    *res = res_bldr;

    return true;
}


/**
 * @brief       Tries to convert a c_string into a signed inegral.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_integral<TpTarget>::value &&
                std::is_signed<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using signed_type = TpTarget;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<TpSource>>>;
    using string_pointer_type = const char_type*;

    constexpr signed_type min_signed = std::numeric_limits<signed_type>::min();
    constexpr signed_type max_signed = std::numeric_limits<signed_type>::max();

    string_pointer_type str = arg;
    signed_type res_bldr = 0;
    char_type sgn;
    char_type digt;

    if (str == nullptr)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }

    while (isspace(*str))
    {
        ++str;
    }

    sgn = *str;
    if (sgn == '-' || sgn == '+')
    {
        ++str;
    }

    if (*str == '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_INVALID_SYNTAX),
            err_code);

        return false;
    }

    while ((digt = *str) != '\0')
    {
        if (!isdigit(digt))
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_INVALID_SYNTAX),
                err_code);

            return false;
        }

        digt -= '0';

        if ((res_bldr < min_signed / 10) ||
            (res_bldr == min_signed / 10 && digt > -(min_signed % 10)))
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
                err_code);

            return false;
        }

        res_bldr = res_bldr * 10 - digt;
        ++str;
    }

    if (sgn != '-')
    {
        if (res_bldr < -max_signed)
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
                err_code);

            return false;
        }

        res_bldr *= -1;
    }

    *res = res_bldr;

    return true;
}


// TODO: The conditions for the generation are kinda weird.
/**
 * @brief       Tries to convert a c_string into a float.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_float<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using floating_point_type = TpTarget;

    errno = 0;
    char* endptr;
    floating_point_type res_bldr;

    res_bldr = strtof(arg, &endptr);

    if (endptr == arg || *endptr != '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }

    if (res_bldr == HUGE_VALF && errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
            err_code);

        return false;
    }

    if (errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_UNDERFLOW_RANGE),
            err_code);

        return false;
    }

    *res = res_bldr;

    return true;
}


/**
 * @brief       Tries to convert a c_string into a double.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                       std::remove_pointer_t<
                       std::decay_t<
                       typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_double<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using floating_point_type = TpTarget;

    errno = 0;
    char* endptr;
    floating_point_type res_bldr;

    res_bldr = strtod(arg, &endptr);

    if (endptr == arg || *endptr != '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }

    if (res_bldr == HUGE_VAL && errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
            err_code);

        return false;
    }

    if (errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_UNDERFLOW_RANGE),
            err_code);

        return false;
    }

    *res = res_bldr;

    return true;
}


/**
 * @brief       Tries to convert a c_string into a long double.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_long_double<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using floating_point_type = TpTarget;

    errno = 0;
    char* endptr;
    floating_point_type res_bldr;

    res_bldr = strtold(arg, &endptr);

    if (endptr == arg || *endptr != '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }

    if (res_bldr == HUGE_VALL && errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
            err_code);

        return false;
    }

    if (errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_UNDERFLOW_RANGE),
            err_code);

        return false;
    }

    *res = res_bldr;

    return true;
}


/**
 * @brief       Tries to convert a c_string into a string.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_string<TpTarget>::value &&
                speed::type_traits::is_char_pointer<std::decay_t<TpSource>>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    try
    {
        std::size_t len = speed::stringutils::strlen(arg);
        res->assign(arg, arg + len);
        return true;
    }
    catch (...)
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::OTHER),
                err_code);
    }

    return false;
}


// TODO: Range error?
/**
 * @brief       Tries to convert a c_string into a wstring.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_wstring<TpTarget>::value &&
                speed::type_traits::is_char_pointer<std::decay_t<TpSource>>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    if (!speed::system::codecs::convert_c_string_to_wstring(arg, res))
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::RANGE_ERROR),
                err_code);

        return false;
    }

    return true;
}


/**
 * @brief       Tries to convert a w_string into a wstring.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_wstring<TpTarget>::value &&
                speed::type_traits::is_wchar_pointer<std::decay_t<TpSource>>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    try
    {
        std::size_t len = speed::stringutils::strlen(arg);
        res->assign(arg, arg + len);
        return true;
    }
    catch (...)
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::OTHER),
                err_code);
    }

    return false;
}


/**
 * @brief       Tries to convert a w_string into a string.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_string<TpTarget>::value &&
                speed::type_traits::is_wchar_pointer<std::decay_t<TpSource>>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    if (!speed::system::codecs::convert_w_string_to_string(arg, res))
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::RANGE_ERROR),
                err_code);

        return false;
    }

    return true;
}


// TODO: Fix this, it is not implemented correctly and the enable_if is also not as correct.
/**
 * @brief       Tries to convert a c_string into a basic_regex.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_basic_regex<TpTarget>::value &&
                speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    try
    {
        std::size_t len = speed::stringutils::strlen(arg);
        res->assign(arg, arg + len);
        return true;
    }
    catch (...)
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::OTHER),
                err_code);
    }

    return false;
}


/**
 * @brief       Tries to convert a c_string into a path.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_path<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    try
    {
        res->assign(arg);
        return true;
    }
    catch (...)
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::FILESYSTEM_INVALID_PATH),
                err_code);

        return false;
    }
}


/**
 * @brief       Tries to convert a c_string into a valid_path.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_base_of_v<speed::filesystem::valid_path, TpTarget>,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    try
    {
        res->assign(arg);
        return res->is_valid(err_code);
    }
    catch (...)
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::FILESYSTEM_INVALID_PATH),
                err_code);

        return false;
    }
}


/**
 * @brief       Tries to convert a basic_string into a type.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpChar, typename TpCharTraits, typename TpCharAlloc>
inline bool __try_type_cast(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& arg,
        TpTarget* res,
        std::error_code* err_code
) noexcept
{
    return __try_type_cast(arg.c_str(), res, err_code);
}


} /* __private */
/** @endcond */


/**
 * @brief       Tries to convert the source into the target.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
inline bool try_type_cast(
    const TpSource& arg, 
    TpTarget* res, 
    std::error_code* err_code
) noexcept
{
    return __private::__try_type_cast(arg, res, err_code);
}


}


#endif
