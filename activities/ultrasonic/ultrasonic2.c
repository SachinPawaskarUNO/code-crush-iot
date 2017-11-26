#include <stdint.h>
#include <stdio.h>

int main()
{
	FILE *pwm,*duty,*period,*polarity,*run,*gpi,*p45,*direction, *direction2, *gpi2;

	FILE *output;

	int value,i,count,enable;


	output = fopen("output.csv", "w");

	uint16_t input=0;
	uint8_t input_no=0,input_state=0;
	unsigned int duty_cycle =500000;
	pwm = fopen("/sys/devices/bone_capemgr.9/slots", "w");
	if(pwm == 0)
	{
		printf("Error opening slots\n");
		return 1;
	}
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"am33xx_pwm");
	fflush(pwm);

	fprintf(pwm,"bone_pwm_P9_16");
	fflush(pwm);
	period = fopen("/sys/devices/ocp.3/pwm_test_P9_16.15/period", "w");
	if(period == 0)
	{
		printf("Error opening period\n");
		return 1;
	}
	fseek(period,0,SEEK_SET);
	fprintf(period,"%d",5000000);
	fflush(period);


	duty = fopen("/sys/devices/ocp.3/pwm_test_P9_16.15/duty", "w");
	if(duty == 0)
	{
		printf("Error opening duty\n");
		return 1;
	}
	fseek(duty,0,SEEK_SET);
	fprintf(duty, "%d", 12000);
	fflush(duty);
	run = fopen("/sys/devices/ocp.3/pwm_test_P9_16.15/run", "w");
	if(run == 0)
	{
		printf("Error opening run\n");
		return 1;
	}



	polarity = fopen("/sys/devices/ocp.3/pwm_test_P9_16.15/polarity", "w");
	if(polarity == 0)
	{
		printf("Error opening polarity\n");
		return 1;
	}
	fseek(polarity, 0, SEEK_SET);
	fprintf(polarity, "%d", 0);
	fflush(polarity);


	fseek(run,0,SEEK_SET);
	fprintf(run,"%d",1);
	fflush(run);


	p45 = fopen("/sys/class/gpio/export", "w");
	fseek(p45,0,SEEK_SET);
	fprintf(p45,"%d",45);
	fflush(p45);
	fprintf(p45, "%d", 60);
	fflush(p45);

	//For some reason the program wont run if the following is not commented out
	//But in order for the gpio pin to function, it needs to be put back into the code and ran at least once
	//It can be commented out again after being ran once

	direction = fopen("/sys/class/gpio/gpio45/direction", "w");
	fseek(direction,0,SEEK_SET);
	fprintf(direction,"%s","in");
	fflush(direction);

	direction2 = fopen("/sys/class/gpio/gpio60/direction", "w");
	fseek(direction2,0, SEEK_SET);
	fprintf(direction2,"%s", "in");
	fflush(direction2);

	while(1)
	{

		gpi2 = fopen("/sys/class/gpio/gpio60/value", "r");
		fseek(gpi2,0,SEEK_SET);
		fscanf(gpi2,"%d",&enable);
		fclose(gpi2);

		if(enable == 1)
			continue;

		gpi = fopen("/sys/class/gpio/gpio45/value","r");
		fseek(gpi,0,SEEK_SET);
		fscanf(gpi,"%d",&value);
		fclose(gpi);

		if(value==1)
		{
			count++;
		}
		else
		{
			if(count!=0)
			{
				//printf("         Centimeters: %5d\n",count*2);
				//			printf("         Inches: %5d\n",count*5);
				printf("%d\n",count*2);
				fprintf(output, "%d\n", count*2);
				fflush(output);

				count=0;

			}
		}


		//		printf("       value: %5d\n",value);
		//		for(i = 0; i<100000;i++);



	}
	fclose(output);

	return 0;

}

