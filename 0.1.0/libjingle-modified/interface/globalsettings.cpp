

#include "globalsettings.h"



GlobalSettings* GlobalSettings::ms_instance = 0;

GlobalSettings::GlobalSettings()
{
    pthread_mutex_init(&mutex_, NULL);
}

GlobalSettings::~GlobalSettings()
{
    pthread_mutex_destroy(&mutex_);
}

GlobalSettings* GlobalSettings::Instance()
{
	if(ms_instance == 0){
		ms_instance = new GlobalSettings();
	}
	return ms_instance;
}

void GlobalSettings::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

void GlobalSettings::setCards(const std::string& mic, const std::string& speaker)
{
    pthread_mutex_lock(&mutex_);
    mic_soundcard_ = mic;
    speaker_soundcard_ = speaker;
    pthread_mutex_unlock(&mutex_);
}

void GlobalSettings::getCards(std::string& mic, std::string& speaker) const
{
    pthread_mutex_lock(&mutex_);
    mic = mic_soundcard_;
    speaker = speaker_soundcard_;
    pthread_mutex_unlock(&mutex_);
}
