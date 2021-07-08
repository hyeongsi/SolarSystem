#pragma once
#include <Windows.h>

class GameTimer
{
private:
	double mSecondsPerCount;
	double mDeltaTime;
	double mTick;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;

public:
	GameTimer();

	float TotalTime() const;	// �� ����
	float DeltaTime() const;	// �� ����

	void Reset();		// �޽��� ���� ������ ȣ���ؾ� ��
	void Start();		// Ÿ�̸� ����, �簳 �� ȣ���ؾ� ��
	void Stop();		// �Ͻ� ���� �� ȣ���ؾ� ��
	void Tick();		// �� �����Ӹ��� ȣ���ؾ� ��

};

