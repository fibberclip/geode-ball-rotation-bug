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
