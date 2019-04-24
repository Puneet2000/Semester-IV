#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>

#include <myshm.h>
//#include <linux/fcntl.h> // for open
//#include <linux/unistd.h> // for close

MODULE_DESCRIPTION("My kernel module - mykmod");
MODULE_AUTHOR("cs17btech11029 [at] iith.ac.in");
MODULE_LICENSE("GPL");

// Dynamically allocate major no
#define MYKMOD_DEV_MAJOR 0
#define BUFFER_SIZE 4096
static int mykmod_init_module(void);
static void mykmod_cleanup_module(void);

static int mykmod_open(struct inode *inode, struct file *filp);
static int mykmod_close(struct inode *inode, struct file *filp);
static ssize_t mykmod_read(struct file *, char *, size_t, loff_t *);
static ssize_t mykmod_write(struct file *, const char *, size_t, loff_t *);
static loff_t mykmod_lseek(struct file *, loff_t, int);
static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma);

module_init(mykmod_init_module);
module_exit(mykmod_cleanup_module);

static struct file_operations mykmod_fops = {
	.owner  = THIS_MODULE,	/* owner (struct module *) */
	.open   = mykmod_open,	/* open */
	.release  = mykmod_close,     /* release */
	.read   = mykmod_read,	/* read */
	.write  = mykmod_write,	/* write */
	.llseek = mykmod_lseek, /* lseek */
	.mmap = mykmod_mmap,	/* mmap */
};

static void mykmod_vm_open(struct vm_area_struct *vma);
static void mykmod_vm_close(struct vm_area_struct *vma);
//static int mykmod_vm_fault(struct vm_fault *vmf);
static int mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf);

struct mykmod_dev_info {
	char *data;
	size_t size;
	int reference;
};

static const struct vm_operations_struct mykmod_vm_ops = {
	.open  = mykmod_vm_open,
	.close = mykmod_vm_close,
	.fault = mykmod_vm_fault
};

int mykmod_major;

static int mykmod_init_module(void) {

	printk("mykmod loaded\n");
	printk("mykmod initialized at=%p\n", init_module);

	if ((mykmod_major = register_chrdev(MYKMOD_DEV_MAJOR,"mykmod",&mykmod_fops))<0) {
		printk(KERN_WARNING "Failed to register character device\n");
		return 1;
	}
	else {
		printk("register character device %d\n", mykmod_major);
		printk("&fops=%p open=%p close=%p\n", &mykmod_fops, mykmod_open, mykmod_close);
	}

	return 0;
}

static void mykmod_cleanup_module(void) {

	printk("mykmod unloaded\n");
	unregister_chrdev(mykmod_major,"mykmod");

	return;
}

static int
mykmod_open(struct inode *inodep, struct file *filep)
{
	// TODO 1
	struct mykmod_dev_info *info;
	printk("mykmod_open: inodep=%p filep=%p i->private_data=%p f->private_data=%p\n",
		inodep, filep, inodep->i_private, filep->private_data);

	if(!inodep->i_private){
		info = kmalloc(sizeof(struct mykmod_dev_info), GFP_KERNEL);
		info->data = (char*)kmalloc(1024*1024, GFP_KERNEL);
		memcpy(info->data, "Opening File",12);
		inodep->i_private = info;
	}

	return 0;
}

static int
mykmod_close(struct inode *inodep, struct file *filep)
{
	// TODO 2
	struct mykmod_dev_info *info;
	printk("mykmod_close: inodep=%p filep=%p\n", inodep, filep);
	info = filep->private_data;
	//kfree(info);
	//inodep->i_private = NULL;

	return 0;
}

static ssize_t
mykmod_read(struct file *filep, char *userbufp, size_t size, loff_t *loffp)
{
	
	// TODO 3
	struct mykmod_dev_info *info;
	printk("mykmod_read: filep=%p userbuf=%p size=%lu loff=%lld\n", filep, userbufp, size, *loffp);
	info = filep->f_inode->i_private;
	size = (size>BUFFER_SIZE)? BUFFER_SIZE : size;
	if (copy_to_user(userbufp,info->data+(*loffp),size)!=0)
		return -size;
	(*loffp)+=size;
	return size;
}

static ssize_t
mykmod_write(struct file *filep, const char *userbufp, size_t size, loff_t *loffp)
{
	// TODO 4
	struct mykmod_dev_info *info;
	printk("mykmod_write: filep=%p userbuf=%p size=%lu loff=%lld\n", filep, userbufp, size, *loffp);
	info = filep->f_inode->i_private;
	size = (size>BUFFER_SIZE)? BUFFER_SIZE : size;
	if (copy_from_user(info->data+(*loffp), userbufp, size)!=0)
		return -size;
	(*loffp)+=size;
	return size;
}

static loff_t
mykmod_lseek(struct file *filep, loff_t loff, int whence)
{
	loff_t pos;
	printk("mykmod_lseek: filep=%p loff=%lld whence=%d\n", filep, loff, whence);

	// TODO 5
	if(whence == SEEK_SET)
		pos = loff;
	else if (whence == SEEK_CUR)
		pos = filep->f_pos + loff;
	else if (whence == SEEK_END)
		pos = filep->f_inode->i_size + loff;
	else
		return -EINVAL;

	if (pos<0) return -EINVAL;
	filep->f_pos = pos;

	return pos;
}

static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma)
{
	printk("mykmod_mmap: filp=%p vma=%p\n", filp, vma);

	// TODO 6
	vma->vm_ops = &mykmod_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_private_data = filp->f_inode->i_private;
	mykmod_vm_open(vma);

	return 0;
}

static void
mykmod_vm_open(struct vm_area_struct *vma)
{
	// TODO 7
	struct mykmod_dev_info *info;
	printk("mykmod_vm_open: vma=%p\n", vma);
	info = (struct mykmod_dev_info*)vma->vm_private_data;
	info->reference++;
}

static void
mykmod_vm_close(struct vm_area_struct *vma)
{
	// TODO 8
	struct mykmod_dev_info *info;
	printk("mykmod_vm_close: vma=%p\n", vma);
	info = (struct mykmod_dev_info*)vma->vm_private_data;
	info->reference--;
}

static int
mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	// TODO 9
	struct page *page;
	struct mykmod_dev_info *info;
	printk("mykmod_vm_fault: vma=%p vmf=%p pgoff=%lu\n", vma, vmf, vmf->pgoff);
	info = (struct mykmod_dev_info*)vma->vm_private_data;
	if(info->data){
		page = virt_to_page(info->data);
		get_page(page);
		vmf->page =  page;
	}
	return 0;
}



