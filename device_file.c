/*******************************************************************************
 *                                                                             *
 *   Program    : LED Driver                                                   *
 *   Programmer : Andrej Pistek (450966)                                       *
 *   Date       : 02-July-2018                                                 *
 *                                                                             *
 ******************************************************************************/

#include "device_file.h"
#include <linux/fs.h>           /* file stuff */
#include <linux/kernel.h>       /* printk() */
#include <linux/errno.h>        /* error codes */
#include <linux/module.h>       /* THIS_MODULE */
#include <linux/cdev.h>         /* char device stuff */
#include <linux/uaccess.h>      /* copy_to_user() */
#include <linux/device.h>
#include <asm/delay.h>
#include <linux/io.h>
#include <linux/gpio.h>

/*
 *  Function: set_coordinates
 *  ---------------------------
 *  Sets assigned pins of 7-segment display with provided argument using gpio.h library.
 *  returns: void
 */
static inline void set_coordinates(int a, int b, int c, int d,int e, int f, int g){
    gpio_set_value(16, a);
    gpio_set_value(17, b);
    gpio_set_value(18, c);
    gpio_set_value(19, d);
    gpio_set_value(20, e);
    gpio_set_value(21, f);
    gpio_set_value(22, g);
}

/*
 *  Function: device_file_write
 *  ---------------------------
 *  Sets assigned pins of 7-segment display with provided argument taken from user_buffer.
 *  returns: 1 or -EFAULT if getting the value from user fails
 */
static ssize_t device_file_write(struct file *filp, const char *user_buffer, size_t count, loff_t *pos) {
    char c;
    if (copy_from_user(&c, user_buffer, sizeof(char))) {
        return -EFAULT;
    }
    switch (c){
        case '0':
            set_coordinates(0,0,0,0,0,0,1);
            break;
        case '1':
            set_coordinates(1,0,0,1,1,1,1);
            break;
        case '2':
            set_coordinates(0,0,1,0,0,1,0);
            break;
        case '3':
            set_coordinates(0,0,0,0,1,1,0);
            break;
        case '4':
            set_coordinates(1,0,0,1,1,0,0);
            break;
        case '5':
            set_coordinates(0,1,0,0,1,0,0);
            break;
        case '6':
            set_coordinates(0,1,0,0,0,0,0);
            break;
        case '7':
            set_coordinates(0,0,0,1,1,1,1);
            break;
        case '8':
            set_coordinates(0,0,0,0,0,0,0);
            break;
        case '9':
            set_coordinates(0,0,0,0,1,0,0);
            break;
    }
    return 1;
}

/*
 *  Function: device_file_read
 *  ---------------------------
 *  Sets assigned pins of 7-segment display with provided argument taken from user_buffer.
 *  returns: count or -EFAULT if recognizing of number fails
 */
static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer,size_t count, loff_t *possition){
    char output_message[35] =  {"Following number is displayed: "};
    ssize_t output_message_size;
    int a = gpio_get_value(16);
    int b = gpio_get_value(17);
    int c = gpio_get_value(18);
    int d = gpio_get_value(19);
    int e = gpio_get_value(20);
    int f = gpio_get_value(21);
    int g = gpio_get_value(22);
    
    printk(KERN_NOTICE"Andrej-driver: Device file is read at offset = %i, read bytes count = %u", (int)*possition,(unsigned int)count );
    
    if (a==0 && b==0 && c==0 && d==0 && e==0 && f==0 && g ==1){
        strcat(output_message,"0\n\0");
    }
    else if(a==1 && b==0 && c==0 && d==1 && e==1 && f==1 && g ==1){
        strcat(output_message,"1\n\0");
    }
    else if(a==0 && b==0 && c==1 && d==0 && e==0 && f==1 && g ==0){
        strcat(output_message,"2\n\0");
    }
    else if(a==0 && b==0 && c==0 && d==0 && e==1 && f==1 && g ==0){
        strcat(output_message,"3\n\0");
    }
    else if(a==1 && b==0 && c==0 && d==1 && e==1 && f==0 && g ==0){
        strcat(output_message,"4\n\0");
    }
    else if(a==0 && b==1 && c==0 && d==0 && e==1 && f==0 && g ==0){
        strcat(output_message,"5\n\0");
    }
    else if(a==0 && b==1 && c==0 && d==0 && e==0 && f==0 && g ==0){
        strcat(output_message,"6\n\0");
    }
    else if(a==0 && b==0 && c==0 && d==1 && e==1 && f==1 && g ==1){
        strcat(output_message,"7\n\0");
    }
    else if(a==0 && b==0 && c==0 && d==0 && e==0 && f==0 && g ==0){
        strcat(output_message,"8\n\0");
    }
    else if(a==0 && b==0 && c==0 && d==0 && e==1 && f==0 && g ==0){
        strcat(output_message,"9\n\0");
    }else{
        return -EFAULT;
    }
    output_message_size = sizeof(output_message);
    if( *possition >= output_message_size  )
        return 0;
    if( *possition + count > output_message_size  )
        count = output_message_size  - *possition;
    if(copy_to_user(user_buffer, output_message+ *possition, count) != 0 )
        return -EFAULT;
    *possition += count;
    return count;
}

static struct file_operations simple_driver_fops =
{
    .owner   = THIS_MODULE,
    .write   = device_file_write,
    .read    = device_file_read,
};

static int device_file_major_number = 0;

static const char device_name[] = "Andrej-driver";

/*
 *  Function: pix_gpio_init
 *  ---------------------------
 *  Initialize gpio pins  and sets direction of them.
 *  returns: void
 */
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

/*
 *  Function: pix_gpio_exit
 *  ---------------------------
 *  Releases previously initialized ports by pix_gpio_init.
 *  returns: void
 */
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
    int result;
    pix_gpio_init();
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

void unregister_device(void) {
    pix_gpio_exit();
    printk(KERN_NOTICE
           "Andrej-driver: unregister_device() is called" );
    if (device_file_major_number != 0) {
        unregister_chrdev(device_file_major_number, device_name);
    }
}

