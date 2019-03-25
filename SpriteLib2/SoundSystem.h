#pragma once
#include <fmod\fmod.hpp>
#include <fmod\fmod_errors.h>
#include <string>
typedef FMOD::Sound* Sound;
typedef FMOD::Channel* Channel;
enum Channels {
	MUSIC = 0,
	PLAYER = 1,
	ENEMIES = 2
};
class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void createSound(Sound m_sound, std::string filePath);
	void createChannel(Channel channel, Channels id);
	void playSound(Sound sound, Channel channel, bool loop);
private:
	FMOD::System *m_system;
};

