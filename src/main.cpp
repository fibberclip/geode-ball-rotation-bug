#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting
        bool isCutting = false;      // Indicates whether the trail is currently being cut
    };

    virtual void update(float delta) {
        // Call the base update first to ensure proper behavior
        CCMotionStreak::update(delta);

        // Ensure the trail is active before trying to cut it
        if (m_uNuPoints > 0 && m_bStroke) {
            m_fields->elapsedTime += delta;

            // Check if it's time to cut the trail
            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval;  // Reset the timer

                // Toggle cutting state if the trail is active
                if (m_fields->isCutting) {
                    this->resumeStroke();  // Resume the trail
                    CCLOG("Trail Resumed");
                } else {
                    this->stopStroke();    // Stop the trail temporarily
                    CCLOG("Trail Stopped");
                }

                m_fields->isCutting = !m_fields->isCutting;  // Toggle the state
            }
        } else {
            // If the trail isn't active, ensure it resumes correctly
            if (m_fields->isCutting) {
                this->resumeStroke();
                m_fields->isCutting = false;
                CCLOG("Trail Reset");
            }
        }
    }

    virtual void draw() {
        // If the trail is active and has points, draw it
        if (m_bStroke && m_uNuPoints > 0) {
            CCMotionStreak::draw();
        } else {
            // If the trail is not active, let it fade naturally without interrupting the process
            CCLOG("Trail is inactive, fading...");
        }
    }
};
