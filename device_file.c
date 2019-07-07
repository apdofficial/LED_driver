#include "device_file.h"
#include <linux/fs.h> 	     /* file stuff */
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
static const char    g_s_Hello_World_string[] = "Hello world from kernel mode!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);
static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;

static volatile u32 *gpio_base = NULL;
/*===============================================================================================*/
static ssize_t device_file_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	unsigned long time_sleep = 0;
	if(copy_from_user(&c,buf,sizeof(char))){
		return -EFAULT;
	}
	if(c == '0'){
		gpio_set_value(16,0);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,0);
		gpio_set_value(20,0);
		gpio_set_value(21,0);
		gpio_set_value(22,1);
	}
	
	else if(c == '1'){
		gpio_set_value(16,1);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,1);
		gpio_set_value(20,1);
		gpio_set_value(21,1);
		gpio_set_value(22,1);
	}
	else if(c == '2'){
		gpio_set_value(16,0);
		gpio_set_value(17,0);
		gpio_set_value(18,1);
		gpio_set_value(19,0);
		gpio_set_value(20,0);
		gpio_set_value(21,1);
		gpio_set_value(22,0);
	}
	else if(c == '3'){
		gpio_set_value(16,0);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,0);
		gpio_set_value(20,1);
		gpio_set_value(21,1);
		gpio_set_value(22,0);
	}
	else if(c == '4'){
		gpio_set_value(16,1);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,1);
		gpio_set_value(20,1);
		gpio_set_value(21,0);
		gpio_set_value(22,0);
	}
	else if(c == '5'){
		gpio_set_value(16,0);
		gpio_set_value(17,1);
		gpio_set_value(18,0);
		gpio_set_value(19,0);
		gpio_set_value(20,1);
		gpio_set_value(21,0);
		gpio_set_value(22,0);
	}
	else if(c == '6'){
		gpio_set_value(16,0);
		gpio_set_value(17,1);
		gpio_set_value(18,0);
		gpio_set_value(19,0);
		gpio_set_value(20,0);
		gpio_set_value(21,0);
		gpio_set_value(22,0);
	}
	else if(c == '7'){
		gpio_set_value(16,0);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,1);
		gpio_set_value(20,1);
		gpio_set_value(21,1);
		gpio_set_value(22,1);
	}
	else if(c == '8'){
		gpio_set_value(16,0);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,0);
		gpio_set_value(20,0);
		gpio_set_value(21,0);
		gpio_set_value(22,0);
	}
	else if(c == '9'){
		gpio_set_value(16,0);
		gpio_set_value(17,0);
		gpio_set_value(18,0);
		gpio_set_value(19,0);
		gpio_set_value(20,1);
		gpio_set_value(21,0);
		gpio_set_value(22,0);
	}
        return 1;
}
/*===============================================================================================*/
static struct file_operations simple_driver_fops = 
{
   .owner   = THIS_MODULE,
   .write    = device_file_write,
};

static int device_file_major_number = 0;

static const char device_name[] = "Simple-driver";

/*===============================================================================================*/
void pix_gpio_init(void){
  printk(KERN_INFO "7segment: starting gpio...");
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
  printk(KERN_INFO "7segment: starting gpio done.");
}

void pix_gpio_exit(void){
  printk(KERN_INFO "7segmet: stopping gpio...");
  gpio_free(16);
  gpio_free(17);
  gpio_free(18);
  gpio_free(19);
  gpio_free(20);
  gpio_free(21);
  gpio_free(22);
  printk(KERN_INFO "7segmet: stopping gpio done.");
}

int register_device(void)
{
      pix_gpio_init();
      int result = 0;

      printk( KERN_NOTICE "Simple-driver: register_device() is called." );

      result = register_chrdev( 0, device_name, &simple_driver_fops );
      if( result < 0 )
      {
         printk( KERN_WARNING "Simple-driver:  can\'t register character device with errorcode = %i", result );
         return result;
      }

      device_file_major_number = result;
      printk( KERN_NOTICE "Simple-driver: registered character device with major number = %i and minor numbers 0...255"
                  , device_file_major_number );

      return 0;
}
/*-----------------------------------------------------------------------------------------------*/
void unregister_device(void)
{
   pix_gpio_exit();
   printk( KERN_NOTICE "Simple-driver: unregister_device() is called" );
   if(device_file_major_number != 0)
   {
      unregister_chrdev(device_file_major_number, device_name);
   }
}
