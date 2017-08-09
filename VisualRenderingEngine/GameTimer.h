#pragma once

class CGameTimer
{
public:
	CGameTimer();
	~CGameTimer();

	float TotalTime() const; 
	float DeltaTime() const;

	void Reset(); 
	void Start(); 
	void Stop();  
	void Tick(); 
	float CalculateFrameStats();

private:
	double m_dSecondsPerCount;
	double m_dDeltaTime;

	__int64 m_iBaseTime;
	__int64 m_iPausedTime;
	__int64 m_iStopTime;
	__int64 m_iPrevTime;
	__int64 m_iCurrTime;

	bool m_bStopped;
};

