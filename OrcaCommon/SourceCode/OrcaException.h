#pragma once
#include<cassert>
#include<type_traits>

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


#ifdef _DEBUG
#define Orca_Assert(expression) \
(std::is_bounded_array_v<std::remove_reference_t<decltype(expression)>>&& \
std::is_same_v<const char, std::remove_all_extents_t<std::remove_reference_t<decltype(expression)>>>) \
? \
(void)((_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) : \
assert(expression)
#else
#define Orca_Assert(expression)
#endif

/**
 *  @def   MHTUNIMPLEMENTED
 *  @brief �������ł���Ɠ`���A�A�T�[�V�������s��
 */
#define Orca_Unimplemented Orca_Assert("�������ł�");
