#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include <memory>
#include<wrl.h>
#include<vector>

#include"Graphics.h"
#include"ConstantBuffer.h"

// �O���錾
namespace OrcaGraphics
{
    class DescriptorPool;
}

namespace Model
{
    // Obj�t�@�C���̃N���X
    class Obj
    {
        // �O���錾
        struct VertexData;
        struct Cb_Obj;

    public:
        Obj() = default;
        ~Obj();

        // ������
        void Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, OrcaGraphics::DescriptorPool* pPool_, const wchar_t* ObjPath_);
        void Update(float Dt_); // �X�V
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_); // �`��R�}���h��ς�
    private:
        static void Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_, std::vector<uint32_t>& Indices_);
        void CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<VertexData>& Vertices_);
        void CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<uint32_t>& Indices_);
        void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, OrcaGraphics::DescriptorPool* pPool_);
    private:
        // ------------------------------- ���_�ǂݍ��� ------------------------------
        struct VertexData
        {
            DirectX::XMFLOAT3 m_Position{};
            DirectX::XMFLOAT3 m_Normal{};
            
        };
        Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexBuffer{};
        D3D12_VERTEX_BUFFER_VIEW mVbView{};
        UINT m_VertexCounts{};  // ���_��
        // ----------------------------- �C���f�b�N�X�o�b�t�@ ----------------------------
        Microsoft::WRL::ComPtr<ID3D12Resource> mpIndexBuffer{};
        D3D12_INDEX_BUFFER_VIEW mIbView{};

        // ------------------------------- �萔�o�b�t�@ ------------------------------
        struct alignas(256)Cb_Obj
        {
            DirectX::XMMATRIX World{};
        };
       OrcaGraphics::ConstantBuffer mCb{};


    };
}
