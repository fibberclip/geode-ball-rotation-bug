#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Static map to associate CCMotionStreak instances with their states
static std::unordered_map<CCMotionStreak*, bool> streakStates;

// Global settings for cutting frequency and mode
static double cutFreq = 0.2; // Default frequency

$execute {
    listenForSettingChanges("cutting-freq", [](double value) {
        cutFreq = value; // Update frequency dynamically
        CCLOG("Cutting Frequency updated: %f", cutFreq);
    });
 }

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
        // If this streak is active for cutting logic
        if (streakStates[this]) {
            m_fields->elapsedTime += delta;

            if (m_fields->elapsedTime >= cutFreq) {
                m_fields->elapsedTime -= cutFreq;

                if (m_fields->isCutting) {
                    this->stopStroke();
                } else {
                    this->resumeStroke();
                }

                m_fields->isCutting = !m_fields->isCutting;
            }
        }

        CCMotionStreak::update(delta);
    }
};

class $modify(PlayerObject) {
    
    void activateStreak() {
        PlayerObject::activateStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Activate trail cutting
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Deactivate trail cutting
            }
        }
    }

    bool isGroundMode() const {
        // Helper function to check if the player is in a ground gamemode
        return !(m_isShip || m_isSwing || m_isDart || m_isBird);
    }

    void update(float delta) {
        PlayerObject::update(delta);

        // Update cutting logic based on current state
        if (isGroundMode()) {
            if (m_hasGroundParticles || m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4) {
                // Disable cutting if ground mode and on the ground
                if (m_regularTrail) {
                    auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                    if (streak) {
                        streakStates[streak] = false;
                    }
                }
            }
        } else {
            // Always enable cutting for air gamemodes
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = true;
                }
            }
        }
    }
};
