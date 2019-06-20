#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
    int numrv;
    long int res;
    int pacCount = 0;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    printf("Socket success\n");
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    
    //create a socket
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    //bind
    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    
    //listen
    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }
    
    
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
        
        //open the file to transfer
        FILE *fp = fopen(argv[1],"rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return 1;
        }
        fseek(fp, 0L, SEEK_END);
        
        // calculating the size of the file
        res = ftell(fp);
        
        
        fp = fopen(argv[1],"rb");
        //start reading the file
        //check if still there is still data to be read
        while(1)
        {
            //read data in packets
            //each data packet is of 1500B
            pacCount= pacCount + 1;
            printf("packet number = %d\n", pacCount);
            unsigned char buff[1500]={0};
            int nread = fread(buff,1,1500,fp);
            printf("Bytes read %d \n", nread);
            
            //if read successfully then send the packet
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
            }
            
            //end condition
            if (nread < 1500)
            {
                //end of the file
                if (feof(fp))
                    printf("file length: %ld\n",res);
                printf("End of file\n");
                //error
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
            
            
        }
        
        close(connfd);
        sleep(1);
    }
    
    
    
    return 0;
}

