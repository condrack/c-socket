#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>


int makerandom();
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    	int sockfd, portno, n;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
    	char buffer[256];
	int client =100;
	int receive;
	int tickets=0;

		srand(time(NULL));

    	if (argc < 3)
	{
       		fprintf(stderr,"usage %s hostname port\n", argv[0]);
       		exit(0);
    	}

    	portno = atoi(argv[2]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);

    	if (sockfd < 0)
       		error("ERROR opening socket");

    	server = gethostbyname(argv[1]);

    	if (server == NULL)
	{
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
    	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    	serv_addr.sin_port = htons(portno);

    	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        	error("ERROR connecting");

//	n=write(sockfd, &client,4);
	int i=0;
	int result, num;
	for(i;i<15;i++)
	{
        	num = rand()%10;
        if(num<8)
                result= 0;
        else
                result= 1;
		if(result==0)
		{
    			printf("Requesting ticket\n");
			receive=-1;
			tickets++;
		}
		else
		{
			printf("returning ticket\n");
			receive=1;
			tickets--;
		}
	n=write(sockfd,&receive,4);
	if(n<0) error("Error writing to socket");
	bzero(buffer,256);
	n=read(sockfd,buffer,255);
	if(n<0) error("Error reading from socket");
	}
	close(sockfd);
    	return 0;
}

int makerandom()
{
	srand(time(NULL));

	int num = rand()%10;

	if(num<8)
		return 0;
	else
		return 1;
}
