#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xavier Pouyadux");
MODULE_DESCRIPTION("4 Button/LED");
MODULE_VERSION("0.1");


static unsigned int gpioLED1 = 20;      
//static unsigned int gpioLED2 = 16;      
static unsigned int gpioButton1 = 21; 
//static unsigned int gpioButton2 = 13; 
//static unsigned int gpioButton3 = 19; 
//static unsigned int gpioButton4 = 26; 

static unsigned int irqNumber1; 
//static unsigned int irqNumber2; 
//static unsigned int irqNumber3; 
//static unsigned int irqNumber4; 

static unsigned int numberPresses = 0;
static bool ledOn = 0;        

static irq_handler_t  ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);
 

static int __init ebbgpio_init(void){
   int result = 0;
   printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");
   
   if (!gpio_is_valid(gpioLED1)){
      printk(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
      return -ENODEV;
   }
   
   
   gpio_request(gpioLED1, "sysfs");         
   gpio_direction_output(gpioLED1, ledOn);  
   gpio_export(gpioLED1, false);           
                     
   gpio_request(gpioButton1, "sysfs");      
   gpio_direction_input(gpioButton1);       
   gpio_export(gpioButton1, false);          
                     
   irqNumber1 = gpio_to_irq(gpioButton1);
  // irqNumber2 = gpio_to_irq(gpioButton2);
  // irqNumber3 = gpio_to_irq(gpioButton3);
  // irqNumber4 = gpio_to_irq(gpioButton4);
   

   printk(KERN_INFO "GPIO_TEST: The button is mapped to IRQ: %d\n", irqNumber1);
 
   result = request_irq(irqNumber1,           
                        (irq_handler_t) ebbgpio_irq_handler, 
                        IRQF_TRIGGER_RISING,   
                        "ebb_gpio_handler",    
  			NULL);    
  
   printk(KERN_INFO "GPIO_TEST: The interrupt request result is: %d\n", result);
   return result;
}

static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
   ledOn = true;                        
   gpio_set_value(gpioLED1, ledOn);         
   printk(KERN_INFO "GPIO_TEST: Interrupt! (button state is %d)\n", gpio_get_value(gpioButton1));
   numberPresses++;                        
   return (irq_handler_t) IRQ_HANDLED;     
}

static void __exit ebbgpio_exit(void){
   printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton1));
   printk(KERN_INFO "GPIO_TEST: The button was pressed %d times\n", numberPresses);
   gpio_set_value(gpioLED1, 0);             
   gpio_unexport(gpioLED1);                 
   free_irq(irqNumber1, NULL);              
   gpio_unexport(gpioButton1);              
   gpio_free(gpioLED1);                     
   gpio_free(gpioButton1);                  
   printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n");
}


module_init(ebbgpio_init);
module_exit(ebbgpio_exit);
