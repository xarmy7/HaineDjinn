
#include <iostream>
#include <filesystem>

#include <glad/gl.h>
#include <crtdbg.h>

#include "Core/App.h"

#include <reactphysics3d/reactphysics3d.h>

#include "Core/Event.h"
#include "Core/EventSubscriber.h"

//#include <crtdbg.h>

int main(int argc, char* argv[])
{
  /*  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(12543);*/
    std::filesystem::current_path("../Assets");
    std::cout << "Current dir: " << std::filesystem::current_path() << std::endl;

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GLFWwindow* window = initOpenGL();

    if (window == nullptr)
        return -1;

    App app(window);

    app.run();

    return 0;
}
