/***********************************************************
 *
 *   Copyright (c) 1997-2010  Asiainfo-Linkage Technology Co.,Ltd.
 *
 *   All rights reserved.
 *
 *   �ļ����ƣ�CPacketComm.cpp
 *
 *   ��Ҫ������Э������������
 *
 *   ��ǰ�汾��1.0
 *
 *   ����/�޸��ߣ��ָ���  lingy@asiainfo-linkage.com
 *
 *   ��д���ڣ�2010/10/30
 *
 ************************************************************/

#include "CPacketComm.h"

#ifdef  DEF_LINUX
#define MSG_PARSE_UTF_ "UTF-8"
#define MSG_PARSE_GBK_ "gb2312"
#endif
#ifdef  DEF_HP
#define MSG_PARSE_UTF_ "UTF8"
#define MSG_PARSE_GBK_ "hp15CN"
#endif
#ifdef  DEF_AIX
#define MSG_PARSE_UTF_ "UTF-8"
#define MSG_PARSE_GBK_ "GB18030"
#endif

/****************************************************************************
 * CPacketUtil ������
 ****************************************************************************/

CPacketUtil::CPacketUtil()
{
}
CPacketUtil::~CPacketUtil()
{
}

/*
 * -----------------------------------------------------
 * ����	           ��һ�ֽ�	                                �ڶ��ֽ�
 * -----------------------------------------------------
 * GB2312	0xB0-0xF7(176-247) 	0xA0-0xFE(160-254)
 * GBK	    0x81-0xFE(129-254)	0x40-0xFE(64-254)
 * Big5	    0x81-0xFE(129-255)	0x40-0x7E(64-126)
 *                              0xA1��0xFE(161-254)
 * -----------------------------------------------------
 **/
/******************************************************************************************************************
 *       ��������:     ChineseCharCount
 *       ��������:     ���㺺���ַ�����
 *       �������:     sMsg	       ��Ϣ��ַ
 *                    iFirst   ��ʼ��ַ
 *                    iLast    ��ֹ��ַ
 *       �������:     ��
 *       �� �� ֵ:      ���㺺���ַ�����
 ******************************************************************************************************************/
int CPacketUtil::ChineseCharCount(const char *sMsg, int iFirst, int iLast)
{
	int iCount = 0;
	unsigned char uiValue = 0;

	for (int i = iFirst; i <= iLast; i++)
	{
		uiValue = (unsigned char) sMsg[i];

		if (uiValue >= 64 && uiValue <= 254)
		{
			iCount++;
		}
	}
	return iCount;
}

/******************************************************************************************************************
 *       ��������:     splitMessage
 *       ��������:     ��Ϣ���
 *       �������:     sMsgContent	  ����ֵ���Ϣ����
 *                    iMsgLen       ����ֵ���Ϣ����
 *       �������:     vOutMsg       ��ֺ����Ϣ
 *       �� �� ֵ:      0:�ɹ�; ��0:ʧ��
 ******************************************************************************************************************/
int CPacketUtil::splitMessage(const char *sMsgContent, int iMsgLen, int iSingleMsgLen, vector<string> &vOutMsg)
{
	int i, iCurPos = 0, iPrePos = 0, iOffset = 5, iOffsetCopy, iCopyLen = 0;
	int iSumLen = 0; //ԭʼ�ܳ���
	int iPerLen = iSingleMsgLen - iOffset; //ÿ�����ų���
	int iSmNo = 0, iSmCounts = 0;
	unsigned char uiChar1, uiChar2, uiChar3;
	char sSmText[150];

	memset(sSmText, 0, sizeof(sSmText));

	vOutMsg.clear();

	if (sMsgContent == NULL)
	{
		return (-1);
	}

	if (iMsgLen <= iSingleMsgLen)
	{
		iSumLen = strlen(sMsgContent);
		if (iSumLen <= iSingleMsgLen)
		{
			vOutMsg.push_back(string(sMsgContent));
			return 0;
		}
	}
	else
	{
		iSumLen = iMsgLen;
	}

	iCurPos = -1; //��ǰλ��
	iPrePos = -1; //ǰһ��λ��
	iSmNo = 0; //��ǰ��������
	iSmCounts = static_cast<int> (iSumLen / iPerLen) + 1; //������

	try
	{
		while (iSmNo < iSmCounts)
		{
			iSmNo++;
			sprintf(sSmText, "(%d/%d)", iSmNo, iSmCounts);

			iOffsetCopy = strlen(sSmText);

			iPrePos = iCurPos;
			iCurPos += iPerLen;

			if (iCurPos < iSumLen - 1) //���ʣ��Ҫ�������Ϣ���ȴ��ڹ涨��һ����Ϣ�ĳ���
			{
				uiChar2 = (unsigned char) sMsgContent[iCurPos];

				if (uiChar2 >= 64 && uiChar2 <= 254) //�Ǻ����ַ�
				{
					int iCalcCount = ChineseCharCount(sMsgContent, iPrePos + 1, iCurPos);

					if ((iCalcCount % 2) != 0)
					{
						//�ǵ������������ַ�,˵���Ǹ����ֵĵ�һ���ֽ�,��λ����һλ
						iCurPos--;
					}
				}
			}
			else
			{
				//ʣ���ֽ�ȫ������,�ǲ�ֺ�����һ������
				iCurPos = iSumLen - 1;
			}
			iCopyLen = iCurPos - iPrePos;
			memcpy(sSmText + iOffsetCopy, (sMsgContent + iPrePos + 1), iCopyLen);
			sSmText[iOffsetCopy + iCopyLen] = '\0';
			vOutMsg.push_back(string(sSmText));
		}
	}
	catch(exception &ex)
	{
		printf("<< splitMessage >> exception = %s \n", ex.what());
		return 1;
	}
	catch(...)
	{
		printf("<< splitMessage >>  UNKNOWN ERROR. \n");
		return 2;
	}
	return 0;
}

/*
 * ����:�ֽ��¼(�ָ���)
 * in_iStyle == 1 �ķ��
 *   123|456|789|
 *    ��ÿ���ֶζ���ͬ���ķָ���
 * in_iStyle == 2 �ķ��
 *    123|456|789\n
 *    ���һ���ֶεķָ�����ͬ,�ǻ��з�\n
 * */

/******************************************************************************************************************
 *       ��������:           splitRecord
 *       ��������: �ֽ��¼(���ָ���)
 * in_iStyle == 1 �ķ��
 *   123|456|789|
 *    ��ÿ���ֶζ���ͬ���ķָ���
 * in_iStyle == 2 �ķ��
 *    123|456|789\n
 *    ���һ���ֶεķָ�����ͬ,�ǻ��з�\n
 *
 *       �������:           in_sOneEvent    ��¼
 *                           in_sSep    �ָ���
 *                           in_iStyle       �ָ�������(1��2)
 *       �������:           out_vFieldList    ��¼���
 *       �� �� ֵ��                              0:�ɹ�, ��0:ʧ��
 ******************************************************************************************************************/
int CPacketUtil::splitRecord(string &in_sOneEvent, const string &in_sSep, int in_iStyle, vector<string> &out_vFieldList)
{
	int iPos = 0, iCount = 0;

	out_vFieldList.clear();

	if (in_sOneEvent.length() == 0)
	{
		return -1;
	}

	try
	{
		iPos = in_sOneEvent.find(in_sSep, 0);
		while (iPos >= 0)
		{
			out_vFieldList.push_back(in_sOneEvent.substr(0, iPos));
			in_sOneEvent.erase(0, iPos + 1);
			iCount++;

			iPos = in_sOneEvent.find(in_sSep, 0);
		}
		if (in_iStyle == EN_EVENT_RETURN)
		{
			//ȡ���һ���ֶ�
			iPos = in_sOneEvent.find("\n", 0);
			if (iPos > 0)
			{
				out_vFieldList.push_back(in_sOneEvent.substr(0, iPos));
			}
			else
			{
				out_vFieldList.push_back(in_sOneEvent);
			}
		}
	}
	catch(exception &ex)
	{
		printf("%s(%d), exception = %s\n", __FILE__, __LINE__, ex.what());
		return -2;
	}
	catch(...)
	{
		printf("%s(%d), other error!\n", __FILE__, __LINE__);
		return -3;
	}
	return 0;
}

/******************************************************************************************************************
 *       ��������:           calcbuf2
 *       ��������:           ���������ֽڵĻ�����
 *       �������:           buf       ��������ַ
 *                           bTrans    �Ƿ�����ֽ���ת��(true: ת��; false: ��ת��)
 *       �������:           ��
 *       �� �� ֵ��                              ������
 ******************************************************************************************************************/
int CPacketUtil::calcbuf2(char *buf, bool bTrans)
{
	int iret;
	char s_buf[4];

	if (bTrans)
	{
		s_buf[3] = 0x0;
		s_buf[2] = 0x0;
		s_buf[1] = buf[0];
		s_buf[0] = buf[1];
	}
	else
	{
		s_buf[0] = 0x0;
		s_buf[1] = 0x0;
		s_buf[2] = buf[0];
		s_buf[3] = buf[1];
	}

	memcpy(&iret, s_buf, 4);

	return iret;

}

/******************************************************************************************************************
 *       ��������:           calcbuf3
 *       ��������:           ���������ֽڵĻ�����
 *       �������:           buf       ��������ַ
 *                           bTrans    �Ƿ�����ֽ���ת��(true: ת��; false: ��ת��)
 *       �������:           ��
 *       �� �� ֵ��                              ������
 ******************************************************************************************************************/
int CPacketUtil::calcbuf3(char *buf, bool bTrans)
{
	int iret;
	char s_buf[4];

	if (bTrans)
	{
		s_buf[3] = 0x0;
		s_buf[2] = buf[0];
		s_buf[1] = buf[1];
		s_buf[0] = buf[2];
	}
	else
	{
		s_buf[0] = 0x0;
		s_buf[1] = buf[0];
		s_buf[2] = buf[1];
		s_buf[3] = buf[2];
	}

	memcpy(&iret, s_buf, 4);

	return iret;
}

/******************************************************************************************************************
 *       ��������:           calcbuf4
 *       ��������:           �����ĸ��ֽڵĻ�����
 *       �������:           buf       ��������ַ
 *                           bTrans    �Ƿ�����ֽ���ת��(true: ת��; false: ��ת��)
 *       �������:           ��
 *       �� �� ֵ��                              ������
 ******************************************************************************************************************/
int CPacketUtil::calcbuf4(char *buf, bool bTrans)
{
	int iret;
	char s_buf[4];

	if (bTrans)
	{
		s_buf[3] = buf[0];
		s_buf[2] = buf[1];
		s_buf[1] = buf[2];
		s_buf[0] = buf[3];
		memcpy(&iret, s_buf, 4);
	}
	else
	{
		memcpy(&iret, buf, 4);
	}

	return iret;

}

void CPacketUtil::Int32toArray(char *buf, uint32 uiValue)
{
	*(buf) = (uchar) (uiValue >> 24);
	*(buf + 1) = (uchar) ((uiValue >> 16) & 255);
	*(buf + 2) = (uchar) ((uiValue >> 8) & 255);
	*(buf + 3) = (uchar) (uiValue & 255);
}

uint32 CPacketUtil::ArraytoInt32(char *buf)
{
	return (uint32) (((uchar) buf[0] << 24) + ((uint32) buf[1] << 16) + ((uint32) buf[2] << 8) + (uchar) buf[3]);
}

void CPacketUtil::Int16toArray(char *buf, uint16 uiValue)
{
	*(buf) = (uchar) (uiValue >> 8);
	*(buf + 1) = (uchar) (uiValue & 255);
}

uint16 CPacketUtil::ArraytoInt16(char *buf)
{
	return (uint32) (((uchar) buf[0] << 8) + (uchar) buf[1]);
}

/******************************************************************************************************************
 *       ��������:           convert
 *       ��������:           ����ת��
 *       �������:           sFromCharset  ԭ����
 *                           sToCharset   Ŀ�����
 *                           sInBuf       ���뻺������ַ
 *                           iInLen       ���뻺��������
 *       �������                                sOutBuf       �����������ַ
 *                           iOutLen      �������������
 *       �� �� ֵ��                              ������                0: �ɹ�, ��0: ʧ��
 ******************************************************************************************************************/
int CPacketUtil::convert(const char *sFromCharset,const char *sToCharset,const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	iconv_t cd;
	int iRet;
	int iTmpLength = 200;
	char out[MAX_CONVBUF_LEN];
	char *inbuf = (char *) sInBuf;
	char *outbuf = out;
	size_t inlen = iInLen;
	size_t outlen = MAX_CONVBUF_LEN;

	if (inlen <= 0)
		inlen = strlen(sInBuf);

	if (inlen == 0)
	{
		iOutLen = 0;
		return 0;
	}

	//�򿪾��
	cd = iconv_open(sToCharset, sFromCharset);
	if (cd == (iconv_t) - 1)
	{
		printf("iconv_open(\"%s\", \"%s\") failed[%s]\n", sToCharset, sFromCharset, strerror(errno));
		return (-1);
	}

	memset(out, 0, sizeof(out));

	iRet = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);

	if (iRet == (size_t) -1)
	{
		if (errno == E2BIG)
		{
			printf("Skipping input line with length greater than column width of %d characters:\n%s[%s]\n", 255, sInBuf,
					strerror(errno));
		}
		else
		{
			printf("iconv() %s failed[%s]\n", sInBuf, strerror(errno));
		}

		//�رվ��
		iconv_close(cd);

		return (-2);
	}

	//ת����ĳ���
	iOutLen = MAX_CONVBUF_LEN - outlen;

	memcpy(sOutBuf, out, iOutLen);
	sOutBuf[iOutLen] = '\0';

	//�رվ��
	iconv_close(cd);

	return 0;

}

//UTF8��תΪGB2312��
int CPacketUtil::Utf8ToGB2312(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("UTF-8", "GB2312", sInBuf, iInLen, sOutBuf, iOutLen);
}
//GB2312��תΪUTF8��
int CPacketUtil::GB2312ToUtf8(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("GB2312", "UTF-8", sInBuf, iInLen, sOutBuf, iOutLen);
}

//UTF8��תΪGBK��
int CPacketUtil::Utf8ToGBK(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("UTF-8", "GBK", sInBuf, iInLen, sOutBuf, iOutLen);
}
//GBK��תΪUTF8��
int CPacketUtil::GBKToUtf8(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("GBK", "UTF-8", sInBuf, iInLen, sOutBuf, iOutLen);
}

//UCS2��תΪGB2312��
int CPacketUtil::Ucs2ToGB2312(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("UCS-2", "GB2312", sInBuf, iInLen, sOutBuf, iOutLen);
}
//GB2312��תΪUCS2��
int CPacketUtil::GB2312ToUcs2(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("GB2312", "UCS-2", sInBuf, iInLen, sOutBuf, iOutLen);
}

//UCS2��תΪGBK��
int CPacketUtil::Ucs2ToGBK(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("UCS-2", "GBK", sInBuf, iInLen, sOutBuf, iOutLen);
}
//GBK��תΪUCS2��
int CPacketUtil::GBKToUcs2(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert("GBK", "UCS-2", sInBuf, iInLen, sOutBuf, iOutLen);
}

//ֻ����,��ת��
int CPacketUtil::copy(const char *sInBuf, char *sOutBuf, int &iOutLen)
{
	int iInLen = strlen(sInBuf);
	return copy(sInBuf, iInLen, sOutBuf, iOutLen);
}

//ֻ����,��ת��
int CPacketUtil::copy(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	if (iInLen <= 0)
	{
		iOutLen = 0;
	}
	else
	{
		strncpy(sOutBuf, sInBuf, iInLen);
		iOutLen = iInLen;
	}
	return iOutLen;
}

//����ת��
int CPacketUtil::convert(int iConvertSchema,const char *sInBuf, char *sOutBuf, int &iOutLen)
{
	int iInLen = strlen(sInBuf);
	return convert(iConvertSchema, sInBuf, iInLen, sOutBuf, iOutLen);
}

//����ת��
int CPacketUtil::convert(int iConvertSchema,const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	switch (iConvertSchema)
	{
		case EN_NO_CONVERT://��ת��
			return copy(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_Utf8ToGB2312:
			return Utf8ToGB2312(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_GB2312ToUtf8:
			return GB2312ToUtf8(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_Utf8ToGBK:
			return Utf8ToGBK(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_GBKToUtf8:
			return GBKToUtf8(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_Ucs2ToGB2312:
			return Ucs2ToGB2312(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_GB2312ToUcs2:
			return GB2312ToUcs2(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_Ucs2ToGBK:
			return Ucs2ToGBK(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		case EN_GBKToUcs2:
			return GBKToUcs2(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
		default://��ת��
			return copy(sInBuf, iInLen, sOutBuf, iOutLen);
			break;
	}
}

// ϵͳ����תUTF8
int CPacketUtil::Sys2Utf(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert(MSG_PARSE_GBK_, MSG_PARSE_UTF_, sInBuf, iInLen, sOutBuf, iOutLen);
}

// UTF8תϵͳ����
int CPacketUtil::Utf2Sys(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen)
{
	return convert(MSG_PARSE_UTF_, MSG_PARSE_GBK_, sInBuf, iInLen, sOutBuf, iOutLen);
}

/*
 int main(void)
 {
 char *in_utf8 = "正在安装";
 char *in_gb2312 = "���ڰ�װ";
 char out[MAX_CONVBUF_LEN];
 int ioutlen = MAX_CONVBUF_LEN;
 int iinlen = strlen(in_gb2312);
 string ss("");
 string ss1("");
 int ret;

 CPacketUtil oPacketUtil;

 memset(out, 0, sizeof(out));

 ret = oPacketUtil.gb2312_to_utf8(in_gb2312, iinlen, out, ioutlen);
 ss = out;
 cout << "out=[" << out << "]" << endl;
 cout << "ss1=[" << ss << "]" << endl;

 memset(out, 0, sizeof(out));
 ioutlen = MAX_CONVBUF_LEN;
 ret = oPacketUtil.utf8_to_gb2312((char *) ss.c_str(), ss.length(), out, ioutlen);
 ss1 = out;

 cout << "out=[" << out << "]" << endl;
 cout << "ss2=[" << ss1 << "]" << endl;

 iinlen = strlen(in_utf8);
 memset(out, 0, sizeof(out));
 ioutlen = MAX_CONVBUF_LEN;
 ret = oPacketUtil.utf8_to_gb2312(in_utf8, iinlen, out, ioutlen);
 ss = out;
 cout << "out=[" << out << "]" << endl;
 cout << "ss3=[" << ss << "]" << endl;

 memset(out, 0, sizeof(out));
 ioutlen = MAX_CONVBUF_LEN;
 ret = oPacketUtil.gb2312_to_utf8((char *) ss.c_str(), ss.length(), out, ioutlen);
 ss1 = out;

 cout << "out=[" << out << "]" << endl;
 cout << "ss4=[" << ss1 << "]" << endl;

 return 0;
 }*/
