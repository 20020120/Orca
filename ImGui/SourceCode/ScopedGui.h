#pragma once

namespace OrcaGui
{
    // --------------------------- Gui�̃E�B���h�E���쐬����N���X ---------------------------
    class ScopedWindow
    {
    public:

        ScopedWindow(const char* WindowName_);
        ~ScopedWindow();

        
        ScopedWindow(const ScopedWindow& RHS_) = delete;
        ScopedWindow(ScopedWindow&& LHS_) = delete;
        const ScopedWindow& operator=(const ScopedWindow& RHS_) = delete;
        const ScopedWindow& operator=(ScopedWindow&& LHS_) = delete;
    };
}
