#pragma once
#include<vector>
#include<memory>

namespace System
{ 
    namespace Base
    {
        template<class ComType, class SystemType>
        class SystemBase
        {
        public:
            virtual void OnAwake() = 0; // ���̉�
            void Add(std::shared_ptr<ComType> pComponent_); // �v�f��ǉ�����
            void BaseUpdate(float Dt_); // ���ʂ̍X�V����

        protected:
            std::vector<std::shared_ptr<ComType>> mComponents{};     // ���[�v�p�R���e�i
            std::vector<std::shared_ptr<ComType>> mEraser{};         // �����p�R���e�i


            // -----------------------------------------------------------------
            protected:
            SystemBase() = default;
            protected:
            ~SystemBase() = default;
            public:
            static SystemType& Instance();
        };
    }
}


template <class ComType, class SystemType>
SystemType& System::Base::SystemBase<ComType, SystemType>::Instance()
{
    static SystemType instance{};
    return instance;
}

template <class ComType, class SystemType>
void System::Base::SystemBase<ComType, SystemType>::Add(std::shared_ptr<ComType> pComponent_)
{
    mComponents.emplace_back(pComponent_);
}

template <class ComType, class SystemType>
void System::Base::SystemBase<ComType, SystemType>::BaseUpdate(float Dt_)
{
    for(auto& p: mComponents)
    {
        if(!p->GetIsAlive())
        {
            mEraser.emplace_back(p);
        }
    }

    for(auto& erase: mEraser)
    {
        if(auto it = std::ranges::find(mComponents, erase); it!=mComponents.end())
        {
            mComponents.erase(it);
        }
    }
    mEraser.clear();
}