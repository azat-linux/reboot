#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/printk.h>
#include <linux/stacktrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Azat Khuzhin <a3at.mail@gmail.com>");

static int rebootInit(void) __init;
static void rebootExit(void) __exit;

module_init(rebootInit);
module_exit(rebootExit);


int rebootCallback(struct notifier_block *nb, unsigned long action, void *data)
{
    printk(KERN_ALERT "Reboot pressed\n");
    dump_stack();

    return 0;
}
struct notifier_block rebootNotifier = {
    .notifier_call = rebootCallback,
};

static int __init rebootInit(void)
{
    return register_reboot_notifier(&rebootNotifier);
}
static void __exit rebootExit(void)
{
    unregister_reboot_notifier(&rebootNotifier);
}
