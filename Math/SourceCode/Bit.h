#pragma once
namespace Math
{
    namespace Bit
    {
        // -------------------------------- ƒrƒbƒgŒ^ -------------------------------
        using BIT = unsigned int;

        //--------------------<³Œë”»’è>--------------------//
        bool Check(BIT Master_, BIT Flag_); // Master‚ÉFlag‚ªŠÜ‚Ü‚ê‚Ä‚¢‚é‚©

        //--------------------<‘«‚µZ>--------------------//
        unsigned char Add(unsigned char Master_, unsigned char Flag_)
        {
            return Master_ |= Flag_;
        }
        //--------------------<ˆø‚«Z>--------------------//
        inline unsigned char Sub(unsigned char Master_, unsigned char Flag_)
        {
            return Master_ &= ~Flag_;
        }
        //--------------------<”½“]>--------------------//
        inline unsigned char Inverse(unsigned char Master_, unsigned char Flag_)
        {
            return Master_ ^= Flag_;
        }
        inline unsigned char Inverse(unsigned int Master_, unsigned int Flag_)
        {
            return Master_ ^= Flag_;
        }
        //--------------------<ƒrƒbƒg‚ª—§‚Á‚Ä‚¢‚é”‚ğæ“¾>--------------------//
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