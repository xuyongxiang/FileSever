#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#define IP "192.168.1.92"
#define port 8888
void * func(void * arg)
{
	int newfd = *(int *)arg;
	char buf[128];
	int res = 0;
	while(1)
	{
		bzero(buf,sizeof(buf));
		res = recv(newfd,buf,sizeof(buf),0);
		if(res<0)
		{
			perror("recv");
			return NULL;
		}
		else if (res==0)
		{
			printf("客户端关闭\n");
			break;
		}
		if(buf[0]=='L')
		{
			DIR * dp = opendir("./");
			if(dp==NULL)
			{
				perror("opendir");
				return NULL;
			}
			char buf[100];
			while(1)
			{
				bzero(buf,sizeof(buf));
				struct dirent * info = readdir(dp);
				if(info==NULL)
				{
					if(send(newfd,"end",sizeof(buf),0)<0)
					{
						perror("send");					
						return NULL;
					}
					break;
				}
				if(*(info->d_name)!='.')
				{
					strcpy(buf,info->d_name);
					if(send(newfd,buf,sizeof(buf),0)<0)
					{
						perror("send");					
						return NULL;
					}
				}

			}

		}
		else if (buf[0]=='D')
		{
			int fd = open(buf+1,O_RDONLY);
			if(fd<0)
			{
				perror("open");
				buf[0] = 'N';
				if(send(newfd,buf,1,0)<0)
				{
					perror("send");					
					return NULL;
				}
				return NULL;
			}
			if(send(newfd,buf,1,0)<0)
			{
				perror("send");					
				return NULL;
			}

			while(1)
			{
				bzero(buf,sizeof(buf));
				res = read(fd,buf,sizeof(buf));
				if(res==0)
				{
					break;
				}
				if(send(newfd,buf,res,0)<0)
				{
					perror("send");					
					return NULL;
				}

			}
		}
		else if (buf[0]=='U')
		{
			int fd = open(buf+1,O_WRONLY|O_TRUNC|O_CREAT,0664);
			if(fd<0)
			{
				perror("open");
				return NULL;
			}
			while(1)
			{
				bzero(buf,sizeof(buf));
				res =recv(newfd,buf,sizeof(buf),0);
				write(fd,buf,res);
				if(res<100)
				{
					break;
				}
			}
		}

	}
	pthread_exit(NULL);
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
	int reuse = 1;
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0)
	{
		perror("setsockopt");
		return -1;
	}
	if(bind(sfd,(struct sockaddr*)&sin,sizeof(sin))<0)
	{
		perror("bind");
		return -1;
	}
	if(listen(sfd,100)<0)
	{
		perror("listen");
		return -1;
	}
	int newfd = 0;
	struct sockaddr_in cin;
	pthread_t tid;
	int len = sizeof(cin);
	while(1)
	{
		newfd = accept(sfd,(struct sockaddr*)&cin,&len);
		if(newfd<0)
		{
			perror("accept");
			return -1;
		}
		pthread_create(&tid,NULL,func,&newfd);
		pthread_detach(tid);
		
	}
	close(sfd);
	return 0;
}
