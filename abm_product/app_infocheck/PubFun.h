/*
������������
���żƷ��з���
���ʤ
2010.5.25
*/
#ifndef _PUBFUN_H_201005_
#define _PUBFUN_H_201005_
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <regex.h>
#include <dirent.h>
#include <sys/mman.h>//mmap
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
using namespace std;
namespace nsFuntion
{
//�ָ��ַ���
    std::vector<std::string> StringSplit(const std::string& _soureString, const std::string& _dilimiter);

//Ч��Unix·���ķ���Ȩ��
    bool AccessOfPath(const std::string& _pathName);

//���ַ������滻ĳ���ַ�
//����ֵΪ�滻�ĸ���
    int replaceChar(std::string& _sourceString, const char _oldChar, const char _replaceChar);

//������ʽ��ƥ�䷵���棬������߲�ƥ�䷵�ؼ�
    bool regexExpression(const std::string& _sourceString, const std::string& _expression);

//�ַ����Ƿ�Ϊʮ����������
    bool isNumber(const std::string& _sourceString);

    std::string itoa(const int& _number);

//�ļ�ӳ��洢
    int MapFile(const std::string &,std::string&);
}
#endif
