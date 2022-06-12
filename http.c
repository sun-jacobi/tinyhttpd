/* annotated by Jacobi-SUN*/
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h> 
#include <string.h> 

#define ISspace(x) isspace((int)(x))


void accept_request(int);
void cat(int, FILE *);
int startup(u_short *);
void error_die(const char*);
void accept_request(int);
int get_line(int, char*, int);
void headers(int, const char *);
void unimplemented(int);
void not_found(int);
void execute_cgi(int, const char *, const char *,  const char *);



void accept_request(int client)
{
  char buf[1024];
  int numchars;
  char method[255];
  char url[255];
  char path[512];
  size_t i, j;
  struct stat st;
  int cgi = 0;
  
  char *query_string = NULL;
  numchars = get_line(client, buf, sizeof(buf));
  i = 0; j = 0;
  // parse the method
  while(!ISspace(buf[j]) && (i < sizeof(method) - 1))
  {
    method[i] = buf[j];
    i++; j++;
  }
  method[i] = '\0';  
  // Only support GET and POST
  if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
  {
    unimplemented(client);
    return;
  }
  
  if (strcasecmp(method, "POST") == 0)
   cgi = 0;

  i = 0;
  while (ISspace(buf[j]) && (j < sizeof(buf)))
    j++;
  
  while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
  {
    url[i] = buf[j];
    i++; j++;
  }
  url[i] = '\0';
  if (strcasecmp(method, "GET") == 0)
  {
    query_string = url;
    while ((*query_string != '?') && (*query_string != '\0'))
      query_string++;
    // ignore the request parameter
    if (*query_string == '?') 
    {
      cgi = 1;
      *query_string = '\0';
      query_string++;
    } 
  }
  sprintf(path, "htdocs%s", url);
  // if url end with '/', then direct to index.html(Default)
  if (path[strlen(path) - 1] == '/')
    strcat(path, "index.html");
  if  (stat(path, &st) == -1) {
    while((numchars > 0) && strcmp("\n", buf))
     numchars = get_line(client, buf, sizeof(buf));
     not_found(client);
  }
  else
  {
    if ((st.st_mode & S_IFMT) == S_IFDIR)
      strcat(path, "/index.html");
    if ((st.st_mode & S_IXUSR) ||
        (st.st_mode & S_IXGRP) ||
        (st.st_mode & S_IXOTH)     )
      cgi = 1;
    if (!cgi)
      serve_file(client, path);
    else 
      execute_cgi(client, path, method, query_string);
  }
  close(client);
}


void error_die(const char *sc){
    perror(sc);
    exit(1);
} 

void accept_request(int client){
    char buf[1024];
    int numchars;
    char method[255];
    char url[255];
    char path[512];
    size_t i,j; 
    struct stat st; /* from stat.h header  */
    int cgi = 0; /* becomes true if server decide
                this is a CGI program */
    char* query_string = NULL;
    numchars = get_line(client, buf, sizeof(buf));
    i = 0; j = 0;
    while (!ISspace(buf[j]) && (i < sizeof(method) - 1)){
        method[i] = buf[j];
        i++; j++;
    }
    method[i] = '\0';
    if (strcasecmp(method,"GET") && strcasecmp(method,"POST")){
        unimplemented(client);
        return;
    }
    if (strcasecmp(method,"POST") == 0){
        cgi = 1;
    }
    i = 0;
    
}


void not_found(int client)
{

}



void serve_file(int client, const char *filename)
{
  FILE *resource = NULL;
  int numchars = 1;
  char buf[1024];
  buf[0] = 'A'; buf[1] = '\0';
  while((numchars > 0) & strcmp("\n", buf))
    numchars = get_line(client, buf, sizeof(buf));
  resource = fopen(filename, "r");
  if (resource == NULL) 
    not_found(client);
  else
  {
    headers(client, filename);
    cat(client, resource);
  }
  fclose(resource);

}






int startup(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name;

    httpd = socket(PF_INET,SOCK_STREAM,0); /*create a socket for POSIX, TCP, IP*/ 
    if (httpd == -1) {
        error_die("socket"); /* if failed, then exit */ 
    }
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(httpd, (struct sockaddr*)&name,sizeof(name)) < 0 )
      error_die("bind");
    if (*port == 0)
    {
      int namelen = sizeof(name);
      if (getsockname(httpd,(struct sockaddr*)&name,&namelen) == -1)
        error_die("getsockname");
      *port = ntohs(name.sin_port); 
    } /* if the port is 0, then allocate a port  */
    if (listen(httpd,5) < 0){
        error_die("bind");      
    }
    return(httpd);
}


/**********************************************************************/

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

    // while loop run the server
    // server accept the client_sock from serve_sock.
    // and create a thread to deal with it
    while(1)
    {
        client_sock = accept(server_sock,
                             (struct sockaddr*)&client_name,
                             &client_name_len); 
        if (client_sock == -1)
          error_die("accept"); /* accept client socket fail -> exit  */    
        if (pthread_create(&newthread,NULL,accept_request,client_sock) != 0){
          perror("pthread_create"); /*p_thread create fail  */
        }
    }
    close(server_sock);
    return 0;
}