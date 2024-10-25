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
 * @file        speed/lowlevel/operations.hpp
 * @brief       operations header.
 * @author      Killian Valverde
 * @date        2018/10/09
 */

#ifndef SPEED_LOWLEVEL_OPERATIONS_HPP
#define SPEED_LOWLEVEL_OPERATIONS_HPP

#include <cstdint>
#include <utility>


namespace speed::lowlevel {


/**
 * @brief       Convert a one-hot encoding number to a binary number.
 * @param       val : The value to conver.
 * @return      If function was successful the specified value converted to binary is returned,
 *              otherwise 0 is returned.
 */
template<typename TpOnehot>
constexpr std::uint8_t onehot_to_binary(const TpOnehot& val)
{
    constexpr std::uint8_t n_bits = sizeof(val) * 8;
    
    for (std::uint8_t i = 0; i < n_bits; i++)
    {
        if (val & (1 << i))
        {
            return static_cast<std::uint8_t>(i + 1);
        }
    }
    
    return 0;
}


}


#endif
