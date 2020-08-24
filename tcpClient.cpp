#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>


struct Data{
	int age;
	char name[32];
};

#define SEV_PORT 6666
#define SEV_IP "127.0.0.1"
int main(){
	int cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd == -1)
	{
		perror("socket:");
		exit(-1);
	}
	struct sockaddr_in sev;
	sev.sin_family = AF_INET;
	sev.sin_port = htons(SEV_PORT);
	inet_pton(AF_INET, SEV_IP, &sev.sin_addr.s_addr);
	
	int ret = connect(cfd, (struct sockaddr*)&sev, sizeof(sev));
	if(ret==-1)
	{
		perror("connect");
		exit(-1);
	}

	char buff[BUFSIZ];
	ret = recv(cfd, buff, BUFSIZ, 0);
	if(ret <=0 ){
		perror("recv:");
		exit(-1);
	}
	std::cout << buff << std::endl;

	while(1){
		scanf("%s",buff);
		if(0==strcmp(buff, "exit"))
		{
			printf("客户端关闭连接\n");
			break;
		}
		send(cfd, buff, strlen(buff)+1, 0);
		if(0==strcmp(buff, "getInfo")){
			struct Data data = {};
			ret = recv(cfd, &data, sizeof(struct Data), 0);
			if(ret <=0 ){
				perror("recv:");
				exit(-1);
			}
			printf("收到客户端数据:age:%d, name:%s\n",data.age, data.name);
		}else{
			char reBuff[BUFSIZ] = {};
			ret = recv(cfd, reBuff, BUFSIZ, 0);
			if(ret <=0 ){
				perror("recv:");
				exit(-1);
			}
			printf("收到客户端数据:%s\n", reBuff);
		}
	}
	close(cfd);
	return 0;
}
