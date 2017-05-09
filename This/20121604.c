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

    //wrong input
    if(argc==1){
            printf("Error. You didn't enter anything\n");
            exit(1);
    }

    //wrong input
    if(argc>4){
            printf("Error. You entered too much\n");
            exit(1);
    }

    //put input to variable
    user_in.timer_span = atoi(argv[1]);
    user_in.number = atoi(argv[2]);
    user_in.start_option = atoi(argv[3]);

//    printf("useer : %d %d %d\n",user_in.timer_span, user_in.number, user_in.start_option); 

    //system call
    //return value is 4byte format data
    sys_data=syscall(378, &user_in);
  //  printf("syscall returnVal %d\n" ,sys_data);

    //device open
    dev = open(FND_DEVICE, O_RDWR);
    if (dev<0) {
        printf("Device open error : %s\n",FND_DEVICE);
        exit(1);
    }

    //use ioctl function
    retVal=ioctl(dev,IOCTL_SET_MSG,sys_data);

	close(dev);

	return(0);
}
