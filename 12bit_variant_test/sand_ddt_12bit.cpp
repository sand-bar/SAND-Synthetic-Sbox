#include <vector>
#include <cstdint>
#include <cstdio>
#include <assert.h>
#include "ddt_ssb.h"

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

// generating DDT theory
auto DDT_theory(){
    std::vector<u32> ddt(1 << 24);
    for(u32 d = 0; d < (1 << 12); d++){
        for(auto const& out0 : ddt_ssb[(d >> 0) & 0xF]){
            for(auto const& out1 : ddt_ssb[(d >> 4) & 0xF]){
                for(auto const& out2 : ddt_ssb[(d >> 8) & 0xF]){
                    u16 out_G0 = ((out0[0] >> 4) & 0xF) | \
                                 ((out1[0] >> 4) & 0xF) << 4 | \
                                 ((out2[0] >> 4) & 0xF) << 8;
                    u16 out_G1 = (out0[0] & 0xF) | \
                                 (out1[0] & 0xF) << 4 | \
                                 (out2[0] & 0xF) << 8;
                    u16 out_G = (u16)(ROTL(out_G1, 4) ^ out_G0);
                    ddt[(d << 12) | out_G] += out0[1] * out1[1] * out2[1];
                }
            }
        }
    }
    return ddt;
}

// generating real DDT
auto DDT_real(){
    std::vector<u32> ddt(1 << 24);
    for(u32 d = 0; d < (1 << 12); d++)
        for(u32 x = 0; x < (1 << 12); x++)
            ddt[(d << 12) | (G(x) ^ G(x ^ d))] += 1;
    return ddt;
}

int main(){
    auto ddt_r = DDT_real();
    printf("Finish real DDT generating, with %lu size.\n", ddt_r.size());
    auto ddt_t = DDT_theory();
    printf("Finish theory DDT generating, with %lu size.\n", ddt_t.size());
    assert(ddt_r == ddt_t);
    printf("Theory matches real DDT!\n");
    return 0;
}
