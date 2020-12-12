#ifndef OLC_PARTICLE_ENGINE
#define OLC_PARTICLE_ENGINE

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Particles2D.hpp"
#include <sstream>

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

        Circle& operator << (const vf2d& cfp) { this->m_Pos.x = cfp.x; this->m_Pos.y = cfp.y; }
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
    int numParticles = 7;
    int currentBehaviorID;
    bool toggleSwitch = true;
    Circle destination = Circle({}, 6);
    Circle origin = Circle({}, 6);

    #ifdef DEBUG_APPLICATION
    int iTotalFrames;
    int iFPS;
    int iTargetFPS = 60;
    int iLastNumParts;
    float fTotalRunTime;
    float fOneSecTimer;
    float fTimeSinceLastDrop;

    ostringstream ss1;
    string st1;
    #endif

private:

    bool OnUserCreate() override
    {
        SetPixelMode(Pixel::NORMAL);

        origin << ScreenSize*0.55f;
        destination << ScreenSize*0.5f;

        PartData.behavior = Particles2D::ParticleBehavior::ShotGun;
        PartData.color = Pixel(200,214,0);
        PartData.duration = 2.0f;
        PartData.fade = true;
        PartData.size = 8;
        PartData.speed = 128.0f;
        PartSys.init(numParticles, PartData);

        #ifdef DEBUG_APPLICATION
        cout << "\n#### Enter Desired FPS ####\n";
        cin >> iTargetFPS;

        PartData.size = 0;
        PartData.speed = 300.0f;
        PartData.duration = 0.75f;
        
        #endif
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(BLACK);

        #ifdef DEBUG_APPLICATION

        if (fOneSecTimer >= 1.0f)
        {
            fOneSecTimer = 0.0f;
            iLastNumParts = numParticles;
            if (iFPS < iTargetFPS)
            numParticles -= iFPS * 2;
            else
            numParticles += iFPS * 0.25f;
        }

        if (iFPS < iTargetFPS)
        {
            fTimeSinceLastDrop = 0.0f;
        }
        else
        {
            fTimeSinceLastDrop += GetElapsedTime();
        }
        
        iFPS = GetFPS();

        iTotalFrames++;
        fTotalRunTime += GetElapsedTime();
        fOneSecTimer += GetElapsedTime();

        int tRunTime = floor(fTotalRunTime);
        int tLastDropTime = floor(fTimeSinceLastDrop);

        DrawStringDecal({20,20},"Ideal Maximum number of Particles is about: " + to_string(numParticles));
        DrawStringDecal({20,40},"Time Since FPS Dropped Below " + to_string(iTargetFPS) + ": " + to_string(tLastDropTime) + "s.");
        DrawStringDecal({20,60},"RunTime: " + to_string(tRunTime) + "s.");

        #endif

        Circle cMouse(GetMousePos(), 12); 
        
        if (GetMouse(0).bHeld)
        {
            if (CircleCollides(origin, cMouse))
            origin << GetMousePos();
        }
        if (GetKey(Key::TAB).bPressed)
        {
            toggleSwitch = !toggleSwitch;
        }

        DrawCircle(destination.getPos(), destination.getRadius(), RED);
        FillCircle(origin.getPos(), origin.getRadius(), RED);
        DrawLine(origin.getPos(), destination.getPos(), RED);

        if (PartSys.IsRunning())
        {
            PartSys.update(GetElapsedTime());
            PartSys.drawBubbleStyle(this);
        }
        else
        {
            PartSys.destroy();

            PartData.color = Pixel(rand() % 255,rand() % 255,rand() % 255);

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

int main(int argc, char const *argv[])
{
    olcParticleEngine demo;

    if (demo.Construct(demo.ScreenSize.x, demo.ScreenSize.y, 1, 1))
        demo.Start();
    return 0;
}