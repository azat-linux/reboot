#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/printk.h>
#include <linux/stacktrace.h>
#include <linux/delay.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Azat Khuzhin <a3at.mail@gmail.com>");

static int rebootInit(void) __init;
static void rebootExit(void) __exit;

module_init(rebootInit);
module_exit(rebootExit);


int rebootCallback(struct notifier_block *nb, unsigned long action, void *data)
{
    struct task_struct *p = current->parent;

    printk(KERN_ALERT "Reboot called by:\n");
    while (p) {
        printk(KERN_ALERT "\tPID: %d Comm: %.20s -> \n",
               p->pid, p->comm);

        p = current->real_parent;
    }
    dump_stack();
    /** Sleep for 10 secs */
    msleep(10 * 1000);

    return 0;
}
struct notifier_block rebootNotifier = {
    .notifier_call = rebootCallback,
    .priority = INT_MAX,
};

static int __init rebootInit(void)
{
    return register_reboot_notifier(&rebootNotifier);
}
static void __exit rebootExit(void)
{
    unregister_reboot_notifier(&rebootNotifier);
}
