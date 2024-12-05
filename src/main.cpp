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
        // Call the base update first to maintain trail behavior
        CCMotionStreak::update(delta);

        // Ensure the trail is active before applying logic
        if (m_uNuPoints > 0 && m_bStroke) {
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle cutting state
                if (m_fields->isCutting) {
                    this->resumeStroke(); // Resume the trail
                } else {
                    this->stopStroke();   // Temporarily stop the trail
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip the state

                // Debugging: Log the current state
                CCLOG("Trail Cutting: %s", m_fields->isCutting ? "OFF" : "ON");
            }
        } else {
            // Reset cutting state if trail is inactive
            if (m_fields->isCutting) {
                this->resumeStroke();
                m_fields->isCutting = false;
            }
        }
    }

    // Restore fade-out by overriding draw method if needed
    virtual void draw() override {
        // Call original draw method
        CCMotionStreak::draw();

        // If necessary, modify draw logic to restore fading
        // This depends on how the fade is handled internally
    }
};
