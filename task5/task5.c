/*
 *
 * Task5: Writing a Module which loads when any USB based Keyboard attached
 *        This could be loaded using depmod utility.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vijaya kumar");
MODULE_DESCRIPTION("Task5 Usb Keyboard");
MODULE_VERSION("t.a.s.k.v5");

/* table of devices that work with this driver */
static const struct usb_device_id task5UsbKeyboard_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD)
	},
	/* Terminating entry */
	{ }
};
MODULE_DEVICE_TABLE(usb, task5UsbKeyboard_table);

static int __init task5_init(void)
{
	pr_info("USB Keyboard Attached....Loading Module 'usbcall2'\n");
	return 0;
}

static void __exit task5_exit(void)
{
	pr_info("'usbcall2' module unloaded.\n");
}

module_init(task5_init);
module_exit(task5_exit);
