#if !defined(AAL_HPP)
#define AAL_HPP

#include "AudioFile.h"

#include "aurora/utils/utils.hpp"

#include "AL/al.h"
#include "AL/alc.h"

//#define AL_EXT_float32
#include "AL/alext.h"


struct Listener
{
	glm::vec3 position;
};

struct QueueEntry
{
	ALuint source;
};

struct AudioMgr
{

AURORA_API	AudioMgr();

	Listener* mainListener;

AURORA_API	void Update();

AURORA_API	void AddQueueEntry(QueueEntry entry) { entries.push_back(entry); }

AURORA_API	void Destroy();

private:

	ALCdevice* alcDevice;
	ALCcontext* alcContext;

	std::vector<QueueEntry> entries;
};

struct SoundBuffer
{
	size_t size;
	u8* data;

	u32 channels;
	u32 sampleRate;

	ALuint alBuffer;
	ALenum format;

AURORA_API	void CreateALBuffer();

private:

	friend struct Sound;

	AudioFile<float> audioFile;
};

struct SoundCreateInfo
{

	SoundBuffer buffer;

	glm::vec3 position;
};

struct Sound
{

AURORA_API	Sound(SoundCreateInfo* createInfo);

	glm::vec3 position;

AURORA_API	void Play(AudioMgr* audioMgr);
AURORA_API	void Update();
AURORA_API	void Destroy();

AURORA_API	static SoundBuffer LoadSoundBuffer(std::string path);

	float pitch = 1.f;
	float gain = 1.f;
	bool loop = true;
private:

	ALuint source=-1;
	ALint state = -1;
	SoundBuffer soundBuffer;
};

#endif // AAL_HPP
