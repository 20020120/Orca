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
            // std::unordered_map<HLSL�ł̕ϐ���, Resource::Dx12ResourceInfo>;
            using ResourcesType = std::unordered_map<std::string, Dx12ResourceInfo>;
        public:
            // �v�f��V�K�쐬����
            static void AddResourceInfo(const Shader::ShaderType& ShaderType_,
                OrcaComPtr(ID3D12ShaderReflection) pReflector_, 
                uint32_t MaxResources_, uint32_t& StartIndex_);
            static const ResourcesType& GetResourceMap(const Shader::ShaderType& ShaderType_);
        private:

            inline static std::map<Shader::ShaderType, ResourcesType> mHolder{};
            Dx12ResourceInfoCreator() = default;

        private:
            // --------------------------- ���쐬�̂��߂̃w���p�[�֐� --------------------------
            OrcaNodiscard static ResourceType GetResourceType(const D3D_SHADER_INPUT_TYPE& InputType_);
        };
    }
}