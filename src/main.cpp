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
static std::string cuttingMode = "stopStroke"; // Default cutting mode

// Listen for changes to settings at mod load time
$execute {
    listenForSettingChanges("cutting-freq", [](double value) {
        cutFreq = value; // Update frequency dynamically
        CCLOG("Cutting Frequency updated: %f", cutFreq);
    });

    listenForSettingChanges("cutting-mode", [](std::string value) {
        cuttingMode = value; // Update cutting mode dynamically
        CCLOG("Cutting Mode updated: %s", cuttingMode.c_str());
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
                    // Use dynamic cutting mode
                    if (cuttingMode == "stopStroke") {
                        this->stopStroke();
                    } else if (cuttingMode == "reset") {
                        this->reset();
                    }
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
    void bumpPlayer(float p0, int p1, bool p2, GameObject* p3) {
        // Call the original bumpPlayer functionality
        PlayerObject::bumpPlayer(p0, p1, p2, p3);

        // Trigger the trail logic
        this->activateStreak();
    }

    void flipGravity(bool p0, bool p1) {
        // Call the original flipGravity functionality
        PlayerObject::flipGravity(p0, p1);

        // Trigger the trail logic
        this->activateStreak();
    }

    void activateStreak() {
        // Call the original activateStreak method
        PlayerObject::activateStreak();

        // Custom logic for enabling trail cutting
        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = true; // Activate trail cutting
            }
        }
    }

    void resetStreak() {
        // Call the original resetStreak method
        PlayerObject::resetStreak();

        // Custom logic for disabling trail cutting
        if (m_regularTrail) {
            auto streak = reinterpret_cast<CCMotionStreak*>(m_regularTrail);
            if (streak) {
                streakStates[streak] = false; // Deactivate trail cutting
            }
        }
    }
};

