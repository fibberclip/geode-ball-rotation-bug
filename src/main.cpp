#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.15f;
        bool isCutting = false;
    };

    virtual void update(float delta)
    {
        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval; 

            if (m_fields->isCutting) {
                this->stopStroke(); 
            } else {
                this->resumeStroke(); 
            }

            m_fields->isCutting = !m_fields->isCutting; 
        }

        CCMotionStreak::update(delta);
    }
};
