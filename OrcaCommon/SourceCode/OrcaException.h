#pragma once
#include<cassert>
// 例外を投げるマクロ

// nullptrかどうかを判定するマクロ
#define Orca_NullException(Ptr)\
    if(!(Ptr))\
        assert(#Ptr##"Is Null");
// nullptrでないかどうかを判定するマクロ
#define Orca_NotNullException(Ptr)\
    if((Ptr))\
        assert(#Ptr##"Is Not Null");

// 0以下かどうかを判定するマクロ
#define Orca_LessZeroException(Num)\
    if((Num)<=0)\
        assert(#Num##"Less Zero");
