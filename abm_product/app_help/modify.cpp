#include "stdio.h"
#include "Xml.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
	//����������
	Xml newXml;	
	if(!newXml.loadFile(argv[1]))
	{
		cout<<"����"<<argv[1]<<"�ļ���ʽ!"<<endl;
		return false;
	}
	
	newXml.m_xml.FindElem(); //"help"�ڵ�
	string option=newXml.m_xml.GetAttrib("option");
	
	/*
	newXml.m_xmlHelp->m_versionHelp.m_sVersion="";
	newXml.m_xmlHelp->m_versionHelp.m_sEnvironment="";
	*/
	newXml.loadCommand();
	newXml.loadParam();
	
	//����Ŀ���ļ�
  char filename[200];
  const char* file=getenv("BILLDIR");
  sprintf(filename,"%s/etc/help.xml", file);
	CMarkup oldXml;
  if(!oldXml.Load(filename))
  {
		cout<<"����Ŀ���ļ�����!"<<endl;
		return false;
	}
	
	if(strcmp(option.c_str(),"add")==0)
	{
		/******************************�������****************************/
		oldXml.FindChildElem("commandHelp");
		oldXml.IntoElem();
			
		vector <struct command *>::iterator iter=newXml.m_xmlHelp->m_pCommandHelp.begin();
  	for(;iter!=newXml.m_xmlHelp->m_pCommandHelp.end();++iter)
  	{
  		//�¼�һ������			
			bool bFind=false;
			while(oldXml.FindChildElem("command"))
			{
				oldXml.IntoElem();
				string cmd_value=CMarkup::UTF8ToA(oldXml.GetAttrib("code"));
				if(strcmp(cmd_value.c_str(),(*iter)->m_sCode.c_str())==0)
				{
					bFind=true;
					break;
				}
				oldXml.OutOfElem();
			}
			
			int size=(*iter)->m_pSubcommand.size();
			if(!bFind&&size>0)
			{
				oldXml.AddChildElem("command");
				oldXml.IntoElem();
				oldXml.AddAttrib("code",CMarkup::AToUTF8((*iter)->m_sCode.c_str()));
			}
  		
  		vector <struct subcommand *>::iterator subiter=(*iter)->m_pSubcommand.begin();
  		for(;subiter!=(*iter)->m_pSubcommand.end();++subiter)
  		{
  			bool bSubFind=false;
				while(oldXml.FindChildElem("subcommand"))
				{
					oldXml.IntoElem();
					string subcmd_value=CMarkup::UTF8ToA(oldXml.GetAttrib("subcode"));
					if(strcmp(subcmd_value.c_str(),(*subiter)->m_sSubCode.c_str())==0)
					{
						bSubFind=true;
						break;
					}
					oldXml.OutOfElem();
				}
				
				if(!bSubFind)
				{
					//�¼�������
					oldXml.AddChildElem("subcommand");
					oldXml.IntoElem();
					oldXml.AddAttrib("subcode",CMarkup::AToUTF8((*subiter)->m_sSubCode.c_str()));
					
					oldXml.AddChildElem("function",CMarkup::AToUTF8((*subiter)->m_sFunction.c_str()));
					oldXml.AddChildElem("format",CMarkup::AToUTF8((*subiter)->m_sFormat.c_str()));
					oldXml.AddChildElem("description",CMarkup::AToUTF8((*subiter)->m_sDescription.c_str()));
						
					oldXml.OutOfElem();
					cout<<(*iter)->m_sCode.c_str()<<" "<<(*subiter)->m_sSubCode.c_str()<<" ��������ӳɹ�!"<<endl;
				}
				else
				{
					oldXml.OutOfElem();
					cout<<(*iter)->m_sCode.c_str()<<" "<<(*subiter)->m_sSubCode.c_str()<<" �������Ѿ�����!"<<endl;
				}
				
				oldXml.ResetChildPos();
  		}
  		
  		if(bFind||(!bFind&&size>0))
  			oldXml.OutOfElem();
  			
  		oldXml.ResetChildPos();
  	}
  	
  	oldXml.OutOfElem();
  	
  	/******************************��Ӳ���****************************/
  	oldXml.FindChildElem("paramHelp");
		oldXml.IntoElem();
			
  	vector <struct param *>::iterator paramiter=newXml.m_xmlHelp->m_pParamHelp.begin();
  	for(;paramiter!=newXml.m_xmlHelp->m_pParamHelp.end();++paramiter)
  	{
			//�¼�һ������			
			bool bFind=false;
			while(oldXml.FindChildElem("param"))
			{
				oldXml.IntoElem();
				string param_value=CMarkup::UTF8ToA(oldXml.GetAttrib("code"));
				if(strcmp(param_value.c_str(),(*paramiter)->m_sCode.c_str())==0)
				{
					bFind=true;
					break;
				}
				oldXml.OutOfElem();
			}
			
			if(!bFind)
			{
				oldXml.AddChildElem("param");
				oldXml.IntoElem();
				oldXml.AddAttrib("code",CMarkup::AToUTF8((*paramiter)->m_sCode.c_str()));
				
				oldXml.AddChildElem("name",CMarkup::AToUTF8((*paramiter)->m_sName.c_str()));
				oldXml.AddChildElem("description",CMarkup::AToUTF8((*paramiter)->m_sDescription.c_str()));
				oldXml.AddChildElem("attributes");
				
				oldXml.IntoElem();
				oldXml.AddChildElem("rule",CMarkup::AToUTF8((*paramiter)->m_attributes.m_sRule.c_str()));
				oldXml.AddChildElem("range",CMarkup::AToUTF8((*paramiter)->m_attributes.m_sRange.c_str()));
				oldXml.AddChildElem("value",CMarkup::AToUTF8((*paramiter)->m_attributes.m_sValue.c_str()));
				
				oldXml.OutOfElem();
				oldXml.OutOfElem();
				
				cout<<(*paramiter)->m_sCode.c_str()<<" ������ӳɹ�!"<<endl;
			}
			else
			{
				oldXml.OutOfElem();
				cout<<(*paramiter)->m_sCode.c_str()<<" �����Ѿ�����!"<<endl;
			}
			
			oldXml.ResetChildPos();
  	}
  	
  	oldXml.OutOfElem();
	}
	else if(strcmp(option.c_str(),"delete")==0)
	{
		/******************************ɾ������****************************/
		oldXml.FindChildElem("commandHelp");
		oldXml.IntoElem();
			
		vector <struct command *>::iterator iter=newXml.m_xmlHelp->m_pCommandHelp.begin();
  	for(;iter!=newXml.m_xmlHelp->m_pCommandHelp.end();++iter)
  	{
			//ɾ������
  		bool bFind=false;
			while(oldXml.FindChildElem("command"))
			{
				oldXml.IntoElem();
				string cmd_value=CMarkup::UTF8ToA(oldXml.GetAttrib("code"));
				if(strcmp(cmd_value.c_str(),(*iter)->m_sCode.c_str())==0)
				{
					bFind=true;
					break;
				}
				oldXml.OutOfElem();
			}
			
			if(bFind)
			{
				int size=(*iter)->m_pSubcommand.size();
				if(size>0)
  			{
  				//ɾ��������
  				vector <struct subcommand *>::iterator subiter=(*iter)->m_pSubcommand.begin();
  				for(;subiter!=(*iter)->m_pSubcommand.end();++subiter)
  				{
  					bool bSubFind=false;
						while(oldXml.FindChildElem("subcommand"))
						{
							oldXml.IntoElem();
							string subcmd_value=CMarkup::UTF8ToA(oldXml.GetAttrib("subcode"));
							if(strcmp(subcmd_value.c_str(),(*subiter)->m_sSubCode.c_str())==0)
							{
								bSubFind=true;
								break;
							}
							oldXml.OutOfElem();
						}
						
						if(bSubFind)
						{
							oldXml.RemoveElem();
							oldXml.OutOfElem();
							cout<<"�ɹ�ɾ�� "<<(*iter)->m_sCode.c_str()<<" "<<(*subiter)->m_sSubCode.c_str()<<" ������!"<<endl;
						}
						else
						{
							cout<<"ɾ��ʧ�� û�� "<<(*iter)->m_sCode.c_str()<<" "<<(*subiter)->m_sSubCode.c_str()<<" ������!"<<endl;
						}
						oldXml.ResetChildPos();
  				}
  				
  				//������ȫ��ɾ����,ɾ��������ڵ�
  				bool isTrue=oldXml.FindChildElem("subcommand");
  				if(!isTrue)
  				{
  					oldXml.RemoveElem();
  				}
  				
  				oldXml.OutOfElem();
  			}
  			else
  			{
					oldXml.RemoveElem();
					oldXml.OutOfElem();
					cout<<"�ɹ�ɾ�� "<<(*iter)->m_sCode.c_str()<<" ����!"<<endl;
  			}
			}
			else
			{
				cout<<"ɾ��ʧ�� û�� "<<(*iter)->m_sCode.c_str()<<" ����!"<<endl;
			}
  		
  		oldXml.ResetChildPos();
		}
		
  	oldXml.OutOfElem();
		
		/******************************ɾ������****************************/
		oldXml.FindChildElem("paramHelp");
		oldXml.IntoElem();
			
  	vector <struct param *>::iterator paramiter=newXml.m_xmlHelp->m_pParamHelp.begin();
  	for(;paramiter!=newXml.m_xmlHelp->m_pParamHelp.end();++paramiter)
  	{
			//ɾ������
			bool bFind=false;
			while(oldXml.FindChildElem("param"))
			{
				oldXml.IntoElem();
				string param_value=CMarkup::UTF8ToA(oldXml.GetAttrib("code"));
				if(strcmp(param_value.c_str(),(*paramiter)->m_sCode.c_str())==0)
				{
					bFind=true;
					break;
				}
				oldXml.OutOfElem();
			}
			
			if(bFind)
			{
				oldXml.RemoveElem();
				oldXml.OutOfElem();
				cout<<"�ɹ�ɾ�� "<<(*paramiter)->m_sCode.c_str()<<" ����!"<<endl;
			}
			else
			{
				cout<<"ɾ��ʧ�� û�� "<<(*paramiter)->m_sCode.c_str()<<" ����!"<<endl;
			}
			
			oldXml.ResetChildPos();
  	}
  	
  	oldXml.OutOfElem();
	}
	else if(strcmp(option.c_str(),"modify")==0)
	{
		cout<<"�޸ĳɹ�!"<<endl;
	}
	else
	{
		cout<<argv[1]<<"�ļ���ʽ����!"<<endl;
		return false;
	}
	
	oldXml.Save("./help.xml");
	
	return 1;
}
