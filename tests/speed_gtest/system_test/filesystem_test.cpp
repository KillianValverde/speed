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
 * @file        speed_gtest/system_test/filesystem_test.cpp
 * @brief       filesystem unit test.
 * @author      Killian Valverde
 * @date        2018/06/01
 */

#include <gtest/gtest.h>

#include "speed/system/system.hpp"


TEST(system_filesystem, access)
{
    ASSERT_TRUE(speed::system::filesystem::access(".", speed::system::filesystem::am_t::EXISTS));
    ASSERT_TRUE(speed::system::filesystem::access(".", speed::system::filesystem::am_t::EXISTS,
                                                  speed::system::filesystem::ft_t::DIRECTORY));
}


TEST(system_filesystem, can_directory_be_created)
{
    ASSERT_TRUE(speed::system::filesystem::can_directory_be_created(
            "f57475c9df7a624a05e842b5f852336015de44ba"));

    ASSERT_TRUE(speed::system::filesystem::can_directory_be_created(
            "."
            SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR
            "87e54df0169fce4cec5c064d8f1dfa6162388226"));

    ASSERT_TRUE(!speed::system::filesystem::can_directory_be_created(
            "f57475c9df7a624a05e842b5f852336015de44ba"
            SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR
            "87e54df0169fce4cec5c064d8f1dfa6162388226"));
}


TEST(system_filesystem, can_regular_file_be_created)
{
    ASSERT_TRUE(speed::system::filesystem::can_regular_file_be_created(
            "f57475c9df7a624a05e842b5f852336015de44ba"));

    ASSERT_TRUE(speed::system::filesystem::can_regular_file_be_created(
            "."
            SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR
            "87e54df0169fce4cec5c064d8f1dfa6162388226"));

    ASSERT_TRUE(!speed::system::filesystem::can_regular_file_be_created(
            "f57475c9df7a624a05e842b5f852336015de44ba"
            SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR
            "87e54df0169fce4cec5c064d8f1dfa6162388226"));
}


TEST(system_filesystem, chdir)
{
    ASSERT_TRUE(speed::system::filesystem::chdir("."));
}


TEST(system_filesystem, closedir)
{
    speed::system::filesystem::directory_entity dir_ent;

    ASSERT_TRUE(speed::system::filesystem::opendir(&dir_ent, "."));
    ASSERT_TRUE(speed::system::filesystem::closedir(&dir_ent));
}


TEST(system_filesystem, file_exists)
{
    ASSERT_TRUE(speed::system::filesystem::file_exists("."));
}


TEST(system_filesystem, get_file_inode)
{
    ASSERT_TRUE(speed::system::filesystem::get_file_inode(".") != ~0ull);
    ASSERT_TRUE(speed::system::filesystem::get_file_inode("..") != ~0ull);
    ASSERT_TRUE(speed::system::filesystem::get_file_inode(
            "f57475c9df7a624a05e842b5f852336015de44ba") == ~0ull);
}


TEST(system_filesystem, get_file_uid)
{
    auto file_uid = speed::system::filesystem::get_file_uid(".");
    auto user_uid = speed::system::process::get_uid();
    ASSERT_TRUE(file_uid == user_uid && file_uid != -1);
}


TEST(system_filesystem, get_file_gid)
{
    auto file_gid = speed::system::filesystem::get_file_gid(".");
    auto user_gid = speed::system::process::get_gid();
    ASSERT_TRUE(file_gid == user_gid && file_gid != -1);
}


TEST(system_filesystem, get_modification_time)
{
    speed::system::time::system_time system_tme;
    ASSERT_TRUE(speed::system::filesystem::get_modification_time(".", &system_tme));
}


TEST(system_filesystem, get_temporal_path)
{
    auto s = speed::system::filesystem::get_temporal_path();
    ASSERT_TRUE(speed::system::filesystem::get_temporal_path() != nullptr);
}


TEST(system_filesystem, is_block_device)
{
    ASSERT_TRUE(!speed::system::filesystem::is_block_device("."));
}


TEST(system_filesystem, is_character_device)
{
    ASSERT_TRUE(!speed::system::filesystem::is_character_device("."));
}


TEST(system_filesystem, is_directory)
{
    ASSERT_TRUE(speed::system::filesystem::is_directory("."));
}


TEST(system_filesystem, is_fifo)
{
    ASSERT_TRUE(!speed::system::filesystem::is_pipe("."));
}


TEST(system_filesystem, is_regular_file)
{
    ASSERT_TRUE(!speed::system::filesystem::is_regular_file("."));
}


TEST(system_filesystem, is_socket)
{
    ASSERT_TRUE(!speed::system::filesystem::is_socket("."));
}


TEST(system_filesystem, is_symlink)
{
    ASSERT_TRUE(!speed::system::filesystem::is_symlink("."));
}


TEST(system_filesystem, mkdir)
{
    ASSERT_TRUE(speed::system::filesystem::mkdir("6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c"));
}


TEST(system_filesystem, mkdir_recursively)
{
    // ASSERT_TRUE(speed::system::filesystem::mkdir_recursively(
    //         "f57475c9df7a624a05e842b5f852336015de44ba"
    //         SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR
    //         "87e54df0169fce4cec5c064d8f1dfa6162388226"));
}


TEST(system_filesystem, opendir)
{
    speed::system::filesystem::directory_entity dir_ent;

    ASSERT_TRUE(speed::system::filesystem::opendir(&dir_ent, "."));
    ASSERT_TRUE(speed::system::filesystem::closedir(&dir_ent));
}


TEST(system_filesystem, readdir)
{
    speed::system::filesystem::directory_entity dir_ent;

    ASSERT_TRUE(speed::system::filesystem::opendir(&dir_ent, "."));
    ASSERT_TRUE(speed::system::filesystem::readdir(&dir_ent));
    ASSERT_TRUE(speed::system::filesystem::readdir(&dir_ent));
    ASSERT_TRUE(speed::system::filesystem::closedir(&dir_ent));
}


TEST(system_filesystem, rmdir)
{
    ASSERT_TRUE(speed::system::filesystem::rmdir("6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c"));
}


TEST(system_filesystem, shortcut)
{
    ASSERT_TRUE(speed::system::filesystem::shortcut(".",
            "6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c"));
    ASSERT_TRUE(speed::system::filesystem::unlink("6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c"
            SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_CSTR));
}


TEST(system_filesystem, touch)
{
    ASSERT_TRUE(speed::system::filesystem::touch("6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c"));
}


TEST(system_filesystem, unlink)
{
    ASSERT_TRUE(speed::system::filesystem::unlink("6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c"));
}
