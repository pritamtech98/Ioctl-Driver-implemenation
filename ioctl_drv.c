#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/ioctl.h>
#include<linux/jiffies.h>
#define RD_JIFFIE _IOR('a', 1, unsigned long *)
#define PRINT_JIFFIE _IO('a', 2)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRIT");
MODULE_DESCRIPTION("The ioctl sys call implementation");
#define MINOR_NO 0

dev_t dev=0;
struct cdev *my_cdev;


static int my_open(struct inode *inode, struct file *f){
  return 0;
}


static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
  unsigned long jiff = jiffies;
  switch(cmd){
    case RD_JIFFIE :
      if(copy_to_user((unsigned long *)arg, &jiff, sizeof(unsigned long))){
        return -1;
      }
      break;

    case PRINT_JIFFIE:
      printk(KERN_INFO"Value of jiffie is %lu", jiff);
      break;

    default:
      return -1;
      break;
  }

  return 0;
}


static int my_release(struct inode *inode, struct file *f){
  printk(KERN_INFO"dev closed success\n");
  return 0;
}

struct file_operations my_dev_ops = {
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_release,
  .unlocked_ioctl = my_ioctl,
};

static int __init chr_dev_init(void){
  int val, major_no, minor_no;
  if((val = alloc_chrdev_region(&dev, MINOR_NO, 1, "my_dev")) < 0){
    printk(KERN_INFO"failed to register the device numbers\n");
    return -1;
  }
  major_no = MAJOR(dev);
  minor_no = MINOR(dev);

  my_cdev = cdev_alloc();
  my_cdev->owner = THIS_MODULE;
  my_cdev->ops = &my_dev_ops;
  if((val = cdev_add(my_cdev,dev, 1)) < 0){
    printk(KERN_INFO"failed to add the device driver\n");
    return -1;
  }
  printk(KERN_INFO"Dev added success with major no as : {%d} and minor no as : {%d}\n", major_no, minor_no);
  return 0;
}


static void __exit chr_dev_exit(void){
  unregister_chrdev_region(dev, 1);
  cdev_del(my_cdev);
  printk(KERN_INFO"device removed.. \n");
}


module_init(chr_dev_init);
module_exit(chr_dev_exit);
