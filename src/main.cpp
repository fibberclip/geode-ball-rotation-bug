#include <Geode/Bindings.hpp>
#include <Geode/Modify.hpp>
#include <random>

using namespace geode::prelude;

class $modify(PlayerTrail) {
    void updateTrail() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        
        float skipProbability = Mod::get()->getSetting("skip_probability")->getValue<float>();
        
        if (dist(rng) < skipProbability) {
            return; // Skip this frame's trail update
        }

        PlayerTrail::updateTrail(); // Call original logic
    }
};

$execute {
    Mod::get()->addSetting("skip_probability", 0.1f); // Default frequency
}
