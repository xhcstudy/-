#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERVER_PORT 666

int main(void){
    
    int sock;//代表信箱
    int i;
    struct sockaddr_in server_addr;//标签
    
    //1创建信箱	socket函数第一个参数网络地址（网络通信家族，指定tcp/ip）第二个参数是指定tcp协议
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    //2清空标签，写上地址和端口号
    bzero(&server_addr, sizeof(server_addr));

    //server_addr是一个地址（结构体）
    server_addr.sin_family = AF_INET;//选择协议族IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//监听本地所有IP地址	
    server_addr.sin_port = htons(SERVER_PORT);//绑定端口号
    
    //实现标签贴到收信的信箱上  
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    //把信箱挂置传达室，这样，就可以接受信件了
    
    listen(sock, 128);

    //万事俱备，只等来信
    printf("等待客户端的连接\n");

    
    int done = 1;
    
    while(done){
        struct sockaddr_in client;
        int client_sock;
        char client_ip[64];
        char buf[256];
        int len;//接受客户端发送的长度
        
        socklen_t client_addr_len;
        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
        
	//打印客户端地址和端口号
        printf("client ip: %s\t port : %d\n", 
                   inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)),
	 	   ntohs(client.sin_port));
        //读取客户端发送的数据
        len = read(client_sock, buf, sizeof(buf) - 1);//为什么要-1，防止越界，只让最大读255个字节 
        
        //read函数不会给你在读到的buf中len位置添加结束符,需要自己添加
        buf[len] = '\0';
 
        printf("receive[%d]: %s\n", len, buf); 

        //转换成大写
        /*for(i = 0; i < len; i++){
            if(buf[i] >= 'a' && buf[i] <= 'z'){
            	buf[i] -= 32;
            }
        }*/
        for(i = 0; i < len ; i++){
    	     buf[i] = toupper(buf[i]);
	}
 
        len = write(client_sock, buf, len);

        printf("write finished. len: %d\n", len);
        close(client_sock); 

    }
    return 0;

}
