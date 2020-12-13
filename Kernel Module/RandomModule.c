#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static dev_t first; // Global variable for the first device number
static struct cdev _cdev; // Global variable for the character device structure
static struct class *_class; // Global variable for the device class

static int valueRandom; //Global variable for the result of random module

//Function open device: Base initialization to others task can active 
static int device_open(struct inode* inode, struct file *file)
{
    printk(KERN_INFO "Driver: open!!\n");
    return 0;
}

//Function close device: Destroy all 
static int device_close(struct inode *inode, struct file *file){
    printk(KERN_INFO "Driver: close!!");
    return 0;
}

/* Function read the content int device: This function is called whenever device is being read from user space 
 data is being sent from the device to the user. In this case is uses the copy_to_user()
  function to send the buffer string to the user and captures any errors.*/
static ssize_t device_read(struct file *file, char __user *buffer, size_t count, loff_t *position){

     int error;
    printk(KERN_INFO "Driver: read!!");

    get_random_bytes(&valueRandom,4);
    printk(KERN_INFO "Random Number: %d",valueRandom);
    if (*position > 0) return 0;

    error = copy_to_user(buffer,&valueRandom, sizeof(int));
    if (error==0)
    {
        return 0;
    }
    else
    {
        return -EFAULT;
    }
}

//Declare sturct file
static struct file_operations random_fOperations =
{
        .owner = THIS_MODULE,
        .open = device_open,
        .release = device_close,
        .read = device_read,
};

//Function create new device
static int __init _init(void){
    printk(KERN_INFO "Driver: registered");
    //register dynamic region
    if (alloc_chrdev_region(&first, 0, 1, "RandomNumber") < 0)
    {
        return -1;
    }

    /* This is used to create a struct class pointer that can then be used in
       calls to device_create.*/
    if ((_class = class_create(THIS_MODULE, "charDevice")) == NULL)
    {
        unregister_chrdev_region(first, 1);
        return -1;
    }

    /*This function can be used by char device classes.
     A struct device will be created in sysfs, registered to the specified class.*/
    if (device_create(_class, NULL, first, NULL, "randomNumber") == NULL)
    {
        class_destroy(_class);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    //character device will have to be initialized
    cdev_init(&_cdev, &random_fOperations);
    if (cdev_add(&_cdev, first, 1) == -1)//the kernel will have to be notified
    {
        device_destroy(_class, first);
        class_destroy(_class);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    return 0;
}

static void __exit exitModule(void) /* Destructor */
{
     
    cdev_del(&_cdev); //function remove device
    device_destroy(_class, first);//destroy a struct device
    class_destroy(_class);//
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Driver: unregistered");
}

module_init(_init);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("18120331 - 18120352 - 18120408");
MODULE_DESCRIPTION("Generate a random number 4 bytes");
module_exit(exitModule);