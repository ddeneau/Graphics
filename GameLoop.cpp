#include "GameLoop.h"
#include <thread>

// Module purpose: Enforce fixed timestep game loop with frame rate control.
// Uses steady_clock for reliable timing independent of system clock adjustments.
int frame = 1;

// Constructor initializes timing state; explicit keyword prevents accidental implicit conversions
GameLoop::GameLoop(int target_fps)
    : target_fps(target_fps), running(false), delta_time(0.0f),
      last_frame_time(std::chrono::steady_clock::now())  // Member initializer list preferred for efficiency
{
    // Constructor body empty; all state initialized in initializer list (no redundant assignments)
}

// Main game loop - runs callback repeatedly at target FPS with calculated delta_time
// This is a "frame-based" loop; contrasts with "time-based" loops that process events until time elapsed
void GameLoop::run(UpdateCallback update_callback)
{
    running = true;  // Signal loop is active

    // Loop continues until stop() is called; condition checked at start of each iteration
    while (running) {
        // Calculate elapsed time since last frame; used for physics/animation timesteps
        calculate_frame_time();

        // Invoke caller's update callback; std::function wraps any callable (function, lambda, functor)
        update_callback();

        // Sleep remainder of frame to maintain target FPS; prevents CPU from spinning at 100%
        sleep_to_target_fps();
    }
}

// Single frame iteration - executes one cycle of calculate_frame_time -> callback -> sleep
// Allows caller to manage loop structure; useful for integrating with external event loops (Win32 message pump)
void GameLoop::update(UpdateCallback update_callback)
{
    // Calculate elapsed time since last frame; used for physics/animation timesteps
    calculate_frame_time();

    // Invoke caller's update callback; std::function wraps any callable (function, lambda, functor)
    update_callback();

    // Sleep remainder of frame to maintain target FPS; prevents CPU from spinning at 100%
    sleep_to_target_fps();

	frame++;
}

// Sets running = false; causes loop condition to fail on next iteration (non-blocking exit)
void GameLoop::stop()
{
    running = false;
}

// Setter with validation; prevents setting FPS to 0 (which would divide-by-zero in sleep calculation)
void GameLoop::set_target_fps(int fps)
{
    if (fps > 0) {  // Guard clause: early return for invalid input
        target_fps = fps;
    }
}

// Const member function signals this is a query with no side effects; encourages const-correctness
int GameLoop::get_target_fps() const
{
    return target_fps;
}

// Getter for delta_time; game logic uses this for frame-rate independent movement
float GameLoop::get_delta_time() const
{
    return delta_time;
}

// Private helper: calculates elapsed time since last frame in seconds (as float for physics use)
void GameLoop::calculate_frame_time()
{
    // steady_clock provides monotonic time; immune to system clock adjustments (unlike system_clock)
    auto current_time = std::chrono::steady_clock::now();

    // duration_cast converts internal representation to seconds; explicit cast prevents narrowing
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(
        current_time - last_frame_time
    );

    // Store elapsed time; clamped to prevent large delta_time jumps (e.g., during debugger pause)
    delta_time = std::min(elapsed.count(), 0.1f);  // 0.1s = 100ms max; prevents single frame from advancing too far

    // Update timestamp for next frame; must be done at end to measure actual frame time including callback
    last_frame_time = current_time;
}

// Private helper: sleeps to maintain target FPS; allows other processes to use CPU
void GameLoop::sleep_to_target_fps()
{
    // Guard: if target_fps is 0, skip sleeping (prevents division by zero)
    if (target_fps <= 0) {
        return;
    }

    // Calculate target frame duration in milliseconds: 1000ms / 60fps = ~16.67ms
    // Using milliseconds for sleep_for() for better precision on most systems
    float target_frame_ms = 1000.0f / static_cast<float>(target_fps);

    // Actual frame time includes time spent in update_callback; calculate remainder
    auto current_time = std::chrono::steady_clock::now();
    auto frame_elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(
        current_time - last_frame_time
    );

    // Sleep duration = target frame time - time already spent this frame
    float sleep_time_ms = target_frame_ms - (frame_elapsed.count() * 1000.0f);

    // Only sleep if there's time to sleep; prevents negative sleep durations
    if (sleep_time_ms > 0.0f) {
        // sleep_for with milliseconds provides OS scheduler control; yields CPU to other threads
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long long>(sleep_time_ms))
        );
    }
}

int GameLoop::get_frame() {
    return frame;
}
