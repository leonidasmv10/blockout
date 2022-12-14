#include "Application.h"

int main(int argc, char *argv[])
{
    Application application("BlockOut", "1.0");
    application.ParseArguments(argc, argv);
    application.Init();
    return application.Run();
}
