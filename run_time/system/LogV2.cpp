#include "LogV2.h"
#include "Date.h"
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int LogV2::m_iLogLevel = 0;

char LogV2::m_sLogName[256] = {0};

LogV2::LogV2()
{
}

LogV2::~LogV2()
{
	
}

void LogV2::setLogLevel(const int m_iLevel)
{
		m_iLogLevel = m_iLevel;
}

void LogV2::setFileName(const char *sName)
{
	char *p = getenv("ABM_LOG_LEVEL");
	if (p != NULL)
		m_iLogLevel = atoi(p);
	strncpy(m_sLogName, sName, sizeof(m_sLogName)-1);	
}

void LogV2::log(const char *sFile, const int iLine, int iLogLevel, char const * sFormt, ...)
{
	char sTemp[8192];
	char sText[8192];

	if (iLogLevel > m_iLogLevel) 
		return;

	FILE *fp = fopen(m_sLogName, "a+");
	if (fp == NULL)
		return;

	Date dt;
	snprintf(sTemp, sizeof(sTemp), "[%s][LogLevel_%d]: %s at %s:%d\n", dt.toString("yyyy-mm-dd hh:mi:ss"), \
		iLogLevel, sFormt, sFile, iLine);

	va_list ap;
	va_start(ap, sFormt);
	vsnprintf(sText, sizeof(sText), sTemp, ap);
	va_end(ap);
	
	fwrite(sText, strlen(sText), 1, fp);
	
	while (fclose(fp) != 0) {
		if (errno == EINTR)
			continue;
		else
			break;
	}

	return;
}

void LogV2::logExcep(ABMException &oExp)
{
	FILE *fp = fopen(m_sLogName, "a+");
	if (fp != NULL)	
		oExp.printErr(NULL, fp);
	return;	
}


