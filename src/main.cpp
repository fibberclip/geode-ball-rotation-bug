#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
        auto parent = this->getParent();
        if (!parent || !dynamic_cast<PlayerObject*>(parent)) {
            CCMotionStreak::update(delta);
            return;
        }

        auto player = static_cast<PlayerObject*>(parent);
        auto myself = player->m_regularTrail;
        if (!player->m_regularTrail) {
            CCMotionStreak::update(delta);
            return;
        }

        m_fields->elapsedTime += delta;

        if (m_fields->elapsedTime >= m_fields->cutInterval) {
            m_fields->elapsedTime -= m_fields->cutInterval;

            // Perform cutting logic only when the trail is visible
                if (m_fields->isCutting) {
                    myself->stopStroke();
                } else {
                    myself->resumeStroke();
                }

                m_fields->isCutting = !m_fields->isCutting;
        }

        CCMotionStreak::update(delta);
    }
};
