#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak)
{
    struct Fields {
        float elapsedTime = 0.0f;    // Tracks the elapsed time
        float cutInterval = 0.2f;    // Interval for the trail cutting
        bool isCutting = false;      // Indicates whether cutting is active
    };

    virtual void update(float delta) {
        // Call base class update to ensure normal trail functionality
        CCMotionStreak::update(delta);

        // Ensure trail is active before applying custom logic
        if (m_uNuPoints > 0 && m_bStroke) {
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval; // Reset the timer

                // Toggle cutting without disrupting internal rendering states
                if (m_fields->isCutting) {
                    m_pPointState = 1.0f; // Allow points to render normally
                } else {
                    m_pPointState = 0.0f; // Hide points temporarily
                }

                m_fields->isCutting = !m_fields->isCutting; // Flip cutting state

                // Debugging: Log trail cutting state
                CCLOG("Trail Cutting State: %s, NuPoints: %u", 
                    m_fields->isCutting ? "OFF" : "ON", m_uNuPoints);
            }
        } else {
            // Reset cutting state when trail is inactive
            if (m_fields->isCutting) {
                m_pPointState = 1.0f; // Ensure points render normally
                m_fields->isCutting = false;
            }
        }
    }

    virtual void draw() override {
        // Debugging: Log internal state
        CCLOG("Draw Call - Stroke: %d, NuPoints: %u, PointState: %f", 
            m_bStroke, m_uNuPoints, m_pPointState);

        // Call base draw method to ensure fade effect remains
        CCMotionStreak::draw();
    }
};
