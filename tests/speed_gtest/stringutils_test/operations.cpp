/* Copyright (C) 2017 Killian Valverde.
   This file is part of speed.

   speed is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   speed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with speed. If not, see <http://www.gnu.org/licenses/>. */

/**
 * @file        speed_gtest/stringutils_test/operations.cpp
 * @brief       operations unit test.
 * @author      Killian Valverde
 * @date        2017/12/24
 */

#include <gtest/gtest.h>

#include "speed/stringutils/stringutils.hpp"


TEST(stringutils_operations, strlen)
{
    const char* str1 = "hello, world";
    const wchar_t* str2 = L"hello, world";
    char str3[] = "hello, world";
    wchar_t str4[] = L"hello, world";
    char* str5 = nullptr;
    std::string str6 = "12345";
    std::wstring str7 = L"12345";
    
    EXPECT_TRUE(speed::stringutils::strlen(str1) == 12);
    EXPECT_TRUE(speed::stringutils::strlen(str2) == 12);
    EXPECT_TRUE(speed::stringutils::strlen(str3) == 12);
    EXPECT_TRUE(speed::stringutils::strlen(str4) == 12);
    EXPECT_TRUE(speed::stringutils::strlen(str5) == 0);
    EXPECT_TRUE(speed::stringutils::strlen(str6) == 5);
    EXPECT_TRUE(speed::stringutils::strlen(str7) == 5);
}


TEST(stringutils_operations, strcpy)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L"world";
    std::string str_res = "world";
    
    speed::stringutils::strcpy(str1, str2);
    
    EXPECT_TRUE(str_res == str1);
}


TEST(stringutils_operations, strncpy)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L"world";
    std::string str_res = "wo";
    
    speed::stringutils::strncpy(str1, str2, 2);
    
    EXPECT_TRUE(str_res == str1);
}


TEST(stringutils_operations, strcat)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L", world";
    std::string str_res = "hello, world";
    
    speed::stringutils::strcat(str1, str2);
    
    EXPECT_TRUE(str_res == str1);
}


TEST(stringutils_operations, strncat)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L", world";
    std::string str_res = "hello, ";
    
    speed::stringutils::strncat(str1, str2, 2);
    
    EXPECT_TRUE(str_res == str1);
}


TEST(stringutils_operations, strcmp)
{
    char str1[32] = "kkk";
    wchar_t str2[32] = L"kkk";
    const char* str3 = "aaa";
    const char* str4 = "zzz";
    
    EXPECT_TRUE(speed::stringutils::strcmp(str1, str2) == 0);
    EXPECT_TRUE(speed::stringutils::strcmp(str1, str3) == 1);
    EXPECT_TRUE(speed::stringutils::strcmp(str1, str4) == -1);
}


TEST(stringutils_operations, strncmp)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    const char* str3 = "aa";
    const char* str4 = "zz";
    
    EXPECT_TRUE(speed::stringutils::strncmp(str1, str2, 2) == 0);
    EXPECT_TRUE(speed::stringutils::strncmp(str1, str2, 4) == -1);
    EXPECT_TRUE(speed::stringutils::strncmp(str1, str3, 2) == 1);
    EXPECT_TRUE(speed::stringutils::strncmp(str1, str4, 2) == -1);
}


TEST(stringutils_operations, strchr)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    
    EXPECT_TRUE(speed::stringutils::strchr(str1, 'b') == &str1[2]);
    EXPECT_TRUE(speed::stringutils::strchr(str2, 'z') == &str2[2]);
    EXPECT_TRUE(speed::stringutils::strchr(str1, 'l') == nullptr);
    EXPECT_TRUE(speed::stringutils::strchr(str2, 'l') == nullptr);
}


TEST(stringutils_operations, strnchr)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    
    EXPECT_TRUE(speed::stringutils::strnchr(str1, 'b', 4) == &str1[2]);
    EXPECT_TRUE(speed::stringutils::strnchr(str1, 'b', 2) == nullptr);
    EXPECT_TRUE(speed::stringutils::strnchr(str2, 'z', 4) == &str2[2]);
    EXPECT_TRUE(speed::stringutils::strnchr(str2, 'z', 2) == nullptr);
    EXPECT_TRUE(speed::stringutils::strnchr(str1, 'l', 4) == nullptr);
    EXPECT_TRUE(speed::stringutils::strnchr(str2, 'l', 4) == nullptr);
}


TEST(stringutils_operations, strrchr)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    
    EXPECT_TRUE(speed::stringutils::strrchr(str1, 'b') == &str1[3]);
    EXPECT_TRUE(speed::stringutils::strrchr(str2, 'z') == &str2[3]);
    EXPECT_TRUE(speed::stringutils::strrchr(str1, 'l') == nullptr);
    EXPECT_TRUE(speed::stringutils::strrchr(str2, 'l') == nullptr);
}


TEST(stringutils_operations, strrnchr)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    
    EXPECT_TRUE(speed::stringutils::strrnchr(str1, 'b', 4) == &str1[3]);
    EXPECT_TRUE(speed::stringutils::strrnchr(str1, 'b', 2) == nullptr);
    EXPECT_TRUE(speed::stringutils::strrnchr(str2, 'z', 4) == &str2[3]);
    EXPECT_TRUE(speed::stringutils::strrnchr(str2, 'z', 2) == nullptr);
    EXPECT_TRUE(speed::stringutils::strrnchr(str1, 'l', 4) == nullptr);
    EXPECT_TRUE(speed::stringutils::strrnchr(str2, 'l', 4) == nullptr);
}


TEST(stringutils_operations, strsplit)
{
    char str1[32] = "1;2;3;4;5;6";
    wchar_t str2[32] = L"1;2;3;4;5;6";
    std::size_t i;
    
    auto vec1 = speed::stringutils::strsplit(str1, ';');
    
    i = '1';
    for (auto& x : vec1)
    {
        EXPECT_TRUE(x[0] == i++);
    }
    
    auto vec2 = speed::stringutils::strsplit(str2, ';');
    
    i = '1';
    for (auto& x : vec2)
    {
        EXPECT_TRUE(x[0] == i++);
    }
}


TEST(stringutils_operations, strdisclower)
{
    char str1[32] = "123456";
    wchar_t str2[32] = L"123456";
    std::string str1_res = "456";
    std::wstring str2_res = L"56";
    
    EXPECT_TRUE(speed::stringutils::strdisclower(str1, '4') == str1_res);
    EXPECT_TRUE(speed::stringutils::strdisclower(str2, '5') == str2_res);
}
