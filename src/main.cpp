#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Static map to track streak activity
static std::unordered_map<CCMotionStreak*, bool> streakStates;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
        if (streakStates[this]) {
            // Cutting effect logic
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval;

                if (m_fields->isCutting) {
                    this->stopStroke();
                } else {
                    this->resumeStroke();
                }

                m_fields->isCutting = !m_fields->isCutting;
            }
        } else {
            // Ensure the trail stops and isn't visible when not active
            this->stopStroke();
            m_fields->isCutting = false; // Reset cutting state
            this->setVisible(false); // Hide trail
        }

        CCMotionStreak::update(delta);
    }
};

class $modify(PlayerObject) {
    void activateStreak() {
        PlayerObject::activateStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Activate streak
                streak->resumeStroke();      // Start cutting effect
                streak->setVisible(true);    // Ensure the trail is visible
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Deactivate streak
                streak->stopStroke();         // Stop cutting effect
                streak->setVisible(false);    // Ensure the trail is hidden
            }
        }
    }
};
