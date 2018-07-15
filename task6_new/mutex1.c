#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>

#define ID_SIZE	(sizeof(id))
#define ID_LEN	(ID_SIZE - 1)

static const char id[] = "123456789ABCDEF";

static struct semaphore slock;

static struct mutex mlock;

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
	return 0;
}

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{

	int ret = -1;
	static int i;
	//down(&slock);

#if 1 
	if (mutex_is_locked(&mlock)) {
		printk(KERN_INFO "MUTEX IS LOCKED, exiting..... with EAGAIN.\n");
		ret = -EAGAIN;
	}

	if (mutex_trylock(&mlock)) {
#else
		mutex_lock(&mlock);
#endif

		if (ret = down_trylock(&slock)) {
			printk(KERN_INFO "SEMAPHORE lock acquiring Failed, returning... I:%d ret :%d\n", i++, ret);
			goto unlock;
		}

		/* This is a rare case, though handling is good. */
		if (count < ID_SIZE) {
			ret = -EFAULT;
			goto exit;
		}

		if (copy_to_user(buf, id + *ppos, 1)) {
			ret = -EFAULT;
			goto exit;
		}

		*ppos += 1;
		up(&slock);
		mutex_unlock(&mlock);
	} else {
		printk(KERN_INFO "Mutex lock acquiring Failed\n");
		return -EBUSY;
	}

	return ID_SIZE - *ppos;
exit:
	up(&slock);
unlock:
	mutex_unlock(&mlock);
	return ret;
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

	sema_init(&slock, 1);
	static DEFINE_MUTEX(mlock);

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
