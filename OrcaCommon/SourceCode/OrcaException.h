#pragma once
#include<cassert>
#include<type_traits>

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
 *  @brief 未実装であると伝え、アサーションを行う
 */
#define Orca_Unimplemented Orca_Assert("未実装です");
