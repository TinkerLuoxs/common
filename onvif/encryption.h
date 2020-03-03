//
// Created by Luoxs on 2017-03-16
//
#pragma once


namespace base64
{
    std::string encode(const char *src, unsigned int len);

    std::string decode(const std::string& src);
}

namespace sha
{

#if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
#define SHA1_LITTLE_ENDIAN
#endif

    class sha1
    {
    public:
        sha1();

        void reset();
        // Hash in binary data and strings
        void append(const char* pbData, size_t uLen);
        // Finalize hash; 
        void finish(unsigned char* pbDest20);

    private:
        // Private SHA-1 transformation
        void transform(unsigned int* pState, const void* pBuffer);

        // Member variables
        unsigned int m_state[5];
        unsigned int m_count[2];
        unsigned char m_buffer[64];
        typedef union
        {
            unsigned char c[64];
            unsigned int l[16];
        } SHA1_WORKSPACE_BLOCK;
        unsigned char m_workspace[64];
        SHA1_WORKSPACE_BLOCK* m_block; // SHA1 pointer to the byte array above
    };

}
