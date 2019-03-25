#include "SoundSystem.h"
#include <iostream>
/*#define error(result)
{
if (result != FMOD_OK) {
std::cout << "FMOD error! " <<__LINE__ << __FILE__ << result << FMOD_ErrorString(result) << std::endl;
}
}
*/
typedef FMOD::Sound* Sound;

SoundSystem::SoundSystem()
{
	auto res = FMOD::System_Create(&m_system);
	if (res != FMOD_OK) {
		std::cout << "FMOD didn't work! :( \n";
		return;
	}
		
	int driverCount = 0;
	m_system->getNumDrivers(&driverCount);
	if (driverCount == 0) {
		std::cout << "There are no drivers! :( \n";
	}

	m_system->init(32, FMOD_INIT_NORMAL, NULL);	//set channels (mono is 1 channel, stereo is 2 channels. Can only have 32 channels so either 32 mono or 16 stereo
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::createSound(Sound sound, std::string filePath) {
	m_system->createSound(filePath.c_str(), FMOD_2D | FMOD_LOOP_NORMAL, NULL, &sound);
}

void SoundSystem::createChannel(Channel channel, Channels id) {
	m_system->getChannel(id, &channel);
}

void SoundSystem::playSound(Sound sound, Channel channel, bool loop = false) {
	if (!loop) {
		sound->setMode(FMOD_2D | FMOD_LOOP_OFF);
	}
	else {
		sound->setMode(FMOD_2D | FMOD_LOOP_NORMAL);
		sound->setLoopCount(-1);
	}
	m_system->playSound(sound, nullptr, false, &channel);
}