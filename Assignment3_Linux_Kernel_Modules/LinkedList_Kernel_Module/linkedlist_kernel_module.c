/* Linux Kernel Module code by Tanmay Chaturvedi
 * Date: Feb 15, 2019
 * A linux kernel module that implements a data structure (linked list)
 * and ....
 * Reference[1]: http://www.roman10.net/2011/07/28/linux-kernel-programminglinked-list/
 * 
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/sort.h>
#include <linux/string.h>
#include <linux/slab.h>


#define	INPUT_ARRAY_SIZE	(8)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanmay Chaturvedi");
MODULE_DESCRIPTION("A Kernel Module that works on Linked List");
MODULE_VERSION("1.0");


/*Array of animals*/
static char* animal_list[] = {"Cat", "Cat", "Dog", "Dog", "Alligator", "Mouse", "Tiger", "Zebra"};

/*Compare func, Reference[1] start*/
/*Compare func, Reference[1] end*/

static int compare_func(const void *left_string, const void *right_string)
 {
 
    return (strcmp(*(char **)left_string,*(char **)right_string));
}

struct animal_struct 
{
	uint8_t	animal_count; /* number of occurence */
	char* animal_type; /* type of animal */
	struct list_head list; /* list of all animal_struct structures */
};

struct animal_struct animal_list_head, *animal_nodes_ptr;

static int animal_ecosystem_init(void)
{
	int i;
	int compare_flag = 0;
	INIT_LIST_HEAD(&animal_list_head.list); /*List head*/
	printk(KERN_INFO "initialize kernel module \n");
	sort(animal_list, INPUT_ARRAY_SIZE, sizeof(animal_list[0]), compare_func, NULL);

	
	for( i =0; i< INPUT_ARRAY_SIZE; i++)
	{
		printk("Sorted Element = %s\n", animal_list[i]);
	}
 

	animal_nodes_ptr = (struct animal_struct*) (kmalloc(sizeof(animal_list_head),GFP_KERNEL));
	animal_nodes_ptr->animal_count = 0;
	
		for(i=1; i<INPUT_ARRAY_SIZE; i++)
		{
						
			if(!(strcmp(animal_list[i-1], animal_list[i])))	/*If 2 strings are equal*/
			{
				printk("string equal\n");	
				animal_nodes_ptr->animal_count ++;
				compare_flag = 0;
			}
			else
			{
				list_add_tail(&(animal_nodes_ptr->list), &(animal_list_head.list));	
				animal_nodes_ptr = (struct animal_struct*) (kmalloc(sizeof(animal_list_head), GFP_KERNEL));
						
					animal_nodes_ptr->animal_type = animal_list[i];
				animal_nodes_ptr->animal_count  = 1;
				compare_flag = 1;
			}
		}
			list_add_tail(&(animal_nodes_ptr->list), &(animal_list_head.list));	
		
		
		list_for_each_entry(animal_nodes_ptr, &animal_list_head.list, list)
		{
			printk("Animal Name: = %s\n", animal_nodes_ptr->animal_type);
			printk("Respective Count = %d\n\n", animal_nodes_ptr->animal_count);
		}
			return 0;

	}


static void animal_ecosystem_end(void)
{
	printk(KERN_INFO "Exited the kernel module\n");
	//del_timer_sync(&tanmay_timer); /* Deleting the timer */
}




module_init(animal_ecosystem_init);
module_exit(animal_ecosystem_end);
