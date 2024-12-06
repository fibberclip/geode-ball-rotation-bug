#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode;
using namespace cocos2d;

class $modify(CCMotionStreak) : public CCMotionStreak {
    struct Fields {
        // Add custom fields if needed
    };

    void stopStroke() {
        // Custom logic for stopping the stroke
        CCMotionStreak::reset();
    }

    void resumeStroke() {
        // Custom logic for resuming the stroke
        CCMotionStreak::setOpacity(255); // Example functionality
    }
};

class $modify(PlayerObject) : public PlayerObject {
    struct Fields {
        bool trailCuttingEnabled = false; // Example custom field
        float cuttingInterval = 0.4f; // Default time interval for trail cutting
        float timeSinceLastCut = 0.0f; // Time tracker
    };

    void update(float delta) {
        PlayerObject::update(delta); // Call the original update method

        // Custom logic to simulate trail cutting
        auto& fields = m_fields();
        fields.timeSinceLastCut += delta;

        if (fields.trailCuttingEnabled && fields.timeSinceLastCut >= fields.cuttingInterval) {
            auto streak = dynamic_cast<CCMotionStreak*>(this->getChildByTag(123)); // Replace with the actual tag or logic
            if (streak) {
                streak->stopStroke();
                streak->resumeStroke();
            }
            fields.timeSinceLastCut = 0.0f;
        }
    }

    void toggleTrailCutting() {
        auto& fields = m_fields();
        fields.trailCuttingEnabled = !fields.trailCuttingEnabled;
    }
};
