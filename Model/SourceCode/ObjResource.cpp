#include"pch.h"
#include"ObjResource.h"

#include <filesystem>
#include<vector>
#include<DirectXMath.h>
#include<fstream>
#include<string>
#include<ResourceUploadBatch.h>
#include "GraphicsLogger.h"


void Model::ObjResource::Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_,
    std::vector<uint32_t>& Indices_, std::wstring& TextureName_)
{
    // Obj�t�@�C�����璸�_�f�[�^���p�[�X����
    Vertices_.clear();
    Indices_.clear();

    uint32_t current_index{ 0 };
    std::vector<Math::Vector3> positions;
    std::vector<Math::Vector3> normals;
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
                //���̕�����L'/'�̏ꍇ
                if (L'/' == fin.peek())
                {
                    // 1�����폜
                    fin.ignore(1);
                    //���̕�����L'/'�ł͂Ȃ��ꍇ
                    if (L'/' != fin.peek())
                    {
                        fin >> vt;
                        vertex.mUv = uvs.at(vt - 1);
                    }
                    //���̕�����L'/'�̏ꍇ
                    if (L'/' == fin.peek())
                    {
                        //1�����폜
                        fin.ignore(1);
                        fin >> vn;
                        //�@�����̓���
                        vertex.mNormal = normals.at(vn - 1);
                    }
                }
                Vertices_.push_back(vertex);
                Indices_.push_back(current_index++);
            }
            //��ԏ�ɂ���P�s���폜
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
            //��ԏ�ɂ���P�s���폜
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    // �}�e���A�������p�[�X
    std::filesystem::path mtlFilename(ObjPath_);
    //�t�@�C�����̕����̂�MTL�t�@�C�����ɓ���ւ���
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

void Model::ObjResource::CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_,
                                            const std::vector<VertexData>& Vertices_)
{
    // ------------------------------ ���_�o�b�t�@���쐬���� ------------------------------
    // �q�[�v�v���p�e�B�̐ݒ�
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(VertexData) * Vertices_.size();
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

    OrcaDebug::GraphicsLog("���_�o�b�t�@���쐬", hr);

    // �}�b�s���O����
    void* ptr{};
    hr = mpVertexBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("�}�b�s���O", hr);

    // ���_�f�[�^���}�b�s���O��Ɏw��
    memcpy(ptr, Vertices_.data(), sizeof(VertexData) * Vertices_.size());

    // �}�b�s���O����
    mpVertexBuffer->Unmap(0, nullptr);

    // �r���[�̐ݒ�
    mVbView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
    mVbView.SizeInBytes = sizeof(VertexData) * static_cast<UINT>(Vertices_.size());
    mVbView.StrideInBytes = sizeof(VertexData);
}

void Model::ObjResource::CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_,
    const std::vector<uint32_t>& Indices_)
{
    // �q�[�v�v���p�e�B��ݒ�
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
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

    // ���\�[�X�𐶐�
    auto hr = pDevice_->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpIndexBuffer.ReleaseAndGetAddressOf())
    );
    OrcaDebug::GraphicsLog("�C���f�b�N�X�o�b�t�@���쐬", hr);

    // �}�b�s���O
    void* ptr = nullptr;
    hr = mpIndexBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("�}�b�s���O", hr);
    memcpy(ptr, Indices_.data(), sizeof(uint32_t) * Indices_.size());
    mpIndexBuffer->Unmap(0, nullptr);

    // �C���f�b�N�X�o�b�t�@�r���[���쐬
    mIbView.BufferLocation = mpIndexBuffer->GetGPUVirtualAddress();
    mIbView.Format = DXGI_FORMAT_R32_UINT;
    mIbView.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * Indices_.size());
}

void Model::ObjResource::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_,
    OrcaGraphics::DescriptorPool* pPool_)
{
    mCb = std::make_unique<OrcaGraphics::Resource::ConstantBuffer>(sizeof(Cb_Obj), 1);
    mCb->Mapping(reinterpret_cast<void**>(&mCbData));
}

void Model::ObjResource::CreateTexture(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_,
    OrcaGraphics::DescriptorPool* pPool_, Microsoft::WRL::ComPtr<ID3D12CommandQueue> pCommandQueue_,
    std::wstring TexturePath_)
{
    // �e�N�X�`�����쐬
    DirectX::ResourceUploadBatch batch(pDevice_.Get());
    batch.Begin();
    // ���\�[�X�𐶐�
    mTexture.Initialize(pDevice_, pPool_, TexturePath_.c_str(), batch);

    // �R�}���h�����s
    const auto future = batch.End(pCommandQueue_.Get());
    future.wait();
}
