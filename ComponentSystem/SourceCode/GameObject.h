#pragma once
#include<memory>
#include<vector>
#include<string>
namespace ComponentSystem
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class Component;

    class GameObject final 
    {

        
    private:
        std::string mName{};    // 名前
        std::vector <std::shared_ptr<Component>> mComponents{}; // 所持しているコンポ―ンネント
    };

}
