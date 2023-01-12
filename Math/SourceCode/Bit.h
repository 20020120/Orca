#pragma once
namespace Math
{
    namespace Bit
    {
        // -------------------------------- ビット型 -------------------------------
        using BIT = unsigned int;

        //--------------------<正誤判定>--------------------//
        bool Check(BIT Master_, BIT Flag_); // MasterにFlagが含まれているか

        //--------------------<足し算>--------------------//
        unsigned char Add(unsigned char Master_, unsigned char Flag_)
        {
            return Master_ |= Flag_;
        }
        //--------------------<引き算>--------------------//
        inline unsigned char Sub(unsigned char Master_, unsigned char Flag_)
        {
            return Master_ &= ~Flag_;
        }
        //--------------------<反転>--------------------//
        inline unsigned char Inverse(unsigned char Master_, unsigned char Flag_)
        {
            return Master_ ^= Flag_;
        }
        inline unsigned char Inverse(unsigned int Master_, unsigned int Flag_)
        {
            return Master_ ^= Flag_;
        }
        //--------------------<ビットが立っている数を取得>--------------------//
        inline unsigned int DigitsCount(unsigned char v)
        {
            v = (v & 0x55555555) + (v >> 1 & 0x55555555);
            v = (v & 0x33333333) + (v >> 2 & 0x33333333);
            v = (v & 0x0f0f0f0f) + (v >> 4 & 0x0f0f0f0f);
            //v = (v & 0x00ff00ff) + (v >> 8 & 0x00ff00ff);
            //v = (v & 0x0000ffff) + (v >> 16 & 0x0000ffff);
            return v;
        }
        inline unsigned int DigitsCount(unsigned int v)
        {
            v = (v & 0x55555555) + (v >> 1 & 0x55555555);
            v = (v & 0x33333333) + (v >> 2 & 0x33333333);
            v = (v & 0x0f0f0f0f) + (v >> 4 & 0x0f0f0f0f);
            v = (v & 0x00ff00ff) + (v >> 8 & 0x00ff00ff);
            v = (v & 0x0000ffff) + (v >> 16 & 0x0000ffff);
            return v;
        }


    }
}