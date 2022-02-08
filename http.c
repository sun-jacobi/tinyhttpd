/* annotated by Jacobi-SUN*/
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h> 

int startup(u_short *);
int error_die(const char*);
void accept_request(int);


int main(void){
    int server_sock = -1;
    u_short port = 0; /* unsigned short in <sys/types.h> */ 
    int client_sock = -1;
    struct sockaddr_in client_name; 
    /*sockaddr_in was defined in the <netinet/in.h>, which is composed of  */ 
    int client_name_len = sizeof(client_name);
    pthread_t newthread; /* defined in <sys/types.h> to create a thread*/ 
    server_sock = startup(&port);
    printf("httpd running on port %d \n,port");
    while(1){
        client_sock = accept(server_sock,
        (struct sockaddr*)&client_name,
        &client_name_len);
        if (client_sock == -1){
            error_die("accept");
        }       
        if (pthread_create(&newthread,NULL,accept_request,client_sock) != 0){
            perror("pthread_create");
        }
    }
    close(server_sock);
    return 0;
}