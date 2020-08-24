#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>


struct Data{
	int age;
	char name[32];
};

#define SEV_IP "127.0.0.1"
#define SEV_PORT 6666
int main(){
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if(lfd==-1)
		perror("Socket 创建失败");
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SEV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret == -1)
	{
		perror("bind");
		exit(-1);
	}
	ret = listen(lfd, 5);
	if(ret==-1)
	{
		perror("listen:");
		exit(-1);
	}
	printf("绑定端口:%d成功，正自监听.....\n",ntohs(serv_addr.sin_port));
	
	int cfd;
	struct sockaddr_in client_addr = {};
	socklen_t len = sizeof(client_addr);
	
	char myMsg[] = "服务器连接成功";
	cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);
	if(cfd == -1){
		perror("accept:");
		exit(-1);
	}
	char clientIP[BUFSIZ];
	printf("client ip:%s, client port:%d已经上线\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, clientIP, sizeof(clientIP)), ntohs(client_addr.sin_port));
	char buf[BUFSIZ] = {};
	send(cfd, myMsg, strlen(myMsg)+1, 1);
	while(1){
		int len = recv(cfd, buf, BUFSIZ, 0);
		if(len<=0)
		{
			printf("客户端已经下线.\n");
			printf("服务端下线.\n");
			break;
		}
		printf("收到客户端指令:%s\n",buf);
		if(0 == strcmp(buf, "getInfo")){
			struct Data data = {18, "wu"};
			send(cfd, &data, sizeof(struct Data), 0);
		}else{
			char sendBuff[BUFSIZ] = "???????";
			send(cfd, sendBuff, strlen(sendBuff)+1, 0);	
		}
	}
	close(lfd);
	close(cfd);
	return 0;
}
