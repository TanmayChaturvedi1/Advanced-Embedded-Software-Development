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


#define	INPUT_ARRAY_SIZE	(50)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanmay Chaturvedi");
MODULE_DESCRIPTION("A Kernel Module that works on Linked List");
MODULE_VERSION("1.0");

static int count_greater_than = 0;	
static char* animal_type = "default";
/*static unsigned long timer_delay = 0; /*User input timer interrupt delay
struct timer_list tanmay_timer;*/

module_param(count_greater_than, int, 0);
module_param(input_animal_type, charp, 0);

static void initial_sort(void);

/*Array of animals*/
static char* animal_list[] = {"Cat", "Cat", "Dog", "Dog", "Alligator", "Mouse", "Tiger", "Zebra", "Donkey", "Rabbit", \
	"Cat", "Cat", "Dog", "Dog", "Alligator", "Mouse", "Tiger", "Zebra", "Anteater", "Rabbit", \
	"Cat", "Cat", "Dog", "Dog", "Alligator", "Mouse", "Tiger", "Zebra", "Ape", "Rabbit", \
	"Cat", "Cat", "Dog", "Dog", "Alligator", "Mouse", "Tiger", "Zebra", "Deer", "Rabbit", \
	"Cat", "Cat", "Dog", "Dog", "Alligator", "Mouse", "Tiger", "Zebra", "Gorilla", "Rabbit", };

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

struct animal_struct_filtered
{
	uint8_t	animal_count_filtered; /* number of occurence */
	char* animal_type_filtered; /* type of animal */
	struct list_head list_filtered; /* list of all animal_struct structures */
}; 

struct animal_struct animal_list_head, *animal_nodes_ptr;
struct animal_struct_filtered animal_list_head_1, *animal_nodes_ptr_1;


static int animal_ecosystem_init(void)
{
	initial_sort();
	return 0;

}
static void initial_sort(void)	
{
	int i;
	int nodes_counter = 0;
	int compare_flag = 0;
	INIT_LIST_HEAD(&animal_list_head.list); /*List head*/
	printk(KERN_INFO "initialize kernel module \n");
	sort(animal_list, INPUT_ARRAY_SIZE, sizeof(animal_list[0]), compare_func, NULL);

	
	for( i = 0; i< INPUT_ARRAY_SIZE; i++)
	{
		printk("Sorted Element = %s\n", animal_list[i]);
	}
 

	animal_nodes_ptr = (struct animal_struct*) (kmalloc(sizeof(animal_list_head),GFP_KERNEL));
	//animal_nodes_ptr->animal_count;
	
		for(i=1; i<INPUT_ARRAY_SIZE; i++)
		{
			if(i==1)
			{
				animal_nodes_ptr->animal_type = animal_list[i-1] ;
					animal_nodes_ptr->animal_count  = 1;
			}
			
						
			if(!(strcmp(animal_list[i-1], animal_list[i])))	/*If 2 strings are equal*/
			{
				
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
				nodes_counter ++;
			}
		}
			list_add_tail(&(animal_nodes_ptr->list), &(animal_list_head.list));	
			nodes_counter ++;
			
			list_for_each_entry(animal_nodes_ptr, &animal_list_head.list, list)
				{
					printk("Animal Name: = %s and Respective Count = %d\n", animal_nodes_ptr->animal_type, animal_nodes_ptr->animal_count);
				}
				printk("Total Nodes = %d", nodes_counter);
				printk("Total amount of memory dynamically allocated for nodes = %d bytes", nodes_counter * sizeof(animal_list_head));
		
		/*Section 1 done before*/
}
		
static void filtered_linked_list(void)
{
	INIT_LIST_HEAD(&animal_list_head_1.list); /*List head for new linked List*/
	
	if(!strcmp(input_animal_type, "default")) /*strings equal*/
	{
		printk("Animal Name: = %s and Respective Count = %d\n", animal_nodes_ptr->animal_type, animal_nodes_ptr->animal_count);
	}
	
	else if (animal_nodes_ptr->animal_count > count_greater_than)
	{
			animal_nodes_ptr_1 = (struct animal_struct_filtered *) (kmalloc(sizeof(animal_list_head_1)));
			animal_nodes_ptr_1->animal_type = animal;
	}
	
	list_for_each_entry(animal_nodes_ptr, &animal_list_head.list, list)/*Iterating on the previously sorted linkedlist*/
	{
		/*default case*/
	if ((!strcmp(input_animal_type, "default")) && count_greater_than == 0 )
	{
			
	}
		
		if ( input_animalnum == "none" )
		{
			if ( input_animaltype  == "none")
			{
				list_for_each_entry(animal_nodes_ptr, &animal_list_head.list, list)
				{
					printk("Animal Name: = %s\n", animal_nodes_ptr->animal_type);
					printk("Respective Count = %d\n\n", animal_nodes_ptr->animal_count);
				}
			}
			else 
			{
				
			}
		}

		
		*/
		



static void animal_ecosystem_end(void)
{
	printk(KERN_INFO "Exited the kernel module\n");
	size_t size = 0;
	list_for_each_entry(animal_nodes_ptr, &animal_list_head.list, list)
	{
	size = size + sizeof(*animal_nodes_ptr);
	kfree(animal_nodes_ptr);
	}

	printk("Total size freed = %d bytes", size);
}




module_init(animal_ecosystem_init);
module_exit(animal_ecosystem_end);
