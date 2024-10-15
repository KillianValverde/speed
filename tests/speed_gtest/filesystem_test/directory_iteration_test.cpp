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
 * @file        speed_gtest/filesystem_test/directory_iteration_test.cpp
 * @brief       directory_iteration unit test.
 * @author      Killian Valverde
 * @date        2024/10/14
 */

#include <gtest/gtest.h>

#include "speed/filesystem/filesystem.hpp"


TEST(filesystem_directory_iteration, basic)
{
    std::string str;
    speed::filesystem::directory_iteration dir_iteration(".", "^.*\\.cpp$",
            speed::system::filesystem::file_types::REGULAR_FILE);

    for (auto dir_it = dir_iteration.begin(); dir_it != dir_iteration.end(); ++dir_it)
    {

        EXPECT_NO_THROW(str = dir_it->string());
    }

    for (auto dir_it = dir_iteration.cbegin(); dir_it != dir_iteration.cend(); ++dir_it)
    {
        EXPECT_NO_THROW(str = dir_it->string());
    }

    for (auto& x : dir_iteration)
    {
        EXPECT_NO_THROW(str = x.string());
    }
}
