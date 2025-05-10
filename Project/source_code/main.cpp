#include "Manager/game_manager.hpp"

int WINAPI WinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        LPSTR     lpCmdLine,
    _In_        int       nShowCmd)
{
    std::unique_ptr<GameManager> game_manager(std::make_unique<GameManager>());
    game_manager->Run();

    return 0;
}
