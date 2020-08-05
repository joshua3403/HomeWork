#include "Log.h"

int main()
{
	SYSLOGCLASS* Log = SYSLOGCLASS::GetInstance();

	Log->SYSLOG_DIRECTORY(L"LogFile");
	WCHAR temp[64] = L"Battle";
	WCHAR temp1[32] = L"%d";
	Log->LOG(temp, LOG_LEVEL::e_DEBUG, temp1, 10);
}