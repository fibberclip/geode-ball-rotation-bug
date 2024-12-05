#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

// Intercept PlayerObject to track streak activation
class $modify (PlayerObject) {
    struct Fields {
        bool isTrailActive = false; // Tracks whether the trail is active
    };

    void activateStreak(bool active) {
        m_fields->isTrailActive = active; // Track the active state
        PlayerObject::activateStreak(active); // Call the original method
    }
};

// Modify CCMotionStreak to apply trail cutting
class $modify (CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.4f;    // Interval for the trail cutting (default: 0.4s)
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta)
    {
        // Get the PlayerObject and check if the trail is active
        auto player = reinterpret_cast<PlayerObject*>(this->getParent()); // Get the parent object
        if (!player || !player->m_fields->isTrailActive) {
            CCMotionStreak::update(delta);
            return;
        }

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
