#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <unordered_map>

using namespace geode::prelude;

// Static map to associate CCMotionStreak instances with their states
static std::unordered_map<cocos2d::CCMotionStreak*, bool> streakStates;

class $modify(cocos2d::CCMotionStreak) : public cocos2d::CCMotionStreak {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
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

        cocos2d::CCMotionStreak::update(delta);
    }
};

class $modify(PlayerObject) : public PlayerObject {
    void update(float delta) {
        PlayerObject::update(delta);

        if ((m_hasGroundParticles || m_isOnGround || m_isOnGround2 || m_isOnGround3 || m_isOnGround4) &&
            !m_isShip && !m_isSwing && !m_isDart) {
            if (m_regularTrail) {
                auto streak = reinterpret_cast<cocos2d::CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = false;
                    streak->stopStroke();
                }
            }
        } else {
            if (m_regularTrail) {
                auto streak = reinterpret_cast<cocos2d::CCMotionStreak*>(m_regularTrail);
                if (streak) {
                    streakStates[streak] = true;
                }
            }
        }
    }

    void activateStreak() {
        PlayerObject::activateStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<cocos2d::CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true;
            }
        }
    }

    void resetStreak() {
        PlayerObject::resetStreak();

        if (m_regularTrail) {
            auto streak = reinterpret_cast<cocos2d::CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false;
            }
        }
    }
};
