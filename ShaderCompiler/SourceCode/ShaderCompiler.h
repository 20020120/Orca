#pragma once
namespace ShaderCompiler
{
    /**
     * \brief ���݂���S�Ă�HLSL�t�@�C�����R���p�C��
     * \param HLSLDir_ HLSL������f�B���N�g��
     * \param OutDir_ �o�C�i�����o�͂���f�B���N�g��
     * \param SettingFileDir_ �ݒ�t�@�C���̃f�B���N�g��
     */
    void AllCompile(const char* HLSLDir_, const char* OutDir_, const char* SettingFileDir_);
}
