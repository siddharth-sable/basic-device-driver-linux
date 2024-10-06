#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>  // For copy_from_user and copy_to_user
#include <linux/slab.h>     // For kmalloc and kfree

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Sabale");
MODULE_DESCRIPTION("Mentor and Teacher - @inpyjama on youtube");

static struct proc_dir_entry *custom_proc_node;
#define BUFFER_SIZE 100

static char *message_buffer;
static size_t message_length = 0;

static ssize_t pyjama_read(struct file* file_pointer, char *user_space_buffer, size_t count, loff_t* offset) {
    ssize_t len = message_length;

    printk("pyjama_read\n");

    if (*offset >= len || message_length == 0) {
        return 0; 
    }

    if (copy_to_user(user_space_buffer, message_buffer, len)) {
        return -EFAULT; 
    }

    *offset += len;
    return len;
}

static ssize_t pyjama_write(struct file* file_pointer, const char __user *user_space_buffer, size_t count, loff_t* offset) {
    size_t bytes_to_copy = min(count, (size_t)BUFFER_SIZE - 1);  // Ensure we don't overflow the buffer

    printk("pyjama_write\n");

    // Clear the buffer before writing new data
    memset(message_buffer, 0, BUFFER_SIZE);

    // Copy the data from user space into the kernel buffer
    if (copy_from_user(message_buffer, user_space_buffer, bytes_to_copy)) {
        return -EFAULT;  
    }

    message_length = bytes_to_copy;  // Store the length of the message
    message_buffer[bytes_to_copy] = '\0';  // Null-terminate the string

    printk("Received from user: %s\n", message_buffer);

    return count; 
}

struct proc_ops driver_proc_ops = {
    .proc_read = pyjama_read,
    .proc_write = pyjama_write,
};


static int pyjama_module_init(void) {
    printk("pyjama_module_init: entry\n");

    // Allocate memory for the message buffer
    message_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!message_buffer) {
        printk("pyjama_module_init: error allocating memory\n");
        return -ENOMEM;
    }

    // Create a /proc entry
    custom_proc_node = proc_create("pyjama_driver", 0666, NULL, &driver_proc_ops);
    if (custom_proc_node == NULL) {
        printk("pyjama_module_init: error creating proc entry\n");
        kfree(message_buffer);  // Free allocated memory
        return -1;
    }

    printk("pyjama_module_init: exit\n");
    return 0;
}

static void pyjama_module_exit(void) {
    printk("pyjama_module_exit: entry\n");

    proc_remove(custom_proc_node);  // Remove the /proc entry
    kfree(message_buffer);  // Free the message buffer

    printk("pyjama_module_exit: exit\n");
}

module_init(pyjama_module_init)
module_exit(pyjama_module_exit)
