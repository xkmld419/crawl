/*
*	version js at 2010-06-10
*
*/
#ifndef READCFG_H_HEADER_INCLUDED
#define READCFG_H_HEADER_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

#include "ABMException.h"

#define T_COLUMN_MAX    32
#define C_CONTEXT_MAX   64 

#define TAG_NAME_MAX    32
#define FILE_NAME_MAX    32

typedef struct {
	char m_sRow[T_COLUMN_MAX][C_CONTEXT_MAX];
} _RowText;

class ReadCfg
{    
public:
	
	ReadCfg();
	
	virtual ~ReadCfg();
	
	//�õ�����
	//char * getTable();
	
	//�õ�����
	int getRows();
	
	//�õ�����
	int getColumns();
	
    //��������ȡֵ�ģ�������һ����һ��ʼ��
	//�õ�ĳ��ĳ�е�ֵ��������false
	bool getValue(ABMException &oExp, char *sValue,int iColumn,int iLine, int iMaxLen=C_CONTEXT_MAX);
	
	//�õ�ĳ��ĳ�е�ֵ��������false
	bool getValue(ABMException &oExp, char *sValue,char *sColName,int iLine, int iMaxLen=C_CONTEXT_MAX);
		
	//�õ�ĳ��ĳ�е�ֵ��������false��ֵΪ�յĻ�valueֵ����0
	bool getValue(ABMException &oExp, long &lValue,int iColumn,int iLine);
	
	//�õ�ĳ��ĳ�е�ֵ��������false��ֵΪ�յĻ�valueֵ����0
	bool getValue(ABMException &oExp, long &lValue,char *sColName,int iLine);
	
	//�õ�ĳ��ĳ�е�ֵ��������false��ֵΪ�յĻ�valueֵ����0
	bool getValue(ABMException &oExp, int &iValue,int iColumn,int iLine);
	
	//�õ�ĳ��ĳ�е�ֵ��������false��ֵΪ�յĻ�valueֵ����0
	bool getValue(ABMException &oExp, int &iValue,char *sColName,int iLine);
	
	//��ĳ���ļ���ȡĳ����ʶ���ָ���Ϊ|�����ݵ�
	int read(ABMException &oExp, const char *pf, char const *pTag, char sSign='|');
	
private:
	int _read(ABMException &oExp, char *pSrc, char row[][C_CONTEXT_MAX], char sign);
	
private:
	
	vector<_RowText> m_vTab;
    map<string, int> m_mIndex;

    char m_sTagName[TAG_NAME_MAX];
    char m_sFileName[FILE_NAME_MAX];
	
	vector<_RowText>::iterator m_itr;
	bool m_bAttach;
		
};
#endif


