#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    float elapsedTime = 0.0f;    // Tracks the elapsed time
    float cutInterval = 0.4f;    // Interval for the trail cutting (default: 0.4s)
    bool isCutting = false;      // Indicates whether the trail is currently being cut

    virtual void update(float delta)
    {
        elapsedTime += delta;

        if (elapsedTime >= cutInterval) {
            elapsedTime -= cutInterval; // Reset the timer

            // Toggle cutting state
            if (isCutting) {
                this->resumeStroke(); // Resumes the trail
            } else {
                this->stopStroke(); // Stops the trail for a bit
            }

            isCutting = !isCutting; // Flip the state
        }

        // Update the trail's behavior, applying the delta time
        CCMotionStreak::update(delta);
    }
};
