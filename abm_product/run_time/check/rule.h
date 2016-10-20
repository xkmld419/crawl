/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�rule.h 
* ժ    Ҫ���ļ�����������У�����õ����к���
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��11��11��
*/

#ifndef __RULE_H_
#define __RULE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TimesTenAccess.h"
#include "common.h"
#include "hssstack.h"


struct URule {
	int 		ruleId;
	int 		ruleType;
	int 		ruleFlag;
	char		ruleValues[128];

	URule() {
		memset(ruleValues, 0x00, sizeof(ruleValues));
	}
};

struct Rule {
	int 		checkId;
	int 		type;
	char		rule[32];
	int 		result;
	Rule() {
		memset(rule, 0x00, sizeof(rule));
	}
};

class RuleData {
public:
	static RuleData* getInstance() {
		if (!m_poInstance) {
			m_poInstance = new RuleData();

			if (!m_poInstance) {
				exit(-1);
			}
		}

		return m_poInstance;
	}

	int getRule(const int& idx, Rule* &rule) const;

	int getURule(const int& idx, URule* &rule, int& cout) const;

	int init();
protected:
	RuleData() {
		m_poURule = NULL;
		m_poRule  = NULL;

		m_uiURuleSize = 0;
		m_uiRuleSize  = 0;
	}

	virtual ~RuleData() {
		if (m_poURule) {
			delete m_poURule;
			m_poURule = NULL;
		}
		if (m_poURule) {
			delete m_poURule;
			m_poURule = NULL;
		}
		m_uiURuleSize = 0;
		m_uiRuleSize  = 0;
	}

	int loadURule(TimesTenCMD * cursor);

	int loadRule(TimesTenCMD * cursor);
public:

	URule * m_poURule;
	Rule  * m_poRule;
	unsigned int m_uiURuleSize;
	unsigned int m_uiRuleSize;

	static RuleData* m_poInstance;
};

class RuleCheck {
public:
	static RuleCheck* getInstance() {
		if (!m_poInstance) {
			m_poInstance = new RuleCheck();

			if (!m_poInstance) {
				exit(-1);
			}
			
			if (RuleData::getInstance()->init()) {
				exit(-2);
			}
		}

		return m_poInstance;
	}

	/*
	* �������ܣ�У�����ں���
	* �����������У�������
	*					idx �Ǳ�У��, argc ��ָ����У��Ĳ�������, argv�ǲ���ֵ
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int doCheck(int idx, int argc, Param argv[]);

	
protected:
	RuleCheck():stack(64) {

	}

	virtual ~RuleCheck() {

	}

	/*
	* �������ܣ���һ���򵥵�У��ʽ���м���
	* �����������У�������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int URuleheck(int idx, int argc, Param argv[]);

	
	int RuleCheck::doCompare(const Param & argv, const URule * uRule);
protected:
	HSS_Stack<char>	stack;

	static RuleCheck* m_poInstance;
};

#endif

