#include "SoundManager.h"
#include "../sdlutils/SDLUtils.h"

void SoundManager::setGeneralVolume(int volumeMusic, int volumeEffects){
	volumenGeneral = volumeMusic;
	volumenEfectos = volumeEffects;

	setMusicVolume(volumenGeneral);
	setEffectsVolume(volumenEfectos);
}

void SoundManager::playMainMusic()
{
	sdlutils().musics().at(pauseMusic).setChannelVolume(0);
	sdlutils().musics().at(mainMusic).setChannelVolume(volumenGeneral);

	sdlutils().musics().at(mainMusic).playforMusic();
	sdlutils().musics().at(pauseMusic).playforMusic();
}

void SoundManager::setMusicVolume(int volume)
{
	if (volumenGeneral > volumenPausa)
	{
		volumenGeneral = volume;
		if (volumenGeneral >= 0 && volumenGeneral < 128)
		{
			sdlutils().musics().at(mainMusic).setChannelVolume(volumenGeneral);
		}
		else
		{
			if (volumenGeneral < 0) { volumenGeneral = 0; }
			else if (volumenGeneral >= 128) { volumenGeneral = 127; }
		}
	}
	else
	{
		volumenPausa = volume;
		if (volumenPausa >= 0 && volumenPausa < 128)
		{
			sdlutils().musics().at(pauseMusic).setChannelVolume(volumenPausa);
		}
		else
		{
			if (volumenPausa < 0) { volumenPausa = 0; }
			else if (volumenPausa >= 128) { volumenPausa = 127; }
		}
	}
	
}

void SoundManager::fadeOutMusic()
{
	sdlutils().musics().at(mainMusic).fadeOutMusic(2000);
	sdlutils().musics().at(pauseMusic).fadeOutMusic(2000);
}

void SoundManager::setEffectsVolume(int volume)
{
	volumenEfectos = volume;

	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
	while (it != sdlutils().soundEffects().end())
	{
		it->second.setChannelVolume(volumenEfectos);
		++it;
	}

	sdlutils().musics().at(mainMusic).setChannelVolume(volumenGeneral);
	sdlutils().musics().at(pauseMusic).setChannelVolume(volumenPausa);


}

void SoundManager::ChangeMainMusic(std::string newMusic)
{
	sdlutils().musics().at(mainMusic).haltChannel();
	sdlutils().musics().at(pauseMusic).haltChannel();

	cancionPausada = newMusic;
	mainMusic = newMusic;
	pauseMusic = newMusic + "Pausa";

	playMainMusic();
}

void SoundManager::playPauseMusic()
{
	volumenPausa = volumenGeneral;
	volumenGeneral = 0;
	sdlutils().musics().at(mainMusic).setChannelVolume(0);

	
	sdlutils().musics().at(pauseMusic).setChannelVolume(volumenPausa);
}

void SoundManager::resumeMainMusic()
{
		volumenGeneral = volumenPausa;
		volumenPausa = 0;
		sdlutils().musics().at(pauseMusic).setChannelVolume(0);
		sdlutils().musics().at(mainMusic).setChannelVolume(volumenGeneral);
}

void SoundManager::pauseMainMusic(std::string newMusic)
{
	sdlutils().musics().at(mainMusic).pauseChannel();
	sdlutils().musics().at(pauseMusic).pauseChannel();

	cancionPausada = mainMusic;

	mainMusic = newMusic;
	pauseMusic = newMusic + "Pausa";

	playMainMusic();
}

void SoundManager::stopMusic()
{
	sdlutils().musics().at(mainMusic).haltChannel();
}

void SoundManager::playSoundEffect(std::string effect, Uint32 seg)
{
	if (timer + seg < SDL_GetTicks())
	{
		timer = SDL_GetTicks();
		sdlutils().soundEffects().at(effect).play();
	}

}

