#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE(" ");

static int my_init(void) {
printk(KERN_NOTICE "heyyyyyyyyyy\n");
return 0;
}

static void my_exit(void) {
printk(KERN_NOTICE "ggggggggg\n");
return;
}

module_init(my_init);
module_exit(my_exit);
