#pragma once
#include <chrono>

class cooldown
{
public:
	typedef std::chrono::nanoseconds n_seconds;
	typedef unsigned long long time;

	cooldown();
	cooldown(time);
	~cooldown();

	void reset();
	void expire();
	void changeLength(time);
	n_seconds elapsed();
	bool ready();
	static time toSeconds(time);
	static time toMilliseconds(time);
	void log();
private:
	std::chrono::high_resolution_clock::time_point start;
	n_seconds timer;
};

