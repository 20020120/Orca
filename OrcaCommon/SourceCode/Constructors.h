#pragma once

// --------------------------------- コピーコンストラクタ --------------------------------
#define Delete_CopyConstructor(Type)    Type(const Type& RHS_)=delete;

// --------------------------------- ムーブコンストラクタ --------------------------------
#define Delete_MoveConstructor(Type)    Type(Type&& LHS_)noexcept=delete;
    
    
