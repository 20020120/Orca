#pragma once

#define OrcaNodiscard [[nodiscard]]

#ifdef _DEBUG
// �f�o�b�O���̂ݐ�����}�N���֐�
#define OrcaExecuteDebug(AAA)\
    AAA
#else
#define OrcaExecuteDebug(AAA)
#endif



