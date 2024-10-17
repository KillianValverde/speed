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
 * @file        speed_gtest/type_casting_test/try_type_cast_test.cpp
 * @brief       try_type_cast unit test.
 * @author      Killian Valverde
 * @date        2018/06/07
 */

#include <filesystem>
#include <regex>

#include <gtest/gtest.h>

#include "speed/filesystem/filesystem.hpp"
#include "speed/type_casting/type_casting.hpp"


TEST(type_casting_try_type_cast, basic_string_to_type)
{
    int res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>(std::string("347865"), &res));
    ASSERT_TRUE(res == 347865);
}


TEST(type_casting_try_type_cast, c_string_to_string)
{
    std::string res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::string>("☆☆☆", &res));
    ASSERT_TRUE(res == "☆☆☆");
}


TEST(type_casting_try_type_cast, c_string_to_wstring)
{
    std::wstring res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::wstring>("☆☆☆", &res));
    ASSERT_TRUE(res == L"☆☆☆");
}


TEST(type_casting_try_type_cast, c_wstring_to_wstring)
{
    std::wstring res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::wstring>(L"☆☆☆", &res));
    ASSERT_TRUE(res == L"☆☆☆");
}


TEST(type_casting_try_type_cast, c_wstring_to_string)
{
    std::string res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::string>(L"☆☆☆", &res));
    ASSERT_TRUE(res == "☆☆☆");
}


TEST(type_casting_try_type_cast, c_string_to_floating_point)
{
    float res_f;
    double res_d;
    long double res_ld;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<float>("23.345", &res_f));
    ASSERT_TRUE(speed::type_casting::try_type_cast<double>("23.345", &res_d));
    ASSERT_TRUE(speed::type_casting::try_type_cast<long double>("23.345", &res_ld));
}


TEST(type_casting_try_type_cast, c_string_to_integral_signed)
{
    int res;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("-342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("+342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("12387645", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<int>("", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<int>("\0", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<int>("1238*7645", &res));
}


TEST(type_casting_try_type_cast, c_string_to_integral_unsigned)
{
    unsigned int res;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<unsigned int>("342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<unsigned int>("12387645", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<unsigned int>("+342432", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("-342432", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("\0", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("1238*7645", &res));
}


TEST(type_casting_try_type_cast, c_string_to_regex)
{
    std::regex rgx;

    ASSERT_TRUE(speed::type_casting::try_type_cast<std::regex>("^.*$", &rgx));
}


TEST(type_casting_try_type_cast, c_string_to_path)
{
    std::filesystem::path pth;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::filesystem::path>(".", &pth));
}


TEST(type_casting_try_type_cast, c_string_to_valid_path)
{
    speed::filesystem::r_regular_file_path reg_pth;
    speed::filesystem::r_directory_path dir_pth;

    ASSERT_TRUE(!speed::type_casting::try_type_cast<speed::filesystem::r_regular_file_path>(
            ".", &reg_pth));
    ASSERT_TRUE(speed::type_casting::try_type_cast<speed::filesystem::r_directory_path>(
            ".", &dir_pth));
}
