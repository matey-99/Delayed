#include "Application.h"

int main(int argc, char** argv)
{
    auto app = Application::Create("PBL");
    app->Run();

    return 0;
}