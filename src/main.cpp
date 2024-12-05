#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

// Hook into CCMotionStreak to apply the cutting effect
class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for cutting
        bool isCutting = false;      // Indicates if the trail is being cut
        bool isTrailActive = false;  // Whether the trail is naturally active
    };

    virtual void update(float delta) {
        // Ensure the trail is only manipulated when active
        if (!m_fields->isTrailActive) {
            CCMotionStreak::update(delta); // Default behavior
            return;
        }

        // Apply the cutting effect when active
        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval;

            if (m_fields->isCutting) {
                this->resumeStroke(); // Resume the trail
            } else {
                this->stopStroke(); // Temporarily cut the trail
            }

            m_fields->isCutting = !m_fields->isCutting; // Toggle state
        }

        CCMotionStreak::update(delta); // Default behavior
    }
};

// Hook into PlayerObject to track when the trail starts/stops
class $modify(PlayerObject) {
    void activateStreak(CCMotionStreak* streak) {
        // Mark the trail as active when the streak starts
        streak->m_fields->isTrailActive = true;
        PlayerObject::activateStreak(streak); // Call original
    }

    void deactivateStreak(CCMotionStreak* streak) {
        // Mark the trail as inactive when the streak stops
        streak->m_fields->isTrailActive = false;
        PlayerObject::resetStreak(streak); // Call original
    }
};
