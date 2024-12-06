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
    void update(float p0) {
        PlayerObject::update(p0); // Call the original update function with the correct parameter

        // Get current gamemode and ground status
        bool isGroundMode = (m_isBall || m_isRobot || m_isSpider); // Cube is implicitly handled by absence of m_isBall, etc.
        bool isOnGround = m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4; // Check if player is on ground
        
        // If player is in a ground mode and on the ground, disable cutting
        if (isGroundMode && isOnGround) {
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak && cuttingStates[streak]) {
                    streak->stopStroke(); // Disable cutting effect
                    cuttingStates[streak] = false;
                }
            }
        }
        else {
            // Allow cutting effect if player is in the air or in a non-ground gamemode
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak && !cuttingStates[streak]) {
                    streak->resumeStroke(); // Enable cutting effect
                    cuttingStates[streak] = true;
                }
            }
        }
    }
};
