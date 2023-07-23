#include "Pch.hpp"
#include "Threading.hpp"
#include "Renderer.hpp"

int main() {
    //FreeConsole();
    g_Renderer = std::make_unique<Renderer>();
    g_Threading.ThreadLoop();
    while (g_Running) {
        g_Renderer->Present();
         while (PeekMessageA(&g_Msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&g_Msg);
            DispatchMessageA(&g_Msg);
            if (g_Msg.message == WM_QUIT)
                g_Running = false;
        }
    }
    g_Threading.Clear();
    g_Renderer.reset();
    return 0;
}