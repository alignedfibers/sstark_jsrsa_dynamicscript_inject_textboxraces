#include "talk/xmpp/xmppclientsettings.h"
#include "talk/login/xmppthread.h"
#include "talk/login/xmppauth.h"

#include "errorhandler.h"

ErrorHandler* ErrorHandler::ms_instance = 0;

ErrorHandler::ErrorHandler()
{
	m_errorCodes[buzz::XmppEngine::ERROR_NONE] = "";
	m_errorCodes[buzz::XmppEngine::ERROR_XML] = "Malformed XML or encoding error";
	m_errorCodes[buzz::XmppEngine::ERROR_STREAM] = "XMPP stream error";
	m_errorCodes[buzz::XmppEngine::ERROR_VERSION] = "XMPP version error";
	m_errorCodes[buzz::XmppEngine::ERROR_UNAUTHORIZED] = "Wrong username or password";
	m_errorCodes[buzz::XmppEngine::ERROR_TLS] = "TLS could not be negotiated";
	m_errorCodes[buzz::XmppEngine::ERROR_AUTH] = "Authentication could not be negotiated";
	m_errorCodes[buzz::XmppEngine::ERROR_BIND] = "Resource or session binding could not be negotiated";
	m_errorCodes[buzz::XmppEngine::ERROR_CONNECTION_CLOSED] = "Connection closed by output handler";
	m_errorCodes[buzz::XmppEngine::ERROR_DOCUMENT_CLOSED] = "Closed by </stream:stream>";
	m_errorCodes[buzz::XmppEngine::ERROR_SOCKET] = "Socket error";
	
	m_errorCodes[eInvalidRequest] = "Invalid request";
	m_errorCodes[eCannotFindContact] = "Cannot find contact";
	m_errorCodes[eCannotStartFileTransfer] = "Cannot start file transfer if already sending or receiving";
}

ErrorHandler::~ErrorHandler()
{
	ms_instance = 0;
}

ErrorHandler* ErrorHandler::Instance()
{
	if(ms_instance == 0){
		ms_instance = new ErrorHandler();
	}
	return ms_instance;
}

void ErrorHandler::Release()
{
	delete ms_instance;	
}


const std::string ErrorHandler::ErrToStr(int err)
{
	if (err == 0)
	    return "";
		
	std::string ret = m_errorCodes[err];
	if (ret == "")
	    return "Unknown error";
	else
	    return ret;

}
