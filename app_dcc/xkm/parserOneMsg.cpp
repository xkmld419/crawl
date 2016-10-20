#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <sys/timeb.h>
#include "OcpMsgParser.h"

#define MAXSIZE 1024

int main(int argc,char* argv[])
{
    
    char sDictFile[256];
    char *p;
    int iResult;
    OcpMsgParser *poOcpMsgParser;
    
    char s_buff[]={0x01,0x00,0x01,0xBC,0x80,0x00,0x01,0x10,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x06, 
0x00,0x00,0x00,0x08,0x00,0x00,0x01,0x07,0x40,0x00,0x00,0x3E,0x53,0x43,0x50,0x30, 
0x30,0x31,0x2E,0x63,0x68,0x69,0x6E,0x61,0x74,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E, 
0x63,0x6F,0x6D,0x3B,0x33,0x34,0x36,0x33,0x30,0x33,0x35,0x34,0x32,0x36,0x3B,0x39, 
0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x3B,0x30,0x30,0x30,0x30,0x30,0x30, 
0x30,0x31,0x00,0x00,0x00,0x00,0x01,0x08,0x40,0x00,0x00,0x1F,0x53,0x43,0x50,0x30, 
0x30,0x31,0x2E,0x63,0x68,0x69,0x6E,0x61,0x74,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E, 
0x63,0x6F,0x6D,0x00,0x00,0x00,0x01,0x28,0x40,0x00,0x00,0x13,0x77,0x77,0x77,0x2E, 
0x61,0x62,0x63,0x2E,0x63,0x6F,0x6D,0x00,0x00,0x00,0x01,0x1B,0x40,0x00,0x00,0x1A, 
0x77,0x77,0x77,0x2E,0x6C,0x69,0x61,0x6E,0x63,0x68,0x75,0x61,0x6E,0x67,0x2E,0x63, 
0x6F,0x6D,0x00,0x00,0x00,0x00,0x01,0x02,0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x04, 
0x00,0x00,0x01,0xCD,0x40,0x00,0x00,0x1E,0x76,0x6F,0x69,0x63,0x65,0x40,0x63,0x68, 
0x69,0x6E,0x61,0x74,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F,0x6D,0x00,0x00, 
0x00,0x00,0x01,0xA0,0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x9F, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x01,0x00,0x00,0x13,0x89,0xC0,0x00,0x00,0x14, 
0x00,0x01,0x3C,0x68,0x00,0x00,0x00,0x2C,0xBE,0x1D,0x9A,0x60,0x00,0x00,0x01,0x25, 
0x40,0x00,0x00,0x0C,0x6F,0x63,0x73,0x32,0x00,0x00,0x00,0x37,0x40,0x00,0x00,0x0C, 
0xCE,0x69,0xBA,0x22,0x00,0x00,0x01,0xBB,0x40,0x00,0x00,0x28,0x00,0x00,0x01,0xC2, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xBC,0x40,0x00,0x00,0x14, 
0x30,0x35,0x33,0x31,0x38,0x33,0x30,0x31,0x30,0x37,0x38,0x30,0x00,0x00,0x01,0x27, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x1A,0x40,0x00,0x00,0x0C, 
0x20,0x72,0x72,0x30,0x00,0x00,0x01,0x1A,0x40,0x00,0x00,0x0C,0x20,0x72,0x72,0x31, 
0x00,0x00,0x01,0x1A,0x40,0x00,0x00,0x0C,0x20,0x72,0x72,0x32,0x00,0x00,0x01,0x1A, 
0x40,0x00,0x00,0x0C,0x20,0x72,0x72,0x33,0x00,0x00,0x01,0x1A,0x40,0x00,0x00,0x0C, 
0x20,0x72,0x72,0x34,0x00,0x00,0x01,0xC7,0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x00, 
0x00,0x00,0x01,0xC8,0x40,0x00,0x00,0x14,0x00,0x00,0x03,0x68,0x40,0x00,0x00,0x0C, 
0x00,0x00,0x00,0x01,0x00,0x00,0x4E,0x84,0xC0,0x00,0x00,0x18,0x00,0x00,0x28,0xAF, 
0x00,0x00,0x4F,0x4C,0xC0,0x00,0x00,0x0C,0x00,0x00,0x28,0xAF};
    
     if ((p=getenv ("ABMAPP_DEPLOY")) == NULL)
     {
      cout<<"Please set env value ABM_HOME. For example  export ABM_HOME=/home/abm/ABM_HOME"<<endl;
      return -1;
	 }
	  //get dictionary file
    sprintf (sDictFile, "%s/config/dictionary.xml", p);

    poOcpMsgParser = new OcpMsgParser(sDictFile);
    
    if(poOcpMsgParser != NULL){
    	iResult=poOcpMsgParser->parseRawToApp(s_buff,sizeof(s_buff));
    } else {
    	cout<<"new error!"<<endl;
    	exit(-1);
    }
    if (iResult<0){
    	 printf("parser error.\n");
    }

    poOcpMsgParser->getAllAvp(true);
    delete poOcpMsgParser;
    exit(0);
    	
}