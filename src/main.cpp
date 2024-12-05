#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
        bool trailActive = false; // To track if the trail is currently active
    };

    virtual void update(float delta)
    {
        if (this->m_bStroke) { // Check if the trail exists (is active)
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval;

                if (m_fields->isCutting) {
                    this->stopStroke(); // Resume trail
                } else {
                    this->resumeStroke(); // Stop trail for a bit
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip state
            }
        }

        CCMotionStreak::update(delta);
    }
};

class $modify (PlayerObject)
{
    struct Fields {
        CCMotionStreak* m_streak; // Assuming this points to the current streak object
    };

    void activateStreak()
    {
        PlayerObject::activateStreak(); // Call original method
        if (m_fields->m_streak) {
            m_fields->m_streak->trailActive = true; // Enable cutting effect directly
        }
    }

    void resetStreak()
    {
        PlayerObject::resetStreak(); // Call original method
        if (m_fields->m_streak) {
            m_fields->m_streak->trailActive = false; // Disable cutting effect directly
        }
    }
};
