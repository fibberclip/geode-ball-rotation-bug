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
        if (!player->m_regularTrail || !player->m_regularTrail->isVisible()) {
            CCMotionStreak::update(delta);
            return;
        }

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

        CCMotionStreak::update(delta);
    }
};
