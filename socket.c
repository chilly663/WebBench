/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int Socket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;
    struct sockaddr_in ad;
    struct hostent *hp;
    //sockaddr_in 的结构
    //sin_family 是族域名，区分ipv4和ipv6的应该是
    memset(&ad, 0, sizeof(ad));//清空域名结构体，为了防止垃圾无效的数字
    ad.sin_family = AF_INET;  //设置ipv4，这里用ipv4纯是因为当时没有ipv6，之后试试嫩不能改成ipv6兼容


    inaddr = inet_addr(host);//将域名变成32位存储，但是inet_addr貌似不支持ipv6，而且不安全
    if (inaddr != INADDR_NONE)//INADDR_NONE是用来判断是否合法，但是对于255.255.255.255会出问题
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    else//可能主机
    {
        hp = gethostbyname(host);
        if (hp == NULL)
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    ad.sin_port = htons(clientPort);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)
        return -1;
    return sock;
}

