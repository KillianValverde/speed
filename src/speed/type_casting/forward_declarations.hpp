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
 * @file        speed/type_casting/forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#ifndef SPEED_TYPE_CASTING_FORWARD_DECLARATIONS_HPP
#define SPEED_TYPE_CASTING_FORWARD_DECLARATIONS_HPP

#include <system_error>


namespace speed::type_casting {


template<typename TpTarget, typename TpSource, typename TpComponent>
class basic_type_caster;

class error_category;

enum class error_codes : std::uint8_t;

enum class error_conditions : std::uint8_t;

template<typename TpTarget, typename TpSource>
bool try_type_cast(const TpSource&, TpTarget*, std::error_code* = nullptr) noexcept;

template<typename TpTarget, typename TpSource>
TpTarget type_cast(const TpSource&);

template<typename TpTarget, typename TpSource, typename TpDefaultValue>
TpTarget type_cast(const TpSource&, TpDefaultValue&&) noexcept;

template<typename TpSource>
class type_caster_base;

inline void assign_type_casting_error_code(int, std::error_code*);

inline std::error_condition make_error_condition(speed::type_casting::error_conditions);


}


#endif
