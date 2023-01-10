#include"pch.h"
#include"Obj.h"
#include<vector>
#include<DirectXMath.h>
#include<fstream>
#include<d3d12.h>
#include"GraphicsLogger.h"
#include"DescriptorPool.h"

Model::Obj::~Obj()
{
    // 定数バッファを削除
    mCb.Finalize();
}

void Model::Obj::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, OrcaGraphics::DescriptorPool* pPool_, const wchar_t* ObjPath_)
{
    std::vector<VertexData> vertices{};
    std::vector<uint32_t> indices{};
    // パスからデータをパースする
    Parse(ObjPath_, vertices, indices);
    // 頂点バッファを作成する
    CreateVertexBuffer(pDevice_, vertices);
    // インデックスバッファを作成する
    CreateIndexBuffer(pDevice_, indices);
    // 定数バッファを作成
    CreateConstantBuffer(pDevice_, pPool_);

    // -------------------------------- 変数を初期化する -------------------------------
    m_VertexCounts = static_cast<UINT>(vertices.size());
}

void Model::Obj::Update(float Dt_)
{
    static float angle = 0.0f;
    angle += DirectX::XMConvertToRadians(60.0f) * Dt_;
    const auto ptr = mCb.GetPtr<Cb_Obj>();
    ptr->World = DirectX::XMMatrixRotationX(45.0f) * DirectX::XMMatrixRotationY(angle) *
        DirectX::XMMatrixTranslation(0.0f, 0.0f, -10.0f);
    ptr->World = DirectX::XMMatrixTranspose(ptr->World);
}

void Model::Obj::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    // -------------------------------- コマンドを積む --------------------------------
    pCmdList_->SetGraphicsRootDescriptorTable(1, mCb.GetGPU());
    pCmdList_->IASetVertexBuffers(0, 1, &mVbView);
    pCmdList_->IASetIndexBuffer(&mIbView);
    pCmdList_->DrawIndexedInstanced(m_VertexCounts, 1, 0, 0, 0);
}

void Model::Obj::Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_, std::vector<uint32_t>& Indices_)
{
    // Objファイルから頂点データをパースする
    Vertices_.clear();
    Indices_.clear();

    uint32_t current_index{ 0 };
    std::vector<DirectX::XMFLOAT3> positions;
    std::vector<DirectX::XMFLOAT3> normals;

    std::wifstream fin(ObjPath_);
    _ASSERT_EXPR(fin, L"'OBJ file not found.");
    while (fin)
    {
        wchar_t command[256];
        fin >> command;
        if (0 == wcscmp(command, L"v"))
        {
            float x, y, z;
            fin >> x >> y >> z;
            positions.push_back({ x, y, z });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            FLOAT i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i, j, k });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                VertexData vertex;
                size_t v;

                fin >> v;
                vertex.m_Position = positions.at(v - 1);
                if (L'/' == fin.peek())
                {
                    fin.ignore(1);
                    if (L'/' != fin.peek())
                    {
                        size_t vt;
                        fin >> vt;
                    }
                    if (L'/' == fin.peek())
                    {
                        size_t vn;
                        fin.ignore(1);
                        fin >> vn;
                        vertex.m_Normal = normals.at(vn - 1);
                    }
                }
                Vertices_.push_back(vertex);
                Indices_.push_back(current_index++);
            }
            fin.ignore(1024, L'\n');
        }
        else
        {
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();
}

void Model::Obj::CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_,
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

void Model::Obj::CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<uint32_t>& Indices_)
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

void Model::Obj::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, OrcaGraphics::DescriptorPool* pPool_)
{
    mCb.Initialize(pDevice_, pPool_, sizeof(Cb_Obj));
    const auto eyePos = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
    const auto targetPos = DirectX::XMVectorZero();
    const auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    constexpr auto fovY = DirectX::XMConvertToRadians(30.0f);
    constexpr auto aspect = Orca::ScreenWidth / Orca::ScreenHeight;

    // 変換行列の設定
    const auto ptr = mCb.GetPtr<Cb_Obj>();
    ptr->World = DirectX::XMMatrixIdentity();
    ptr->ViewMat = DirectX::XMMatrixLookAtLH(eyePos, targetPos, upward);
    ptr->ProjMat = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, 1.0f, 1000.0f);
}
