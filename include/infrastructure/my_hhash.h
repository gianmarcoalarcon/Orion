#ifndef __hhash
#define __hhash

/**extra 'h' before hhash can avoid some strange error by the compiler*/
#include <iostream>
#include <immintrin.h>
#include <wmmintrin.h>
#include <cassert>
#include <cstring>
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

    void print_128() {
        int64_t v64val[2];
        memcpy(v64val, &h0, sizeof(v64val));
        printf("h0: 0x%.16llx 0x%.16llx\n", v64val[1], v64val[0]);

        memset(v64val, 0, sizeof(v64val));
        memcpy(v64val, &h1, sizeof(v64val));
        printf("h1: 0x%.16llx 0x%.16llx\n", v64val[1], v64val[0]);
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
