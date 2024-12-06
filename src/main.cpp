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

        // Check if the trail is active (i.e., has points and is currently drawing)
        if (m_uNuPoints > 0 && m_bStroke) {
            // Update elapsed time for cutting interval
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle the cutting state
                if (m_fields->isCutting) {
                    this->resumeStroke(); // Resume the trail
                } else {
                    this->stopStroke(); // Temporarily stop the trail
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip the state
            }
        } else {
            // Reset cutting state if the trail isn't active
            if (m_fields->isCutting) {
                this->resumeStroke(); // Ensure the trail is resumed when it was active
                m_fields->isCutting = false;
            }
        }
    }

    virtual void draw() {
        // Call the original draw method to ensure normal behavior
        if (m_bStroke) {
            CCMotionStreak::draw();
        } else {
            // Optionally, add custom behavior for when the trail isn't active
        }
    }
};
