#pragma once
#include<cassert>
// 例外を投げるマクロ

// nullptrかどうかを判定するマクロ
#define Orca_NullExeption(Ptr)\
    if(!(Ptr))\
        assert(#Ptr##"Is Null");


