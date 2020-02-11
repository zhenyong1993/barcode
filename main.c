#include <linux/input.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "barcode.h"

#define SCANNER_DEV "/dev/input/event2"

struct input_event buff; 
int fd; 
int read_nu;
char trans(unsigned short data);

int main(int argc, char *argv[])
{
    barcode* bc = new barcode;
        char a[128] = {};

        fd = open(SCANNER_DEV, O_RDONLY);//打开usb扫描枪设备
        if (fd < 0)
        { 
                perror("can not open device usbscanner!"); 
                exit(1); 
        } 
        int i = 0;
        printf("--fd:%d--\n",fd);
        while(1)
        {
                while(read(fd,&buff,sizeof(struct input_event))==0)
                {
                        ;
                }
//                printf("type:%d code:%d value:%d\n",buff.type,buff.code,buff.value); 
                if(buff.code != 28 && buff.value == 1)//CRLF=28
                {
                    a[i++] = trans(buff.code);
                    printf("----- %c %d-------\n", a[i-1],buff.code);
                }
                else if(buff.code==28 && buff.value == 1)
                {
                    printf("========= str is %s ========\n", a);
                    bc -> onGetStr(a);
                    i=0;
                    memset(a, 0, 128);
                }
        }
        close(fd); 
        return 1;
}

char trans(unsigned short data)
{
    switch(data)
    {
            case 2 : return '1';
            case 3 : return '2';
            case 4 : return '3';
            case 5 : return '4';
            case 6 : return '5';
            case 7 : return '6';
            case 8 : return '7';
            case 9 : return '8';
            case 10: return '9';
            case 11: return '0';
            case 12: return '-';
            case 13: return '=';
            case 14: return '\b';
            case 15: return '\t';
            case 16: return 'Q';
            case 17: return 'W';
            case 18: return 'E';
            case 19: return 'R';
            case 20: return 'T';
            case 21: return 'Y';
            case 22: return 'U';
            case 23: return 'I';
            case 24: return 'O';
            case 25: return 'P';
            case 26: return '[';
            case 27: return ']';
            case 30: return 'A';
            case 31: return 'S';
            case 32: return 'D';
            case 33: return 'F';
            case 34: return 'G';
            case 35: return 'H';
            case 36: return 'J';
            case 37: return 'K';
            case 38: return 'L';
            case 39: return ';';
            case 40: return '\'';
            case 44: return 'Z';
            case 45: return 'X';
            case 46: return 'C';
            case 47: return 'V';
            case 48: return 'B';
            case 49: return 'N';
            case 50: return 'M';
            case 51: return ',';
            case 52: return '.';
            case 53: return '/';
            case 57: return ' ';
            default: return '?';
    }
}
