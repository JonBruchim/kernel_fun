#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include "linux/uaccess.h"

#define MY_IOCTL _IOW(0xFF, 0xFF, int32_t*)

static int perf_enabled __read_mostly;
static DEFINE_SPINLOCK(perf_lock);


static int my_open(struct inode *inode, struct file *file)
{
	spin_lock(&perf_lock);
	if (perf_enabled) {
		spin_unlock(&perf_lock);
		return -EBUSY;
	}
	perf_enabled = 1;
 	spin_unlock(&perf_lock);

	return 0;
}

/*
 * Close the device.
 */
static int my_release(struct inode *inode, struct file *file)
{
	spin_lock(&perf_lock);
	perf_enabled = 0;
	spin_unlock(&perf_lock);

	return 0;
}

/*
 * ioctl routine
 * All routines effect the processor that they are executed on.  Thus you
 * must be running on the processor that you wish to change.
 */

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long error_start;
	uint32_t tmp = 0xFF;
	int error = 0;

	switch (cmd) {
		case MY_IOCTL:
			if (copy_to_user((void __user *)arg, &tmp,
					sizeof (tmp)) != 0) {
				error =  -EFAULT;
			}

			break;
		default:
  	 		error = -ENOTTY;
	}

	return error;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t cnt, loff_t *ppos)
{
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos)
{
	return 0;
}

static const struct file_operations my_fops = {
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl,
	/*.compat_ioctl = perf_ioctl, TODO: Relevant?*/
	.open = my_open,
	.release = my_release
};

static struct miscdevice my_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "my_misc",
	.fops = &my_fops,
	.mode = S_IRWXUGO,
};


/*
 * Initialize the module
 */
static int __init my_init(void)
{
	int ret = 0;

	ret = misc_register(&my_misc_dev);

	if (ret) {
		printk(KERN_ERR "Performance monitoring counters: "
			"cannot register misc device.\n");
		return ret;
	}

	return ret;
}

static void __exit my_exit(void)
{
 	misc_deregister(&my_misc_dev);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}


module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonbru");
MODULE_DESCRIPTION("hello world module");
