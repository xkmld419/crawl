#include "CommandCom.h"

int main()
{
    char tmp[1204] = {0};
	printf("Ҫ��ʾ�Ĳ���: ");
	scanf("%s",tmp);
	string str(tmp);
	int pos = str.find('.');
	if(pos <=0 )
		return 0;
	string sect;
	sect.assign(str,0,pos);
	string param;
	param.assign(str,pos+1,str.size());
	char sHFKey[1024] = {0};
	if(tmp[0] != '\0')
	{
		CommandCom *p = new CommandCom();
		if(p)
		{
			p->InitClient();
			p->readIniString((char*)sect.c_str(),(char*)param.c_str(),sHFKey,NULL);
			if(sHFKey[0] != '\0')
				printf("%s%s\n","��ǰֵ:",sHFKey);
			else
				printf("%s\n","û��ֵ");
		}
	}
	return 0;
}