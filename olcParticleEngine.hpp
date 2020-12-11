#ifndef OLC_PARTICLE_ENGINE
#define OLC_PARTICLE_ENGINE

#include "olcPixelGameEngine.h"
#include "Particles2D.hpp"

using namespace olc;

class Circle 
{
    private:
        vf2d m_Pos;
        float m_Radius;
    public:
        Circle(){}
        Circle(vf2d pos, float radius) : m_Pos(pos), m_Radius(radius) {}
        ~Circle(){}

        vf2d getPos() { return m_Pos; }
        void setPos(vf2d position) { m_Pos = position; }
        
        float getRadius() { return m_Radius; }
        void setRadius(float radius) { m_Radius = radius; }

        Circle& operator >> (const vf2d& cfp) { this->m_Pos.x = cfp.x; this->m_Pos.y = cfp.y; }
};

bool CircleCollides(Circle c1o, Circle c2o)
{
    float distanceX = c1o.getPos().x - c2o.getPos().x;
    float distanceY = c1o.getPos().y - c2o.getPos().y;
    float diameter = (c1o.getRadius() + c2o.getRadius());
    if (distanceX * distanceX + distanceY * distanceY < diameter * diameter)
    return true;
    return false;
}

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
    int numParticles = 314;
    int currentBehaviorID;
    bool toggleSwitch = true;
    Circle destination = Circle({}, 6);
    Circle origin = Circle({}, 6);

private:

    bool OnUserCreate() override
    {
        origin >> vf2d(32,32);
        destination >> ScreenSize/2;

        PartData.behavior = Particles2D::ParticleBehavior::ShotGun;
        PartData.color = Pixel(200,214,0);
        PartData.duration = 2.0f;
        PartData.fade = true;
        PartData.size = 4;
        PartData.speed = 600.0f;
        PartSys.init(numParticles, PartData);
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(BLACK);

        Circle cMouse(GetMousePos(), 12); 
        
        if (GetMouse(0).bHeld)
        {
            if (CircleCollides(origin, cMouse))
            origin >> GetMousePos();
        }

        DrawCircle(destination.getPos(), destination.getRadius(), RED);
        FillCircle(origin.getPos(), origin.getRadius(), RED);
        DrawLine(origin.getPos(), destination.getPos(), RED);

        if (PartSys.IsRunning())
        {
            PartSys.update(GetElapsedTime());
            PartSys.drawParticles(this);
        }
        else
        {
            PartSys.destroy();

            PartData.color = Pixel(200,200,0);

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
                PartSys.emitParticles(origin.getPos(), destination.getPos());
            }
            else if (currentBehaviorID < 3) //FireWorks and Pulse
                PartSys.emitParticles(origin.getPos());
            else //Smoke
                PartSys.emitParticles(origin.getPos());

        }
        

        return true;
    }
    bool OnUserDestroy() override
    {
        return true;
    }

};

#endif