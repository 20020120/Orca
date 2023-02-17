#pragma once

namespace OrcaBinary
{
    /**
     * \brief バイナリを読み込む関数
     * \param FileName_     ファイル名
     * \param pBegin_       先頭アドレス
     * \param BufferSize_   読み込まれる大きさ（関数内で決定される）
     */
    void Load(const wchar_t* FileName_, void** pBegin_, size_t& BufferSize_);
}
