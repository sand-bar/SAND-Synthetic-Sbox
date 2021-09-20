#include <vector>
#include <cstdint>
#include <cstdio>
#include <assert.h>
#include "lat_ssb.h"

typedef uint16_t u16;
typedef uint32_t u32;

static inline u16 ROTL(u16 x, int shift){
    return (x << shift) | (x >> (16 - shift));
}

static inline u16 G(u16 x){
    u16 y = ROTL(x, 4);
    x ^= (x >> 3) & (x >> 2) & 0x1111;
    x ^= (x << 3) & (x << 2) & 0x8888;
    y ^= (y >> 1) & (y << 1) & 0x4444;
    y ^= (y << 1) & (y >> 1) & 0x2222;
    return x ^ y;
}

 static inline int bit_count(u16 x){
    x = (x & 0x5555) + ((x >>  1) & 0x5555);
    x = (x & 0x3333) + ((x >>  2) & 0x3333);
    x = (x & 0x0F0F) + ((x >>  4) & 0x0F0F);
    x = (x & 0x00FF) + ((x >>  8) & 0x00FF);
    return x & 0x1;
}

// check inputmask, outputmask with LAT real
static inline int LAT_real_check(u16 inputmask, u16 outputmask){
    int t = - (1 << 15);
    for(u32 x = 0; x < (1 << 16); x++)
        if(!bit_count((inputmask & x) ^ (outputmask & G(x))))
            t += 1;
    return t;
}

// generating LAT theory
void LAT_theory_check(){
    for(u32 a = 0; a < (1 << 16); a++){
        for(auto const& out0 : lat_ssb[(a >> 0) & 0xF]){
            for(auto const& out1 : lat_ssb[(a >> 4) & 0xF]){
                for(auto const& out2 : lat_ssb[(a >> 8) & 0xF]){
                    for(auto const& out3 : lat_ssb[(a >> 12) & 0xF]){
                        u16 out_G0 = ((out0[0] >> 4) & 0xF) | \
                                     ((out1[0] >> 4) & 0xF) << 4 | \
                                     ((out2[0] >> 4) & 0xF) << 8 | \
                                     ((out3[0] >> 4) & 0xF) << 12;
                        u16 out_G1 = (out0[0] & 0xF) | \
                                     (out1[0] & 0xF) << 4 | \
                                     (out2[0] & 0xF) << 8 | \
                                     (out3[0] & 0xF) << 12;
                        if(ROTL(out_G1, 4) == out_G0)
                            assert(LAT_real_check(a, out_G0) == \
                                   out0[1] * out1[1] * out2[1] * out3[1]* 8);
                    }
                }
            }
        }
    }
}

int main(){
    LAT_theory_check();
    printf("Theory matches real LAT!\n");
    return 0;
}
