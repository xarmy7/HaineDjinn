
#include <GLFW/glfw3.h>

#include "Core/Time.h"

Timer::Timer(float duration, bool looping)
{
	maxTime = duration; 
	time = maxTime;
	isLooping = looping;
}

void Timer::Update()
{
	if (time > 0.f)
		time -= (float)(Time::DeltaTime());

	if (time <= 0.f)
	{
		hasEnded = true;

		if (isLooping)
			time = maxTime;
	}
	else
		hasEnded = false;
}
void Timer::Restart(float duration, bool looping)
{
	hasEnded = false;

	if (duration <= 0.f)
		time = maxTime;

	isLooping = looping;
}

namespace Time
{
	namespace
	{
		double time = 0.f;
		double lastFrame = 0.f;
		double deltaTime = 0.f;
		double unscaledDeltaTime = 0.f;
		const double fixedDeltaTime = 1.f/60.f;

		const double deltaMax = 1.0f;
	}

	void Update()
	{
		time = glfwGetTime();

		double actualDelta = time - lastFrame;
	
		if (actualDelta > deltaMax)
		{
			unscaledDeltaTime = 1.f / (float)(targetFramerate);
			deltaTime = unscaledDeltaTime * timeScale;
		}
		else
		{
			unscaledDeltaTime = actualDelta;
			deltaTime = actualDelta * timeScale;
		}

		lastFrame = time;
	}

	double Time()
	{
		return time;
	}

	double DeltaTime()
	{
		return deltaTime;
	}

	double FixedDeltaTime()
	{
		return fixedDeltaTime;
	}

	double UnscaledDeltaTime()
	{
		return unscaledDeltaTime;
	}
}
