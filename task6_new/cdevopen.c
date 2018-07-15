#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define ID_SIZE	(sizeof(id))
#define ID_LEN	(ID_SIZE - 1)

static const char id[] = "b761d1a77009";
static int task6_open (struct inode *inode, struct file *filp);

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);

static ssize_t task6_write_id(struct file *file, char const __user *buf,
		size_t count, loff_t *ppos);

static struct file_operations task6_fops = {
	.owner	= THIS_MODULE,
	.open	= task6_open,
	.read	= task6_read_id,
	.write	= task6_write_id,
};

static struct miscdevice task6_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "eudyptula",
	.fops	= &task6_fops,
};

static int task6_open (struct inode *inode, struct file *filp)
{
	static int ref;

	printk(KERN_INFO "Char driver Opened, ref count :%d\n", ref++);
	return 0;
}

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{

	/* This is a rare case, though handling is good. */
	if (count < ID_SIZE)
		return -EFAULT;

	if (copy_to_user(buf, id + *ppos, 1))
		return -EFAULT;
	*ppos += 1;

	return ID_SIZE - *ppos;
}

static ssize_t task6_write_id(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char kbuf[ID_SIZE];

	if (count != ID_SIZE)
		return -EINVAL;

	if (copy_from_user(kbuf, buf, ID_SIZE))
		return -EFAULT;

	if (memcmp(id, kbuf, ID_LEN))
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
