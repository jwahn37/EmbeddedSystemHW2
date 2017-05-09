#include <unistd.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MAJOR_NUM 242
#define DEVICE_FILE_NAME "/dev/dev_driver"
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char*)

struct user_input {
	char timer_span;
	char number;
	int start_option;
};


int main(int argc, char* argv[]){
//	int i=syscall(377, 11);
//	printf("syscall %d\n" ,i);

	struct user_input user_in;
	int returnVal;
	int dev_file;
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
//	printf("%s\n",argv[1]);
	printf("user : %d %d %d\n",user_in.timer_span, user_in.number, user_in.start_option);	

	returnVal=syscall(378, &user_in);
    printf("syscall returnVal %d\n" ,returnVal);

    dev_file = open(DEVICE_FILE_NAME, 0);

    if(dev_file<0){
        printf("Cant not open device file: %s \n", DEVICE_FILE_NAME);
        exit(-1);
    }


    returnVal = ioctl(dev_file, IOCTL_SET_MSG, returnVal);

    close(dev_file);

	return 0;
}
