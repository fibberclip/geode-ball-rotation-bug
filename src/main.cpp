#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(CCMotionStreak) {
    struct Fields {
        float elapsedTime = 0.0f;
        float cutInterval = 0.2f;
        bool isCutting = false;
    };

    virtual void update(float delta) {
        // Get the associated PlayerObject
        auto player = reinterpret_cast<PlayerObject*>(this->getParent());
        if (!player || !player->m_regularTrail || !player->m_regularTrail->isVisible()) {
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

class $modify(PlayLayer) {
    void postUpdate(float p0) {
        PlayLayer::postUpdate(p0);

        // Ensure trails are visible only when they should be
        if (m_player1 && m_player1->m_regularTrail) {
            m_player1->m_regularTrail->setVisible(!m_player1->m_isDart);
        }
        if (m_player2 && m_player2->m_regularTrail) {
            m_player2->m_regularTrail->setVisible(!m_player2->m_isDart);
        }
    }
};
