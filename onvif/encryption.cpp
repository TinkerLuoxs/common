//
// Created by Luoxs on 2017-03-16
//
#include <string>
#include "encryption.h"


namespace base64
{
static const std::string base64_alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string encode(const char *src, unsigned int in_len)
{
    int i = 0;
    int j = 0;
    unsigned char array3[3];
    unsigned char array4[4];
    std::string enc64;
    const unsigned char *p = (const unsigned char *)src;

    while (in_len--)
    {
        array3[i++] = *(p++);
        if (i == 3)
        {
            array4[0] = (array3[0] & 0xfc) >> 2;
            array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
            array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
            array4[3] = array3[2] & 0x3f;

            for (i = 0; i < 4; i++)
            {
                enc64 += base64_alphabet[array4[i]];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            array3[j] = '\0';
        }

        array4[0] = (array3[0] & 0xfc) >> 2;
        array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
        array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
        array4[3] = array3[2] & 0x3f;

        for (j = 0; j < i + 1; j++)
        {
            enc64 += base64_alphabet[array4[j]];
        }

        while ((i++ < 3))
        {
            enc64 += '=';
        }
    }

    return enc64;
}

std::string decode(const std::string& src)
{
    int i = 0;
    int j = 0;
    unsigned char array3[3];
    unsigned char array4[4];

    size_t in_len = src.size();
    const char *p = src.data();
    std::string dec64;

    while (in_len-- && (*p != '=') && is_base64(*p))
    {
        array4[i++] = *p++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                array4[i] = (unsigned char)base64_alphabet.find(array4[i]);
            }

            array3[0] = (array4[0] << 2) + ((array4[1] & 0x30) >> 4);
            array3[1] = ((array4[1] & 0xf) << 4) + ((array4[2] & 0x3c) >> 2);
            array3[2] = ((array4[2] & 0x3) << 6) + array4[3];

            for (i = 0; (i < 3); i++)
            {
                dec64 += array3[i];
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            array4[j] = 0;
        }

        for (j = 0; j < 4; j++)
        {
            array4[j] = (unsigned char)base64_alphabet.find(array4[j]);
        }

        array3[0] = (array4[0] << 2) + ((array4[1] & 0x30) >> 4);
        array3[1] = ((array4[1] & 0xf) << 4) + ((array4[2] & 0x3c) >> 2);
        array3[2] = ((array4[2] & 0x3) << 6) + array4[3];

        for (j = 0; j < i - 1; j++)
        {
            dec64 += array3[j];
        }
    }

    return dec64;
}

}

namespace sha
{

#define ROL32(p_val32,p_nBits) (((p_val32)<<(p_nBits))|((p_val32)>>(32-(p_nBits))))

#ifdef SHA1_LITTLE_ENDIAN
#define SHABLK0(i) (m_block->l[i] = \
	(ROL32(m_block->l[i],24) & 0xFF00FF00) | (ROL32(m_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (m_block->l[i])
#endif

#define SHABLK(i) (m_block->l[i&15] = ROL32(m_block->l[(i+13)&15] ^ \
	m_block->l[(i+8)&15] ^ m_block->l[(i+2)&15] ^ m_block->l[i&15],1))

// SHA-1 rounds, (R0+R1), R2, R3, R4 are the different operations used in SHA1
#define S_R0(v,w,x,y,z,i) {z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5);w=ROL32(w,30);}
#define S_R1(v,w,x,y,z,i) {z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5);w=ROL32(w,30);}
#define S_R2(v,w,x,y,z,i) {z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5);w=ROL32(w,30);}
#define S_R3(v,w,x,y,z,i) {z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5);w=ROL32(w,30);}
#define S_R4(v,w,x,y,z,i) {z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5);w=ROL32(w,30);}

sha1::sha1()
{
    m_block = (SHA1_WORKSPACE_BLOCK*)m_workspace;
    reset();
}

void sha1::reset()
{
    // SHA1 initialization constants
    m_state[0] = 0x67452301;
    m_state[1] = 0xEFCDAB89;
    m_state[2] = 0x98BADCFE;
    m_state[3] = 0x10325476;
    m_state[4] = 0xC3D2E1F0;

    m_count[0] = 0;
    m_count[1] = 0;

    memset(m_buffer, 0, sizeof(m_buffer));
}

void sha1::append(const char* pbData, size_t uLen)
{
    unsigned int j = ((m_count[0] >> 3) & 0x3F);

    if ((m_count[0] += (uLen << 3)) < (uLen << 3))
        ++m_count[1]; // Overflow

    m_count[1] += (uLen >> 29);

    unsigned int i;
    if ((j + uLen) > 63)
    {
        i = 64 - j;
        memcpy(&m_buffer[j], pbData, i);
        transform(m_state, m_buffer);

        for (; (i + 63) < uLen; i += 64)
            transform(m_state, &pbData[i]);

        j = 0;
    }
    else i = 0;

    if ((uLen - i) != 0)
        memcpy(&m_buffer[j], &pbData[i], uLen - i);

}

void sha1::finish(unsigned char* pbDest20)
{
    unsigned int i;

    char pbFinalCount[8];
    for (i = 0; i < 8; ++i)
        pbFinalCount[i] = static_cast<char>((m_count[((i >= 4) ? 0 : 1)] >>
        ((3 - (i & 3)) * 8)) & 0xFF); // Endian independent

    append("\200", 1);

    while ((m_count[0] & 504) != 448)
        append("\0", 1);

    append(pbFinalCount, 8); // Cause a Transform()

    for (i = 0; i < 20; ++i)
        pbDest20[i] = static_cast<unsigned char>((m_state[i >> 2] >> ((3 -
        (i & 3)) * 8)) & 0xFF);

    // Wipe variables for security reasons
    reset();
    transform(m_state, m_buffer);
}

void sha1::transform(unsigned int* pState, const void* pBuffer)
{
    unsigned int a = pState[0], b = pState[1], c = pState[2], d = pState[3], e = pState[4];

    memcpy(m_block, pBuffer, 64);

    // 4 rounds of 20 operations each, loop unrolled
    S_R0(a, b, c, d, e, 0); S_R0(e, a, b, c, d, 1); S_R0(d, e, a, b, c, 2); S_R0(c, d, e, a, b, 3);
    S_R0(b, c, d, e, a, 4); S_R0(a, b, c, d, e, 5); S_R0(e, a, b, c, d, 6); S_R0(d, e, a, b, c, 7);
    S_R0(c, d, e, a, b, 8); S_R0(b, c, d, e, a, 9); S_R0(a, b, c, d, e, 10); S_R0(e, a, b, c, d, 11);
    S_R0(d, e, a, b, c, 12); S_R0(c, d, e, a, b, 13); S_R0(b, c, d, e, a, 14); S_R0(a, b, c, d, e, 15);
    S_R1(e, a, b, c, d, 16); S_R1(d, e, a, b, c, 17); S_R1(c, d, e, a, b, 18); S_R1(b, c, d, e, a, 19);
    S_R2(a, b, c, d, e, 20); S_R2(e, a, b, c, d, 21); S_R2(d, e, a, b, c, 22); S_R2(c, d, e, a, b, 23);
    S_R2(b, c, d, e, a, 24); S_R2(a, b, c, d, e, 25); S_R2(e, a, b, c, d, 26); S_R2(d, e, a, b, c, 27);
    S_R2(c, d, e, a, b, 28); S_R2(b, c, d, e, a, 29); S_R2(a, b, c, d, e, 30); S_R2(e, a, b, c, d, 31);
    S_R2(d, e, a, b, c, 32); S_R2(c, d, e, a, b, 33); S_R2(b, c, d, e, a, 34); S_R2(a, b, c, d, e, 35);
    S_R2(e, a, b, c, d, 36); S_R2(d, e, a, b, c, 37); S_R2(c, d, e, a, b, 38); S_R2(b, c, d, e, a, 39);
    S_R3(a, b, c, d, e, 40); S_R3(e, a, b, c, d, 41); S_R3(d, e, a, b, c, 42); S_R3(c, d, e, a, b, 43);
    S_R3(b, c, d, e, a, 44); S_R3(a, b, c, d, e, 45); S_R3(e, a, b, c, d, 46); S_R3(d, e, a, b, c, 47);
    S_R3(c, d, e, a, b, 48); S_R3(b, c, d, e, a, 49); S_R3(a, b, c, d, e, 50); S_R3(e, a, b, c, d, 51);
    S_R3(d, e, a, b, c, 52); S_R3(c, d, e, a, b, 53); S_R3(b, c, d, e, a, 54); S_R3(a, b, c, d, e, 55);
    S_R3(e, a, b, c, d, 56); S_R3(d, e, a, b, c, 57); S_R3(c, d, e, a, b, 58); S_R3(b, c, d, e, a, 59);
    S_R4(a, b, c, d, e, 60); S_R4(e, a, b, c, d, 61); S_R4(d, e, a, b, c, 62); S_R4(c, d, e, a, b, 63);
    S_R4(b, c, d, e, a, 64); S_R4(a, b, c, d, e, 65); S_R4(e, a, b, c, d, 66); S_R4(d, e, a, b, c, 67);
    S_R4(c, d, e, a, b, 68); S_R4(b, c, d, e, a, 69); S_R4(a, b, c, d, e, 70); S_R4(e, a, b, c, d, 71);
    S_R4(d, e, a, b, c, 72); S_R4(c, d, e, a, b, 73); S_R4(b, c, d, e, a, 74); S_R4(a, b, c, d, e, 75);
    S_R4(e, a, b, c, d, 76); S_R4(d, e, a, b, c, 77); S_R4(c, d, e, a, b, 78); S_R4(b, c, d, e, a, 79);

    // Add the working vars back into state
    pState[0] += a;
    pState[1] += b;
    pState[2] += c;
    pState[3] += d;
    pState[4] += e;
}

}
