#ifndef OLC_PARTICLE_2D
#define OLC_PARTICLE_2D

#include "olcPixelGameEngine.h"

using namespace std;
using namespace olc;

namespace Particles2D
{
    float PI = 3.141592f;

    class Particle
    {
        private:
            vf2d m_Pos;
            vf2d m_Vel;
        public:
            Particle(vf2d direction) { m_Vel = direction; }
            ~Particle(){}

            void SetPosition(vf2d position)
            {
                m_Pos = position;
            }
            vf2d GetPosition()
            {
                return m_Pos;
            }
            void SetVelocity(vf2d velocity)
            {
                m_Vel = velocity;
            }
            vf2d GetVelocity()
            {
                return m_Vel;
            }

            void update(float fElapsedTime)
            {
                m_Pos += m_Vel * fElapsedTime;
            }
    };

    enum class ParticleBehavior { FireWorks=1, Pulse=2, Laser=3, ShotGun=4, Smoke=5, Custom };

    class ParticleData
    {
        public:
        ParticleData(){}
        ~ParticleData(){}

        ParticleBehavior behavior = ParticleBehavior::FireWorks;
        Pixel color = YELLOW;
        float duration = 2.0f;
        float speed = 100;
        float size = 0.0f;
        bool fade = false;
    };

    class ParticlesSystem
    {
        private:
            vector<Particle> m_Particles;
            ParticleData m_Setup = ParticleData();
            bool m_running;
            float m_Timer;
            
        public:
            ParticlesSystem(){};
            ParticlesSystem(int numberofparticles){};
            ~ParticlesSystem(){};

            void SetCustomDrawFunc(std::function<void(olc::PixelGameEngine *pge)> draw)
            {
                drawParticles = draw;
            }

            std::function<void(olc::PixelGameEngine *pge)> drawCircleStyle = [&](olc::PixelGameEngine *pge)
            {
                for (int i = 0; i < m_Particles.size(); i++)
                {
                    float fAlphaOpacity = float(m_Timer / m_Setup.duration) * 255.0f;
                    if (!m_Setup.fade)
                    fAlphaOpacity = 255.0f;

                    pge->FillCircle(m_Particles[i].GetPosition(), m_Setup.size, Pixel(m_Setup.color.r, m_Setup.color.g, m_Setup.color.b, fAlphaOpacity));
                }
            };

            std::function<void(olc::PixelGameEngine *pge)> drawBubbleStyle = [&](olc::PixelGameEngine *pge)
            {
                for (int i = 0; i < m_Particles.size(); i++)
                {
                    float fAlphaOpacity = float(m_Timer / m_Setup.duration) * 255.0f;
                    if (!m_Setup.fade)
                    fAlphaOpacity = 255.0f;

                    pge->DrawCircle(m_Particles[i].GetPosition(), m_Setup.size, Pixel(m_Setup.color.r, m_Setup.color.g, m_Setup.color.b, fAlphaOpacity));
                }
            };

            std::function<void(olc::PixelGameEngine *pge)> drawSquareStyle = [&](olc::PixelGameEngine *pge)
            {
                for (int i = 0; i < m_Particles.size(); i++)
                {
                    float fAlphaOpacity = float(m_Timer / m_Setup.duration) * 255.0f;
                    if (!m_Setup.fade)
                    fAlphaOpacity = 255.0f;

                    pge->FillRect(m_Particles[i].GetPosition() - vf2d(m_Setup.size,m_Setup.size)/2, vf2d(m_Setup.size,m_Setup.size),Pixel(m_Setup.color.r, m_Setup.color.g, m_Setup.color.b, fAlphaOpacity));
                }
            };

            std::function<void(olc::PixelGameEngine *pge)> drawTriangleStyle = [&](olc::PixelGameEngine *pge)
            {
                for (int i = 0; i < m_Particles.size(); i++)
                {
                    float fAlphaOpacity = float(m_Timer / m_Setup.duration) * 255.0f;
                    if (!m_Setup.fade)
                    fAlphaOpacity = 255.0f;

                    vf2d v1 = m_Particles[i].GetPosition();
                    v1.y -= m_Setup.size;
                    vf2d v2 = m_Particles[i].GetPosition();
                    v2.x -= m_Setup.size * 0.5f;
                    vf2d v3 = m_Particles[i].GetPosition();
                    v3.x += m_Setup.size * 0.5f;

                    pge->FillTriangle(v1,v2,v3, Pixel(m_Setup.color.r, m_Setup.color.g, m_Setup.color.b, fAlphaOpacity));
                }
            };
            
            std::function<void(olc::PixelGameEngine *pge)> drawParticles;

            void init(int numberofparticles, ParticleData PS = ParticleData())
            {
                SetCustomDrawFunc(drawBubbleStyle);

                m_Setup = PS;

                vf2d direction;
                for (int i = 0; i < numberofparticles; i++)
                {
                    srand(time(0) + i);

                    if (m_Setup.behavior == ParticleBehavior::FireWorks)
                    {
                        float fSpeed = (rand() % (int)m_Setup.speed/2) + m_Setup.speed/2;
                        float fAngle = (rand() % 360) * PI / 180.0f;

                        direction = {cos(fAngle) * fSpeed, sin(fAngle) * fSpeed};
                    }
                    if (m_Setup.behavior == ParticleBehavior::Pulse)
                    {
                        float fSpeed = m_Setup.speed;
                        float fTheta = PI * 2 / numberofparticles;

                        direction = {cos(fTheta * i) * fSpeed, sin(fTheta * i) * fSpeed};
                    }

                    m_Particles.push_back(Particle(direction));
                }
            }
            void destroy()
            {
                m_Particles.clear();
            }
            void update(float fElapsedTime)
            {
                m_Timer -= fElapsedTime;

                int index;
                for (auto it = m_Particles.begin(); it != m_Particles.end(); it++)
                {
                    index++;
                    srand(time(0) + index);
                    if (m_Setup.behavior == ParticleBehavior::Smoke)
                    {
                        
                    }
                    it->update(fElapsedTime);
                }

                if (m_Timer < 0)
                {
                    m_running = false;
                }
            }

            void emitParticles(vf2d startPos, vf2d destination = vf2d())
            {
                m_running = true;

                m_Timer = m_Setup.duration;

                int index;
                for (auto i = m_Particles.begin(); i != m_Particles.end(); i++)
                {
                    index++;
                    i->SetPosition(startPos);
                    
                    vf2d direction;
                    float fAngle = atan2(destination.y - startPos.y, destination.x - startPos.x);
                    float fIndexRatio = float((float)index / m_Particles.size());

                    if (m_Setup.behavior == ParticleBehavior::Laser)
                    {
                        float fSpeed = (m_Setup.speed * (float(index)/m_Particles.size()));
                        if (fSpeed < m_Setup.speed * 0.5f) fSpeed += m_Setup.speed * 0.5f;
                        direction = (destination - startPos).norm() * fSpeed;
                        i->SetVelocity(direction);
                    }
                    if (m_Setup.behavior == ParticleBehavior::ShotGun)
                    {
                        float fSpeed = m_Setup.speed;
                        float fTheta = PI * 0.25f / m_Particles.size();
                        float fAngle = atan2(destination.y - startPos.y, destination.x - startPos.x);
                        float fRotation = fAngle;
                        int iHalfOfParticles = m_Particles.size() / 2 + 1 | 0;

                        direction = vf2d(cos(fTheta * (index-iHalfOfParticles) + fAngle) * fSpeed, sin(fTheta * (index-iHalfOfParticles) + fAngle) * fSpeed);

                        i->SetVelocity(direction);
                    }
                }
            }
            
            bool IsRunning()
            {
                return m_running;
            }
    };
}

#endif