#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting
        bool isCutting = false;      // Indicates whether the trail is currently being cut
        bool isTrailActive = false;  // Tracks whether the trail is active
    };

    virtual void reset() {
        // Called to reset the trail (naturally stops existing)
        m_fields->isTrailActive = false;
        m_fields->elapsedTime = 0.0f; // Reset timer
        m_fields->isCutting = false; // Reset cutting state
        CCMotionStreak::reset();     // Call base class reset
    }

    virtual void update(float delta) {
        if (!m_fields->isTrailActive) {
            // Check if the trail starts
            if (this->isStartingPositionInitialized()) {
                m_fields->isTrailActive = true; // Trail has started
            }
        }

        if (m_fields->isTrailActive) {
            // Cutting logic runs only while the trail is active
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                if (m_fields->isCutting) {
                    this->resumeStroke(); // Resume the trail
                } else {
                    this->stopStroke(); // Stop the trail briefly
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip cutting state
            }
        }

        // Call the base class update
        CCMotionStreak::update(delta);

        // Reset the trail state if it stops naturally
        if (m_fields->isTrailActive && !this->isStartingPositionInitialized()) {
            m_fields->isTrailActive = false; // Trail has naturally ended
        }
    }
};
