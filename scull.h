#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/fs.h>

#define SCULL_MAJOR 0
#define DEV_NAME "scull"
int scull_major=0,scull_minor=0;
int scull_nr_devs=1;
dev_t dev;

struct scull_qset{
	void **data;
	struct scull_qset *next;
};

struct scull_dev{
	struct scull_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	unsigned int access_key;
	struct semaphore sem;
	struct cdev cdev;
};

loff_t llseek(struct file* filep,loff_t )

//this function is used to release all data in data field
int scull_trim(struct scull_dev *dev);

//find item-th scull_qset
struct scull_qset* scull_follow(struct scull_dev *dev, int item);

int scull_open(struct innode *inode,struct file *filp);

int scull_release(struct inode *inode,struct file *filp){
	return 0;
}

ssize_t scull_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);

ssize_t scull_write(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);

struct file_operation scull_fops={
	.ower =     THIS_MODULE,
	.llseek =   scull_llseek,
	.read =     scull_read,
	.write =    scull_write,
	.ioctl =    scull_ioctl,
	.open =     scull_open,
	.release =  scull_release,
};