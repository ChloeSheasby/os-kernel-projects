#include <linux/init.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/module.h>

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(birthday_list);

//This function creates and traverses all birthdays.
int load_birthdays(void)
{
    //create birthdays
    struct birthday *person1, *person2, *person3, *person4, *person5;
    
    person1 = kmalloc(sizeof(*person1), GFP_KERNEL);
    person1->day = 24;
    person1->month = 1;
    person1->year = 1969;
    INIT_LIST_HEAD(&person1->list);
    list_add_tail(&person1->list, &birthday_list);

    person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
    person2->day = 24;
    person2->month = 11;
    person2->year = 1971;
    INIT_LIST_HEAD(&person2->list);
    list_add_tail(&person2->list, &birthday_list);
    
    person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
    person3->day = 13;
    person3->month = 9;
    person3->year = 2000;
    INIT_LIST_HEAD(&person3->list);
    list_add_tail(&person3->list, &birthday_list);

    person4 = kmalloc(sizeof(*person4), GFP_KERNEL);
    person4->day = 10;
    person4->month = 2;
    person4->year = 2004;
    INIT_LIST_HEAD(&person4->list);
    list_add_tail(&person4->list, &birthday_list);

    person5 = kmalloc(sizeof(*person5), GFP_KERNEL);
    person5->day = 1;
    person5->month = 1;
    person5->year = 1;
    INIT_LIST_HEAD(&person5->list);
    list_add_tail(&person5->list, &birthday_list);

    //print birthdays
    struct birthday *ptr;
    int num = 1;
    list_for_each_entry(ptr, &birthday_list, list)
    {
        printk(KERN_INFO "%d", num);
        printk(KERN_INFO " Birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
        num++;
    }
}

/* This function is called when the module is loaded. */
int birthday_init(void)
{
    printk(KERN_INFO "Loading Birthday Module\n");
    load_birthdays();

    return 0;
}

//This function deletes the linked list from the kernel memory
int remove_birthdays(void)
{
   struct birthday *ptr, *next;

    list_for_each_entry_safe(ptr, next, &birthday_list, list)
    {
        printk(KERN_INFO "Removing Birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
        list_del(&ptr->list);
        kfree(ptr);
    }
}

/* This function is called when the module is removed. */
void birthday_exit(void)
{
    printk(KERN_INFO "Removing Birthday Module\n");
    remove_birthdays();
    printk(KERN_INFO "Memory freed.\n");
}

/* Macros for registering module entry and exit points. */
module_init(birthday_init);
module_exit(birthday_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Birthday Module");
MODULE_AUTHOR("Chloe Sheasby");