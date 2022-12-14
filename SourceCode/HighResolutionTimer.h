#pragma once
#include <Windows.h>

class HighResolutionTimer
{
public:
	HighResolutionTimer();
    ~HighResolutionTimer() = default;
	HighResolutionTimer(const HighResolutionTimer&) = delete;
	HighResolutionTimer& operator=(const HighResolutionTimer&) = delete;
	HighResolutionTimer(HighResolutionTimer&&) noexcept = delete;
	HighResolutionTimer& operator=(HighResolutionTimer&&) noexcept = delete;

	// Returns the total time elapsed since Reset() was called, NOT counting any
	// time when the clock is stopped.
	float TimeStamp() const; // in seconds;
	float TimeInterval() const;  // in seconds
	void reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop(); // Call when paused.
	void Tick(); // Call every frame.

private:
	double mSecondsPerCount{ 0.0 };
	double mDeltaTime{ 0.0 };

	LONGLONG mBaseTime{ 0LL };
	LONGLONG mPausedTime{ 0LL };
	LONGLONG mStopTime{ 0LL };
	LONGLONG mLastTime{ 0LL };
	LONGLONG mThisTime{ 0LL };

	bool mStopped{ false };
};
