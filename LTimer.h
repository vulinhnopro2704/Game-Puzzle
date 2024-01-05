#pragma once
//The application time based timer
#include "CommonFunc.h"

class LTimer
{
public:
    //Initializes variables
    LTimer(Uint32 time = 0);

    LTimer(int time);

    //The various clock actions
    void start(Uint32 time = 0);
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;
    Uint32 time;
    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};