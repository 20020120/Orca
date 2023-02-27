#pragma once
namespace ShaderCompiler
{
    /**
     * \brief 存在する全てのHLSLファイルをコンパイル
     * \param HLSLDir_ HLSLがあるディレクトリ
     * \param OutDir_ バイナリを出力するディレクトリ
     * \param SettingFileDir_ 設定ファイルのディレクトリ
     */
    void AllCompile(const char* HLSLDir_, const char* OutDir_, const char* SettingFileDir_);
}
