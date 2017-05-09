/* FPGA FND Test Application
File : fpga_test_fnd.c
Auth : largest@huins.com */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <string.h>
#define MAJOR_NUM 242
#define MAX_DIGIT 4
#define FND_DEVICE "/dev/dev_driver"
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char*)

struct user_input {
    char timer_span;
    char number;
    int start_option;
};


int main(int argc, char **argv)
{
	int dev;
    struct user_input user_in;
    int retVal;
    int sys_data;
    if(argc==1){
            printf("Error. You didn't enter anything\n");
            exit(1);
    }

    if(argc>4){
            printf("Error. You entered too much\n");
            exit(1);
    }
    user_in.timer_span = atoi(argv[1]);
    user_in.number = atoi(argv[2]);
    user_in.start_option = atoi(argv[3]);

    printf("useer : %d %d %d\n",user_in.timer_span, user_in.number, user_in.start_option); 

    sys_data=syscall(378, &user_in);
    printf("syscall returnVal %d\n" ,sys_data);

/*	memset(data,0,sizeof(data));

	if(argc!=2) {
		printf("please input the parameter! \n");
		printf("ex)./test_led a1\n");
		return -1;
	}

    str_size=(strlen(argv[1]));
    if(str_size>MAX_DIGIT)
    {
        printf("Warning! 4 Digit number only!\n");
        str_size=MAX_DIGIT;
    }

    for(i=0;i<str_size;i++)
    {
        if((argv[1][i]<0x30)||(argv[1][i])>0x39) {
            printf("Error! Invalid Value!\n");
            return -1;
        }
        data[i]=argv[1][i]-0x30;
    }
*/

    dev = open(FND_DEVICE, O_RDWR);
    if (dev<0) {
        printf("Device open error : %s\n",FND_DEVICE);
        exit(1);
    }
    retVal=ioctl(dev,IOCTL_SET_MSG,sys_data);

	close(dev);

	return(0);
}
