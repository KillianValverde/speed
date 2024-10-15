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
 * @file        speed/filesystem/valid_path.hpp
 * @brief       valid_path main header.
 * @author      Killian Valverde
 * @date        2024/05/20
 */

#ifndef SPEED_FILESYSTEM_VALID_PATH_HPP
#define SPEED_FILESYSTEM_VALID_PATH_HPP

#include <filesystem>
#include <system_error>


namespace speed::filesystem {


/**
 * @brief       Path class that allows performing validity checks throught decorators.
 */
class valid_path : public std::filesystem::path
{
public:
    /** Inherits all the path constructors. */
    using std::filesystem::path::path;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual inline bool is_valid(std::error_code* err_code = nullptr)
    {
        return speed::system::filesystem::access(c_str(),
                speed::system::filesystem::access_modes::EXISTS, err_code);
    }
};


/**
 * @brief       Path class decorator that checks whether the path can be read.
 */
template<typename TpComponent>
class read_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::access(TpComponent::c_str(),
                speed::system::filesystem::access_modes::READ, err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/**
 * @brief       Path class decorator that checks whether the path can be written.
 */
template<typename TpComponent>
class write_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::access(TpComponent::c_str(),
                speed::system::filesystem::access_modes::WRITE, err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/**
 * @brief       Path class decorator that checks whether the path can be executed.
 */
template<typename TpComponent>
class execute_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::access(TpComponent::c_str(),
                speed::system::filesystem::access_modes::EXECUTE, err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/**
 * @brief       Path class decorator that checks whether the path is a regular file.
 */
template<typename TpComponent>
class regular_file_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::is_regular_file(TpComponent::c_str(), err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/**
 * @brief       Path class decorator that checks whether the path is a directory.
 */
template<typename TpComponent>
class directory_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::is_directory(TpComponent::c_str(), err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/**
 * @brief       Path class decorator that checks whether the regular file path can be used as an
 *              output.
 */
template<typename TpComponent>
class output_regular_file_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::access(TpComponent::c_str(),
                    speed::system::filesystem::access_modes::WRITE,
                    speed::system::filesystem::file_types::REGULAR_FILE, err_code) ||
            speed::system::filesystem::touch(TpComponent::c_str(), 0755, err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/**
 * @brief       Path class decorator that checks whether the directory path can be used as an
 *              output.
 */
template<typename TpComponent>
class output_directory_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::access(TpComponent::c_str(),
                    speed::system::filesystem::access_modes::WRITE,
                    speed::system::filesystem::file_types::DIRECTORY, err_code) ||
            speed::system::filesystem::mkdir(TpComponent::c_str(), 0755, err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


/** Regular file path type. */
using regular_file_path = regular_file_path_decorator<valid_path>;

/** Directory path type. */
using directory_path = directory_path_decorator<valid_path>;

/** Regular file path type that can be executed. */
using x_regular_file_path = execute_path_decorator<regular_file_path>;

/** Regular file path type that can be written. */
using w_regular_file_path = write_path_decorator<regular_file_path>;

/** Regular file path type that can be written and executed. */
using wx_regular_file_path = write_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path type that can be read. */
using r_regular_file_path = read_path_decorator<regular_file_path>;

/** Regular file path type that can be read and executed. */
using rx_regular_file_path = read_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path type that can be read and written. */
using rw_regular_file_path = read_path_decorator<write_path_decorator<regular_file_path>>;

/** Regular file path type that can be read, written and executed. */
using rwx_regular_file_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<regular_file_path>>>;

/** Directory path type that can be executed. */
using x_directory_path = execute_path_decorator<directory_path>;

/** Directory path type that can be written. */
using w_directory_path = write_path_decorator<directory_path>;

/** Directory path type that can be written and executed. */
using wx_directory_path = write_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path type that can be read. */
using r_directory_path = read_path_decorator<directory_path>;

/** Directory path type that can be read and executed. */
using rx_directory_path = read_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path type that can be read and written. */
using rw_directory_path = read_path_decorator<write_path_decorator<directory_path>>;

/** Directory path type that can be read, written and executed. */
using rwx_directory_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<directory_path>>>;

/** Regular file path type that can be used as an output file. */
using output_regular_file_path = output_regular_file_path_decorator<valid_path>;

/** Directory path type that can be used as an output file. */
using output_directory_path = output_directory_path_decorator<valid_path>;


}


#endif
