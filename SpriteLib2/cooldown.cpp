#include "cooldown.h"
#include <iostream>

typedef std::chrono::nanoseconds n_seconds;

cooldown::cooldown()
{
}

// keep in mind that 1 second is 1,000,000,000 nanoseconds
// unsigned long long goes up to 18,446,744,073,709,551,615 nanoseconds
//                            or 18,446,744,073.709551615   seconds
//                            or 307,445,734.56182586025    hours
//                            or ~35,096.5                  years
cooldown::cooldown(time nanoseconds)
{
	reset();
	changeLength(nanoseconds);
}

cooldown::~cooldown()
{
}

void cooldown::reset() {
	start = std::chrono::high_resolution_clock::now();
}

void cooldown::expire()
{
	start = std::chrono::high_resolution_clock::now() - timer;
}

void cooldown::changeLength(time nanoseconds) {
	timer = n_seconds(nanoseconds);
}

n_seconds cooldown::elapsed() {
	return std::chrono::duration_cast<n_seconds>(std::chrono::high_resolution_clock::now() - start);
}

bool cooldown::ready() {
	if (elapsed() > timer) return true;
	return false;
}

unsigned long long cooldown::toSeconds(time t)
{
	return t * 1000000000;
}

unsigned long long cooldown::toMilliseconds(time t)
{
	return t * 1000000;
}

void cooldown::log()
{
	std::cout << timer.count() << std::endl;
}


