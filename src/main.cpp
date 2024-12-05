#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting (default: 0.4s)
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta)
    {
        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

            // Toggle cutting state
            if (m_fields->isCutting) {
                this->resumeStroke(); // Resumes the trail
            } else {
                this->stopStroke(); // Stops the trail for a bit
            }

            m_fields->isCutting = !m_fields->isCutting; // Flip the state
        }

        // Update the trail's behavior, applying the delta time
        CCMotionStreak::update(delta);
    }
};
