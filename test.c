#include <unistd.h>
#include <syscall.h>
#include <stdio.h>

struct mystruct {
	int myvar;
	int myvar2;
};

int main(void){
//	int i=syscall(377, 11);
//	printf("syscall %d\n" ,i);

	struct mystruct my_st;
	my_st.myvar =999;
	my_st.myvar2 = 2;
	
	int i2=syscall(377, &my_st);
	printf("syscall %d\n" ,i2);

	return 0;
}
