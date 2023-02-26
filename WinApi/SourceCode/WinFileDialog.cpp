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
    o.lStructSize = sizeof(o);		//	構造体サイズ
    o.hwndOwner = hwnd;				//	親ウィンドウのハンドル
    o.lpstrInitialDir = IniDir_;//	初期フォルダー
    o.lpstrFile = FileName_;			//	取得したファイル名を保存するバッファ
    o.nMaxFile = Size_;				//	取得したファイル名を保存するバッファサイズ
    o.lpstrFilter = FileName_;
    o.lpstrDefExt = "fbx";
    o.lpstrTitle = "モデルデータを選択";
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
    OFN.lpstrTitle = "名前を付けて保存";
    return GetSaveFileNameA(&OFN);
}

bool WinApi::FileDialog::GetFolderNameA(std::string& FileName_)
{
    const auto hwnd = GetForegroundWindow();
    IFileDialog* pDialog{};
    FILEOPENDIALOGOPTIONS options{};
    // インスタンス生成
    auto hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));
    if (FAILED(hr))
        return false;

    // 設定の初期化
    pDialog->GetOptions(&options);
    pDialog->SetOptions(options | FOS_PICKFOLDERS);

    // フォルダ選択ダイアログを表示
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
    // ダイアログインスタンスの開放
    pDialog->Release();
    return true;
}
