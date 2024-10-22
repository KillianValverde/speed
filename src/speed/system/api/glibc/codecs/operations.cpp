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
 * @file        speed/system/api/glibc/codecs/operations.cpp
 * @brief       codecs operations source.
 * @author      Killian Valverde
 * @date        2024/10/16
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include "operations.hpp"

#include <iconv.h>
#include <string.h>
#include <wchar.h>


namespace speed::system::api::glibc::codecs {


bool convert_c_string_to_wstring(
        const char* c_str,
        std::wstring* wstr,
        std::error_code* err_code
) noexcept
{
    try
    {
        constexpr std::size_t wchar_t_sz = sizeof(wchar_t);
        constexpr const char* encodng = sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE";

        iconv_t conv_desc;
        std::size_t c_str_len;
        std::size_t wstr_sz;
        std::size_t in_bytes_left;
        std::size_t out_bytes_left;
        char* in_buf;
        char* out_buf;

        conv_desc = iconv_open(encodng, "UTF-8");
        if (conv_desc == (iconv_t)-1)
        {
            assign_system_error_code(EINVAL, err_code);
            return false;
        }

        c_str_len = strlen(c_str);
        wstr_sz = (c_str_len + 1) * wchar_t_sz;
        wstr->resize(wstr_sz);

        in_buf = (char*)c_str;
        out_buf = (char*)&(*wstr)[0];

        in_bytes_left = c_str_len;
        out_bytes_left = wstr_sz;

        if (iconv(conv_desc, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) == (size_t)-1)
        {
            iconv_close(conv_desc);
            assign_system_error_code(errno, err_code);
            return false;
        }

        wstr->resize((wstr_sz - out_bytes_left) / wchar_t_sz);

        iconv_close(conv_desc);
        return true;
    }
    catch (const std::bad_alloc& ba)
    {
        assign_system_error_code(ENOMEM, err_code);
    }
    catch (...)
    {
        assign_system_error_code(EINVAL, err_code);
    }

    return false;
}


bool convert_w_string_to_string(
        const wchar_t* w_str,
        std::string* str,
        std::error_code* err_code
) noexcept
{
    try
    {
        constexpr std::size_t wchar_t_sz = sizeof(wchar_t);
        constexpr const char* encodng = sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE";

        iconv_t conv_desc;
        std::size_t w_str_len;
        std::size_t str_sz;
        std::size_t in_bytes_left;
        std::size_t out_bytes_left;
        char* in_buf;
        char* out_buf;

        conv_desc = iconv_open("UTF-8", encodng);
        if (conv_desc == (iconv_t)-1)
        {
            assign_system_error_code(EINVAL, err_code);
            return false;
        }

        w_str_len = wcslen(w_str);
        str_sz = w_str_len * 4 + 1;
        str->resize(str_sz);

        in_buf = (char*)w_str;
        out_buf = (char*)&(*str)[0];

        in_bytes_left = w_str_len * wchar_t_sz;
        out_bytes_left = str_sz;

        if (iconv(conv_desc, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) == (size_t)-1)
        {
            iconv_close(conv_desc);
            assign_system_error_code(errno, err_code);
            return false;
        }

        str->resize(str_sz - out_bytes_left);

        iconv_close(conv_desc);
        return true;
    }
    catch (const std::bad_alloc& ba)
    {
        assign_system_error_code(ENOMEM, err_code);
    }
    catch (...)
    {
        assign_system_error_code(EINVAL, err_code);
    }

    return false;
}


}


#endif