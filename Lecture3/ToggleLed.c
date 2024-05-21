#define SERIALTERMINAL      "/dev/ttyUSB0"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>


const int SRM_LENGTH = 7;
const unsigned char SYNT_REPLY_MESSAGE[SRM_LENGTH] = {0x41, 0x44, 0x46, 0x35, 0x33, 0x35, 0x35};

struct termios tty, tty_tmp;

unsigned int up17_Down22to25_27(unsigned int* gpio_base_addres)
{

//Register Number       Register Name   Description     Controls pins
//Register 00   GPFSEL0 GPIO Function Select 0  pins 0-9
//Register 01   GPFSEL1 GPIO Function Select 1  pins 10-19
//Register 02   GPFSEL1 GPIO Function Select 2  pins 20-29

    int arrGPIO[6] = {17,22,23,24,25,27};
    for(int i=0;i<6;i++)
    {
        int reg   =  arrGPIO[i]/10;
        int shift = (arrGPIO[i]%10)*3;
        gpio_base_addres[reg] = (gpio_base_addres[reg] &~ (7 << shift)) | (1 <<shift);
    }

    return 0;
}


int main(int argc, char* argv[])
{
    int dev_mem;// = (int)MAP_FAILED;
    dev_mem = open("/dev/gpiomem", O_RDWR | O_SYNC);
    unsigned int* gpio_base_addres = (unsigned int*)mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, 0x7E200000);
    if(gpio_base_addres < 0)
    {
        printf("mmap error\n\r");
        return 1;
    }
    printf("gpio_base_address = %p \n\r", gpio_base_addres);
    up17_Down22to25_27(gpio_base_addres);

    while (1)
    {
        gpio_base_addres[7] = 1 << 17;//Up
        gpio_base_addres[7] = 1 << 22;//Up
        gpio_base_addres[7] = 1 << 27;//Up
        printf("up\n\r");
        usleep(1000000);
        gpio_base_addres[10] = 1 << 17;//Down
        gpio_base_addres[10] = 1 << 22;//Down
        gpio_base_addres[10] = 1 << 27;//Down
        printf("down\n\r");
        usleep(1000000);
    }
}
