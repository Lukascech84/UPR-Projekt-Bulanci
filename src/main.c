#include "engine.h"

int main()
{
    if (eng_init("Bulánci", 1024, 576))
    {
        printf("Engine failed to initialize.\n");
        return 1;
    }

    eng_run();

    eng_quit();

    return 0;
}