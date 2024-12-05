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

        if (isCutting) {
            // Stop emitting the trail (cutting effect)
            this->setDrawState(false); // stop drawing the trail
        } else {
            // Resume emitting the trail
            this->setDrawState(true); // resume drawing the trail
        }

        // Update the trail's behavior, applying the delta time, which controls its appearance
        CCMotionStreak::update(delta);
    }
};
