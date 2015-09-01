#include "framework/audio/audio.h"

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
    if(!BASS_Init(-1, 44100, 0, 0, NULL))
    	LOGI("BASS_Init: %i", BASS_ErrorGetCode());
}

void AudioManager::terminate()
{
	for(auto it : m_audioId)
	{
		BASS_StreamFree(it.second->getId());
		delete it.second;
	}

	m_audioId.clear();
    BASS_Free();
}

int32_t AudioManager::add(const std::string& path)
{
    wchar_t wpath[4096];
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath, 4096);

	if(uint32_t id = BASS_StreamCreateFile(false, wpath, 0, 0, BASS_UNICODE))
	{
		m_lastAudioId++;
		if(m_lastAudioId > 0x7FFFFFFF)
			m_lastAudioId = 0;

		Audio* audio = new Audio(id);
		audio->m_tagCard = (TAG_CART*)BASS_ChannelGetTags(id, BASS_TAG_ID3V2);

		m_audioId.insert(std::pair<int32_t, Audio*>(m_lastAudioId, audio));

		return m_lastAudioId;
	}
	else
    	LOGI("BASS_StreamCreateFile: %i : %ls", BASS_ErrorGetCode(), wpath);

	return 0;
}

void AudioManager::play(int32_t id)
{
	const Audio* audio = getAudioById(id);
	if(!audio)
	{
    	LOGI("Audio not found: %i", id);
    	return;
	}

	if(!BASS_ChannelPlay(audio->getId(), false))
    	LOGI("[BASS_ChannelPlay] error: %i", BASS_ErrorGetCode());

	BASS_ChannelSetSync(audio->getId(), BASS_SYNC_END, 0, &AudioManager::CallBack_OnEnd, this);

	if(!BASS_ChannelSetAttribute(audio->getId(), BASS_ATTRIB_VOL, m_volume / 100.f))
    	LOGI("[BASS_ChannelSetAttribute] error: %i", BASS_ErrorGetCode());
}

void AudioManager::pause(int32_t id)
{
	const Audio* audio = getAudioById(id);
	if(!audio)
	{
    	LOGI("Audio not found: %i", id);
    	return;
	}

	if(!BASS_ChannelPause(audio->getId()))
    	LOGI("error: %i", BASS_ErrorGetCode());
}

void AudioManager::stop(int32_t id)
{
	const Audio* audio = getAudioById(id);
	if(!audio)
	{
    	LOGI("Audio not found: %i", id);
    	return;
	}

	if(!BASS_ChannelStop(audio->getId()))
    	LOGI("error: %i", BASS_ErrorGetCode());
}

void AudioManager::remove(int32_t id)
{
	auto it = m_audioId.find(id);
	if(it != m_audioId.end())
	{
		BASS_StreamFree(it->second->getId());
		delete it->second;
		m_audioId.erase(it);
	}
}

int32_t AudioManager::getPosition(int32_t id)
{
	if(const Audio* audio = getAudioById(id))
		return (int32_t)BASS_ChannelGetPosition(audio->getId(), BASS_POS_BYTE);
	else
		LOGI("Audio not found: %i", id);

    return 0;
}

void AudioManager::setPosition(int32_t id, int32_t bytes)
{
	if(const Audio* audio = getAudioById(id))
		BASS_ChannelSetPosition(audio->getId(), bytes, BASS_POS_BYTE);
	else
    	LOGI("Audio not found: %i", id);
}

int32_t AudioManager::getLength(int32_t id)
{
	if(const Audio* audio = getAudioById(id))
		return (int32_t)BASS_ChannelGetLength(audio->getId(), BASS_POS_BYTE);
	else
    	LOGI("Audio not found: %i", id);

    return 0;
}

int32_t AudioManager::getSeconds(int32_t id, int32_t bytes)
{
	if(const Audio* audio = getAudioById(id))
		return (int32_t)BASS_ChannelBytes2Seconds(audio->getId(), bytes);
	else
    	LOGI("Audio not found: %i", id);

    return 0;
}

std::string AudioManager::getArtist(int32_t id)
{
	const Audio* audio = getAudioById(id);
	if(!audio)
    	LOGI("Audio not found: %i", id);
	else if(!audio->m_tagCard)
		LOGI("Audio TagCard not found: %i", id);
	else
		return audio->m_tagCard->Artist;

	return "";
}

std::string AudioManager::getTitle(int32_t id)
{
	const Audio* audio = getAudioById(id);
	if(!audio)
    	LOGI("Audio not found: %i", id);
	else if(!audio->m_tagCard)
		LOGI("Audio TagCard not found: %i", id);
	else
		return audio->m_tagCard->Title;

	return "";
}

std::string AudioManager::getAlbum(int32_t id)
{
	const Audio* audio = getAudioById(id);
	if(!audio)
    	LOGI("Audio not found: %i", id);
	else if(!audio->m_tagCard)
		LOGI("Audio TagCard not found: %i", id);
	else if(TAG_ID3* tag = (TAG_ID3*)BASS_ChannelGetTags(audio->getId(), BASS_TAG_ID3))
		return tag->album;

	return "";
}

bool AudioManager::hasTagCard(int32_t id)
{
	if(const Audio* audio = getAudioById(id))
		return audio->m_tagCard;
	else
    	LOGI("Audio not found: %i", id);

	return false;
}

void AudioManager::CallBack_OnEnd(HSYNC, DWORD id, DWORD, void* user)
{
    AudioManager* audioManager = (AudioManager*)(user);
    audioManager->remove(id);
}

void AudioManager::SetVolume(uint8_t volume)
{
	m_volume = volume;
	for(auto it : m_audioId)
		BASS_ChannelSetAttribute(it.second->getId(), BASS_ATTRIB_VOL, m_volume / 100.f);
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
