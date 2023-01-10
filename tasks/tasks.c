#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/module.h>

int tasks_init(void)
{
    printk(KERN_INFO "Loading Tasks Module\n");

    struct task_struct *task;
    for_each_process(task)
    {
        printk(KERN_INFO "Executable Name: %s State: %ld Process ID: %d\n",
        task->comm,
        task->state,
        task->pid);
    };

    return 0;
}

void tasks_exit(void)
{
    printk(KERN_INFO "Removing Tasks Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(tasks_init);
module_exit(tasks_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasks Module");
MODULE_AUTHOR("Chloe Sheasby");