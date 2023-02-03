#pragma once
#include<memory>
/**
 * \brief UniquePtrを生成するマクロ
 * \param pVariable_ 生成したいPtr
 * \param Args_ コンストラクタの引き数
 */
#define ORCA_MAKE_UNIQUE_ARG(pVariable_,Args_)\
    pVariable_=std::make_unique<decltype(pVariable_)::element_type>(Args_)
/**
    @def   ORCA_MAKE_UNIQUE
    @brief UniquePtrを生成するマクロ
    @param pVariable_ - 生成したいPtr
**/
#define ORCA_MAKE_UNIQUE(pVariable_)\
    pVariable_=std::make_unique<decltype(pVariable_)::element_type>()
