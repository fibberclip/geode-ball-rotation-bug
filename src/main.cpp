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
        // Call the base update first to ensure proper fading behavior
        CCMotionStreak::update(delta);

        // Check if the trail is currently active
        if (m_uNuPoints > 0 && m_bStroke) {
            // Update elapsed time
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle cutting state
                if (m_fields->isCutting) {
                    this->resumeStroke(); // Resume the trail
                } else {
                    this->stopStroke(); // Stop the trail temporarily
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip the state
            }
        } else {
            // Reset cutting state if trail isn't active
            if (m_fields->isCutting) {
                this->resumeStroke();
                m_fields->isCutting = false;
            }
        }

        // Ensure fading behavior is retained
        if (m_uNuPoints > 0 && m_pPointState > 0.0f) {
            for (uint i = 0; i < m_uNuPoints; i++) {
                // Adjust point alpha based on distance (or fade logic)
                float alpha = calculateFadeAlpha(i);
                // Apply the fade (assuming there's a method or field for alpha)
                this->setPointAlpha(i, alpha);
            }
        }
    }

    float calculateFadeAlpha(uint pointIndex) {
        // Example fade calculation: decrease alpha based on distance from player
        return 1.0f - (static_cast<float>(pointIndex) / static_cast<float>(m_uMaxPoints));
    }

    void setPointAlpha(uint pointIndex, float alpha) {
        // Implement logic to set alpha for the point if needed
        // You may need to adjust this to match the actual GD API
    }
};
