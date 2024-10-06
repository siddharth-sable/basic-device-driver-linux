#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL"); //*
MODULE_AUTHOR("Siddharth Sabale"); //!
MODULE_DESCRIPTION("Basic dynamically"); //! 

const int permission = 0666;

static struct proc_dir_entry *custom_proc_node;

static ssize_t pyjama_read(struct file* file_pointer, char *user_space_buffer, size_t count, loff_t* offset){
    char msg[] = "Ack!\n";
    size_t len = strlen(msg);
    int result;
    printk("pyjama_read\n");

    if(*offset >= len){
        return 0;
    }

    result = copy_to_user(user_space_buffer, msg, len);
    *offset += len;

    return len;
}

static ssize_t pyjama_write(struct file* file_pointer, const char __user *user_space_buffer, 
size_t count, loff_t* offset){
    char kernel_buffer[100];  // Assuming the write will not exceed 100 bytes
    size_t bytes_to_copy = min(count, sizeof(kernel_buffer));

    printk("pyjama_write\n");

    if (copy_from_user(kernel_buffer, user_space_buffer, bytes_to_copy)) {
        return -EFAULT;  // Return error if copy fails
    }
    return count;
}

struct proc_ops driver_proc_ops = {
    .proc_read = pyjama_read,
    .proc_write = pyjama_write
};

static int pyjama_module_init (void) {
    printk("pyjama_module_init: entry\n");

    custom_proc_node = proc_create("pyjama_driver",
                                permission,
                                NULL,
                                &driver_proc_ops);
    if (custom_proc_node == NULL) {
        printk("pyjama_module_init: error\n");
        return -1;
    }

    printk("pyjama_module_init: exit\n");
    return 0;
}

static void pyjama_module_exit(void) {
    printk("pyjama_module_exit: entry\n");

    proc_remove(custom_proc_node);

    printk("pyjama_module_exit: exit\n");
}

module_init(pyjama_module_init)
module_exit(pyjama_module_exit)