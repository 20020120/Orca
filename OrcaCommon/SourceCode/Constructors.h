#pragma once

// --------------------------------- �R�s�[�R���X�g���N�^ --------------------------------
#define Delete_CopyConstructor(Type)    Type(const Type& RHS_)=delete;

// --------------------------------- ���[�u�R���X�g���N�^ --------------------------------
#define Delete_MoveConstructor(Type)    Type(Type&& LHS_)noexcept=delete;
    
    
