#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/console.hpp>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    // Timer to track elapsed time
    float timer = 0.0f;
    const float cutInterval = 0.4f;  // Set the time interval (0.4 seconds, can be adjusted)

    virtual void update(float delta)
    {
        // Increment the timer with each frame
        timer += delta;

        // Check if the interval has passed
        if (timer >= cutInterval) {
            // Reset the timer and cut the trail
            timer = 0.0f;

            // Toggle the trail state (stop or resume)
            if (this->isStrokeActive()) {
                this->stopStroke(); // Stop the trail
            } else {
                this->resumeStroke(); // Resume the trail
            }
        }

        // Update the trail's behavior, applying the delta time
        CCMotionStreak::update(delta);
    }
};
