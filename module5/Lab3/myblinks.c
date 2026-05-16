#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/tty.h>  
#include <linux/kd.h>   
#include <linux/configfs.h>
#include <linux/vt.h>
#include <linux/vt_kern.h>


struct timer_list my_timer;
struct tty_driver *my_driver;
#define RESTORE_LEDS  0xFF
#define BLINK_DELAY   HZ/5
static struct kobject *my_kobject;
static int led_ctrl;
static int _kbledstatus = 0;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", led_ctrl);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{

        sscanf(buf, "%du", &led_ctrl);
        return count;
}

static struct kobj_attribute foo_attribute =__ATTR(blink_ctrl, 0660, foo_show,
                                                   foo_store);

static void my_timer_func(struct timer_list *ptr)
{
        int *pstatus = &_kbledstatus;
        if (*pstatus == led_ctrl)
                *pstatus = RESTORE_LEDS;
        else
                *pstatus = led_ctrl;
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            *pstatus);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}


static int __init sys_init (void)
{
        int error = 0;
        pr_debug("Module initialized successfully \n");
        my_kobject = kobject_create_and_add("systest",
                                                 kernel_kobj);
        if(!my_kobject)
                return -ENOMEM;

        error = sysfs_create_file(my_kobject, &foo_attribute.attr);
        if (error) {
                pr_debug("failed to create the foo file in /sys/kernel/systest \n");
        }

        int i;
        printk(KERN_INFO "kbleds: loading\n");
        printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
        for (i = 0; i < MAX_NR_CONSOLES; i++) {
                if (!vc_cons[i].d)
                        break;
                printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
                       MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                       (unsigned long)vc_cons[i].d->port.tty);
        }
        printk(KERN_INFO "kbleds: finished scanning consoles\n");
        my_driver = vc_cons[fg_console].d->port.tty->driver;
        printk(KERN_INFO "kbleds: tty driver magic\n");
        timer_setup(&my_timer, my_timer_func, 0);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
        return error;
}

static void __exit sys_exit (void)
{
        printk(KERN_INFO "myblinks: unloading...\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            RESTORE_LEDS);
        pr_debug ("Module un initialized successfully \n");
        kobject_put(my_kobject);
}
MODULE_AUTHOR("Stasyan");
MODULE_DESCRIPTION("Module for blinking a num-, caps- and srolllock LEDs");
MODULE_LICENSE("GPL");
module_init(sys_init);
module_exit(sys_exit);