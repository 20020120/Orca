#pragma once
#include<memory>
/**
 * \brief UniquePtr�𐶐�����}�N��
 * \param pVariable_ ����������Ptr
 * \param Args_ �R���X�g���N�^�̈�����
 */
#define ORCA_MAKE_UNIQUE_ARG(pVariable_,Args_)\
    pVariable_=std::make_unique<decltype(pVariable_)::element_type>(Args_)
/**
    @def   ORCA_MAKE_UNIQUE
    @brief UniquePtr�𐶐�����}�N��
    @param pVariable_ - ����������Ptr
**/
#define ORCA_MAKE_UNIQUE(pVariable_)\
    pVariable_=std::make_unique<decltype(pVariable_)::element_type>()

#ifdef _DEBUG
#define ORCA_DYNAMIC_CAST(Type_,Variable_)\
    std::dynamic_pointer_cast<Type_>(Variable_)
#else
    #define ORCA_DYNAMIC_CAST(Variable_)\
        static_cast<Type_>(Variable_)
#endif