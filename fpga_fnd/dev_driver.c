/* FPGA FND Ioremap Control
FILE : fpga_fpga_driver.c 
AUTH : largest@huins.com */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>
#include <asm/param.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/slab.h>

#define IOM_FND_MAJOR 242		// ioboard fpga device major number
#define IOM_FND_NAME "dev_driver"		// ioboard fpga device name

#define IOM_FND_ADDRESS 0x08000004 // pysical address

#define IOCTL_SET_MSG _IOR(IOM_FND_MAJOR, 0, char*)
//Global variable
static int fpga_fnd_port_usage = 0;
static unsigned char *iom_fpga_fnd_addr;

// define functions...
ssize_t iom_fpga_fnd_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
ssize_t iom_fpga_fnd_read(struct file *inode, char *gdata, size_t length, loff_t *off_what);
int iom_fpga_fnd_open(struct inode *minode, struct file *mfile);
int iom_fpga_fnd_release(struct inode *minode, struct file *mfile);
int device_ioctl(/*struct inode *minode,*/ struct file *mfile, unsigned int ioctl_num, unsigned long ioctl_param);
void device_fnd_write(unsigned char value[4]); 
int timer_init(void);
void timer_handler(unsigned long arg);

// define file_operations structure 
struct file_operations iom_fpga_fnd_fops =
{
	.owner		=	THIS_MODULE,
	.open		=	iom_fpga_fnd_open,
	.write		=	iom_fpga_fnd_write,	
	.read		=	iom_fpga_fnd_read,	
	.release	=	iom_fpga_fnd_release,
    .unlocked_ioctl      =   device_ioctl,
//    .ioctl      =   device_ioctl,
    
};

struct timer_list *driver_timer;
unsigned long data=0;
unsigned char fnd_write[4]={1,2,3,4};
// when fnd device open ,call this function
int iom_fpga_fnd_open(struct inode *minode, struct file *mfile) 
{	
	if(fpga_fnd_port_usage != 0) return -EBUSY;

	fpga_fnd_port_usage = 1;

	return 0;
}

// when fnd device close ,call this function
int iom_fpga_fnd_release(struct inode *minode, struct file *mfile) 
{
	fpga_fnd_port_usage = 0;

	return 0;
}


int device_ioctl(/*struct inode *minode,*/ struct file *mfile, unsigned int ioctl_num, unsigned long ioctl_param)
{
    unsigned int i;
    unsigned long *temp;
    unsigned long *user_cmd;
    unsigned char fnd_write[4];
   // unsigned long 
    
    //i=IOCTL_SET_MSG;
    //printk("ioctl %d %d %d\n",ioctl_param,ioctl_num,i);
    switch(ioctl_num)
    {
        case IOCTL_SET_MSG:
            printk("inner\n");
//            temp = (unsigned long*) ioctl_param;
  //          copy_from_user(user_cmd,temp, sizeof(unsigned long));
  //          printk("%d\n",*user_cmd);
            //  iom_fpga_fnd_write(mfile, (char*)ioctl_param, sizeof(int), 0);
    
            timer_init();
            /*
            fnd_write[0]=1;
            fnd_write[1]=2;
            fnd_write[2]=3;
            fnd_write[3]=5;

            device_fnd_write(fnd_write);
    */
    }

    return 1;
}

int timer_init(void){
    printk("start timer module\n");
    driver_timer= kmalloc(sizeof(struct timer_list), GFP_KERNEL );
    if(driver_timer==NULL)
            return -1;
    init_timer(driver_timer);
    driver_timer->expires = get_jiffies_64() + HZ*1;
    driver_timer->data = data;
    driver_timer->function = timer_handler;
    add_timer(driver_timer);

}

void timer_handler(unsigned long arg)
{
    printk("timer tick : %ld\n", arg);
    driver_timer -> data++;
    driver_timer -> expires = get_jiffies_64() +HZ*1;

   // fnd_write[0]=1;
    //fnd_write[1]=2;
    //fnd_write[2]=3;
    fnd_write[3]=(fnd_write[3]+1)%10;

    device_fnd_write(fnd_write);
    
    add_timer(driver_timer);
}

// when write to fnd device  ,call this function
void device_fnd_write(unsigned char value[4]) 
{
	unsigned short int value_short = 0;
    printk("%d %d %d %d\n",value[0],value[1],value[2],value[3]);
    value_short = value[0] << 12 | value[1] << 8 |value[2] << 4 |value[3];
    outw(value_short,(unsigned int)iom_fpga_fnd_addr);	    

	
}


// when write to fnd device  ,call this function
ssize_t iom_fpga_fnd_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what) 
{
	int i;
	unsigned char value[4];
	unsigned short int value_short = 0;
	const char *tmp = gdata;

	if (copy_from_user(&value, tmp, 4))
		return -EFAULT;

    value_short = value[0] << 12 | value[1] << 8 |value[2] << 4 |value[3];
    outw(value_short,(unsigned int)iom_fpga_fnd_addr);	    

	return length;
}

// when read to fnd device  ,call this function
ssize_t iom_fpga_fnd_read(struct file *inode, char *gdata, size_t length, loff_t *off_what) 
{
	int i;
	unsigned char value[4];
	unsigned short int value_short = 0;
	char *tmp = gdata;

    value_short = inw((unsigned int)iom_fpga_fnd_addr);	    
    value[0] =(value_short >> 12) & 0xF;
    value[1] =(value_short >> 8) & 0xF;
    value[2] =(value_short >> 4) & 0xF;
    value[3] = value_short & 0xF;

    if (copy_to_user(tmp, value, 4))
        return -EFAULT;

	return length;
}

int __init iom_fpga_fnd_init(void)
{
	int result;

	result = register_chrdev(IOM_FND_MAJOR, IOM_FND_NAME, &iom_fpga_fnd_fops);
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}

	iom_fpga_fnd_addr = ioremap(IOM_FND_ADDRESS, 0x4);

	printk("init module, %s major number : %d\n", IOM_FND_NAME, IOM_FND_MAJOR);

	return 0;
}

void __exit iom_fpga_fnd_exit(void) 
{
	iounmap(iom_fpga_fnd_addr);
	unregister_chrdev(IOM_FND_MAJOR, IOM_FND_NAME);
}

module_init(iom_fpga_fnd_init);
module_exit(iom_fpga_fnd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huins");
