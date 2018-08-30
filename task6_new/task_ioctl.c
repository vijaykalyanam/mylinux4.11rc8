/*
 * 
root@vkalyanam-Lenovo-B41-80:~/workstation/eudyptula/mylinux4.11rc8/task6_new# ./a.out 
fd : 3
Killed

If we don't use COPY_FROM/TO_USER, kernel will kill user process.
And,
Kernel Trace will be dumped in dmesg.



Stack:
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206205]  ffff8801acc7bd18 ffffffff8140ba5b ffffffff8211ab8a ffffffff8211af40
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206334]  ffff8801acc7be30 ffffffffc086115c ffffffffc086115c ffff8801acc7bd88
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206463]  ffffffff8140d515 0000000000000001 ffff8801acc7bd88 0000000000000007
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206610] Call Trace:
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206678]  [<ffffffff8140ba5b>] string.isra.4+0x3b/0xd0
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206799]  [<ffffffff8140d515>] vsnprintf+0x135/0x530
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206882]  [<ffffffff8140d91d>] vscnprintf+0xd/0x30
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.206972]  [<ffffffff810dcc73>] vprintk_emit+0x123/0x520
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207075]  [<ffffffff810dd1f9>] vprintk_default+0x29/0x40
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207199]  [<ffffffff811927d0>] printk+0x5a/0x76
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207281]  [<ffffffffc0860029>] task6_ioctl+0x29/0x40 [task_misc]
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207378]  [<ffffffff8122904f>] do_vfs_ioctl+0x2af/0x4b0
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207464]  [<ffffffff812153e9>] ? vfs_write+0x149/0x1a0
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207549]  [<ffffffff812292c9>] SyS_ioctl+0x79/0x90
Aug 30 17:36:51 vkalyanam-Lenovo-B41-80 kernel: [77757.207630]  [<ffffffff81851dc8>] entry_SYSCALL_64_fastpath+0x1c/0xbb

*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define ID_SIZE	(sizeof(id))
#define ID_LEN	(ID_SIZE-1)

static char id[] = "b761d1a77009";

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);

static ssize_t task6_write_id(struct file *file, char const __user *buf,
		size_t count, loff_t *ppos);

static long task6_ioctl(struct file *file, unsigned int cmd, unsigned long data);

const struct file_operations task6_fops = {
	.owner	 = THIS_MODULE,
	.read    = task6_read_id,
	.write   = task6_write_id,
	.unlocked_ioctl	 = task6_ioctl,
};

static struct miscdevice task6_dev = {
	.minor  = MISC_DYNAMIC_MINOR,
	.name   = "eudyptula",
	.fops   = &task6_fops,
};

static long task6_ioctl(struct file *file, unsigned int cmd, unsigned long data)
{
	char *p;
	char buf[32];
	if (cmd)
		p = (char *)data;
	//printk("[%s] --> DUMP STACK cmd :%d data :%s\n", __func__, cmd, p);
	copy_from_user(buf, (const void *) data, sizeof("VIJAY"));
	printk("[%s] --> DUMP STACK cmd :%d data :%s\n", __func__, cmd, buf);

	dump_stack();
	return 0;
}

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
