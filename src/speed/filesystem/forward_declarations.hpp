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
 * @file        speed/filesystem/forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#ifndef SPEED_FILESYSTEM_FORWARD_DECLARATIONS_HPP
#define SPEED_FILESYSTEM_FORWARD_DECLARATIONS_HPP


namespace speed::filesystem {


class directory_iteration;

class valid_path;

template<typename TpComponent>
class read_path_decorator;

template<typename TpComponent>
class write_path_decorator;

template<typename TpComponent>
class execute_path_decorator;

template<typename TpComponent>
class regular_file_path_decorator;

template<typename TpComponent>
class directory_path_decorator;

template<typename TpComponent>
class output_regular_file_path_decorator;

template<typename TpComponent>
class output_directory_path_decorator;


}


#endif
