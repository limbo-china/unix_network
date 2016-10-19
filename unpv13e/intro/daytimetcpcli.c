#include	"unp.h"


int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
    char                buff[MAXLINE+1];
	struct sockaddr_in	servaddr,cliaddr;
    
    socklen_t len;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
    bzero(&cliaddr,sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
    //cliaddr.sin_family = AF_INET;
    //cliaddr.sin_port = htons(12345);
	servaddr.sin_port   = htons(10000);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

    //Bind(sockfd, (SA *)&cliaddr, sizeof(cliaddr));

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
   
    len = sizeof(cliaddr);
    if(getsockname(sockfd,(SA *)&cliaddr,&len ) < 0)
        err_sys("getsockname error");

    printf("local selected ip: %s, port: %d\n",
            inet_ntop(AF_INET,&cliaddr.sin_addr, buff, MAXLINE),
            ntohs(cliaddr.sin_port)); 
    int times=0; 
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        times++;
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

    //printf("times: %d\n",times);
	exit(0);
}
