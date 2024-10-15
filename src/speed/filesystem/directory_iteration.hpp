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
 * @file        speed/filesystem/directory_iteration.hpp
 * @brief       directory_iteration class header.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#ifndef SPEED_FILESYSTEM_DIRECTORY_ITERATION_HPP
#define SPEED_FILESYSTEM_DIRECTORY_ITERATION_HPP

#include <filesystem>
#include <regex>
#include <set>
#include <stack>

#include "../containers/containers.hpp"
#include "../system/system.hpp"
#include "../type_casting/type_casting.hpp"


namespace speed::filesystem {


/**
 * @brief       Class that configures and produce directories iterators.
 */
class directory_iteration
{
public:
    /** Character type used in the class. */
    using char_type = std::filesystem::path::value_type;

    /** Directory entity type. */
    using directory_entity = speed::system::filesystem::basic_directory_entity<char_type>;

    /** String type used in the class. */
    using string_type = std::basic_string<
            char_type, std::char_traits<char_type>, std::allocator<char_type>>;

    /** Regex type used in the class. */
    using regex_type = std::basic_regex<char_type, std::regex_traits<char_type>>;

    /**
     * @brief       Class that represents const iterators.
     */
    class const_iterator
    {
    public:
        /** The class itself. */
        using self_type = const_iterator;

        /** The value encapsulated by the iterator. */
        using value_type = std::filesystem::path;

        /** Constructor with parameters. */
        explicit const_iterator(const directory_iteration* composit);

        /** Destructor. */
        ~const_iterator() noexcept;

        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++();

        /**
         * @brief       Allows knowing whether the two iterators are the same.
         * @param       rhs : The value to compare.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool operator ==(const self_type& rhs) const noexcept;

        /**
         * @brief       Allows knowing whether the iterator is past-the-end or not.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        [[nodiscard]] bool end() const noexcept
        {
            return end_;
        }

        /**
         * @brief       Get the reference of the current node value.
         * @return      The reference of the current node value.
         */
        const value_type& operator *() const noexcept
        {
            return cur_fle_;
        }

        /**
         * @brief       Get the address of the current node value.
         * @return      The address of the current node value.
         */
        const value_type* operator ->() const noexcept
        {
            return &cur_fle_;
        }

    private:
        /**
         * @brief       Tries to open the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool open_directory();

        /**
         * @brief       Tries to read the next entry in the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool read_directory();

        /**
         * @brief       Close and exit the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        void close_directory();

        /**
         * @brief       Exit the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        void exit_directory();

        /**
         * @brief       Check if the current file is valid.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool is_file_valid();

    private:
        /** Current directory. */
        std::filesystem::path cur_dir_;

        /** Current file. */
        std::filesystem::path cur_fle_;

        /** Stack of directories entities used to explore in recursivelly the filesystem. */
        std::stack<directory_entity> path_stck_;

        /** Set of visited inodes to avoid infinite recursions in case of fs corruptions. */
        std::set<std::uint64_t> vistd_inos_;

        /** Pointer to the composite object. */
        const directory_iteration* composit_;

        /** Current level of recursivity. */
        std::uint64_t current_recursivity_levl_;

        /** Represents whether or not the iteration is finished. */
        bool end_;
    };

    /**
     * @brief       Constructor with parameters.
     * @param       root_pth : Path where to do the iteration.
     * @param       regex_to_mtch : Regex that all the iterated files have to match.
     * @param       file_typs : List of file types that are allowed to be iterated.
     * @param       access_mods : Access mods that all the iterated files have to match.
     * @param       recursivity_levl : Maximum level of recursivity allowed. If the maximum value
     *              is specified there is no limit.
     */
    template<typename TpString_ = std::string>
    explicit directory_iteration(
            std::filesystem::path root_pth,
            TpString_&& regex_to_mtch = "^.*$",
            speed::system::filesystem::file_types file_typs =
                    speed::system::filesystem::file_types::ALL,
            speed::system::filesystem::access_modes access_mods =
                    speed::system::filesystem::access_modes::READ,
            std::uint64_t recursivity_levl = ~0ull
    )
            : root_pth_(std::move(root_pth))
            , regex_to_mtch_(speed::type_casting::type_cast<string_type>(
                    std::forward<TpString_>(regex_to_mtch)))
            , file_typs_(file_typs)
            , access_mods_(access_mods)
            , recursivity_levl_(recursivity_levl)
    {
    }

    /**
     * @brief       Get a const iterator for the first file.
     * @return      A const iterator for the first file.
     */
    [[nodiscard]] inline const_iterator begin() noexcept
    {
        return const_iterator(this);
    }

    /**
     * @brief       Get a const iterator to the past-the-end file.
     * @return      A const iterator to the past-the-end file.
     */
    [[nodiscard]] inline const_iterator end() noexcept
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief       Get a const iterator for the first file.
     * @return      A const iterator for the first file.
     */
    [[nodiscard]] inline const_iterator cbegin() const noexcept
    {
        return const_iterator(this);
    }

    /**
     * @brief       Get a const iterator to the past-the-end file.
     * @return      A const iterator to the past-the-end file.
     */
    [[nodiscard]] inline const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }

private:
    /** The root directory of the iteration. */
    std::filesystem::path root_pth_;

    /** Regex that all the iterated files have to match. */
    regex_type regex_to_mtch_;

    /** Maximum level of recursivity allowed. */
    std::uint64_t recursivity_levl_;

    /** List of file types that are allowed to be iterated. */
    system::filesystem::file_types file_typs_;

    /** Access mods that all the iterated files have to match. */
    system::filesystem::access_modes access_mods_;

    friend class const_iterator;
};


}


#endif
