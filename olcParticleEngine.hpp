#ifndef OLC_PARTICLE_ENGINE
#define OLC_PARTICLE_ENGINE

#include "olcPixelGameEngine.h"
#include "Particles2D.hpp"

using namespace olc;

class olcParticleEngine : public olc::PixelGameEngine
{
public:
    vf2d ScreenSize = {512,512};
    olcParticleEngine()
    {
        sAppName = "(PGE) Particle Engine";
    }

private:
    Particles2D::ParticlesSystem PartSys;
    Particles2D::ParticleData PartData;
    int numParticles = 5;
    int currentBehaviorID;
    bool toggleSwitch = false;
    vf2d destination;

private:

    bool OnUserCreate() override
    {
        PartData.behavior = Particles2D::ParticleBehavior::ShotGun;
        PartData.color = Pixel(200,214,0);
        PartData.duration = 2.0f;
        PartData.fade = true;
        PartData.size = 4;
        PartData.speed = 200.0f;
        PartSys.init(numParticles, PartData);
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(BLACK);

        
        if (GetMouse(0).bHeld)
        destination = GetMousePos();

        DrawCircle(destination, PartData.size*2, RED);
        DrawCircle(ScreenSize/2, PartData.size*2, RED);
        DrawLine(ScreenSize/2, destination, RED);

        if (PartSys.IsRunning())
        {
            PartSys.update(GetElapsedTime());
            PartSys.drawParticles(this);
        }
        else
        {
            PartSys.destroy();

            if (toggleSwitch)
            {
                currentBehaviorID++;
                if (currentBehaviorID > 4) { currentBehaviorID = 1;}
                PartData.behavior = Particles2D::ParticleBehavior(currentBehaviorID);
            }
            else
            {
                //Set currentBehaviorID to current behavior (if it's not already)
                if (PartData.behavior != Particles2D::ParticleBehavior(currentBehaviorID))
                for (currentBehaviorID = 0;
                PartData.behavior != Particles2D::ParticleBehavior(currentBehaviorID);
                currentBehaviorID++);
            }
            

            PartData.fade = !PartData.fade;

            PartSys.init(numParticles, PartData);

            if (currentBehaviorID > 2 && currentBehaviorID < 5) //Laser and ShotGun
            {
                srand(time(0) + 512);
                PartSys.emitParticles(ScreenSize/2, destination);
            }
            else if (currentBehaviorID < 3) //FireWorks and Pulse
                PartSys.emitParticles(ScreenSize/2);
            else //Smoke
                PartSys.emitParticles(vf2d((ScreenSize/2).x,ScreenSize.y));

        }
        

        return true;
    }
    bool OnUserDestroy() override
    {
        return true;
    }

};

#endif