#pragma once

namespace OrcaBinary
{
    /**
     * \brief �o�C�i����ǂݍ��ފ֐�
     * \param FileName_     �t�@�C����
     * \param pBegin_       �擪�A�h���X
     * \param BufferSize_   �ǂݍ��܂��傫���i�֐����Ō��肳���j
     */
    void Load(const wchar_t* FileName_, void** pBegin_, size_t& BufferSize_);
}
