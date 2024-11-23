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
 * @file        speed/stringutils/operations.hpp
 * @brief       stringutils operations functions header.
 * @author      Killian Valverde
 * @date        2016/01/08
 */

#ifndef SPEED_STRINGUTILS_OPERATIONS_HPP
#define SPEED_STRINGUTILS_OPERATIONS_HPP

#include <cwctype>
#include <string>
#include <type_traits>
#include <vector>

#include "forward_declarations.hpp"
#include "../type_traits/type_traits.hpp"


namespace speed::stringutils {


/**
 * @brief       Escapes special regex characters in a string.
 * @param       str : Pointer to a null-terminated C-style string.
 * @return      A `std::basic_string` of type `TpChar` with all special regex characters escaped.
 */
template<typename TpChar>
[[nodiscard]] std::basic_string<TpChar> escape_regex_special_characters(const TpChar* str) noexcept
{
    using char_type = TpChar;
    using string_type = std::basic_string<char_type>;
    
    const std::array<char_type, 14> special_chars =
            {'[', '\\', '^', '$', '.', '|', '?', '*', '+', '(', ')', '{', '}', ']'};
    
    string_type res;
    
    if (str != nullptr)
    {
        while (*str != '\0')
        {
            if (std::find(special_chars.begin(), special_chars.end(), *str) != special_chars.end())
            {
                res += '\\';
            }
            
            res += *str++;
        }
    }
    
    return res;
}


// TODO: The std::basic_string type that is returned is not correct.
/**
 * @brief       Escapes special regex characters in a string.
 * @param       str : The input `std::basic_string` containing characters to be escaped.
 * @return      A `std::basic_string` of type `TpChar` with all special regex characters escaped.
 */
template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
[[nodiscard]] auto escape_regex_special_characters(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str
) noexcept
{
    return escape_regex_special_characters(str.c_str());
}


/**
 * @brief       Compares a string to a pattern with wildcard characters '*' and '?'.
 * @param       str : Pointer to the string to match.
 * @param       pattrn : Pointer to the pattern containing wildcards.
 * @param       case_sensitive : Determines whether the match is case-sensitive
 *              (default is `false`).
 * @return      `true` if `str` matches the `pattrn` with wildcards; otherwise, `false`.
 */
template<typename TpChar1, typename TpChar2>
[[nodiscard]] bool matches_wildcard(
        const TpChar1* str,
        const TpChar2* pattrn
) noexcept
{
    const TpChar1* str_backup = nullptr;
    const TpChar2* pattrn_backup = nullptr;
    
    if (str == nullptr || pattrn == nullptr)
    {
        return false;
    }

    while (*str)
    {
        if (*pattrn == '*')
        {
            pattrn++;
            
            if (*pattrn == '\0')
            {
                return true;
            }
            
            pattrn_backup = pattrn;
            str_backup = str;
        }
        else if (*pattrn == '?' || *pattrn == *str)
        {
            pattrn++;
            str++;
        }
        else if (str_backup)
        {
            pattrn = pattrn_backup;
            str = ++str_backup;
        }
        else
        {
            return false;
        }
    }

    while (*pattrn == '*')
    {
        pattrn++;
    }

    return *pattrn == '\0';
}


/**
 * @brief       Copies the C string pointed by source at the end of the array pointed by
 *              destination, including the terminating null character (and stopping at that point).
 * @param       dest : Pointer to the destination array where the content will to be appended.
 * @param       src : C string to be appended.
 * @return      The pointer to the destination array.
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strcat(TpDestinationChar* dest, const TpSourceChar* src) noexcept
{
    TpDestinationChar* orig_dest = dest;
    
    if (dest != nullptr && src != nullptr)
    {
        while (*dest != '\0')
        {
            dest++;
        }
        while (*src != '\0')
        {
            *dest++ = *src++;
        }
    
        *dest = '\0';
    }
    
    return orig_dest;
}


/**
 * @brief       Returns a pointer to the first occurrence of a value in the C string str.
 * @param       str : The C string.
 * @param       val : Character to be located.
 * @return      A pointer to the first occurrence of val in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strchr(TpChar* str, const TpIntegral val) noexcept
{
    if (str != nullptr)
    {
        while (*str != '\0')
        {
            if (*str == val)
            {
                return str;
            }
            
            ++str;
        }
    }
    
    return nullptr;
}


/**
 * @brief       Compare the target string to the destination.
 * @param       src : Comparation source.
 * @param       trg : Comparation target.
 * @return      If the first character that does not match has a lower value in source than in
 *              target <0 is returned, if the contents of both strings are equal 0 is returned, and
 *              if the first character that does not match has a greater value in source than in
 *              target >0 is returned.
 */
template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int strcmp(const TpSourceChar* src, const TpTargetChar* trg) noexcept
{
    if (src == nullptr)
    {
        return (trg == nullptr) ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }
    
    for (; *src != '\0' && *trg != '\0'; ++src, ++trg)
    {
        if (*src != *trg)
        {
            return *src < *trg ? -1 : 1;
        }
    }
    
    if (*src == '\0')
    {
        return (*trg == '\0') ? 0 : -1;
    }
    return 1;
}


/**
 * @brief       Copies the C string pointed by source into the array pointed by destination,
 *              including the terminating null character (and stopping at that point).
 * @param       dest : Pointer to the destination array where the content is to be copied.
 * @param       src : C string to be copied.
 * @return      The pointer to the destination array.
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strcpy(TpDestinationChar* dest, const TpSourceChar* src) noexcept
{
    TpDestinationChar* orig_dest = dest;
    
    if (dest != nullptr && src != nullptr)
    {
        while (*src != '\0')
        {
            *dest++ = *src++;
        }
    
        *dest = '\0';
    }
    
    return orig_dest;
}


/**
 * @brief       Erase the characters after the last specified value in the C string.
 * @param       str : The C string.
 * @param       val : The value.
 * @param       erase_val : If it's true the last specified value is also erased.
 * @return      If function was successful a pointer to the last character of the C string is
 *              returned, otherwise a null pointer is returned.
 */
template<typename TpChar, typename TpIntegral>
TpChar* strcut(TpChar* str, const TpIntegral val, bool erase_val = false) noexcept
{
    TpChar* lst;
    
    if (str != nullptr)
    {
        if ((lst = stringutils::strrchr(str, val)) != nullptr)
        {
            if (erase_val)
            {
                *lst = '\0';
            }
            else
            {
                lst[1] = '\0';
            }

            return lst;
        }
    }
    
    return nullptr;
}


/**
 * @brief       Returns the length of a C string str.
 * @param       str : A C string.
 * @return      The length of the C string.
 */
template<typename TpChar>
[[nodiscard]] std::size_t strlen(const TpChar* str) noexcept
{
    std::size_t len = 0;
    
    if (str != nullptr)
    {
        for (; *str != '\0'; ++str)
        {
            ++len;
        }
    }
    
    return len;
}


/**
 * @brief       Returns the length of a string.
 * @param       str : A string.
 * @return      The length of the string.
 */
template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
[[nodiscard]] auto strlen(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str
) noexcept
{
    return str.length();
}


/**
 * @brief       Appended the C string first cnt characters of source to destination. If the end of
 *              the source C string (which is signaled by a null-character) is found before num
 *              characters have been copied, destination is padded with zeros until a total of nbr
 *              characters have been written to it.
 * @param       dest : Pointer to the destination array where the content will to be appended.
 * @param       src : C string to be appended.
 * @param       nbr : Maximum number of characters to be copied from source.
 * @return      The pointer to the destination array
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strncat(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t nbr
) noexcept
{
    TpDestinationChar* orig_dest = dest;
    TpDestinationChar* end_dest;
    
    if (dest != nullptr && src != nullptr)
    {
        while (*dest != '\0')
        {
            dest++;
        }
        
        end_dest = dest + nbr;
        
        while (*src != '\0' && dest != end_dest)
        {
            *dest++ = *src++;
        }
        
        while (dest != end_dest)
        {
            *dest++ = '\0';
        }
        
        *dest = '\0';
    }
    
    return orig_dest;
}


/**
 * @brief       Returns a pointer to the first occurrence of a value in the C string str. Only the
 *              firs nbr characters will be evaluated.
 * @param       str : The C string.
 * @param       val : Character to be located.
 * @param       nbr : Maximum number of characters in which search.
 * @return      A pointer to the first occurrence of val in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strnchr(TpChar* str, const TpIntegral val, std::size_t nbr) noexcept
{
    std::size_t i;
    
    if (str != nullptr)
    {
        for (i = 0; *str != '\0' && i < nbr; ++i)
        {
            if (*str == val)
            {
                return str;
            }
            
            ++str;
        }
    }
    
    return nullptr;
}


/**
 * @brief       Compare the first nbr characters of source to the destination.
 * @param       src : Comparation source.
 * @param       trg : Comparation target.
 * @param       nbr : Maximum number of characters to be compared from source.
 * @return      If the first character that does not match has a lower value in source than in
 *              target <0 is returned, if the contents of both strings are equal 0 is returned, and
 *              if the first character that does not match has a greater value in source than in
 *              target >0 is returned.
 */
template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int strncmp(
        const TpSourceChar* src,
        const TpTargetChar* trg,
        const std::size_t nbr
) noexcept
{
    const TpSourceChar* const end_src = src + nbr;
    
    if (src == nullptr)
    {
        return (trg == nullptr) ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }
    
    for (; *src != '\0' && *trg != '\0' && src != end_src; ++src, ++trg)
    {
        if (*src != *trg)
        {
            return *src < *trg ? -1 : 1;
        }
    }
    
    if (src != end_src)
    {
        if (*src == '\0')
        {
            return (*trg == '\0') ? 0 : -1;
        }
        if (*trg == '\0')
        {
            return 1;
        }
    }
    
    return 0;
}


/**
 * @brief       Copies the C string first cnt characters of source to destination. If the end of the
 *              source C string (which is signaled by a null-character) is found before num
 *              characters have been copied, destination is padded with zeros until a total of nbr
 *              characters have been written to it.
 * @param       dest : Pointer to the 'destination' array where the content is to be copied.
 * @param       src : C string to be copied.
 * @param       nbr : Maximum number of characters to be copied from source.
 * @return      The pointer to the destination array
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strncpy(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t nbr
) noexcept
{
    TpDestinationChar* orig_dest = dest;
    const TpDestinationChar* const end_dest = orig_dest + nbr;
    
    if (dest != nullptr && src != nullptr)
    {
        while (*src != '\0' && dest != end_dest)
        {
            *dest++ = *src++;
        }
        
        while (dest != end_dest)
        {
            *dest++ = '\0';
        }
        
        *dest = '\0';
    }
    
    return orig_dest;
}


/**
 * @brief       Returns a pointer to the last occurrence of a value in the C string str.
 * @param       str : The C string.
 * @param       val : Character to be located.
 * @return      A pointer to the first occurrence of val in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strrchr(TpChar* str, const TpIntegral val) noexcept
{
    TpChar* lst = nullptr;
    
    if (str != nullptr)
    {
        while (*str != '\0')
        {
            if (*str == val)
            {
                lst = str;
            }
            
            ++str;
        }
    }
    
    return lst;
}


/**
 * @brief       Returns a pointer to the last occurrence of a value in the C string str. Only the
 *              firs nbr characters will be evaluated.
 * @param       str : The C string.
 * @param       val : Character to be located.
 * @param       nbr : Maximum number of characters in which search.
 * @return      A pointer to the first occurrence of val in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strrnchr(TpChar* str, const TpIntegral val, std::size_t nbr) noexcept
{
    std::size_t i;
    TpChar* lst = nullptr;
    
    if (str != nullptr)
    {
        for (i = 0; *str != '\0' && i < nbr; ++i)
        {
            if (*str == val)
            {
                lst = str;
            }
            
            ++str;
        }
    }
    
    return lst;
}


/**
 * @brief       Split a C string by a specified separator.
 * @param       str : The C string to split.
 * @param       sep : The character used to split the string.
 * @return      A 'std::vector' of 'std::basic_string' with all the values obtained.
 */
template<
        typename TpChar,
        typename TpIntegral,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
[[nodiscard]] auto strsplit(const TpChar *str, const TpIntegral sep)
{
    using char_allocator_type = std::allocator_traits<TpAllocator>::template rebind_alloc<TpChar>;
    using string_type = std::basic_string<TpChar, TpCharTraits, char_allocator_type>;
    using string_allocator_type =
            std::allocator_traits<TpAllocator>::template rebind_alloc<string_type>;
    
    std::vector<string_type, string_allocator_type> values;

    string_type value_builder;
    
    if (str != nullptr)
    {
        for (const TpChar* it = str; *it != '\0'; ++it)
        {
            if (*it != sep)
            {
                if (*it == '\\' &&
                    *(it + 1) != '\0' &&
                    *(it + 1) == sep)
                {
                    continue;
                }
                
                else
                {
                    value_builder += *it;
                }
            }
            
            else
            {
                if (it != str && *(it - 1) == '\\')
                {
                    value_builder += *it;
                }
                
                else if (!value_builder.empty())
                {
                    values.push_back(std::move(value_builder));
                }
            }
            
            if (*(it + 1) == '\0' && !value_builder.empty())
            {
                values.push_back(std::move(value_builder));
            }
        }
    }
    
    return values;
}


/**
 * @brief       Searches for the first occurrence of a substring in a string.
 * @param       str : The null-terminated string to search in.
 * @param       substr : The null-terminated substring to search for.
 * @return      Pointer to the first occurrence of `substr` in `str` if found. `nullptr` if
 *              `substr` is not found. If `substr` is an empty string, returns `str`.
 */
template<typename TpChar>
const TpChar* strstr(const TpChar* str, const TpChar* substr)
{
	std::size_t str_len;
    std::size_t substr_len;

	substr_len = speed::stringutils::strlen(substr);
 
	if (substr_len == 0)
    {
        return str;
    }
    
	str_len = speed::stringutils::strlen(str);
 
	while (str_len >= substr_len)
    {
		str_len--;
  
		if (!memcmp(str, substr, substr_len))
        {
            return str;
        }
        
		str++;
	}
 
	return nullptr;
}


// TODO: Replace this function by a generic one with a predicate.
/**
 * @brief       Get a C string without the values lower than the specified value.
 * @param       str : The C string to erase the values lower than the specified value.
 * @param       val : The value used to discard characters.
 * @return      The C string without the values lower than the specified value.
 */
template<typename TpChar, typename TpIntegral>
TpChar* strdisclower(TpChar* str, const TpIntegral val) noexcept
{
    TpChar* it = str;
    bool shift = false;
    
    if (str != nullptr)
    {
        for (; *it != '\0'; ++it)
        {
            if (*it < val)
            {
                shift = true;
                break;
            }
        }
        
        if (shift)
        {
            for (TpChar* forward_it = it; *forward_it != '\0'; ++it, ++forward_it)
            {
                while (*forward_it < val)
                {
                    ++forward_it;
                    
                    if (*forward_it == '\0')
                    {
                        goto for_end;
                    }
                }
        
                *it = *forward_it;
            }
    
            for_end:
            *it = '\0';
        }
    }
    
    return str;
}


// TODO: Use a regex to match a string and not a character.
/**
 * @brief       Discard the last character of a C string if it is the same that the specified one.
 * @param       str : The C string.
 * @param       val : The specified value.
 * @return      If function success true is returned otherwise false is returned.
 */
template<typename TpChar, typename TpIntegral>
bool strdisclastif(TpChar* str, const TpIntegral val) noexcept
{
    std::size_t str_sz = strlen(str);
    
    if (str != nullptr && str_sz > 0)
    {
        if (str[str_sz - 1] == val)
        {
            str[str_sz - 1] = '\0';
            return true;
        }
    }
    
    return false;
}


/**
 * @brief       Converts a character to lowercase using the specified locale.
 * @param       ch : The character to convert to lowercase.
 * @param       loc : The locale to use for conversion (applies to `char` type only).
 * @return      The lowercase equivalent of the input character.
 */
template<typename TpChar>
TpChar tolower(TpChar ch)
{
    const std::locale& loc = std::locale();
    
    if constexpr (std::is_same_v<TpChar, char>)
    {
        return std::tolower(ch, loc);
    }
    else if constexpr (std::is_same_v<TpChar, wchar_t>)
    {
        return std::towlower(ch);
    }
}


/**
 * @brief       Converts all characters in a string to lowercase.
 * @param       str : The input string to convert.
 * @return      A new string with all characters converted to lowercase.
 */
template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
std::basic_string<TpChar, TpCharTraits, TpCharAlloc> tolower(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str)
{
    std::basic_string<TpChar, TpCharTraits, TpCharAlloc> lowr = str;
    std::transform(lowr.begin(), lowr.end(), lowr.begin(), speed::stringutils::tolower<TpChar>);
    
    return lowr;
}


}


#endif
