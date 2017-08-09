#include "stdafx.h"
#include "GameTimer.h"
#include "VREngine.h"

CGameTimer::CGameTimer()
	: m_dSecondsPerCount(0.0), m_dDeltaTime(-1.0), m_iBaseTime(0),
	m_iPausedTime(0), m_iPrevTime(0), m_iCurrTime(0), m_bStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_dSecondsPerCount = 1.0 / (double)countsPerSec;
}

CGameTimer::~CGameTimer()
{

}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float CGameTimer::TotalTime()const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if (m_bStopped)
	{
		return (float)(((m_iStopTime - m_iPausedTime) - m_iBaseTime)*m_dSecondsPerCount);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime

	else
	{
		return (float)(((m_iCurrTime - m_iPausedTime) - m_iBaseTime)*m_dSecondsPerCount);
	}
}

float CGameTimer::DeltaTime()const
{
	return (float)m_dDeltaTime;
}

void CGameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_iBaseTime = currTime;
	m_iPrevTime = currTime;
	m_iStopTime = 0;
	m_bStopped = false;
}

void CGameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	if (m_bStopped)
	{
		m_iPausedTime += (startTime - m_iStopTime);

		m_iPrevTime = startTime;
		m_iStopTime = 0;
		m_bStopped = false;
	}
}

void CGameTimer::Stop()
{
	if (!m_bStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_iStopTime = currTime;
		m_bStopped = true;
	}
}

void CGameTimer::Tick()
{
	if (m_bStopped)
	{
		m_dDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_iCurrTime = currTime;

	// Time difference between this frame and the previous.
	m_dDeltaTime = (m_iCurrTime - m_iPrevTime)*m_dSecondsPerCount;

	// Prepare for next frame.
	m_iPrevTime = m_iCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (m_dDeltaTime < 0.0)
	{
		m_dDeltaTime = 0.0;
	}
}

float CGameTimer::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if (TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		// FPS ·»´õ¸µ ÇÊ¿ä
		frameCnt = 0;
		timeElapsed += 1.0f;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(VR_ENGINE->GetHWnd(), outs.str().c_str());
	}

	return frameCnt;
}

