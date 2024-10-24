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
 * @file        speed/argparse/forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2018/10/27
 */

#ifndef SPEED_ARGPARSE_FORWARD_DECLARATIONS_HPP
#define SPEED_ARGPARSE_FORWARD_DECLARATIONS_HPP


namespace speed::argparse {


template<typename TpAllocator>
class basic_arg_key;

template<typename TpAllocator>
class basic_arg_value;

template<typename TpAllocator>
class basic_base_arg;

template<typename TpAllocator>
class basic_key_arg;

template<typename TpAllocator>
class basic_value_arg;

template<typename TpAllocator>
class basic_version_arg;

template<typename TpAllocator>
class basic_keyless_arg;

template<typename TpAllocator>
class basic_key_value_arg;

template<typename TpAllocator>
class basic_help_arg;

template<typename TpAllocator>
class basic_arg_constraint;

template<typename TpBase, typename TpAllocator>
class basic_one_or_more_constraint;

template<typename TpBase, typename TpAllocator>
class basic_mutually_exclusive;

template<typename TpAllocator>
class basic_help_menu;

template<typename TpAllocator>
class basic_arg_parser;

template<typename TpAllocator, typename TpActual>
class basic_base_arg_setter;

template<typename TpAllocator, typename TpActual>
class basic_key_arg_setter;

template<typename TpAllocator, typename TpActual>
class basic_value_arg_setter;

template<typename TpAllocator, typename TpActual>
class basic_version_arg_setter;

template<typename TpAllocator, typename TpActual>
class basic_keyless_arg_setter;

template<typename TpAllocator, typename TpActual>
class basic_key_value_arg_setter;

template<typename TpAllocator, typename TpActual>
class basic_help_arg_setter;

template<typename TpAllocator>
class basic_help_menu_setter;

template<typename TpAllocator>
class basic_arg_parser_setter;


}


#endif
