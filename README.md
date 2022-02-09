# Tinyhttpd 

A tiny http server created by J. David in 1999 


This is my personal annotation (or learning note) on it. 

With the purpose to learn some C ,and some basic knowledge on http server. 

Here I will write some preliminary knowledge.

## Basic Concept 

Host is identified by IP address, which is a 32-bit quantity. Sending process need to identify the process on the receiving host, since a host could run many application. Port are used to do this.


### Socket programming 
Reference : https://www.geeksforgeeks.org/socket-programming-cc/


>Socket programming is a way
to connect 2 nodes on a network
+ Socket creation: 
```
int sockfd = socket(domain,type,protocol)
```
domain: commnunication domain
*AP_INET and AF_I NET 6 for IPV6*

type: communication type
* SOCK_STREAM : TCP
* SOCK_DGRAM  : UDP

Protocal : value for the IP is 0






