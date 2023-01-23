#pragma once

#include"ShaderType.h"
#include"Dx12ResourceInfo.h"
#include"GraphicsMacro.h"
#include"MacroMinmam.h"

#include <map>
#include <unordered_map>
#include <d3dcommon.h>

struct ID3D12ShaderReflection;
;

namespace OrcaGraphics
{
    namespace Resource
    {
        class Dx12ResourceCreator
        {
        public:
            // 要素を新規作成する
            static void AddResourceInfo(const Shader::ShaderType& ShaderType_, OrcaComPtr(ID3D12ShaderReflection) pReflector_, uint32_t MaxResources_);

        private:
            inline static std::map<Shader::ShaderType, std::unordered_map<std::string, Resource::Dx12ResourceInfo>> mHolder{};
            Dx12ResourceCreator() = default;

        private:
            // --------------------------- 情報作成のためのヘルパー関数 --------------------------
            OrcaNodiscard static Resource::ResourceType GetResourceType(const D3D_SHADER_INPUT_TYPE& InputType_);
        };
    }
}