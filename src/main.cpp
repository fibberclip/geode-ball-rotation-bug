#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

float elapsedTime = 0.0f;
float cutInterval = 0.2f;
bool isCutting = false;
bool streakActive = false; // New flag to track streak state

class $modify(CCMotionStreak) {

    virtual void update(float delta) {
        if (streakActive) { // Only apply cutting logic when streak is active
            elapsedTime += delta;

            if (elapsedTime >= cutInterval) {
                elapsedTime -= cutInterval;

                if (isCutting) {
                    this->stopStroke();
                } else {
                    this->resumeStroke();
                }

                isCutting = !isCutting;
            }
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
                streak->streakActive = true; // Enable cutting logic
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak(); // Call the original method

        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streak->streakActive = false; // Disable cutting logic
            }
        }
    }
};
