#include <cstdlib>              // EXIT_SUCCESS

#include "Core/Game_2D.hpp"     // Game2D class


int main(int argc, char* argv[])
{
    Game2D::Init();
    Game2D::Run();
    Game2D::CleanUp();

    return EXIT_SUCCESS;
}
