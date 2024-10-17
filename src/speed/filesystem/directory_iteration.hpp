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

        /** Stack of directories entities used to explore recursivelly the filesystem. */
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
     */
    template<
            typename TpPath_,
            typename = std::enable_if_t<
                    !std::is_base_of<directory_iteration, std::decay_t<TpPath_>>::value
            >
    >
    explicit directory_iteration(TpPath_&& root_pth)
            : root_pth_(std::forward<TpPath_>(root_pth))
            , regex_to_mtch_(speed::type_casting::type_cast<string_type>("^.*$"))
            , file_typs_(speed::system::filesystem::file_types::ALL)
            , access_mods_(speed::system::filesystem::access_modes::READ)
            , recursivity_levl_(~0ull)
            , follow_symbolic_lnks_(false)
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

    /**
     * @brief       Specify the access modes that the files are mandatory to have.
     * @param       access_mods : Access modes that the files are mandatory to have.
     * @return      The object who call the method.
     */
    inline directory_iteration& access_modes(system::filesystem::access_modes access_mods)
    {
        access_mods_ = access_mods;
        return *this;
    }

    /**
     * @brief       Specify the file types that will be considered during the iteration.
     * @param       file_typs : File types that will be considered during the iteration.
     * @return      The object who call the method.
     */
    inline directory_iteration& file_types(system::filesystem::file_types file_typs)
    {
        file_typs_ = file_typs;
        return *this;
    }

    /**
     * @brief       Specify wether or not the symbolic links will be followed.
     * @param       follow_symbolic_lnks : If true the symbolic links will be followed.
     * @return      The object who call the method.
     */
    inline directory_iteration& follow_symbolic_links(bool follow_symbolic_lnks)
    {
        follow_symbolic_lnks_ = follow_symbolic_lnks;
        return *this;
    }

    /**
     * @brief       Specify the level of recursivity that .
     * @param       recursivity_level : Access modes that the files are mandatory to have.
     * @return      The object who call the method.
     */
    inline directory_iteration& recursivity_level(std::uint64_t recursivity_levl)
    {
        recursivity_levl_ = recursivity_levl;
        return *this;
    }

    /**
     * @brief       Specify the access modes that the files are mandatory to have.
     * @param       access_mods : Access modes that the files are mandatory to have.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    inline directory_iteration& regex_to_match(TpString_&& regex_to_mtch)
    {
        regex_to_mtch_ = speed::type_casting::type_cast<string_type>(
                std::forward<TpString_>(regex_to_mtch));
        return *this;
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

    /** Specify wheter or not follow symbolic links during the iteration. */
    bool follow_symbolic_lnks_;

    friend class const_iterator;
};


}


#endif
