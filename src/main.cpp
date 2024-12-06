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
            // Explicitly ensure the trail is off
            this->stopStroke();
            m_fields->isCutting = false; // Reset cutting state
        }

        CCMotionStreak::update(delta);
    }
};

class $modify(PlayerObject) {
    void activateStreak() {
        PlayerObject::activateStreak(); // Original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Activate streak
                streak->resumeStroke();     // Ensure stroke starts correctly
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak(); // Original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Deactivate streak
                streak->stopStroke();        // Stop the trail
                streak->setVisible(false);   // Ensure visibility is disabled
            }
        }
    }
};
