
#include "device_file.h"
#include <linux/fs.h>         /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <linux/uaccess.h>  /* copy_to_user() */
#include <linux/device.h>
#include <asm/delay.h>
#include <linux/io.h>
#include <linux/gpio.h>

#define time_count 1000



/*===============================================================================================*/
static ssize_t device_file_write(struct file *filp, const char *user_buffer, size_t count, loff_t *pos) {
    char c;
    if (copy_from_user(&c, user_buffer, sizeof(char))) {
        return -EFAULT;
    }
    if (c == '0') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 0);
        gpio_set_value(20, 0);
        gpio_set_value(21, 0);
        gpio_set_value(22, 1);
    } else if (c == '1') {
        gpio_set_value(16, 1);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 1);
        gpio_set_value(20, 1);
        gpio_set_value(21, 1);
        gpio_set_value(22, 1);
    } else if (c == '2') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 0);
        gpio_set_value(18, 1);
        gpio_set_value(19, 0);
        gpio_set_value(20, 0);
        gpio_set_value(21, 1);
        gpio_set_value(22, 0);
    } else if (c == '3') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 0);
        gpio_set_value(20, 1);
        gpio_set_value(21, 1);
        gpio_set_value(22, 0);
    } else if (c == '4') {
        gpio_set_value(16, 1);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 1);
        gpio_set_value(20, 1);
        gpio_set_value(21, 0);
        gpio_set_value(22, 0);
    } else if (c == '5') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 1);
        gpio_set_value(18, 0);
        gpio_set_value(19, 0);
        gpio_set_value(20, 1);
        gpio_set_value(21, 0);
        gpio_set_value(22, 0);
    } else if (c == '6') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 1);
        gpio_set_value(18, 0);
        gpio_set_value(19, 0);
        gpio_set_value(20, 0);
        gpio_set_value(21, 0);
        gpio_set_value(22, 0);
    } else if (c == '7') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 1);
        gpio_set_value(20, 1);
        gpio_set_value(21, 1);
        gpio_set_value(22, 1);
    } else if (c == '8') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 0);
        gpio_set_value(20, 0);
        gpio_set_value(21, 0);
        gpio_set_value(22, 0);
    } else if (c == '9') {
        gpio_set_value(16, 0);
        gpio_set_value(17, 0);
        gpio_set_value(18, 0);
        gpio_set_value(19, 0);
        gpio_set_value(20, 1);
        gpio_set_value(21, 0);
        gpio_set_value(22, 0);
    }
    return 1;
}

static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer,size_t count, loff_t *possition){
    printk(KERN_NOTICE"Andrej-driver: Device file is read at offset = %i, read bytes count = %u", (int)*possition,(unsigned int)count );
    
    char output_message[35];
    
    int a = gpio_get_value(16);
    int b = gpio_get_value(17);
    int c = gpio_get_value(18);
    int d = gpio_get_value(19);
    int e = gpio_get_value(20);
    int f = gpio_get_value(21);
    int g = gpio_get_value(22);
    
    if (a==0 && b==0 && c==0 && d==0 && e==0 && f==0 && g ==1){
        char g[35]= "0 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==1 && b==0 && c==0 && d==1 && e==1 && f==1 && g ==1){
        char g[35] = "1 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==0 && c==1 && d==0 && e==0 && f==1 && g ==0){
        char g[35] = "2 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==0 && c==0 && d==0 && e==1 && f==1 && g ==0){
        char g[35] = "3 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==1 && b==0 && c==0 && d==1 && e==1 && f==0 && g ==0){
        char g[35] = "4 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==1 && c==0 && d==0 && e==1 && f==0 && g ==0){
        char g[35] = "5 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==1 && c==0 && d==0 && e==0 && f==0 && g ==0){
        char g[35] = "6 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==0 && c==0 && d==1 && e==1 && f==1 && g ==1){
        char g[35] = "7 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==0 && c==0 && d==0 && e==0 && f==0 && g ==0){
        char g[35] = "8 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    else if(a==0 && b==0 && c==0 && d==0 && e==1 && f==0 && g ==0){
        char g[35] = "9 has been displayed!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }else{
        char g[35] = "Something went terribly wrong!\n\0";
        size_t destination_size = sizeof (g);
        strncpy(output_message, g, destination_size);
    }
    
    const ssize_t output_message_size = sizeof(output_message);
    
    if( *possition >= output_message_size  )
        return 0;
    
    if( *possition + count > output_message_size  )
        count = output_message_size  - *possition;
    
    if(copy_to_user(user_buffer, output_message+ *possition, count) != 0 )
        return -EFAULT;
    
    *possition += count;
    return count;
}

/*===============================================================================================*/
static struct file_operations simple_driver_fops =
{
    .owner   = THIS_MODULE,
    .write   = device_file_write,
    .read    = device_file_read,
};

static int device_file_major_number = 0;

static const char device_name[] = "Andrej-driver";

/*===============================================================================================*/
void pix_gpio_init(void) {
    printk(KERN_INFO"Andrej-driver: Initializing the gpio.");
    
    gpio_request(16, "a");
    gpio_request(17, "b");
    gpio_request(18, "c");
    gpio_request(19, "d");
    gpio_request(20, "e");
    gpio_request(21, "f");
    gpio_request(22, "g");
    
    gpio_direction_output(16, 1);
    gpio_direction_output(17, 1);
    gpio_direction_output(18, 1);
    gpio_direction_output(19, 1);
    gpio_direction_output(20, 1);
    gpio_direction_output(21, 1);
    gpio_direction_output(22, 1);
    
    printk(KERN_INFO"Andrej-driver: Initialization of the gpio is done.");
}

void pix_gpio_exit(void) {
    printk(KERN_INFO"Andrej-driver: stopping gpio...");
    gpio_free(16);
    gpio_free(17);
    gpio_free(18);
    gpio_free(19);
    gpio_free(20);
    gpio_free(21);
    gpio_free(22);
    printk(KERN_INFO"Andrej-driver: stopping gpio done.");
}

int register_device(void) {
    pix_gpio_init();
    int result;
    
    printk(KERN_NOTICE
           "Andrej-driver: register_device() is called." );
    
    result = register_chrdev(0, device_name, &simple_driver_fops);
    if (result < 0) {
        printk(KERN_WARNING
               "Andrej-driver:  can\'t register character device with errorcode = %i", result );
        return result;
    }
    
    device_file_major_number = result;
    printk(KERN_NOTICE
           "Andrej-driver: registered character device with major number = %i and minor numbers 0...255"
           , device_file_major_number );
    
    return 0;
}

/*-----------------------------------------------------------------------------------------------*/
void unregister_device(void) {
    pix_gpio_exit();
    printk(KERN_NOTICE
           "Andrej-driver: unregister_device() is called" );
    if (device_file_major_number != 0) {
        unregister_chrdev(device_file_major_number, device_name);
    }
}

