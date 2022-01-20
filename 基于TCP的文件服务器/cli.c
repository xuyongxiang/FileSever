#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define IP "192.168.1.92"
#define port 8888
int do_list(int sfd)
{
	printf("111111");
	char type = 'L';
	if(send(sfd,&type,1,0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		if(recv(sfd,buf,sizeof(buf),0)<0)
		{
			perror("recv");
			return -1;
		}
		if(strcmp("end",buf)==0)
		{
			break;
		}
		printf("%s\n",buf);
	}
	return 0;
}
int do_download(int sfd)
{

	while(getchar()!=10);
	printf("请输入要下载的文件名>>>");
	char str[20];
	fgets(str,sizeof(str),stdin);
	str[strlen(str)-1] = 0;
	char fileName[20];
	sprintf(fileName,"%c%s",'D',str);
	if(send(sfd,fileName,sizeof(fileName),0)<0)
	{
		perror("send");
		return -1;
	}
	char buf[100];
	int fd =0;
	int res = recv(sfd,buf,1,0);
	if(buf[0]=='D')
	{
		fd = open(str,O_WRONLY|O_CREAT|O_TRUNC,0664);
		if(fd<0)
		{
			perror("open");
			return -1;
		}
		
	}
	else
	{
		printf("没有该文件\n");
		return -1;
	}

	while(1)
	{
		bzero(buf,sizeof(buf));
		res = recv(sfd,buf,sizeof(buf),0);
		if(res<0)
		{
			perror("recv");
			return -1;
		}
		else if (res==0)
		{
			break;
		}
		write(fd,buf,res);
		if(res<100)
		{
			printf("下载完毕\n");
			break;
		}
		printf("%s\n",buf);
	}
	return 0;
}
int do_upload(int sfd)
{

	while(getchar()!=10);
	printf("请输入要上传的文件名>>>");
	char str[20];
	fgets(str,sizeof(str),stdin);
	str[strlen(str)-1] = 0;
	int fd =open(str,O_RDONLY);
	if(fd<0)
	{
		perror("open");
		return -1;
	}
	char fileName[20];
	sprintf(fileName,"%c%s",'U',str);
	if(send(sfd,fileName,sizeof(fileName),0)<0)
	{
		perror("send");
		return -1;
	}

	char buf[100];
	int res = 0;
	while(1)
	{
		bzero(buf,sizeof(buf));
		res = read(fd,buf,sizeof(buf));
		if(send(sfd,buf,res,0)<0)
		{
			perror("send");
			return -1;
		}
		if(res<100)
		{
			printf("上传完毕\n");
			break;
		}
		printf("%s\n",buf);
	}
	return 0;
}


int main(int argc, const char *argv[])
{

	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(IP);
	if(connect(sfd,(struct sockaddr*)&sin,sizeof(sin))<0)
	{
		perror("connect");
		return -1;
	}
	int select;
	while(1)
	{
		printf("1.list\n");
		printf("2.download\n");
		printf("3.upload\n");
		printf("4.quit\n");
		printf("请选择>>>");
		scanf("%d",&select);
		switch (select)
		{
			case 1:do_list(sfd);
				   break;
			case 2:do_download(sfd);
				   break;
			case 3:do_upload(sfd);
				   break;
			case 4:return 0;
		}
	}
	return 0;
}
