#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting (default: 0.2s)
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta) {
        // Update elapsed time
        m_fields->elapsedTime += delta;

        // Check if the trail is currently active
        if (m_bStroke) {
            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle cutting state
                if (m_fields->isCutting) {
                    this->stopStroke(); // Resume the trail
                } else {
                    this->resumeStroke(); // Stop the trail temporarily
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip the state
            }
        }

        // Call the original update method to ensure normal behavior
        CCMotionStreak::update(delta);
    }
};
