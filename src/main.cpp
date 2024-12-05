#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting (default: 0.2s)
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta) {
        m_fields->elapsedTime += delta;

        // Check if the trail is currently being drawn
        if (this->isDrawing() && m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

            // Toggle cutting state
            m_fields->isCutting = !m_fields->isCutting; // Flip the state

            if (m_fields->isCutting) {
                this->stopStroke(); // Stop adding new points temporarily
            } else {
                this->resumeStroke(); // Resume adding points
            }
        }

        // Update the trail's behavior, applying the delta time
        CCMotionStreak::update(delta);
    }

    bool isDrawing() {
        return m_bStroke && m_uNuPoints > 0; // Check if stroke is active and points exist
    }
};
