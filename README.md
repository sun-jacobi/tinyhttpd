# Tinyhttpd 

A tiny http server created by J. David in 1999 


This is my personal annotation (or learning note) on it. 

With the purpose to learn some C ,and some basic knowledge on http server. 

Here I will write some preliminary knowledge.

## Basic Concept 

Host is identified by IP address, which is a 32-bit quantity. Sending process need to identify the process on the receiving host, since a host could run many application. Port are used to do this.


## Socket programming 
Reference : 

[1] https://www.geeksforgeeks.org/socket-programming-cc/

[2] https://man7.org/linux/man-pages/index.html

>Socket programming is a way

to connect 2 nodes on a network
#### Socket creation: 
```
int sockfd = socket(domain,type,protocal);
```
domain: commnunication domain \
*AP_INET and AF_I NET 6 for IPV6*

type: communication type
* SOCK_STREAM : TCP
* SOCK_DGRAM  : UDP

Protocal : value for the IP is 0

#### Bind : 
```C
int bind(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
```
bind a name (socketaddr) to a socker.bind()
It means to assign the address specified by *addr* to the socket referred to file descriptor sockfd. The *addrlen* specifies the size, in bytes, of the address structure pointed to by addr.

Error: Return -1 

#### Listen : 
```C
int listen(int sockfd, int backlog);
```
listen() makes the server socket wait for the client request

backlog : the maximum length to the queue, if full, then *ECONNREFUSED*
####  Accept:
```C
int new_socket = accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
```
extract the first connection request *sockfd* on the queue, create new connected socket, and return a file descriptor refferring to  the connected socket

#### socket connection
```C
int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
```
