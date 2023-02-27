#include"pch.h"
#include<vector>
#include<DirectXMath.h>
#include<fstream>
#include<string>
#include<filesystem>
#include<ResourceUploadBatch.h>

#include"Obj.h"
#include"GraphicsLogger.h"
#include"DescriptorPool.h"
#include"Graphics.h"
#include"MacroMemory.h"

Resource::Obj::~Obj()
{
    
}

void Resource::Obj::Initialize(const wchar_t* ObjPath_)
{
    std::vector<VertexData> vertices{};
    std::vector<uint32_t> indices{};
    std::wstring textureName{};

    const auto pDevice = Graphics::Graphics::GetDevice();

    // パスからデータをパースする
    Parse(ObjPath_, vertices, indices, textureName);
    // 頂点バッファを作成する
    CreateVertexBuffer(Graphics::Graphics::GetDevice(), vertices);
    // インデックスバッファを作成する
    CreateIndexBuffer(pDevice, indices);
    mTextureName = textureName;

    // -------------------------------- 変数を初期化する -------------------------------
    m_VertexCounts = static_cast<UINT>(vertices.size());
}

void Resource::Obj::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) const
{
    // -------------------------------- コマンドを積む --------------------------------
    pCmdList_->IASetVertexBuffers(0, 1, &mVbView);
    pCmdList_->IASetIndexBuffer(&mIbView);
    pCmdList_->DrawIndexedInstanced(m_VertexCounts, 1, 0, 0, 0);
}

void Resource::Obj::Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_, std::vector<uint32_t>& Indices_, 
    std::wstring& TextureName_)
{
    // Objファイルから頂点データをパースする
    Vertices_.clear();
    Indices_.clear();

    uint32_t current_index{ 0 };
    std::vector<DirectX::XMFLOAT3> positions;
    std::vector<DirectX::XMFLOAT3> normals;
    std::vector<DirectX::XMFLOAT2> uvs;
    std::vector<std::wstring> mtlFileNames;

    std::wifstream fin(ObjPath_);
    _ASSERT_EXPR(fin, L"'OBJ file not found.");
    wchar_t command[256];
    while (fin)
    {
        fin >> command;
        if (0 == wcscmp(command, L"v"))
        {
            float x, y, z;
            fin >> x >> y >> z;
            positions.push_back({ x,y,z });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            FLOAT i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i,j,k });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            uvs.push_back({ u,1.0f - v });

            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                VertexData vertex;
                size_t v, vt, vn;

                fin >> v;
                vertex.mPosition = positions.at(v - 1);
                //次の文字がL'/'の場合
                if (L'/' == fin.peek())
                {
                    // 1文字削除
                    fin.ignore(1);
                    //次の文字がL'/'ではない場合
                    if (L'/' != fin.peek())
                    {
                        fin >> vt;
                        vertex.mUv = uvs.at(vt - 1);
                    }
                    //次の文字がL'/'の場合
                    if (L'/' == fin.peek())
                    {
                        //1文字削除
                        fin.ignore(1);
                        fin >> vn;
                        //法線情報の入力
                        vertex.mNormal = normals.at(vn - 1);
                    }
                }
                Vertices_.push_back(vertex);
                Indices_.push_back(current_index++);
            }
            //一番上にある１行を削除
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"mtllib"))
        {
            wchar_t mtllib[256];
            fin >> mtllib;
            mtlFileNames.push_back(mtllib);
        }
        else
        {
            //一番上にある１行を削除
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    // マテリアル情報をパース
    std::filesystem::path mtlFilename(ObjPath_);
    //ファイル名の部分のみMTLファイル名に入れ替える
    mtlFilename.replace_filename(std::filesystem::path(mtlFileNames[0]).filename());

    fin.open(mtlFilename);
    _ASSERT_EXPR(fin, L"MTL file not found");
    while (fin)
    {
        fin >> command;
        if (0 == wcscmp(command, L"map_Kd"))
        {
            fin.ignore();
            wchar_t map_Kd[256];
            fin >> map_Kd;

            std::filesystem::path path(ObjPath_);
            path.replace_filename(std::filesystem::path(map_Kd).filename());
            TextureName_ = path;
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();
}

void Resource::Obj::CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_,
    const std::vector<VertexData>& Vertices_)
{
    // ------------------------------ 頂点バッファを作成する ------------------------------
    // ヒーププロパティの設定
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // リソースの設定
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(VertexData)*Vertices_.size();
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    auto hr = pDevice_->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpVertexBuffer.GetAddressOf())
    );

    OrcaDebug::GraphicsLog("頂点バッファを作成", hr);

    // マッピングする
    void* ptr{};
    hr = mpVertexBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("マッピング", hr);

    // 頂点データをマッピング先に指定
    memcpy(ptr, Vertices_.data(), sizeof(VertexData) * Vertices_.size());

    // マッピング解除
    mpVertexBuffer->Unmap(0, nullptr);

    // ビューの設定
    mVbView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
    mVbView.SizeInBytes = sizeof(VertexData) * static_cast<UINT>(Vertices_.size());
    mVbView.StrideInBytes = sizeof(VertexData);

}

void Resource::Obj::CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<uint32_t>& Indices_)
{
    // ヒーププロパティを設定
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // リソースの設定
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(uint32_t) * Indices_.size();
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // リソースを生成
    auto hr = pDevice_->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpIndexBuffer.ReleaseAndGetAddressOf())
    );
    OrcaDebug::GraphicsLog("インデックスバッファを作成", hr);

    // マッピング
    void* ptr = nullptr;
    hr = mpIndexBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("マッピング", hr);
    memcpy(ptr, Indices_.data(), sizeof(uint32_t) * Indices_.size());
    mpIndexBuffer->Unmap(0, nullptr);

    // インデックスバッファビューを作成
    mIbView.BufferLocation = mpIndexBuffer->GetGPUVirtualAddress();
    mIbView.Format = DXGI_FORMAT_R32_UINT;
    mIbView.SizeInBytes = static_cast<UINT>(sizeof(uint32_t)*Indices_.size());
}


void Resource::Obj::CreateTexture(OrcaComPtr(ID3D12Device) pDevice_, Graphics::DescriptorPool* pPool_,
    OrcaComPtr(ID3D12CommandQueue) pCommandQueue_, std::wstring TexturePath_)
{
   // // テクスチャを作成
   // DirectX::ResourceUploadBatch batch(pDevice_.Get());
   // batch.Begin();
   // // リソースを生成
   // mTexture = std::make_unique<Graphics::Resource::Texture>(2);
   // mTexture->Load(TexturePath_.c_str(), batch);
   // // コマンドを実行
   // const auto future = batch.End(pCommandQueue_.Get());
   // future.wait();
}
std::wstring Resource::Obj::GetTextureName() const
{
    return mTextureName;
}
