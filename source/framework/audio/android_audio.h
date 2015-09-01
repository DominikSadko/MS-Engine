#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"

class Audio;

class AudioManager : public Lua::Shared
{
	public:
		AudioManager();
		~AudioManager();

		void init();
		void terminate();

		int32_t add(const std::string& path);
		void play(int32_t id);
		void pause(int32_t id);
		void stop(int32_t id);
		void remove(int32_t id);

		int32_t getPosition(int32_t id);
		void setPosition(int32_t id, int32_t bytes);
		int32_t getLength(int32_t id);
		int32_t getSeconds(int32_t id, int32_t bytes);

		std::string getArtist(int32_t id);
		std::string getTitle(int32_t id);
		std::string getAlbum(int32_t id);
		bool hasTagCard(int32_t id);

		void Mute() {};
		void Pause()  {};
		void Resume() {};

		void SetVolume(uint8_t volume);
		uint8_t GetVolume() const { return m_volume; };

		void SetMuted(bool mute) { if((m_mute = mute)) Mute(); }
		bool IsMuted() const { return m_mute; }

	private:
		const Audio* getAudioById(int32_t id) const;

	private:
		int32_t m_lastAudioId;
		std::map<int32_t, Audio*> m_audioId;
		bool m_stop, m_mute;
		uint8_t m_volume;
};

class Audio
{
	public:
		Audio(uint32_t id);
		~Audio();

		void setId(uint32_t id) { m_id = id; }
		uint32_t getId() const { return m_id; }

	private:
		uint32_t m_id;
};

#endif // __AUDIO_H__ //
