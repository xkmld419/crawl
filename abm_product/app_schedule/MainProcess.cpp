
#include "StdAfx.h"
/***********************************************
 *	Function Name	:TrimStr
 *	Description	:����ַ���ǰ��ո��Ʊ�����س�������
 *	Input Param	:Ŀ���ַ���
 *	Returns		:�޸ĺ��ַ���
 *	complete	:
 ***********************************************/
char* TrimStr( char *sp )
{
	char sDest[150];
	char *pStr;
	int i = 0;

	if ( sp == NULL )
		return NULL;

	pStr = sp;
	while ( *pStr == ' ' || *pStr == '\t' )  pStr ++;
	strncpy( sDest, pStr,150 );

	i = strlen( sDest ) - 1;
	while((i>=0)&&(sDest[i] == ' ' || sDest[i] == '\t' || sDest[i] == '\r' || sDest[i] == '\n') )
	{
		sDest[i] = '\0';
		i -- ;
	}

	strcpy( sp, sDest );

	return ( sp );
}
/***********************************************
 *	Function Name	:ProcIsExist
 *	Description	:��鱾�����Ƿ��Ѿ�����
 *	Input Param	:��main����ͬ
 *	Returns		:true(����) or false(������)
 *	complete	:
 ***********************************************/
bool isProcExist(int argc, char *argv[])
{
	int i;
	int iNums = 0;
	FILE *fp = 0;
	char *lv_chPoint = 0;
	char *pchLoginName = 0;
	char chScanBuff[512];
	char chCommandLine[512];
	char chTemp[512];
	memset(chScanBuff, 0, sizeof(chScanBuff));
	memset(chCommandLine, 0, sizeof(chCommandLine));
	memset(chTemp, 0, sizeof(chTemp));

	if ((pchLoginName = getenv("LOGNAME")) == 0)
	{
		//printf("Admserver:Not get the environment \"LOGNAME\" in ProcIsExist!\n");
		Log::log(0,"����ʱ�澯����ȡ��������LOGNAMEʧ��\n");
		return false;
	}
	/*
	lv_chPoint = argv[0];
	for (i = 0; *lv_chPoint; lv_chPoint++, i++)
	{
		if (*lv_chPoint == '/')
		{
			i = -1;
			continue;
		}
		chCommandLine[i] = *lv_chPoint;
	}
	*/
	lv_chPoint = strrchr(argv[0],'/');
	if(lv_chPoint == NULL)
	{
		strncpy(chCommandLine,argv[0],512);
	}
	else
	{
		strncpy(chCommandLine,lv_chPoint+1,512);
	}
	chCommandLine[511] = '\0';

	TrimStr(chCommandLine);
	//sprintf(chScanBuff, "ps -fu %s | cut -c 40-", pchLoginName);
	sprintf(chScanBuff, "ps -fu %s | grep '%s'|grep -v grep", pchLoginName,chCommandLine);
	if ((fp = popen(chScanBuff, "r")) == 0)
	{
		return false;
	}
	while (fgets(chScanBuff, sizeof(chScanBuff), fp) != 0)
	{
		/*
		memset(chTemp, 0, sizeof(chTemp));
		strcpy(chTemp, chScanBuff);
		lv_chPoint = strchr(chScanBuff, ':');
		if (lv_chPoint == 0)
		{
			memset(chScanBuff, 0, sizeof(chScanBuff));
			continue;
		}
		lv_chPoint += 3;
		memset(chTemp, 0, sizeof(chTemp));
		strcpy(chTemp, lv_chPoint);
		TrimStr(chTemp);
		lv_chPoint = chTemp;

		memset(chScanBuff, 0, sizeof(chScanBuff));
		*/
		lv_chPoint = strstr(chScanBuff, argv[0]);
		
		if (lv_chPoint == 0)
		{
			memset(chScanBuff, 0, sizeof(chScanBuff));
			continue;
		}
		
		for (i = 0; (*lv_chPoint) && (*lv_chPoint != ' '); lv_chPoint++, i++)
		{
			if (*lv_chPoint == '/')
			{
				i = -1;
				continue;
			}
			chScanBuff[i] = *lv_chPoint;
		}
		chScanBuff[i] = 0;
		lv_chPoint = strchr(chTemp, ' ');

		TrimStr(chScanBuff);
		TrimStr(chCommandLine);
		if (strcmp(chScanBuff, chCommandLine) == 0)
		{
			iNums++;
		}
		memset(chScanBuff, 0, sizeof(chScanBuff));
	}
	pclose(fp);
	if (iNums > 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main(int argc,char**argv)
{
	string errMsg;
	try
	{
		if(isProcExist(argc,argv)){
			Log::log(0,"[main]��⵽%s�Ѿ�������!\n",argv[0]);
			return 0;
		}
		Schedule sch;
		sch.runEx();
	}
	catch (string ex) {
		errMsg.append("string Error:").append(ex);
	}
	catch (TOCIException &ex) {
		errMsg.append("TOCIException Error:").append(ex.getErrMsg()).append(ex.getErrSrc());
	}
	catch (TException &ex) {
		errMsg.append("TException Error:").append(ex.GetErrMsg());
	}
	catch (exception &ex) {
		errMsg.append("exception Error:").append(ex.what());
	}
	catch ( ... ) {
		errMsg="δ֪����";
	}
	if(errMsg.length()>0)
		Log::log (0, "�������ش���:%s,�����˳�!", errMsg.c_str());
}

