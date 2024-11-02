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
 * @file        speed/cryptography/operations.cpp
 * @brief       operations functions source.
 * @author      Killian Valverde
 * @date        2024/11/01
 */

#include <cstring>

#include "../stringutils/stringutils.hpp"
#include "city_hash.hpp"


namespace speed::cryptography {


typedef std::pair<std::uint64_t, std::uint64_t> uint128_t;


static const std::uint64_t k0 = 0xc3a5c85c97cb3127ULL;
static const std::uint64_t k1 = 0xb492b66fbe98f273ULL;
static const std::uint64_t k2 = 0x9ae16a3b2f90404fULL;


inline std::uint64_t uint_128_low_64(const uint128_t& x)
{
    return x.first;
}


inline std::uint64_t uint_128_high_64(const uint128_t& x)
{
    return x.second;
}


static std::uint32_t unaligned_load_32(const char *p)
{
    std::uint32_t res;
    memcpy(&res, p, sizeof(res));
    return res;
}


static std::uint64_t unaligned_load_64(const char *p)
{
    std::uint64_t res;
    memcpy(&res, p, sizeof(res));
    return res;
}


uint32_t byteswap_32(uint32_t x)
{
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) << 8)  |
           ((x & 0x00FF0000) >> 8)  |
           ((x & 0xFF000000) >> 24);
}


std::uint64_t byteswap_64(std::uint64_t x)
{
    return ((x & 0x00000000000000FFULL) << 56) |
           ((x & 0x000000000000FF00ULL) << 40) |
           ((x & 0x0000000000FF0000ULL) << 24) |
           ((x & 0x00000000FF000000ULL) << 8)  |
           ((x & 0x000000FF00000000ULL) >> 8)  |
           ((x & 0x0000FF0000000000ULL) >> 24) |
           ((x & 0x00FF000000000000ULL) >> 40) |
           ((x & 0xFF00000000000000ULL) >> 56);
}


#ifdef WORDS_BIGENDIAN
#define uint32_in_expected_order(x) (byteswap_32(x))
#define uint64_in_expected_order(x) (byteswap_64(x))
#else
#define uint32_in_expected_order(x) (x)
#define uint64_in_expected_order(x) (x)
#endif


static std::uint32_t fetch_32(const char *p)
{
    return uint32_in_expected_order(unaligned_load_32(p));
}


static std::uint64_t fetch_64(const char *p)
{
    return uint64_in_expected_order(unaligned_load_64(p));
}


static std::uint32_t rotate_32(std::uint32_t val, int shft)
{
    return shft == 0 ? val : ((val >> shft) | (val << (32 - shft)));
}


static std::uint64_t rotate_64(std::uint64_t val, int shft)
{
    return shft == 0 ? val : ((val >> shft) | (val << (64 - shft)));
}


static std::uint64_t shift_mix(std::uint64_t val)
{
    return val ^ (val >> 47);
}


static uint128_t weak_hash_len_32_with_seeds(
        std::uint64_t w,
        std::uint64_t x,
        std::uint64_t y,
        std::uint64_t z,
        std::uint64_t a,
        std::uint64_t b
)
{
    std::uint64_t c;
    
    a += w;
    b = rotate_64(b + a + z, 21);
    c = a;
    a += x;
    a += y;
    b += rotate_64(a, 44);
    
    return {a + z, b + c};
}


static uint128_t weak_hash_len_32_with_seeds(const char* s, std::uint64_t a, std::uint64_t b)
{
    return weak_hash_len_32_with_seeds(fetch_64(s), fetch_64(s + 8), fetch_64(s + 16),
                                       fetch_64(s + 24), a, b);
}


inline std::uint64_t hash_128_to_64(const uint128_t& x)
{
    const std::uint64_t kMul = 0x9ddfea08eb382d69ULL;
    std::uint64_t a;
    std::uint64_t b;
    
    a = (uint_128_low_64(x) ^ uint_128_high_64(x)) * kMul;
    a ^= (a >> 47);
    b = (uint_128_high_64(x) ^ a) * kMul;
    b ^= (b >> 47);
    b *= kMul;
    
    return b;
}


static std::uint64_t hash_len_16(std::uint64_t u, std::uint64_t v)
{
    return hash_128_to_64(uint128_t(u, v));
}


static std::uint64_t hash_len_16(std::uint64_t u, std::uint64_t v, std::uint64_t mul)
{
    std::uint64_t a;
    std::uint64_t b;
    
    a = (u ^ v) * mul;
    a ^= (a >> 47);
    b = (v ^ a) * mul;
    b ^= (b >> 47);
    b *= mul;
    
    return b;
}


std::uint64_t hash_len_0_to_16(const char *s, size_t len)
{
    if (len >= 8)
    {
        std::uint64_t mul = k2 + len * 2;
        std::uint64_t a = fetch_64(s) + k2;
        std::uint64_t b = fetch_64(s + len - 8);
        std::uint64_t c = rotate_64(b, 37) * mul + a;
        std::uint64_t d = (rotate_64(a, 25) + b) * mul;
        return hash_len_16(c, d, mul);
    }
    
    if (len >= 4)
    {
        std::uint64_t mul = k2 + len * 2;
        std::uint64_t a = fetch_32(s);
        return hash_len_16(len + (a << 3), fetch_32(s + len - 4), mul);
    }
    
    if (len > 0)
    {
        std::uint8_t a = static_cast<std::uint8_t>(s[0]);
        std::uint8_t b = static_cast<std::uint8_t>(s[len >> 1]);
        std::uint8_t c = static_cast<std::uint8_t>(s[len - 1]);
        std::uint32_t y = static_cast<std::uint32_t>(a) + (static_cast<std::uint32_t>(b) << 8);
        std::uint32_t z = static_cast<std::uint32_t>(len) + (static_cast<std::uint32_t>(c) << 2);
        return shift_mix(y * k2 ^ z * k0) * k2;
    }
    
    return k2;
}


static std::uint64_t hash_len_17_to_32(const char *s, size_t len)
{
    std::uint64_t mul = k2 + len * 2;
    std::uint64_t a = fetch_64(s) * k1;
    std::uint64_t b = fetch_64(s + 8);
    std::uint64_t c = fetch_64(s + len - 8) * mul;
    std::uint64_t d = fetch_64(s + len - 16) * k2;
    return hash_len_16(rotate_64(a + b, 43) + rotate_64(c, 30) + d,
                       a + rotate_64(b + k2, 18) + c, mul);
}


static std::uint64_t hash_len_33_to_64(const char *s, size_t len)
{
    std::uint64_t mul = k2 + len * 2;
    std::uint64_t a = fetch_64(s) * k2;
    std::uint64_t b = fetch_64(s + 8);
    std::uint64_t c = fetch_64(s + len - 24);
    std::uint64_t d = fetch_64(s + len - 32);
    std::uint64_t e = fetch_64(s + 16) * k2;
    std::uint64_t f = fetch_64(s + 24) * 9;
    std::uint64_t g = fetch_64(s + len - 8);
    std::uint64_t h = fetch_64(s + len - 16) * mul;
    std::uint64_t u = rotate_64(a + g, 43) + (rotate_64(b, 30) + c) * 9;
    std::uint64_t v = ((a + g) ^ d) + f + 1;
    std::uint64_t w = byteswap_64((u + v) * mul) + h;
    std::uint64_t x = rotate_64(e + f, 42) + c;
    std::uint64_t y = (byteswap_64((v + w) * mul) + g) * mul;
    std::uint64_t z = e + f + c;
    a = byteswap_64((x + z) * mul + y) + b;
    b = shift_mix((z + a) * mul + d + h) * mul;
    return b + x;
}


std::uint64_t city_hash_64(const void* ptr, std::size_t sz)
{
    const char* s = (const char*)ptr;
    
    if (sz <= 32)
    {
        if (sz <= 16)
        {
            return hash_len_0_to_16(s, sz);
        }
        else
        {
            return hash_len_17_to_32(s, sz);
        }
    }
    else if (sz <= 64)
    {
        return hash_len_33_to_64(s, sz);
    }
    
    std::uint64_t x = fetch_64(s + sz - 40);
    std::uint64_t y = fetch_64(s + sz - 16) + fetch_64(s + sz - 56);
    std::uint64_t z = hash_len_16(fetch_64(s + sz - 48) + sz, fetch_64(s + sz - 24));
    uint128_t v = weak_hash_len_32_with_seeds(s + sz - 64, sz, z);
    uint128_t w = weak_hash_len_32_with_seeds(s + sz - 32, y + k1, x);
    x = x * k1 + fetch_64(s);
    
    sz = (sz - 1) & ~static_cast<size_t>(63);
    
    do
    {
        x = rotate_64(x + y + v.first + fetch_64(s + 8), 37) * k1;
        y = rotate_64(y + v.second + fetch_64(s + 48), 42) * k1;
        x ^= w.second;
        y += v.first + fetch_64(s + 40);
        z = rotate_64(z + w.first, 33) * k1;
        v = weak_hash_len_32_with_seeds(s, v.second * k1, x + w.first);
        w = weak_hash_len_32_with_seeds(s + 32, z + w.second, y + fetch_64(s + 16));
        std::swap(z, x);
        s += 64;
        sz -= 64;
    }
    while (sz != 0);
    
    return hash_len_16(hash_len_16(v.first, w.first) + shift_mix(y) * k1 + z,
                       hash_len_16(v.second, w.second) + x);
}


}
