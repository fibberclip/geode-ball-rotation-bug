#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Static map to associate CCMotionStreak instances with their states
static std::unordered_map<CCMotionStreak*, bool> streakStates;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = cutFreq;
        bool isCutting = false;
    };

    auto cuttingMode = Mod::get()->getSettingValue<std::string>("cutting-mode");
    auto cutFreq = Mod::get()->getSettingValue<double>("cutting-freq");

    virtual void update(float delta) {
        // If this streak is active for cutting logic
        if (streakStates[this]) {
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

    void update(float delta) {
        PlayerObject::update(delta);

        // Determine mode and ground status
        bool isAirMode = m_isShip || m_isSwing || m_isDart;  // Air gamemodes
        bool isGroundMode = !isAirMode;                     // Non-air gamemodes
        bool onGround = m_isOnGround || m_hasGroundParticles;

        if (isGroundMode) {
            if (onGround) {
                // Disable trail (and cutting) when on the ground
                if (m_regularTrail) {
                    auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                    if (streak) {
                        streakStates[streak] = false; // Deactivate cutting
                    }
                }
            } 
            // Trail cutting should only exist when activateStreak() explicitly triggers it
            // No action needed for being airborne alone
        } else if (isAirMode) {
            // Air gamemodes: Always enable trail cutting
            if (m_regularTrail) {
                auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = true; // Enable cutting logic
                }
            }
        }
    }
};
