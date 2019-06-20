#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

struct ack {
    int ack;
};

struct packet {
    int seq_no;
    char data[1488];
    int source_port;
    int dest_port;
};

int main(int argc, char *argv[])
{
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[1500];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;
    int ack = 0;
    int pacCount = 0;
    //create socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    
    //create socket
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[3])); // port
    serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
    
    //connect to socket
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    
    //file where the read packets are stored
    FILE *fp;
    fp = fopen(argv[1], "ab");
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }
    
    //read each packet of 1500B
    while((bytesReceived = read(sockfd, recvBuff, 1500)) > 0)
    {
        pacCount= pacCount + 1;
        printf("packet number = %d\n", pacCount);
        ack  = 1;
        printf("Bytes received %d\n",bytesReceived);
        printf("ack = %d\n", ack);
        fwrite(recvBuff, 1,bytesReceived,fp);
    }
    
    if(bytesReceived < 0)
    {
        ack = 0;
        printf("\n Read Error \n");
    }
    
    
    return 0;
}

