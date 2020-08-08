#include "scull.h"

int scull_trim(struct scull_dev *dev){
	struct scull_qset *next, *dptr;
	int qset=dev->qset;
	for(dptr=dev->data;dptr;dptr=next){
		if(dptr->data){
			for(int i=0;i<qset;i++)
				kfree(dptr->data[i]);
			kfree(dptr->data);
			dptr->data=NULL
		}
		next=dptr->next;
		kfree(dptr);
	}
	dev->size=0;
	dev->quantum=scull_quantum;
	dev->qset=scull_qset;
	dev->data=NULL;
	return 0;
}

struct scull_qset* scull_follow(struct scull_dev *dev, int item){
	struct scull_qset *dptr;
	dptr=dev->data;
	for(nt i=0; i < item; i++){
		dptr=dptr->next
	}
	return dptr;
}


ssize_t scull_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos){
	struct scull_dev *dev=filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;

	if(down_interruptible(&dev->sem)){
		return -ERESTARTSYS;
	} 

	if(*f_pos >= dev->size)
		goto out;
	if(*f_pos + count > dev->size)
		count=dev->size - *f_pos;

	item = (long)*f_pos/itemsize;
	rest = (long)*f_pos%itemsize;
	s_pos = rest / quantum;
	q_pos = rest % quantum;

	dptr = scull_follow(dev, item);

	if(dptr == NULL || !dptr->data || !dptr->data[s_pos])
		goto out;

	if(count > quantum - q_pos)
		count = quantum - q_pos;

	if(copy_to_user(buff, dptr->data[s_pos] + q_pos, count)){
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;

  out:
  	up(&dev->sem);
  	return retval;
}

ssize_t scull_write(struct file *filp, char __user *buff, size_t count, loff_t *f_pos){
	
}

int scull_open(struct innode *inode,struct file *filp){
	struct scull_dev *dev;

	dev = contain_of(inode->i_cdev,struct scull_dev,cdev);
	filp->private_data=dev;

	if ((filp->flags&O_ACCMODE)==O_WRONLY)
	{
		scull_trim(dev);
	}
	return 0
}



static int __init scull_init(void){
	int result;
	if(scull_mojor){
		dev=MKDEV(scull_mojor,scull_minor);
		result=register_chrdev_region(dev,scull_nr_devs,DEV_NAME);
	}else{
		result=alloc_chrdev_region(&dev,scull_minor,scull_nr_devs,DEV_NAME);
		scull_major=MAJOR(dev);
	}
	if(result<0){
		printk(KERNRL_WARNING"scull:can't get major %d\n",scull_major);
		return result;
	}
}

module_init(scull_init);
module_exit(scull_exit);