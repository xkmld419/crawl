#ifndef BASE_FUNCTION_H_
#define BASE_FUNCTION_H_
#include <vector>
#include <string>
using namespace std;

#ifdef erase
#undef erase
#endif

#ifdef move
#undef move
#endif
/*
#ifdef ID(_n_)
#undef ID(_n_)
#endif
*/

// ����޸�һ�£���ΪȨ�޿���
//int FilterChoiceOrder(char *SecStr,char *OptionStr);

void AllTrimN(char sBuf[]);
char *trimString( char *sp );
void AllTrimNew(char sBuf[]);

//ȥ���ַ������ҿո�
void trimN( string& str ) ;

char toUpper(const char ch);
char toLower(const char ch);
bool Chang2Upper(char *sSrc);
bool Chang2Lower(char *sSrc);

bool getTagStringN(char *sTagList,vector <string> &vec_tag,char sep);

int check(char *sUser,char *sPassWord,int iDroit);

void simple_toupper(char *s); 
// 0==
int compare(char *s1,char *s2);
//ƥ��ͨ���,���ַ�ƥ�����,ƥ��ɹ�����true,���ɹ�����false,ֻ�� * ? Ϊͨ���
bool MaskString ( char* i_sName,char* i_sMask );
void InitDigChAntiTable();
void HashCharsToNum(char srcKey[18],char desKey[18]);
#endif
