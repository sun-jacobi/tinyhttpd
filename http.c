/* annotated by Jacobi-SUN*/
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h> 

int startup(u_short *);
void error_die(const char*);
void accept_request(int);

void error_die(const char *sc){
    perror(sc);
    exit(1);
}

int startup(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name;
    httpd = socket(PF_INET,SOCK_STREAM,0);
    if (httpd == -1){
        error_die("socket");
    }
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(httpd,(struct sockaddr*)&name,sizeof(name)) < 0 ){
        error_die("bind");
    }
    if (*port == 0){
        int namelen = sizeof(name);
        if (getsockname(httpd,(struct sockaddr*)&name,&namelen) == -1){
            error_die("getsockname");
        };
        *port = ntohs(name.sin_port);   
    }
    if (listen(httpd,5) < 0){
        error_die("bind");      
    }
    return(httpd);
}
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
        } /* accept client socket fail -> exit  */    
        if (pthread_create(&newthread,NULL,accept_request,client_sock) != 0){
            perror("pthread_create"); /*p_thread create fail  */
        }
    }
    close(server_sock);
    return 0;
}