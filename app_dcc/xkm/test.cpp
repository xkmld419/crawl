#include <iostream> 
//#include <string.h> 
//#include <netdb.h> 
/***
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/ioctl.h> 
#include <net/if.h> 
#include <net/if_arp.h> 
#include <net/ethernet.h> 
#include <signal.h> 
#include <netinet/ip.h> 
***/
using namespace std;

int main (int argc, char* argv[]) { 
    char s_buff[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x02,0x48,0x80,0x00,0x01,0x10, 
0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x01,0x07, 
0x40,0x00,0x00,0x32,0x41,0x42,0x4D,0x32,0x40,0x30,0x30,0x31,0x2E,0x43,0x68,0x69, 
0x6E,0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F,0x6D,0x3B,0x31,0x33, 
0x30,0x36,0x33,0x30,0x32,0x33,0x30,0x34,0x3B,0x39,0x30,0x35,0x30,0x33,0x00,0x00, 
0x00,0x00,0x01,0x08,0x40,0x00,0x00,0x21,0x41,0x42,0x4D,0x32,0x40,0x30,0x30,0x31, 
0x2E,0x43,0x68,0x69,0x6E,0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F, 
0x6D,0x00,0x00,0x00,0x00,0x00,0x01,0x28,0x40,0x00,0x00,0x1C,0x30,0x30,0x31,0x2E, 
0x43,0x68,0x69,0x6E,0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F,0x6D, 
0x00,0x00,0x01,0x1B,0x40,0x00,0x00,0x1C,0x30,0x30,0x31,0x2E,0x43,0x68,0x69,0x6E, 
0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F,0x6D,0x00,0x00,0x01,0x02, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x04,0x00,0x00,0x01,0xCD,0x40,0x00,0x00,0x2C, 
0x41,0x63,0x74,0x69,0x76,0x65,0x2E,0x4E,0x65,0x74,0x2D,0x43,0x74,0x72,0x6C,0x40, 
0x30,0x30,0x31,0x2E,0x43,0x68,0x69,0x6E,0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D, 
0x2E,0x63,0x6F,0x6D,0x00,0x01,0x39,0x2C,0xC0,0x00,0x00,0x5A,0x00,0x01,0x3C,0x68, 
0x32,0x30,0x31,0x31,0x30,0x35,0x32,0x35,0x31,0x30,0x32,0x31,0x34,0x38,0x31,0x32, 
0x33,0x34,0x35,0x36,0x37,0x38,0x30,0x30,0x2D,0x30,0x30,0x30,0x31,0x30,0x2D,0x30, 
0x30,0x30,0x30,0x32,0x34,0x30,0x30,0x35,0x39,0x39,0x39,0x2D,0x76,0x31,0x2E,0x30, 
0x32,0x2E,0x30,0x33,0x2D,0x30,0x31,0x30,0x30,0x30,0x31,0x2D,0x52,0x65,0x63,0x68, 
0x61,0x72,0x67,0x65,0x2E,0x41,0x42,0x4D,0x32,0x40,0x30,0x30,0x31,0x3B,0x00,0x00, 
0x00,0x00,0x01,0xA0,0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x9F, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xB4,0x40,0x00,0x00,0x0C, 
0x00,0x00,0x00,0x71,0x00,0x00,0x03,0x69,0xC0,0x00,0x00,0xC0,0x00,0x00,0x28,0xAF, 
0x00,0x01,0x38,0x94,0x40,0x00,0x00,0xB4,0x00,0x01,0x38,0x95,0x40,0x00,0x00,0x60, 
0x00,0x01,0x38,0x98,0x00,0x00,0x00,0x21,0x41,0x42,0x4D,0x32,0x40,0x30,0x30,0x31, 
0x2E,0x43,0x68,0x69,0x6E,0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F, 
0x6D,0x00,0x00,0x00,0x00,0x01,0x38,0x9D,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x02, 
0x00,0x01,0x38,0x99,0x40,0x00,0x00,0x0E,0x00,0x01,0x84,0x20,0x18,0x06,0x00,0x00, 
0x00,0x01,0x38,0x9B,0x40,0x00,0x00,0x0C,0x00,0x00,0x17,0x71,0x00,0x01,0x38,0xA6, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x0C,0x00,0x01,0x38,0x9F,0x40,0x00,0x00,0x4C, 
0x00,0x00,0x01,0xCD,0x40,0x00,0x00,0x2A,0x51,0x75,0x65,0x72,0x79,0x2E,0x42,0x61, 
0x6C,0x61,0x6E,0x63,0x65,0x40,0x30,0x30,0x31,0x2E,0x43,0x68,0x69,0x6E,0x61,0x54, 
0x65,0x6C,0x65,0x63,0x6F,0x6D,0x2E,0x63,0x6F,0x6D,0x00,0x00,0x00,0x01,0x38,0xA5, 
0x40,0x00,0x00,0x0C,0x00,0x00,0x00,0x02,0x00,0x01,0x38,0xA4,0x40,0x00,0x00,0x0C, 
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x25,0x40,0x00,0x00,0x20,0x53,0x52,0x32,0x40, 
0x30,0x30,0x31,0x2E,0x43,0x68,0x69,0x6E,0x61,0x54,0x65,0x6C,0x65,0x63,0x6F,0x6D, 
0x2E,0x63,0x6F,0x6D,0x00,0x00,0x00,0x37,0x40,0x00,0x00,0x0C,0x4D,0xDC,0xBA,0xF1};
    int a;
    a=sizeof(s_buff);
    //cout<<b<<endl;
    cout<<a<<endl;
    getchar();
}  /* end of main */ 
