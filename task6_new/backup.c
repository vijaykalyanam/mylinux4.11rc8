#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static char id[] = "b761d1a77009";

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);

const struct file_operations task6_fops = {
        .owner	 = THIS_MODULE,
        .read    = task6_read_id,
};

static struct miscdevice task6_dev = {
        .minor  = MISC_DYNAMIC_MINOR,
        .name   = "testing",
        .fops   = &task6_fops,
};

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	copy_to_user(buf, &id[*ppos], 1);
	*ppos += 1;
	return sizeof(id)-*ppos-1;
}

static void __exit task1_exit(void)
{
	pr_info("Misc Char driver with minor number [%d] unloaded\n",
			task6_dev.minor);
	misc_deregister(&task6_dev);
	return;
}

static int __init task1_init(void)
{
	int ret = -1;
	ret = misc_register(&task6_dev);
	if (ret == 0)
		pr_info("Misc Char driver with minor number [%d] registered\n",
				task6_dev.minor);
	else
		pr_info("Misc Char driver registerion failed with error [%d]",
				ret);
	return ret;
}

module_init(task1_init);
module_exit(task1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("[b761d1a77009]");
MODULE_DESCRIPTION("Task6: Miscellaneous char device driver");
