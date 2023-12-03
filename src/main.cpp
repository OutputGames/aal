
#include "aal/aal.hpp"
#include "aurora/utils/fs.hpp"

int main(int argc, char* argv[])
{

	AudioMgr* audioMgr = new AudioMgr();

	Sound* sound = new Sound(new SoundCreateInfo{Sound::LoadSoundBuffer("testresources/trc.wav")});


	sound->Play(audioMgr);

	bool exit = false;

	while (!exit)
	{
		sound->Update();
		audioMgr->Update();
	}

	sound->Destroy();

	audioMgr->Destroy();

	return 0;

}
