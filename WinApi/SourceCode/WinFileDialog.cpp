#pragma comment(lib, "Comdlg32.lib")
#include"WinFileDialog.h"
#include<Windows.h>
#include<ShObjIdl_core.h>
#include"StringUtil.h"
bool WinApi::FileDialog::OpenFileNameA(char* FileName_, int Size_, const char* IniDir_, const char* Filter_)
{
    const auto hwnd = GetForegroundWindow();
    OPENFILENAMEA o;
    FileName_[0] = '\0';
    ZeroMemory(&o, sizeof(o));
    o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
    o.hwndOwner = hwnd;				//	�e�E�B���h�E�̃n���h��
    o.lpstrInitialDir = IniDir_;//	�����t�H���_�[
    o.lpstrFile = FileName_;			//	�擾�����t�@�C������ۑ�����o�b�t�@
    o.nMaxFile = Size_;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
    o.lpstrFilter = FileName_;
    o.lpstrDefExt = "fbx";
    o.lpstrTitle = "���f���f�[�^��I��";
    o.nFilterIndex = 1;

    return GetOpenFileNameA(&o);
}

bool WinApi::FileDialog::SaveFileNameA(char* FileName_)
{
    const auto hwnd = GetForegroundWindow();
    OPENFILENAMEA OFN{};
    FileName_[0] = '\0';
    ZeroMemory(&OFN, sizeof(OFN));
    OFN.lStructSize = sizeof(OFN);
    OFN.hwndOwner = hwnd;

    OFN.lpstrFilter = nullptr;
    OFN.lpstrFile = FileName_;
    OFN.nMaxFile = MAX_PATH;
    OFN.lpstrTitle = "���O��t���ĕۑ�";
    return GetSaveFileNameA(&OFN);
}

bool WinApi::FileDialog::GetFolderNameA(std::string& FileName_)
{
    const auto hwnd = GetForegroundWindow();
    IFileDialog* pDialog{};
    FILEOPENDIALOGOPTIONS options{};
    // �C���X�^���X����
    auto hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));
    if (FAILED(hr))
        return false;

    // �ݒ�̏�����
    pDialog->GetOptions(&options);
    pDialog->SetOptions(options | FOS_PICKFOLDERS);

    // �t�H���_�I���_�C�A���O��\��
    hr = pDialog->Show(hwnd);
    if (FAILED(hr))
    {
        pDialog->Release();
        return false;
    }
    IShellItem* pItem = nullptr;
    PWSTR pPath = nullptr;
    hr = pDialog->GetResult(&pItem);
    if(FAILED(hr))
    {
        pDialog->Release();
        return false;
    }
    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);
    if (FAILED(hr))
    {
        pDialog->Release();
        return false;
    }
    FileName_ = Util::Strings::WStringToString(pPath);
    pDialog->Close(hr);
    // �_�C�A���O�C���X�^���X�̊J��
    pDialog->Release();
    return true;
}
