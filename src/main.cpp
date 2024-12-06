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

// Listen for changes to settings at mod load time
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

            if (m_fields->elapsedTime >= cutFreq) { // Use dynamic frequency
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
                streakStates[streak] = true; // Activate cutting logic
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Deactivate cutting logic
            }
        }
    }

    void update(float delta) {
        PlayerObject::update(delta);

        // Determine if we should enable or disable the trail cutting
        bool isAirMode = m_isShip || m_isSwing || m_isDart;  // Air gamemodes
        bool isGroundMode = !isAirMode;                     // Non-air gamemodes
        bool onGround = m_isOnGround || m_hasGroundParticles;

        if (isAirMode) {
            // Always enable cutting for air modes
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = true; // Enable cutting logic
                }
            }
        } else if (isGroundMode) {
            if (onGround) {
                // Disable cutting logic when on the ground
                if (m_regularTrail) {
                    auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                    if (streak) {
                        streakStates[streak] = false; // Disable cutting logic
                    }
                }
            } else {
                // Re-enable cutting logic when airborne in ground mode
                if (m_regularTrail) {
                    auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                    if (streak) {
                        streakStates[streak] = true; // Enable cutting logic
                    }
                }
            }
        }
    }
};
