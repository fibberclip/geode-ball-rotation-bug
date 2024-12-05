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
        if (randChance < 75) {  // 5% chance for bug to occur (can adjust for frequency)
            // Apply a "cut" effect, simulating the bug (e.g., stop rendering the trail for a short time)
            this->setVisible(false);
        } else {
            this->setVisible(true);
        }

        // Call original update function with delta adjusted for gameplay speed
        CCMotionStreak::update(delta);
    }
};
