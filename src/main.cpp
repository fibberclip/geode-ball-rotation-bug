#include <Geode/Bindings.hpp>
#include <Geode/Modify.hpp>
#include <random>

using namespace geode::prelude;

class $modify(PlayerObject) {
    void update(float delta) {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        auto setting = Mod::get()->getSetting("skip_probability");
        float skipProbability = setting->getValue<float>(); // Correctly retrieve the float value

        // Randomly skip trail updates
        if (dist(rng) < skipProbability) {
            return; // Skip trail updates for this frame
        }

        PlayerObject::update(delta); // Call the original update logic
    }
};
