#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <unordered_map>

using namespace geode::prelude;

static std::unordered_map<CCMotionStreak*, bool> streakStates;
static std::unordered_map<CCMotionStreak*, bool> cuttingStates; // Track cutting state

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f; // Customize the cutting interval
        bool isCutting = false;
    };

    virtual void update(float delta) {
        if (streakStates[this]) {
            // Cutting effect logic
            m_fields->elapsedTime += delta;

            // Check if it's time to cut the trail
            if (m_fields->elapsedTime >= m_fields->cutInterval) {
                m_fields->elapsedTime -= m_fields->cutInterval;

                // Toggle cutting effect
                if (m_fields->isCutting) {
                    this->stopStroke(); // Stop the stroke
                } else {
                    this->resumeStroke(); // Resume the stroke
                }

                m_fields->isCutting = !m_fields->isCutting; // Toggle state
                cuttingStates[this] = m_fields->isCutting; // Update cutting state
            }
        } else {
            // If the streak is inactive, stop the stroke and hide the trail
            this->stopStroke();
            m_fields->isCutting = false; // Reset cutting state
            this->setVisible(false); // Hide trail when inactive
            cuttingStates[this] = false; // Reset cutting state
        }

        CCMotionStreak::update(delta); // Call the original update method
    }
};

class $modify(PlayerObject) {
    void activateStreak() {
        PlayerObject::activateStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Activate the streak
                cuttingStates[streak] = false; // Reset cutting state when activated
                streak->resumeStroke();      // Start cutting effect
                streak->setVisible(true);    // Make sure the trail is visible
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Deactivate the streak
                cuttingStates[streak] = false; // Reset cutting state
                streak->stopStroke();         // Stop cutting effect
                streak->setVisible(false);    // Hide the trail
            }
        }
    }

    void update(float p0) {
        PlayerObject::update(p0); // Call the original update function

        // Disable cutting effect when on the ground with certain gamemodes
        if ((m_isBall || m_isRobot || m_isSpider) && m_isOnGround) {
            // Disable cutting if on the ground and in a ground-based gamemode
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak && cuttingStates[streak]) {
                    streak->stopStroke(); // Disable cutting
                    cuttingStates[streak] = false;
                }
            }
        }
        else {
            // Allow cutting effect when in the air or non-ground gamemodes
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak && !cuttingStates[streak]) {
                    streak->resumeStroke(); // Enable cutting again if conditions are met
                    cuttingStates[streak] = true;
                }
            }
        }
    }
};
