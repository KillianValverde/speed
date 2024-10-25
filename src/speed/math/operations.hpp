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
 * @file        speed/math/operations.hpp
 * @brief       math functions header.
 * @author      Killian Valverde
 * @date        2017/01/28
 */

#ifndef SPEED_MATH_OPERATIONS_HPP
#define SPEED_MATH_OPERATIONS_HPP

#include <type_traits>


namespace speed::math {


/**
 * @brief       Get the absolute number of a specified integral.
 * @param       val : The integral number to get the absolute number.
 * @return      The absolute number of the specified integral.
 */
template<typename TpIntegral>
inline std::enable_if_t<
        std::is_signed_v<TpIntegral>,
        TpIntegral
>
abs(TpIntegral val) noexcept
{
    if (val < 0)
    {
        return val * -1;
    }
    
    return val;
}


/**
 * @brief       Get the absolute number of a specified integral.
 * @param       val : The integral number to get the absolute number.
 * @return      The absolute number of the specified integral.
 */
template<typename TpIntegral>
inline std::enable_if_t<
        std::is_unsigned_v<TpIntegral>,
        TpIntegral
>
abs(TpIntegral val) noexcept
{
    return val;
}


}


#endif
