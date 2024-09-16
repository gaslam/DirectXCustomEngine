#pragma once
namespace DX
{
    class Timer
    {
    public:
        Timer() = default;
        virtual ~Timer() = default;

		Timer(const Timer&) = delete;
		Timer(Timer&&) noexcept = delete;
		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) noexcept = delete;

        // Get elapsed time since the previous Update call.
        virtual uint64_t GetElapsedTicks() const noexcept = 0;
        virtual double GetElapsedSeconds() const noexcept = 0;

        // Get total time since the start of the program.
        virtual uint64_t GetTotalTicks() const noexcept = 0;
        virtual double GetTotalSeconds() const noexcept = 0;

        // Get total number of updates since start of the program.
        virtual uint32_t GetFrameCount() const noexcept = 0;

        // Get the current framerate.
        virtual  uint32_t GetFramesPerSecond() const noexcept = 0;

        // Set whether to use fixed or variable timestep mode.
        virtual void SetFixedTimeStep(bool isFixed) = 0;

        // Set how often to call Update when in fixed timestep mode.
        virtual void SetTargetElapsedTicks(uint64_t) noexcept = 0;
        virtual  void SetTargetElapsedSeconds(double) noexcept = 0;


        virtual double TicksToSeconds(uint64_t) const noexcept = 0;
        virtual uint64_t SecondsToTicks(double) const noexcept = 0;

        // After an intentional timing discontinuity (for instance a blocking IO operation)
        // call this to avoid having the fixed timestep logic attempt a set of catch-up
        // Update calls.

        virtual void ResetElapsedTime() = 0;

        // Integer format represents time using 10,000,000 ticks per second.
        const uint64_t TicksPerSecond = 10000000;
    };
}

