#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include "dev_driver.h"
#include "submodule/submodule.c"

/* dev_driver_handler argument */
static struct struct_mydata {
	struct timer_list timer;
	int my_interval;
	int cnt;
};
	
static void dev_driver_handler(unsigned long _mydata){
	struct struct_mydata* mydata = (struct struct_mydata*) _mydata;

	mydata->cnt--;
	if(mydata->cnt > 0){
		// update board
		submodule_update();

		//add timer again
		mydata->timer.expires = get_jiffies_64() + mydata->my_interval;
		mydata->timer.data = (unsigned long) mydata;
		add_timer(&(mydata->timer));
	}
	else{
		//clear devices
		submodule_clear();
	}
}

int dev_driver_open(struct inode* minode, struct file* mfile){
	printk("device driver open\n");

	//use memory mapped IO to control devices
	submodule_ioremap();
	return 0;
}

int dev_driver_release(struct inode* minode, struct file* mfile){
	printk("device driver release\n");
	return 0;
}

long dev_driver_ioctl(struct file* mfile, unsigned int cmd, unsigned long _data){
	int state;
	struct dev_driver_data data;
	static struct struct_mydata mydata;
	printk("device driver ioctl\n");
	
	if(cmd == CMD0){
		state = copy_from_user(&data, (struct dev_driver_data*)_data, sizeof(struct dev_driver_data));
		
		//initialize devices
		submodule_init(data.timer_init);

		//init timer
		mydata.my_interval = 10 * HZ / data.timer_interval;
		mydata.cnt = data.timer_cnt;
		mydata.timer.expires = get_jiffies_64() + mydata.my_interval;
		mydata.timer.function = dev_driver_handler;
		mydata.timer.data = (unsigned long) &mydata;
		init_timer(&(mydata.timer));
	}
	else if(cmd == CMD1) {
		add_timer(&(mydata.timer));
	}

	return 0;
}

struct file_operations fops = {
        .open = dev_driver_open,
        .unlocked_ioctl = dev_driver_ioctl,
        .release = dev_driver_release,
};

int __init dev_driver_init(void){
	int state;
	printk("dev_driver_init\n");

	state = register_chrdev(MAJOR_NUMBER, DEV_DRIVER_NAME, &fops);
	if(state < 0) {
		printk("register error\n");
		return state;
	}
	
	return 0;
}

void __exit dev_driver_exit(void){
        unregister_chrdev(MAJOR_NUMBER, DEV_DRIVER_NAME);
	submodule_iounmap();
}

module_init(dev_driver_init);
module_exit(dev_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JungHee Kye");
