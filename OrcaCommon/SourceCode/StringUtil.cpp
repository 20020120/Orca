#include "StringUtil.h"
#include <Windows.h>
std::string Util::Strings::WStringToString(std::wstring Src_)
{
    // wstring �� SJIS
    int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, Src_.c_str()
        , -1, (char*)NULL, 0, NULL, NULL);

    // �o�b�t�@�̎擾
    CHAR* cpMultiByte = new CHAR[iBufferSize];

    // wstring �� SJIS
    WideCharToMultiByte(CP_OEMCP, 0, Src_.c_str(), -1, cpMultiByte
        , iBufferSize, NULL, NULL);

    // string�̐���
    std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    // �o�b�t�@�̔j��
    delete[] cpMultiByte;

    // �ϊ����ʂ�Ԃ�
    return(oRet);
}
