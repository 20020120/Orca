#pragma once

#define OrcaNodiscard [[nodiscard]]

#ifdef _DEBUG
// デバッグ時のみ生きるマクロ関数
#define OrcaExecuteDebug(AAA)\
    AAA
#else
#define OrcaExecuteDebug(AAA)
#endif



