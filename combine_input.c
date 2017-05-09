#include <linux/kernel.h>
#include <linux/uaccess.h>

//user's input
struct user_input {
	char timer_span;
	char number;
	int start_option;
};

asmlinkage int sys_combine_input(struct user_input *user) {
	struct user_input user_in;
	int iter=4;
	int fnd_loc=1;
	int fnd_val=0;
	int return_info=0;

	char timer_span = user_in.timer_span;
	char number = user_in.number;
	int start_option = user_in.start_option;

	//get user's input 
	copy_from_user(&user_in, user, sizeof(user_in));
	
	//printk("sys_combinedInput %c %d %d\n", user_in.timer_span, user_in.number, user_in.start_option);

	//change user's input format to 5byte integer type
	//1byte : fnd location, 1byte : fnd value, 1byte: timer span, 1byte : number of print
	while(iter-- && start_option%10!=0)
	{
		start_option/=10;
		fnd_loc++;
	}
	
	fnd_val=start_option%10;

	return_info += fnd_loc >> 24;
	return_info += fnd_val >> 16;
	return_info += timer_span >> 8;
	return_info += number;

	//printk("return info %d\n",return_info);

	return return_info;
}
