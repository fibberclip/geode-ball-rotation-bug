#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <random>

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    virtual void update(float delta)
    {
        // Generate a random number to simulate the bug happening with a certain frequency
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 100); // Frequency range (0-100%)

        int randChance = dis(gen);
        
        // Customizable chance for the bug to happen (75% here as per your request)
        bool isCutting = randChance < 75;

        // Adjust the speed of the trail to create the cutting effect
        if (isCutting) {
            // Stop the trail from emitting new segments by setting speed to 0
            this->setSpeed(0); // no new trail segments
        } else {
            // Resume normal trail behavior by restoring speed
            this->setSpeed(10); // or any value you want for normal speed
        }

        // Update the trail's behavior, applying the delta time, which controls its appearance
        CCMotionStreak::update(delta);
    }
};
