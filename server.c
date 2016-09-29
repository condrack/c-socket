/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
/*
Group 22:Taylor Condrack, Braxton Stein, Dilip Shah, Bikash Bhattrai
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     	int sockfd, newsockfd, portno;
     	pid_t process;
     	socklen_t clilen;
     	char buffer[256];
     	struct sockaddr_in serv_addr, cli_addr;
	int run=0;
	int n;
	int tickets=25;
	int request;
	int users=0;
     	if(argc < 2)
	{
         	fprintf(stderr,"ERROR, no port provided\n");
         	exit(1);
     	}


     	sockfd = socket(AF_INET, SOCK_STREAM, 0);

     	if (sockfd < 0)
        	error("ERROR opening socket");

     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	portno = atoi(argv[1]);
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);

     	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     		listen(sockfd,5);

	while(run==0||users!=0)
	{
     		clilen = sizeof(cli_addr);

     		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
     		if (newsockfd < 0)
          		error("ERROR on accept");
		run=1;
		bzero(&request,4);
		n= read(newsockfd,&request,4);
		if(n<0) error("error reading");
		if(request==100)
		{
			process=fork();
			users++;
		}
		if(request==-1 && tickets>0)
		{
			printf("Starting");
			tickets--;
		}
		if(request==1)
		{
			printf("returning ticket");
			tickets++;
		}
		if(request==-1&&request<=0)
		{
			n=write(newsockfd,"no tickets",10);
			if(n<0) error("error wrting");
		}
		else close(newsockfd);

}
	close(sockfd);
	close(newsockfd);
/*
    bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
*/
     return 0;
}

