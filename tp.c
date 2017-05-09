//#include <linux/kernel.h>
//#include <linux/uaccess.h>
#include <stdio.h>
struct user_input {
	char timer_span;
	char number;
	int start_option;
};

int main() {
	struct user_input user_in;
	int iter=4;
	int fnd_loc=4;
	int fnd_val=0;
	int return_info=0;

	char timer_span = 1;
	char number = 14;
	int start_option = 700;

//	copy_from_user(&user_in, user, sizeof(user_in));
	
//	printk("sys_combinedInput %d %d %d\n", user_in.timer_span, user_in.number, user_in.start_option);

	
	while(iter-- && start_option%10==0)
	{
		start_option/=10;
		fnd_loc--;
	}
	
	fnd_val=start_option%10;

	return_info += (fnd_loc << 24);
	return_info += (fnd_val << 16);
	return_info += (timer_span << 8);
	return_info += number;

	printf("return info %d %d %d %d %d\n",fnd_loc, fnd_val, timer_span, number, return_info);

	return 0;
}
