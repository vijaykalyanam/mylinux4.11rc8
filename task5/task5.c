/*
 *
 * Task5: Writing a Module which loads when any USB based Keyboard attached
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vijaya kumar");
MODULE_DESCRIPTION("USB KEYBOARD Hello World!!!");
MODULE_VERSION("t.a.s.k.v5");

static int task5_probe(struct usb_interface *iface,
		const struct usb_device_id *table);
static void task5_disconnect(struct usb_interface *iface);

static int task5_probe(struct usb_interface *iface,
		const struct usb_device_id *table)
{
	pr_info("USB task5, driver is not fully functional");
	return -ENODEV;
}

static void task5_disconnect(struct usb_interface *iface)
{
	pr_info("USB task5 Disconnect");
}

/* table of devices that work with this driver */
static const struct usb_device_id task5UsbKeyboard_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD)
	},
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, task5UsbKeyboard_table);

static struct usb_driver task5UsbKeyboard_driver = {
	.name =	"task5-usbkeyboard",
	.id_table = task5UsbKeyboard_table,
	.probe = task5_probe,
	.disconnect = task5_disconnect,
};

static int __init task5_init(void)
{
	int rc = -1;

	pr_info("USB Keyboard Attached.....");
	rc = usb_register(&task5UsbKeyboard_driver);
	if (rc) {
		pr_err("usb_register Failed .... Return value :0x%x", rc);
		return rc;
	}

	return 0;
}

static void __exit task5_exit(void)
{
	usb_deregister(&task5UsbKeyboard_driver);
	pr_info("USB Keyboard Detatched.");
}

module_init(task5_init);
module_exit(task5_exit);
