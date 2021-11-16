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
static unsigned int gpioLED2 = 16;      
static unsigned int gpioButton1 = 21; 
static unsigned int gpioButton2 = 26; 
static unsigned int gpioButton3 = 19; 
static unsigned int gpioButton4 = 13; 

static unsigned int irqNumber1; 
static unsigned int irqNumber2; 
static unsigned int irqNumber3; 
static unsigned int irqNumber4; 

static unsigned int numberPressesbutton1 = 0;
static unsigned int numberPressesbutton2 = 0;
static unsigned int numberPressesbutton3 = 0;
static unsigned int numberPressesbutton4 = 0;

static bool ledOn = 0;        

static irq_handler_t  ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);

static int __init ebbgpio_init(void){
   int result = 0;
   printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");
   
   if (!gpio_is_valid(gpioLED1)){
      printk(KERN_INFO "GPIO_TEST: invalid LED1 GPIO\n");
      return -ENODEV;
   }
if (!gpio_is_valid(gpioLED2)){
      printk(KERN_INFO "GPIO_TEST: invalid LED2 GPIO\n");
      return -ENODEV;
   }

   // inicializar led 1   
   gpio_request(gpioLED1, "sysfs");         
   gpio_direction_output(gpioLED1, ledOn);  
   gpio_export(gpioLED1, false);           
   // inicializar led 2
   gpio_request(gpioLED2, "sysfs");       
   gpio_direction_output(gpioLED2, ledOn); 
   gpio_export(gpioLED2, false);

   // inicializar button 1                  
   gpio_request(gpioButton1, "sysfs");      
   gpio_direction_input(gpioButton1);       
   gpio_export(gpioButton1, false);          
   
   // inicializar button 2                  
   gpio_request(gpioButton2,"sysfs");
   gpio_direction_input(gpioButton2);
   gpio_export(gpioButton2, false);

   // inicializar button 3                 
   gpio_request(gpioButton3,"sysfs");
   gpio_direction_input(gpioButton3);
   gpio_export(gpioButton3, false);

   // inicializar button 4                  
   gpio_request(gpioButton4,"sysfs");
   gpio_direction_input(gpioButton4);
   gpio_export(gpioButton4, false);
	
   irqNumber1 = gpio_to_irq(gpioButton1);
   irqNumber2 = gpio_to_irq(gpioButton2);
   irqNumber3 = gpio_to_irq(gpioButton3);
   irqNumber4 = gpio_to_irq(gpioButton4);
   
   printk(KERN_INFO "GPIO_TEST: The LED 1 IRQ: %d\n", gpio_to_irq(gpioLED1));
   printk(KERN_INFO "GPIO_TEST: The button 1 IRQ: %d\n", irqNumber1);
   printk(KERN_INFO "GPIO_TEST: The button 2 IRQ: %d\n", irqNumber2);
   printk(KERN_INFO "GPIO_TEST: The button 3 IRQ: %d\n", irqNumber3);
   printk(KERN_INFO "GPIO_TEST: The button 4 IRQ: %d\n", irqNumber4);

 //button 1
   result = request_irq(irqNumber1,           
                        (irq_handler_t) ebbgpio_irq_handler, 
                        IRQF_TRIGGER_RISING,   
                        "ebb_gpio_handler",    
  			NULL);    
  
// button 2
   result = request_irq(irqNumber2,        
                        (irq_handler_t) ebbgpio_irq_handler,
                        IRQF_TRIGGER_RISING, 
                        "ebb_gpio_handler",  
                        NULL);               
 

// button 3
   result = request_irq(irqNumber3,        
                        (irq_handler_t) ebbgpio_irq_handler,
                        IRQF_TRIGGER_RISING, 
                        "ebb_gpio_handler",  
                        NULL);               
 
// button 4
   result = request_irq(irqNumber4,        
                        (irq_handler_t) ebbgpio_irq_handler,
                        IRQF_TRIGGER_RISING, 
                        "ebb_gpio_handler",  
                        NULL);               
 
   printk(KERN_INFO "GPIO_TEST: The interrupt request result is: %d\n", result);
   return result;
}



static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
 if ( gpio_get_value(gpioButton1)==1){
  ledOn = true;
  gpio_set_value(gpioLED1,ledOn);
   printk(KERN_INFO "GPIO_TEST: Interrupt! (button1 state is %d)\n", gpio_get_value(gpioButton1));
   numberPressesbutton1++;
 }

  if ( gpio_get_value(gpioButton2)==1){
  ledOn = false;
  gpio_set_value(gpioLED1,ledOn); 
   printk(KERN_INFO "GPIO_TEST: Interrupt! (button2 state is %d)\n", gpio_get_value(gpioButton2));
   numberPressesbutton2++;
 }    
  if ( gpio_get_value(gpioButton3)==1){
  ledOn = true;
  gpio_set_value(gpioLED2,ledOn); 
  printk(KERN_INFO "GPIO_TEST: Interrupt! (button3 state is %d)\n", gpio_get_value(gpioButton3));
  numberPressesbutton3++;
 }    
  if ( gpio_get_value(gpioButton4)==1){
  ledOn = false;
  gpio_set_value(gpioLED2,ledOn); 
  printk(KERN_INFO "GPIO_TEST: Interrupt! (button4 state is %d)\n", gpio_get_value(gpioButton4));
  numberPressesbutton4++;
 }    
   return (irq_handler_t) IRQ_HANDLED;     
}


static void __exit ebbgpio_exit(void){
   printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton1));
   printk(KERN_INFO "GPIO_TEST: The button1 was pressed %d times\n", numberPressesbutton1);
   printk(KERN_INFO "GPIO_TEST: The button2 was pressed %d times\n", numberPressesbutton2);
   printk(KERN_INFO "GPIO_TEST: The button3 was pressed %d times\n", numberPressesbutton3);
   printk(KERN_INFO "GPIO_TEST: The button4 was pressed %d times\n", numberPressesbutton4);

   gpio_set_value(gpioLED1, 0);             
   gpio_unexport(gpioLED1);                  
   gpio_set_value(gpioLED2, 0);             
   gpio_unexport(gpioLED2);                 
   free_irq(irqNumber1, NULL);              
   free_irq(irqNumber2, NULL); 
   free_irq(irqNumber3, NULL);              
   free_irq(irqNumber4, NULL);
   gpio_unexport(gpioButton1);              
   gpio_unexport(gpioButton2);    
   gpio_unexport(gpioButton3);              
   gpio_unexport(gpioButton4);   
   gpio_free(gpioLED1);                      
   gpio_free(gpioLED2);                     
   gpio_free(gpioButton1); 
   gpio_free(gpioButton2);                   
   gpio_free(gpioButton3); 
   gpio_free(gpioButton4);                  
   printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n");
}


module_init(ebbgpio_init);
module_exit(ebbgpio_exit);
