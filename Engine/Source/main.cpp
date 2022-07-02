#include "Application.h"

int main(int argc, char** argv)
{
    auto app = Application::Create("Delayed");
    app->Run();

    return 0;
}