#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

/*
下面为了方便写，统一 uint64_t x = challenge;
*/

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    uint64_t x = challenge;
    uint64_t mask32 = UINT64_C(0xFFFFFFFF);
    uint64_t s0, s1, s2;
    s0 = (UINT16_C(0x7dd5) ^ (x >> 32)) + 2 * x;
    s1 = 5 * ((x >> 32) - ((x >> 16) & mask32)) + UINT16_C(0x38c1);
    s2 = (5 * s1) ^ (UINT16_C(0x7f72) + (x & mask32));
    values[0] = (s2 ^ s0) & mask32;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 6;
    uint64_t x = challenge;
    values[0] = ((x >> 18) & 0xFF) * 3;
    for (int i = 1; i < 6; i++) {
        values[i] = 5 * (values[i-1] + i) - ((x >> (i + 10)) & 0x1f); 
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    uint64_t x = challenge;
    values[0] = x & (0x7);
    uint32_t p0 = (uint32_t)(x >> 32) ^ (uint32_t)(x >> 3);
    switch (values[0])
    {
    case 0:
        values[1] = UINT32_C(0x4b35) + 5 * p0;

        break;
    case 1:
        values[1] = (p0 ^ UINT32_C(0x3ef0)) + (p0 >> 1);
        break;
    case 2:
        values[1] = UINT32_C(0xdb13) + 5 * p0;
        break;
    case 3:
       values[1] = (p0 >> 1) + (UINT32_C(0xf4a5) ^ p0);
        break;
    case 4:
        values[1] = (4 * p0) ^ (UINT32_C(0xff6e));
        break;
    case 5:
        values[1] = UINT32_C(0x46d7f) + 5 * p0;
        break;
    case 6:
        values[1] = UINT32_C(0xc4e4) ^ (2 * p0);
        break;
    case 7:
        values[1] = UINT32_C(0x40cbd)+ 5 * p0;
        break;
    default:
        values[1] = 0;
        break;
    }
    return write_answer(out, cap, values, count);
}

uint32_t p4_walk(uint32_t target, uint32_t low, uint32_t high, uint32_t salt) {
    uint32_t mid = low + (high - low) / 2;
    if (target == mid) {
        return (target + 0xa4u) ^ salt;
    }
    uint32_t val = mid + salt;
    if (target < mid) {
        uint32_t r = p4_walk(target, low, mid-1, salt);
        return (3 * r) ^ val ^ 0x04u;
    } else {
        uint32_t r = p4_walk(target, mid+1, high, salt);
        return (3 * r) ^ val ^ 0x11u; 
    }
}
int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    uint64_t x = challenge;
    uint32_t target = x & 0x7fu;
    uint32_t salt = (x >> 7) ^ (x >> 32);
    values[0] = p4_walk(target, 0, 127, salt);
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    uint64_t x = challenge;
    struct node
    {
        int next;
        uint32_t v1;
        uint32_t v2;
    } nodes[] = {
        {1, 0xa6bu, 0xca7au},
        {2, 0xb32u, 0x21e0u},
        {3, 0x77du, 0xba02u},
        {4, 0x179u, 0x4979u},
        {5, 0xdu, 0x9914u},
        {6, 0x83eu, 0xbec3u},
        {7, 0x968u, 0xbf1au},
        {8, 0x308u, 0x4209u},
        {9, 0x445u, 0xb348u},
        {10, 0x27du, 0xd567u},
        {11, 0x65au, 0x4f89u},
        {-1, 0x577u, 0x25c3u},
    };
    int starts[] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint32_t result = (x >> 6) ^ (x >> 32) ^ 0xbc81u;
    uint32_t i = x & 0x7u;
    uint32_t p = starts[i];
    
    for (uint64_t s = 0; s <= ((x >> 3) & (0x3u)); s++) {
        result += nodes[p].v2;
        result *= 5;
        result = result ^ nodes[p].v1;
        values[1] = nodes[p].v1;
        p = nodes[p].next;
    }
    values[0] = result;
    return write_answer(out, cap, values, count);
}

uint32_t p6_0(uint32_t x) {
    return (x + 0xe29eu) ^ (x >> 2);
}

uint32_t p6_1(uint32_t x) {
    return ((x << 7) | (x >> 25)) ^ (0xa9dfu);
}

uint32_t p6_2(uint32_t x) {
    return 0x546eu + (x ^ (x >> 2));
}

uint32_t p6_3(uint32_t x) {
    return (x ^ 0xbd66u) + 8 * x;
}

uint32_t p6_4(uint32_t x) {
    return (x >> 1) | ((x & 1) << 31);
}

uint32_t p6_5(uint32_t x) {
    return (3 * x) ^ (4 * x + 0x9282u);
}


int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    uint64_t x = challenge;
    uint32_t index1 = x & 0x7u, index2 = (x & 0x38u) / 8;
    uint32_t i1, i2, r1, r2;
    i1 = (x >> 6) ^ (x >> 32);
    switch (index1)
    {
    case 0:
        r1 = p6_1(i1);
        break;
    case 1:
        r1 = p6_4(i1);
        break;
    case 2:
        r1 = p6_5(i1);
        break;
    case 3:
        r1 = p6_2(i1);
        break;
    case 4:
        r1 = p6_4(i1);
        break;
    case 5:
        r1 = p6_0(i1);
        break;
    case 6:
        r1 = p6_1(i1);
        break;
    case 7:
        r1 = p6_3(i1);
        break;
    default:
        break;
    }
    i2 = r1 ^ 0x4b7u;
    switch (index2)
    {
    case 0:
        r2 = p6_1(i2);
        break;
    case 1:
        r2 = p6_4(i2);
        break;
    case 2:
        r2 = p6_5(i2);
        break;
    case 3:
        r2 = p6_2(i2);
        break;
    case 4:
        r2 = p6_4(i2);
        break;
    case 5:
        r2 = p6_0(i2);
        break;
    case 6:
        r2 = p6_1(i2);
        break;
    case 7:
        r2 = p6_3(i2);
        break;
    default:
        break;
    }
    values[0] = r2 + 0xac3eu;
    return write_answer(out, cap, values, count);
}

uint32_t secret_mix(uint32_t rdi) {
    return ((7 * rdi) + 0xf2eu) ^ (0xaec6u);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    uint32_t values[3] = {0};
    uint64_t x = challenge, c = context;
    values[0] = c & 0x3u;
    uint32_t rsi = (x >> 32) ^ x;
    uint32_t r8 = (c >> 32) ^ c;
    uint32_t rdi = rsi ^ r8;
    uint32_t r9 = ((c & 0x3) * (0xf2eu)) ^ (c >> 16);
    values[1] = r9 ^ (secret_mix(rdi));
    rsi = rsi >> 7;
    values[2] = (0xaec6 + r8 + values[0] + rsi) ^ values[1];

    snprintf(
        out, cap,
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN "
        "%" PRIu32 " %" PRIu32 " %" PRIu32,
        values[0], values[1], values[2]
    );
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    return 0;
}
