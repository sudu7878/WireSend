#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>


struct sockaddr_in my_addr;

class Connector{
    private:
        uint16_t port = 8000;
        int sockfd;

    public:
        Connector(){
            sockfd = -1;
        }

        bool CreateSocketFd(){
            sockfd = socket(AF_INET, SOCK_STREAM, 0);

            if(sockfd < 0){
                perror("Socket");
                return false;
            }

            printf("Socket creation successful.\n");
            return true;
        }

        int GetFd(){
            return sockfd;
        }

        void BindSocket(){
            struct sockaddr_in serv_address;
            serv_address.sin_family = AF_INET;
            serv_address.sin_port = htons(port);
            serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

            bind(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address));
            printf("Socket bound to %d at port %d.\n", htons(INADDR_ANY), port);
        }


       ~Connector(){
            if(sockfd >= 0){
                close(sockfd);
                printf("Socket closed.\n");
        }
    }
};

int main(){
    Connector newConnection;
    int sockfd = newConnection.CreateSocketFd();
    newConnection.BindSocket();
    printf("The socket file descriptor is: %d.\n", sockfd);     
}

