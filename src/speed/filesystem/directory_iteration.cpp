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
 * @file        speed/filesystem/directory_iteration.cpp
 * @brief       directory_iteration class methods definition.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#include "../stringutils/stringutils.hpp"

#include "directory_iteration.hpp"


namespace speed::filesystem {


directory_iteration::const_iterator::const_iterator(const directory_iteration* composit)
       : cur_dir_()
       , cur_fle_()
       , path_stck_()
       , vistd_inos_()
       , composit_(composit)
       , end_(false)
       , current_recursivity_levl_(0)
{
    if (composit == nullptr)
    {
        end_ = true;
        return;
    }

    cur_dir_ = composit->root_pth_;
    cur_fle_ = cur_dir_;

    if (open_directory())
    {
        this->operator++();
    }
}


directory_iteration::const_iterator::~const_iterator() noexcept
{
    while (!path_stck_.empty())
    {
        speed::system::filesystem::closedir(&path_stck_.top());
        path_stck_.pop();
    }

    end_ = true;
}


directory_iteration::const_iterator::self_type& directory_iteration::const_iterator::operator ++()
{
    directory_entity& cur_dir_ent = path_stck_.top();

    if (!read_directory())
    {
        close_directory();

        if (end_)
        {
            return *this;
        }
    }
    else
    {
        cur_fle_.replace_filename(cur_dir_ent.nme);

        if (speed::system::filesystem::is_directory(cur_fle_.c_str()))
        {
            cur_dir_ /= cur_dir_ent.nme;

            if (open_directory())
            {
                return this->operator++();
            }
        }
    }

    if (!is_file_valid())
    {
        return this->operator++();
    }

    return *this;
}


bool directory_iteration::const_iterator::operator ==(const self_type& rhs) const noexcept
{
    auto lhs_ino = path_stck_.empty() ? ~0ull : path_stck_.top().ino;
    auto rhs_ino = rhs.path_stck_.empty() ? ~0ull : rhs.path_stck_.top().ino;

    if (end_ == rhs.end_)
    {
        return true;
    }
    if (composit_ != rhs.composit_ || lhs_ino != rhs_ino)
    {
        return false;
    }

    return true;
}


bool directory_iteration::const_iterator::open_directory()
{
    std::uint64_t ino = speed::system::filesystem::get_file_inode(cur_dir_.c_str());

    if (current_recursivity_levl_ > composit_->recursivity_levl_ ||
        vistd_inos_.contains(ino))
    {
        return false;
    }

    ++current_recursivity_levl_;
    cur_fle_ /= ".";
    path_stck_.emplace();

    if (!speed::system::filesystem::opendir(&path_stck_.top(), cur_dir_.c_str()))
    {
        exit_directory();
        return false;
    }

    vistd_inos_.emplace(ino);
    return true;
}


bool directory_iteration::const_iterator::read_directory()
{
    bool succss;
    directory_entity& cur_dir_ent = path_stck_.top();

    do
    {
        succss = speed::system::filesystem::readdir(&cur_dir_ent);
    }
    while (succss && (speed::stringutils::strcmp(cur_dir_ent.nme, ".") == 0 ||
                      speed::stringutils::strcmp(cur_dir_ent.nme, "..") == 0));

    return succss;
}


void directory_iteration::const_iterator::close_directory()
{
    speed::system::filesystem::closedir(&path_stck_.top());
    exit_directory();
}


void directory_iteration::const_iterator::exit_directory()
{
    path_stck_.pop();
    cur_fle_ = cur_dir_;
    cur_dir_ = cur_dir_.parent_path();
    --current_recursivity_levl_;

    if (path_stck_.empty())
    {
        end_ = true;
    }
}


bool directory_iteration::const_iterator::is_file_valid()
{
    if (!std::regex_match(cur_fle_.filename().c_str(), composit_->regex_to_mtch_))
    {
        return false;
    }
    if (!speed::system::filesystem::is_file_type(cur_fle_.c_str(), composit_->file_typs_))
    {
        return false;
    }
    if (!speed::system::filesystem::access(cur_fle_.c_str(), composit_->access_mods_))
    {
        return false;
    }

    return true;
}


}
