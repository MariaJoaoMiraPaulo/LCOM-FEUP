#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int hook_id;
int counter=0;

int timer_set_square(unsigned long timer, unsigned long freq) {

	char command=0x36;

	int ret;

	ret=sys_outb(TIMER_CTRL,command);

	if(ret!=0)

		return -1;

	int n;

	n=(TIMER_FREQ /freq);

	char lsb = (char) n;

	char msb = (char) n>>8;

	ret= sys_outb(TIMER_0,lsb);

	if (ret!=0)

		return -1;

	ret= sys_outb(TIMER_0,msb);

	if (ret!=0)

		return -1;

	return 0;
}

int timer_subscribe_int(void ) {

	int hook_id=5;
	int tmp=BIT(hook_id);

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hook_id)!= 0)  //Subscribes the interruption
		return 1;

	if(sys_irqenable(&hook_id)!=0) //enables the interruption
		return 1;

	return tmp; //returns bit order in interrupt mask if interrupt didn't failed
}

int timer_unsubscribe_int() {

	if (sys_irqrmpolicy(&hook_id)==0)  //unsubscribe the interrupt
		return 0;

	return 1;
}

void timer_int_handler() {

	counter++;
	if(counter%60==0)
		printf("One second passed\n");
}

int timer_get_conf(unsigned long timer, unsigned char *st) {

	char command=0;
	int ret;

	command = command|TIMER_RB_CMD; //put 1 at the last two bits (bit 6 and bit 7)

	switch (timer)
	{
	case 0:
		command=command |TIMER_RB_SEL(0);  //if is the first timer put 1 at bit 1
		break;
	case 1:
		command=command |TIMER_RB_SEL(1); //if is the first timer put 1 at bit 2
		break;
	case 2:
		command=command |TIMER_RB_SEL(2); //if is the first timer put 1 at bit 3
		break;
	}

	ret=  sys_outb(TIMER_CTRL,command);   //put command on control (0x43)
	unsigned long res;

	if (ret !=0 ) //error
		return 1;

	switch (timer)
	{
	case 0:
		ret= sys_inb(TIMER_0,&res);  //put config on first timer
		break;
	case 1:
		ret= sys_inb(TIMER_1,&res); //put config on second timer
		break;
	case 2:
		ret= sys_inb(TIMER_2,&res); //put config on  third timer
		break;
	}


	if(ret!=0)  //error
		return 1;

	*st=res;

	return 0;
}

int timer_display_conf(unsigned char conf) {

	printf("BCD : %d", 0x01 & conf);  //test bit 0

	printf("Programmed Mode : %d%d%d", (0x08 & conf)>> 3, (0x04 & conf)>> 2, (0x02 & conf)>> 1 );  //test bit 1 2 3

	printf("Type of Access : %d%d",(0x20 & conf)>> 5, (0x10 & conf)>> 4);  //test bit 3 4

	printf("Null Count : %d", (0x40 & conf)>> 6);  //test bit 6

	printf("Output : %d", (0x80 & conf)>> 7);//test bit 7

	return 0;

}

int timer_test_square(unsigned long freq) {
	timer_set_square(0, freq);
	return 1;
}

int timer_test_int(unsigned long time) {

	int ipc_status;
	int irq_set=timer_subscribe_int();
	message msg;
	int r;

	while( counter/60 <time ) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ){
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if(timer_unsubscribe_int()==1)
		return 1;

	return 0;

}

int timer_test_config(unsigned long timer) {

	unsigned char st;
	int ret;

	ret = timer_get_conf(timer, &st);

	if (ret != 0)
		return 1;

	ret = timer_display_conf(st);

	if ( ret != 0)
		return 1;

	return 0;
}
