#include"Binary.h"
#include"MacroMinmam.h"
#include"OrcaException.h"
#include<filesystem>
#include<fstream>

void OrcaBinary::Load(const wchar_t* FileName_, void** pBegin_, size_t& BufferSize_)
{
    // �f�o�b�O�r���h�̎��̂݃t�@�C���̐����m�F������
    OrcaExecuteDebug(
        if (!std::filesystem::exists(FileName_))
            Orca_Assert("OrcaBinary::Load__�t�@�C�����J�����Ƃ��ł��܂���"););
    if (*pBegin_)
    {
        Orca_Assert("OrcaBinary::Load__�擪�A�h���X�����Ƀf�[�^�������Ă��܂�");
    }
    // ----------------------------- �t�@�C�����J���ăf�[�^�𗬂� ----------------------------
    std::ifstream ifs;
    ifs.open(FileName_, std::ios::binary | std::ios::in);
    // ------------------------------- �t�@�C���T�C�Y�擾 -------------------------------
    ifs.seekg(0, std::ios_base::end);
    BufferSize_ = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    
    *pBegin_ = new char[BufferSize_]();
    ifs.read(static_cast<char*>(*pBegin_), static_cast<std::streamsize>(BufferSize_));
    ifs.close();
}