/*
 * =====================================================================================
 *
 *       Filename:  uart_com2.c
 *
 *    Description:  Test jz440 uart com2 comm function
 *
 *        Version:  1.0
 *        Created:  02/09/2020 11:03:28 AM
 *       Revision:  none
 *       Compiler:  arm-linux-gcc 3.4.5
 *
 *         Author:  WL
 *   Organization:  
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<termios.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	printf("uart COM2 test program\n");
	int fd;

	fd = open("/dev/s3c2410_serial1",O_RDWR | O_NOCTTY);
	if(fd == -1)
	{
		perror("serialport error\n");
	}
	else {
		printf("open %s succesfully\n", ttyname(fd));
	}

	struct termios Opt;
	int status;

	tcgetattr(fd, &Opt);				//get terminal attributes
	//set speed - 115200
	cfsetispeed(&Opt, B115200);
	cfsetospeed(&Opt, B115200);
	//set databits - 8 bits
	Opt.c_cflag &= ~CSIZE;
	Opt.c_cflag |= CS8;
	//set parity - no parity
	Opt.c_cflag &= ~PARENB;
	Opt.c_iflag &= ~INPCK;
	//set stopbits - 1 bit
	Opt.c_cflag &= ~CSTOPB;

	//others
	Opt.c_cflag &= ~CRTSCTS;  			//no hardware flow control
	Opt.c_iflag &= ~(IXON | IXOFF | IXANY);  	//no software flow control

	Opt.c_cflag |= (CLOCAL | CREAD);		//set local mode and enable receiver
	Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //choose raw input
	Opt.c_oflag &= ~OPOST;				//choose raw output

	Opt.c_cc[VMIN]  = 0;				//min mum of characters to read
	Opt.c_cc[VTIME] = 0;				//time to wait for data 

	status = tcsetattr(fd, TCSANOW, &Opt);		//set configration to terminal attributes now
	if(status != 0) {
		perror("tcsetattr fd");
		return;
	}
										 
	char r_buf[512];
	char s_buf[] = "Hello, world!\n";
	int len_read, len_write;
	int i;

	while(1)
	{   
		while((len_read = read(fd, r_buf, 512))>0)
		{
			printf("\nreceive length is: %d\n",len_read); 
			r_buf[len_read] = '\0';   
			printf("receive data   is: %s\n", r_buf);

			for (i = 0; i < 5; i++)
		        {    
				len_write = write(fd, s_buf, sizeof(s_buf));    
				if (len_write > 0)    
					printf("%d time send %d data successfully\n", i, len_write);    
				else
					printf("send data failed!\n");    
				
				sleep(1);
		        }    
		}
	}

	close(fd);
				
	return 0;
}
