#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <pthread.h>
#include <vector>
#include <string>

class GlobalSettings {

	static GlobalSettings* ms_instance;

public:
	static GlobalSettings* Instance();
	static void Release();

    void setCards(const std::string& mic, const std::string& speaker);
    void getCards(std::string& mic, std::string& speaker) const;
private:
    std::string mic_soundcard_;
    std::string speaker_soundcard_;
    
	GlobalSettings();
	~GlobalSettings();
    
    mutable pthread_mutex_t mutex_;

};

#endif // GLOBALSETTINGS_H
