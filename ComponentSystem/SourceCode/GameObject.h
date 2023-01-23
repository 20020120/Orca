#pragma once
#include<memory>
#include<vector>
#include<string>
namespace ComponentSystem
{
    // ---------------------------------- �O���錾 ---------------------------------
    class Component;

    class GameObject final 
    {

        
    private:
        std::string mName{};    // ���O
        std::vector <std::shared_ptr<Component>> mComponents{}; // �������Ă���R���|�\���l���g
    };

}
