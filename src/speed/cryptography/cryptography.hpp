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
 * @file        speed/cryptography/cryptography.hpp
 * @brief       cryptography main header
 * @author      Killian Valverde
 * @date        2024/11/01
 */

#ifndef SPEED_CRYPTOGRAPHY_CRYPTOGRAPHY_HPP
#define SPEED_CRYPTOGRAPHY_CRYPTOGRAPHY_HPP

#include "city_hash.hpp"


namespace speed {


/**
 * @brief       Contains cryptography operation resources.
 */
namespace cryptography {}


#ifndef SPEED_DISABLE_ALIAS

/**
 * @brief       Contains cryptography operation resources.
 */
namespace crypt = cryptography;

#endif


}


#endif
