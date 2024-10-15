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
 * @file        speed/type_casting/type_casting.hpp
 * @brief       compatibility main header.
 * @author      Killian Valverde
 * @date        2016/08/05
 */

#ifndef SPEED_TYPE_CASTING_TYPE_CASTING_HPP
#define SPEED_TYPE_CASTING_TYPE_CASTING_HPP

#include "forward_declarations.hpp"
#include "basic_type_caster.hpp"
#include "error_category.hpp"
#include "error_codes.hpp"
#include "error_conditions.hpp"
#include "exception.hpp"
#include "try_type_cast.hpp"
#include "type_cast.hpp"
#include "type_caster_base.hpp"


namespace speed {


/**
 * @brief       Contains utilities for type casting.
 */
namespace type_casting {}


#ifndef SPEED_DISABLE_ALIAS

/**
 * @brief       Contains utilities for type casting.
 */
namespace cast = type_casting;

#endif


}


#endif
