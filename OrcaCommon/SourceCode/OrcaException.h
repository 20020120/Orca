#pragma once
#include<cassert>
// ��O�𓊂���}�N��

// nullptr���ǂ����𔻒肷��}�N��
#define Orca_NullExeption(Ptr)\
    if(!(Ptr))\
        assert(#Ptr##"Is Null");


