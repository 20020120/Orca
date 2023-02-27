#pragma once
#include<wrl.h>
// ------------------------------------ ëOï˚êÈåæ -----------------------------------
struct IDxcUtils;
struct IDxcCompiler3;
struct IDxcIncludeHandler;
struct IDxcResult;

namespace ShaderCompiler
{
    struct DXC
    {
        bool Compile(Microsoft::WRL::ComPtr<IDxcUtils> pUtils_,
            Microsoft::WRL::ComPtr<IDxcCompiler3> pCompiler_,
            Microsoft::WRL::ComPtr<IDxcIncludeHandler> pIncludeHandler_,
            const wchar_t* FileName_, const wchar_t* Target_, const wchar_t* OutDir_);

    private:
        Microsoft::WRL::ComPtr<IDxcResult> mpResults;
    };
}
