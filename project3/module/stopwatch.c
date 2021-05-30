#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <asm/gpio.h>
#include <asm/irq.h>
#include <linux/sched.h>
#include <mach/gpio.h>
#include "stopwatch.h"
#include "fnd.c"

static void main_timer_handler(unsigned long data);
static void end_timer_handler(unsigned long data);

static irqreturn_t home_handler(int irq, void* dev_id);
static irqreturn_t back_handler(int irq, void* dev_id);
static irqreturn_t volup_handler(int irq, void* dev_id);
static irqreturn_t voldown_handler(int irq, void* dev_id);
static void home_handler_bottom(struct work_struct*);
static void back_handler_bottom(struct work_struct*);
static void volup_handler_bottom(struct work_struct*);
static void voldown_handler_bottom(struct work_struct*);

static int stopwatch_open(struct inode* minode, struct file* mfile);
static int stopwatch_release(struct inode* minode, struct file* mfile);
static int __init stopwatch_init(void);
static void __exit stopwatch_exit(void);

struct main_timer_data_struct {
	unsigned long paused_jiffies;
	unsigned long start_jiffies;
	unsigned int rest_msec;
};
struct end_timer_data_struct {
	int sec;
};

struct timer_info {
	struct timer_list timer;
	int is_set;
	union {
		struct main_timer_data_struct main_timer_data;
		struct end_timer_data_struct end_timer_data;
	} data;
};

static int min, sec;
static struct timer_info main_timer, end_timer;
static struct work_struct w;
DECLARE_WAIT_QUEUE_HEAD(wq);

static void main_timer_handler(unsigned long data){
	unsigned char fnd[4];

	//set start_jiffies
	main_timer.data.main_timer_data.start_jiffies = get_jiffies_64();
	//increment time and write fnd
	sec++;
	if(sec >= 60){
		sec = 0;
		min++;
	}
	fnd[0] = min / 10;
	fnd[1] = min % 10;
	fnd[2] = sec / 10;
	fnd[3] = sec % 10;
	fnd_write(fnd);
	//add timer
	main_timer.timer.expires = get_jiffies_64() + HZ;
	add_timer(&main_timer.timer);
}

static void end_timer_handler(unsigned long data){
	end_timer.data.end_timer_data.sec++;
	printk("%d ", end_timer.data.end_timer_data.sec);

	if(end_timer.data.end_timer_data.sec >= 3){
		printk("shutting down..\n");
		//del main timer
		main_timer.is_set = 0;
		del_timer(&main_timer.timer);

		fnd_reset();

		wake_up_interruptible(&wq);
		return;
	}
	end_timer.timer.expires = get_jiffies_64() + HZ;
	add_timer(&end_timer.timer);
}

irqreturn_t home_handler(int irq, void* dev_id){
	INIT_WORK(&w, home_handler_bottom);
	schedule_work(&w);
	return IRQ_HANDLED;
}

irqreturn_t back_handler(int irq, void* dev_id){
	//set paused_jiffies, calculate rest miliseconds
	main_timer.data.main_timer_data.paused_jiffies = get_jiffies_64();
	main_timer.data.main_timer_data.rest_msec = jiffies_to_msecs(main_timer.data.main_timer_data.start_jiffies + HZ - main_timer.data.main_timer_data.paused_jiffies);
	main_timer.data.main_timer_data.rest_msec -= main_timer.data.main_timer_data.rest_msec % 100;

	INIT_WORK(&w, back_handler_bottom);
	schedule_work(&w);
	return IRQ_HANDLED;
}

irqreturn_t volup_handler(int irq, void* dev_id){
	INIT_WORK(&w, volup_handler_bottom);
	schedule_work(&w);
	return IRQ_HANDLED;
}

irqreturn_t voldown_handler(int irq, void* dev_id){
	INIT_WORK(&w, voldown_handler_bottom);
	schedule_work(&w);
	return IRQ_HANDLED;
}

void home_handler_bottom(struct work_struct* data){
	printk("[LOG] home handler\n");

	if(main_timer.is_set){
		printk("[DEBUG] timer already set\n");
		return;
	}
	else{
		//add timer
        	main_timer.timer.expires = get_jiffies_64() + msecs_to_jiffies(main_timer.data.main_timer_data.rest_msec);
	        add_timer(&main_timer.timer);
		main_timer.is_set = 1;
	}
}

void back_handler_bottom(struct work_struct* data){
	printk("[LOG] back handler\n");

	if(main_timer.is_set){
		//del timer
		main_timer.is_set = 0;
		del_timer(&main_timer.timer);
	}
}

void volup_handler_bottom(struct work_struct* data){
	printk("[LOG] volup_handler\n");

	//reset fnd
	fnd_reset();
	//reset min, sec
	min = sec = 0;
	//reset main timer
	main_timer.data.main_timer_data.start_jiffies = 0;
	main_timer.data.main_timer_data.paused_jiffies = 0;
	main_timer.is_set = 0;
	del_timer(&main_timer.timer);
}

void voldown_handler_bottom(struct work_struct* data){

	//if pressed
	if(gpio_get_value(IMX_GPIO_NR(5, 14)) == 0){
		printk("[LOG] voldown handler\n");
		//add timer
		end_timer.is_set = 1;
		end_timer.timer.expires = get_jiffies_64() + HZ;
		add_timer(&end_timer.timer);
	}
	//if released
	else {
		printk("\n");
		//del timer
		end_timer.is_set = 0;
		end_timer.data.end_timer_data.sec = 0;
		del_timer(&end_timer.timer);
	}
}

static int stopwatch_open(struct inode* minode, struct file* mfile){
	int ret;
	int irq;

	printk("[LOG] stopwatch open\n");

	//reset fnd
	fnd_init();
	fnd_reset();
	//set min, sec
	min = sec = 0;
	//set main timer
	init_timer(&main_timer.timer);
	main_timer.timer.function = main_timer_handler;
	main_timer.is_set = 0;
	main_timer.data.main_timer_data.start_jiffies = 0;
	main_timer.data.main_timer_data.paused_jiffies = 0;
	//set end timer
	init_timer(&end_timer.timer);
	end_timer.timer.function = end_timer_handler;
	end_timer.is_set = 0;
	end_timer.data.end_timer_data.sec = 0;

	//register handlers
	// home
	gpio_direction_input(IMX_GPIO_NR(1,11));
	irq = gpio_to_irq(IMX_GPIO_NR(1,11));
	ret=request_irq(irq, home_handler, IRQF_TRIGGER_FALLING, "home", 0);
	// back
	gpio_direction_input(IMX_GPIO_NR(1,12));
	irq = gpio_to_irq(IMX_GPIO_NR(1,12));
	ret=request_irq(irq, back_handler, IRQF_TRIGGER_FALLING, "back", 0);
	// volup
	gpio_direction_input(IMX_GPIO_NR(2,15));
	irq = gpio_to_irq(IMX_GPIO_NR(2,15));
	ret=request_irq(irq, volup_handler, IRQF_TRIGGER_FALLING, "volup", 0);
	// voldown
	gpio_direction_input(IMX_GPIO_NR(5,14));
	irq = gpio_to_irq(IMX_GPIO_NR(5,14));
	ret=request_irq(irq, voldown_handler, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "voldown", 0);
	
	//sleep
	interruptible_sleep_on(&wq);
	return 0;
}

int stopwatch_release(struct inode* minode, struct file* mfile){
	printk("[LOG] stopwatch close\n");
	fnd_reset();
	del_timer(&main_timer.timer);
	del_timer(&end_timer.timer);
	free_irq(gpio_to_irq(IMX_GPIO_NR(1, 11)), NULL);
        free_irq(gpio_to_irq(IMX_GPIO_NR(1, 12)), NULL);
        free_irq(gpio_to_irq(IMX_GPIO_NR(2, 15)), NULL);
        free_irq(gpio_to_irq(IMX_GPIO_NR(5, 14)), NULL);

	return 0;
}

static struct file_operations fops = {
        .open = stopwatch_open,
	.release = stopwatch_release,
};

int __init stopwatch_init(void){
	int state;
	printk("[LOG] stopwatch_init\n");
	if((state = register_chrdev(MAJOR_NUMBER, DEVICE_FILE, &fops)) < 0){
		printk("register error\n");
                return state;
	}
	return 0;
}

void __exit stopwatch_exit(void){
	fnd_exit();
        unregister_chrdev(MAJOR_NUMBER, DEVICE_FILE);
	printk(KERN_ALERT "[LOG] Remove Module Success \n");
}

module_init(stopwatch_init);
module_exit(stopwatch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JungHee Kye");
