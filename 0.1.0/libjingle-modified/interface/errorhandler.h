#ifndef __errorhandler__
#define __errorhandler__

#include <string>
#include <map>

enum Errors
{
	eInvalidRequest = 1000,
	eCannotFindContact,
	eCannotStartFileTransfer //WhileSendingOrReceiing
};

class ErrorHandler {

public:
	static ErrorHandler* Instance();
	static void Release();

	const std::string ErrToStr(int err);
private:
	ErrorHandler();
	
	ErrorHandler(const ErrorHandler& rhs);
	ErrorHandler& operator=(const ErrorHandler& rhs);
	
	virtual ~ErrorHandler();
	
	static ErrorHandler* ms_instance;
	
	std::map<int, std::string> m_errorCodes;

};

#define ERR2STR(errId) ErrorHandler::Instance()->ErrToStr(errId)
#endif // __errorhandler__
