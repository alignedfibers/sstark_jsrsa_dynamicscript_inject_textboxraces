/*  Copyright 2008 Jozsef Bekes

    This file is part of gtalX.

    gtalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gtalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gtalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBJINGLEIF_H_
#define LIBJINGLEIF_H_

#include <string>
#include <vector>

class CJingleCallback
{
public:
	enum eContactCapabilities
	{
		chatReady = 0x1,
		callReady = 0x2,
		fileshareReady = 0x4
	};
	enum Statuses
	{
		eConnecting,
		eLoggingIn,
		eLoggedIn,
		eLoggedOut,
		eCalling,
		eCallAnswered,
		eCallNotAnswered,
		eCallInProgress,
		eOtherSideHungUp
	};
    virtual void OnNewStatus(int statusId, const std::string& status){}
	virtual void OnContactStatus(const std::string& contactEmail,
                                 const std::string& contactName,
								 const std::string& status,
								 int capabilities){}
	virtual void OnError(int errId, const std::string& error){}
    virtual void OnIncomingChat(const std::string& iconset,
                                const std::string& contact,
                                const std::string& text){}
	virtual void OnIncomingCall(const std::string& contact){}
	virtual void OnCallDestroyed(){}

	virtual void OnFileReceiveStart(const std::string& sender,
									int dataSize,
									const std::string& description,
									std::string& rootDir){}

	virtual void OnFileTransferProgress(int total, int progress, std::string result){}
};

class CLibjingleIf
{
public:

    static CLibjingleIf* create(CJingleCallback* cb,
								const std::string& gtalkServer,
								int gtalkPort,
								const std::string& ringWav,
								const std::string& callWav,
								const std::string& chatDingWav,
								bool logVerbose,
							    const std::string& proxyHost,
								int proxyPort);

    virtual void connect(const std::string& usn, const std::string& pwd) = 0;
    virtual void call(const std::string& contact) = 0;
    virtual void sendchat(const std::string& contact, const std::string& text) = 0;
	virtual void dingchat() = 0;
    virtual void hangup() = 0;

    virtual void answerIncoming() = 0;
    virtual void rejectIncoming() = 0;

	virtual void sendfile(const std::string& contact, const std::string& path) = 0;
	virtual void cancelFileTransfer() = 0;
    
    virtual void getSoundcardList(std::vector<std::string>& soundCards) = 0;
    virtual void selectSoundCards(const std::string& mic, const std::string& speaker) = 0;
};

#endif
