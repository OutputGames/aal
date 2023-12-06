#include "aal.hpp"

#include "AudioFile.h"

using namespace std;

AudioMgr::AudioMgr()
{

	alcDevice = alcOpenDevice(nullptr);
	if (!alcDevice)
		throw("Failed to get sound device");

	alcContext = alcCreateContext(alcDevice, nullptr);
	if (!alcContext)
		throw("Failed to get sound context");

	if (!alcMakeContextCurrent(alcContext))
		throw("Failed to make context current");

	const ALchar* name = nullptr;
	if (alcIsExtensionPresent(alcDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(alcDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(alcDevice) != AL_NO_ERROR)
		name = alcGetString(alcDevice, ALC_DEVICE_SPECIFIER);

	if (alIsExtensionPresent("AL_EXT_float32"))
		cout << "32bit audio supported." << endl;

	const ALchar* exts = alGetString(AL_EXTENSIONS);

	cout << "Supported extensions: " << exts << endl;
 
	cout << "Opened " << name << endl;

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	mainListener = new Listener;
}

void AudioMgr::Update()
{
	for (QueueEntry entry : entries)
	{
		alSourcePlay(entry.source);
		//audioManager->play3d(*entry.source, mainListener->position.x, mainListener->position.y, mainListener->position.z, 0, 0, 0);
	}
	entries.clear();
}

void AudioMgr::Destroy()
{
	if (!alcMakeContextCurrent(nullptr))
		throw std::runtime_error("Unable to deinit alc contect");
	alcDestroyContext(alcContext);
	alcContext = nullptr;
	if (alcContext)
		throw std::runtime_error("AL context not destroyed");

	if (!alcCloseDevice(alcDevice))
		throw std::runtime_error("Unable to close AL device");
}

void SoundBuffer::CreateALBuffer()
{


	alGenBuffers(1, &alBuffer);
	alBufferData(alBuffer, format, data, size, sampleRate);

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));

		throw;

		if (alBuffer && alIsBuffer(alBuffer))
			alDeleteBuffers(1, &alBuffer);
	}
}

Sound::Sound(SoundCreateInfo* createInfo)
{
	//source = new SoLoud::Wav();
	//source->load("testresources/plazatest.wav");
	position = createInfo->position;

	soundBuffer = createInfo->buffer;

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);

	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);

	alSourcei(source, AL_LOOPING, loop);

	alSourcef(source, AL_ROLLOFF_FACTOR, 1);
	alSourcef(source, AL_REFERENCE_DISTANCE, 6);
	alSourcef(source, AL_MAX_DISTANCE, 15);
	alSourcei(source, AL_BUFFER, soundBuffer.alBuffer);

	alSourcePlay(source);

}

void Sound::Play(AudioMgr* audioMgr)
{
	audioMgr->AddQueueEntry({ source });
	state = AL_PLAYING;
}

void Sound::Update()
{
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
	alSourcei(source, AL_LOOPING, loop);

	if (state != AL_INITIAL) {
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
}

void Sound::Destroy()
{
	alDeleteBuffers(1, &soundBuffer.alBuffer);
	alDeleteSources(1, &source);
}

SoundBuffer Sound::LoadSoundBuffer(std::string path)
{

	SoundBuffer buffer{};

	AudioFile<float> audioFile;

	bool loadResult = audioFile.load(path);

	vector<u8> pcmData;
	audioFile.writePCMToBuffer(pcmData);

	bool isStereo = audioFile.isStereo();

	if (isStereo)
	{
		cout << audioFile.iXMLChunk << endl;
	}

	auto convertFileToOpenALFormat = [](const AudioFile<float>& audioFile) {
		int bitDepth = audioFile.getBitDepth();
		if (bitDepth == 16)
			return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		else if (bitDepth == 8)
			return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		else if (bitDepth == 32)
			return audioFile.isStereo() ? AL_FORMAT_STEREO_FLOAT32 : AL_FORMAT_MONO_FLOAT32;
		else
			return -1; // this shouldn't happen!
	};


	buffer.format = convertFileToOpenALFormat(audioFile);
	buffer.size = pcmData.size();
	buffer.channels = audioFile.getNumChannels();
	buffer.data = pcmData.data();
	buffer.sampleRate = audioFile.getSampleRate();

	buffer.audioFile = audioFile;

	buffer.CreateALBuffer();

	return buffer;
	

}
