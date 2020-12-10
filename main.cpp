#define OLC_PGE_APPLICATION
#include "olcParticleEngine.hpp"

int main(int argc, char const *argv[])
{
    olcParticleEngine demo;

    if (demo.Construct(demo.ScreenSize.x, demo.ScreenSize.y, 1, 1))
        demo.Start();
    return 0;
}
