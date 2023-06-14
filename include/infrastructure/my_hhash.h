#ifndef __hhash
#define __hhash

/**extra 'h' before hhash can avoid some strange error by the compiler*/
#include <iostream>
#include <immintrin.h>
#include <wmmintrin.h>
#include <cassert>
#include <iomanip>
#include "flo-shani-aesni/sha256/flo-shani.h"
// #define USESHA3
#ifdef USESHA3
extern "C"
{
#include "lib/libXKCP.a.headers/SimpleFIPS202.h"
}
#endif

class __hhash_digest
{
public:
	__m128i h0, h1;

     void print() {
        alignas(16) unsigned char bytes[32];
        _mm_store_si128((__m128i*)bytes, h0);
        _mm_store_si128((__m128i*)(bytes + 16), h1);

        // Print h0 and h1 in hexadecimal format
        std::cout << "h0: ";
        for (int i = 0; i < 16; i++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
        }
        std::cout << std::endl;

        std::cout << "h1: ";
        for (int i = 16; i < 32; i++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
        }
        std::cout << std::endl;
    }
};

inline bool equals(const __hhash_digest &a, const __hhash_digest &b)
{
	__m128i v0 = _mm_xor_si128(a.h0, b.h0);
	__m128i v1 = _mm_xor_si128(a.h1, b.h1);
	return _mm_test_all_zeros(v0, v0) && _mm_test_all_zeros(v1, v1);
}
#include <cstring>
inline void my_hhash(const void *src, void *dst)
{
#ifdef USESHA3
	SHA3_256((unsigned char *)dst, (const unsigned char *)src, 64);
#else
	// memset(dst, 0, sizeof(__hhash_digest));
	sha256_update_shani((const unsigned char *)src, 64, (unsigned char *)dst);
#endif
}

#endif
