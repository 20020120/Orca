#pragma once
#include<cassert>
// ��O�𓊂���}�N��

// nullptr���ǂ����𔻒肷��}�N��
#define Orca_NullException(Ptr)\
    if(!(Ptr))\
        assert(#Ptr##"Is Null");
// nullptr�łȂ����ǂ����𔻒肷��}�N��
#define Orca_NotNullException(Ptr)\
    if((Ptr))\
        assert(#Ptr##"Is Not Null");

// 0�ȉ����ǂ����𔻒肷��}�N��
#define Orca_LessZeroException(Num)\
    if((Num)<=0)\
        assert(#Num##"Less Zero");
