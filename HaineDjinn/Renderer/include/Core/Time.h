#pragma once

namespace Time
{
	static int targetFramerate = 60;
	static float timeScale = 1.f;

	void Update();

	double Time();
	double DeltaTime();
	double FixedDeltaTime();
	double UnscaledDeltaTime();
}


class Timer
{
public:
	Timer() = default;
	Timer(float duration, bool looping = false);

	bool hasEnded = false;

	void Update();
	void Restart(float duration = 0.f, bool looping = false);
private:
	float maxTime = 0.f;
	float time = 0.f;
	bool isLooping = false;
};

struct Stopwatch
{
	Stopwatch() {};

	void Update() { time += (float)(Time::DeltaTime()); };

	float time = 0.f;
};
