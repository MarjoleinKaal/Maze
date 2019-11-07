#pragma once
#include <chrono>

using namespace std;

class Timer
{
public:
	Timer()
	{
		begin = std::chrono::high_resolution_clock::now();
		end = std::chrono::high_resolution_clock::now();
	}
	void Start()
	{
		begin = std::chrono::high_resolution_clock::now();
	}
	double GetTime()
	{
		end = std::chrono::high_resolution_clock::now();
		return ((double)(chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count())) * 1e-9;
	}
private:
	chrono::steady_clock::time_point begin;
	chrono::steady_clock::time_point end;
};