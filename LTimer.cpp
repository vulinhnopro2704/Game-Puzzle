#include "LTimer.h"

LTimer::LTimer(Uint32 time) : time(time)
{
	mStartTicks = 0;
	mPausedTicks = 0;

	mStarted = false;
	mPaused = false;
}

void LTimer::start(Uint32 time)
{
	//Start Timer
	mStarted = true;
	this->time = time;
	//Unpause the timer
	mPaused = false;

	//Get Current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//Stop timer
	mStarted = false;
	//Unpause time
	mPaused = false;

	//Clear tick variable
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 timing = time * 1000;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			timing = mPausedTicks + time * 1000;
		}
		else
		{
			//Return the current time minus the start time
			timing = SDL_GetTicks() - mStartTicks + time * 1000;
		}
	}

	return timing;
}

bool LTimer::isStarted()
{ 
	//Timer is running and paused or unpaused	
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}
