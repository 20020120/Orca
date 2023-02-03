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
        class Dx12ResourceInfoCreator
        {
            // std::unordered_map<HLSLでの変数名, Resource::Dx12ResourceInfo>;
            using ResourcesType = std::unordered_map<std::string, Dx12ResourceInfo>;
        public:
            // 要素を新規作成する
            static void AddResourceInfo(const Shader::ShaderType& ShaderType_, OrcaComPtr(ID3D12ShaderReflection) pReflector_, uint32_t MaxResources_);
            static const ResourcesType& GetResourceMap(const Shader::ShaderType& ShaderType_);
        private:

            inline static std::map<Shader::ShaderType, ResourcesType> mHolder{};
            Dx12ResourceInfoCreator() = default;

        private:
            // --------------------------- 情報作成のためのヘルパー関数 --------------------------
            OrcaNodiscard static ResourceType GetResourceType(const D3D_SHADER_INPUT_TYPE& InputType_);
        };
    }
}