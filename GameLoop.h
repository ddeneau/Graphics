#pragma once
#include <functional>
#include <chrono>

// Encapsulates fixed-timestep game loop execution with frame rate control.
// Supports both continuous execution (run) and frame-by-frame control (update).
class GameLoop {
public:
    int frame;
    using UpdateCallback = std::function<void()>;

    explicit GameLoop(int target_fps = 60);
    virtual ~GameLoop() = default;

    // Continuous loop - runs until stop() is called; blocking call
    void run(UpdateCallback update_callback);
    
    // Single frame update - runs one iteration; non-blocking; used in existing game loops
    void update(UpdateCallback update_callback);
    
    void stop();
    
    void set_target_fps(int fps);
    int get_target_fps() const;
    float get_delta_time() const;
    int get_frame();

private:
    int target_fps;
    bool running;
    float delta_time;
    std::chrono::steady_clock::time_point last_frame_time;

    // Private helper: calculates elapsed time since last frame in seconds
    void calculate_frame_time();
    
    // Private helper: sleeps to maintain target FPS; allows other processes to use CPU
    void sleep_to_target_fps();
};