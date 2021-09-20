#include <vector>
#include <cstdint>
#include <cstdio>
#include <assert.h>
#include "lat_ssb.h"

typedef uint16_t u16;
typedef uint32_t u32;

static inline u16 ROTL(u16 x, int shift){
    return (x << shift & 0x0FFF) | (x >> (12 - shift));
}

static inline u16 G(u16 x){
    u16 y = ROTL(x, 4);
    x ^= (x >> 3) & (x >> 2) & 0x0111;
    x ^= (x << 3) & (x << 2) & 0x0888;
    y ^= (y >> 1) & (y << 1) & 0x0444;
    y ^= (y << 1) & (y >> 1) & 0x0222;
    return x ^ y;
}

 static inline int bit_count(u16 x){
    x = (x & 0x5555) + ((x >>  1) & 0x5555);
    x = (x & 0x3333) + ((x >>  2) & 0x3333);
    x = (x & 0x0F0F) + ((x >>  4) & 0x0F0F);
    x = (x & 0x00FF) + ((x >>  8) & 0x00FF);
    return x & 0x1;
}

// generating LAT real
auto LAT_real(){
    std::vector<int32_t> lat(1 << 24);
    for(u16 a = 0; a < (1 << 12); a++){
        for(u16 b = 0; b < (1 << 12); b++){
            lat[(a << 12) | b] = - (1 << 11);
            for(u16 x = 0; x < (1 << 12); x++){
                if(!bit_count((a & x) ^ (b & G(x))))
                    lat[(a << 12) | b] += 1;
            }
        }
    }
    return lat;
}

// generating LAT theory
auto LAT_theory(){
    std::vector<int32_t> lat(1 << 24);
    for(u16 a = 0; a < (1 << 12); a++){
        for(auto const& out0 : lat_ssb[(a >> 0) & 0xF]){
            for(auto const& out1 : lat_ssb[(a >> 4) & 0xF]){
                for(auto const& out2 : lat_ssb[(a >> 8) & 0xF]){
                    u16 out_G0 = ((out0[0] >> 4) & 0xF) | \
                                 ((out1[0] >> 4) & 0xF) << 4 | \
                                 ((out2[0] >> 4) & 0xF) << 8;
                    u16 out_G1 = (out0[0] & 0xF) | \
                                 (out1[0] & 0xF) << 4 | \
                                 (out2[0] & 0xF) << 8;
                    if(ROTL(out_G1, 4) == out_G0)
                        lat[(a << 12) | out_G0] = out0[1]*out1[1]*out2[1]*4;
                }
            }
        }
    }
    return lat;
}

int main(){
    auto lat_r = LAT_real();
    printf("Finish real LAT generating, with %lu size.\n", lat_r.size());
    auto lat_t = LAT_theory();
    printf("Finish theory LAT generating, with %lu size.\n", lat_t.size());
    assert(lat_r == lat_t);
    printf("Theory matches real LAT!\n");
    return 0;
}
