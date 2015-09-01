#include "framework/audio/android_audio.h"

AudioManager::AudioManager()
{
	m_lastAudioId = 0;
	m_stop = false;
	m_mute = false;;
	m_volume = 80;
}

AudioManager::~AudioManager()
{
	terminate();
}

void AudioManager::init()
{
}

void AudioManager::terminate()
{
	for(auto it : m_audioId)
	{
		delete it.second;
	}

	m_audioId.clear();
}

int32_t AudioManager::add(const std::string& path)
{
	return 0;
}

void AudioManager::play(int32_t id)
{
}

int32_t AudioManager::getPosition(int32_t id)
{
    return 0;
}

void AudioManager::setPosition(int32_t id, int32_t bytes)
{
}

int32_t AudioManager::getLength(int32_t id)
{
    return 0;
}

int32_t AudioManager::getSeconds(int32_t id, int32_t bytes)
{
    return 0;
}

std::string AudioManager::getArtist(int32_t id)
{
    return "";
}

std::string AudioManager::getTitle(int32_t id)
{
    return "";
}

std::string AudioManager::getAlbum(int32_t id)
{
    return "";
}

bool AudioManager::hasTagCard(int32_t id)
{
    return false;
}

void AudioManager::pause(int32_t id)
{
}

void AudioManager::stop(int32_t id)
{
}

void AudioManager::remove(int32_t id)
{
}

void AudioManager::SetVolume(uint8_t volume)
{
	m_volume = volume;
	for(auto it : m_audioId)
		{}
}

const Audio* AudioManager::getAudioById(int32_t id) const
{
	const auto it = m_audioId.find(id);
	if(it != m_audioId.end())
		return it->second;

	return nullptr;
}

Audio::Audio(uint32_t id)
{
	m_id = id;
}

Audio::~Audio()
{
}
