/* Linux Kernel Module code by Tanmay Chaturvedi
 * Date: Feb 15, 2019
 * A Linux Kernel Module that invokes a callback function\
 * at every 500ms in default state, or based on user-specified time delay
 * 
 * Reference: http://dinomycle.blogspot.com/2009/07/using-timers-in-linux-kernel-module.html
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

/*statically defined timer increment counter*/
static uint32_t timer_fire_count = 0;

/*Command line argument declaration*/
static int input_period = 0;	/*limiting period to 256 seconds only, and init with 0*/ 
static char* input_string = "Tanmay Chaturvedi";
static unsigned long timer_delay = 0; /*User input timer interrupt delay*/
struct timer_list tanmay_timer;

module_param(input_period, int, 0);
module_param(input_string, charp, 0);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanmay Chaturvedi");
MODULE_DESCRIPTION("A Kernel Module that works on Linux Timer");
MODULE_VERSION("1.0");

/*Time delay variable
 * jiffies + HZ = 1000 msec for ARM Arch
 * jiffies + HZ/2 = 500 msec*/

/*Default timer fire value is 500msec if no user argument, 
 * else, store user input value in input_period*/
 
void check_input_period(void)
{
	if(input_period == 0)
	{
		timer_delay = (50); /*Default val = 500msec*/
		//timer_delay = jiffies + msecs_to_jiffies(input_period);
	}
	else
	{
		//timer_delay = jiffies + msecs_to_jiffies(input_period);
		timer_delay = (100)* (input_period/1000);
	}
}

void timer_IRQ(unsigned long data)
{
	printk(KERN_ALERT "Name is %s  and count is %d\n", input_string, timer_fire_count ++);
	mod_timer(&tanmay_timer, jiffies + timer_delay); /* restarting timer */
}

static int tanmay_timer_init(void)
{
	printk(KERN_INFO "Entered the kernel module\n");
	check_input_period();
	tanmay_timer.function = timer_IRQ;
	tanmay_timer.data = 0;
	tanmay_timer.expires = jiffies + timer_delay;
	init_timer(&tanmay_timer);
	add_timer(&tanmay_timer);
	
	return 0;
}


static void tanmay_timer_end(void)
{
	printk(KERN_INFO "Exited the kernel module\n");
	del_timer_sync(&tanmay_timer); /* Deleting the timer */
}




module_init(tanmay_timer_init);
module_exit(tanmay_timer_end);
