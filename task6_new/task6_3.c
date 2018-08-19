#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define ID_SIZE	(sizeof(id))
#define ID_LEN	(ID_SIZE-1)

static char id[] = "b761d1a77009";

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);

static ssize_t task6_write_id(struct file *file, char const __user *buf,
		size_t count, loff_t *ppos);

const struct file_operations task6_fops = {
	.owner	 = THIS_MODULE,
	.read    = task6_read_id,
	.write   = task6_write_id,
};

static struct miscdevice task6_dev = {
	.minor  = MISC_DYNAMIC_MINOR,
	.name   = "eudyptula",
	.fops   = &task6_fops,
};

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, id, sizeof(id));
}

static ssize_t task6_write_id(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	ssize_t num_bytes;
	char kbuf[ID_SIZE];

	num_bytes = simple_write_to_buffer(kbuf, ID_SIZE, ppos, buf, count);

	if (num_bytes != ID_SIZE ||
			memcmp(id, kbuf, ID_LEN))
		return -EINVAL;
	return ID_SIZE;
}

static void __exit task1_exit(void)
{
	pr_info("Misc Char driver with minor number [%d] unloaded\n",
			task6_dev.minor);
	misc_deregister(&task6_dev);
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
