
#!/bin/bash

ledOn = true;                        
   gpio_set_value(gpioLED1, ledOn);   
   printk(KERN_INFO "GPIO_TEST: Interrupt! (button1 state is %d)\n", gpio_get_value(gpioButton1));
   numberPresses++

exit

