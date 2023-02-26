#pragma once
#include<DirectXMath.h>
namespace Math
{
    struct Vector4 : DirectX::XMFLOAT4
    {
        Vector4();
        Vector4(float x, float y, float z, float w);

        // ------------------------------- アーカイブ -------------------------------
        template<class Archive>
        void serialize(Archive& archive, int version)
        {
            archive(x, y, z, w);
        }
    };
}
