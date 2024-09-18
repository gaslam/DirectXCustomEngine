#pragma once
#include "Timer.h"

namespace DX
{
	class NullTimer final : public Timer
	{
	public:

        // Default constructor
        NullTimer() = default;

        // Default move constructor
        NullTimer(const NullTimer&) = delete;
        NullTimer(NullTimer&&) noexcept = delete;

        // Default move assignment operator
        NullTimer& operator=(const NullTimer&) = delete;
        NullTimer& operator=(NullTimer&&) noexcept = delete;

        // Get elapsed time since the previous Update call.
        uint64_t GetElapsedTicks() const noexcept override { return 0; };
		double GetElapsedSeconds() const noexcept  override { return 0; };

        // Get total time since the start of the program.
        uint64_t GetTotalTicks() const noexcept override { return 0; }
        double GetTotalSeconds() const noexcept override { return 0.0; }

        // Get total number of updates since start of the program.
        uint32_t GetFrameCount() const noexcept override { return 0; }

        // Get the current framerate.
         uint32_t GetFramesPerSecond() const noexcept override { return 0; }

        // Set whether to use fixed or variable timestep mode.
        void SetFixedTimeStep(bool) override {}

        // Set how often to call Update when in fixed timestep mode.
        virtual void SetTargetElapsedTicks(uint64_t) noexcept override{}
        virtual  void SetTargetElapsedSeconds(double) noexcept override{}


        double TicksToSeconds(uint64_t) const noexcept override { return 0; }
        uint64_t SecondsToTicks(double) const noexcept override { return 0; }

        // After an intentional timing discontinuity (for instance a blocking IO operation)
        // call this to avoid having the fixed timestep logic attempt a set of catch-up
        // Update calls.

        virtual void ResetElapsedTime() override {};
	};
}
