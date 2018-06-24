/**
 * Author: Dr. Neerja Mhaskar
 * Course: Operating Systems CS 3SH3, Winter 2018
 * Solution to Practice lab 1b.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

/*Function to print few members of init-task*/

void print_init_PCB(void)
{
	struct task_struct *task;
	task=&init_task;
	printk(KERN_INFO "init pid:%d\n",task->pid);
	printk(KERN_INFO "init state:%ld (-1 => not runable; 0 => runable; 1 => stopped)\n",task->state);
	printk(KERN_INFO "flags:%d\n", task->flags);
	printk(KERN_INFO "runtime priority:%d\n", task->rt_priority);
	printk(KERN_INFO "process policy:%d\n", task->policy);
	printk(KERN_INFO "tgid:%d\n", task->tgid);
	printk(KERN_INFO "link_count:%d\n", task->link_count);
	printk(KERN_INFO "comm:%s\n",task->comm);

}
/* This function is called when the module is loaded. */
int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");
    print_init_PCB(); 
    return 0;
}


/* This function is called when the module is removed. */
void simple_exit(void) 
{
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

