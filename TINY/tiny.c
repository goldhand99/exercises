/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 */
#include "csapp.h"
#include <stdio.h>
#include <stdlib.h>

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize, char *method);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs, char *method);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);
int Open_listenfd(char *port);
void signal_child(int sig);
int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    /* Check command line args */
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if (Signal(SIGCHLD, &signal_child) == SIG_ERR)
    {
        exit(~1);
    }

    port = atoi(argv[1]);
    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen); //line:netp:tiny:accept
        doit(connfd);                                                          //line:netp:tiny:doit
        Close(connfd);                                                         //line:netp:tiny:close
    }
}
/* $end tinymain */

void signal_child(int sig)
{
    int old_errno = errno;
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
    }
    errno = old_errno;
}

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);             //line:netp:doit:readrequest
    sscanf(buf, "%s %s %s", method, uri, version); //line:netp:doit:parserequest
    printf("Buffer with everything %s \n", buf);
    printf("This is method %s uri %s version %s \n", method, uri, version);

    int isHead = 0;

    if (!(strcasecmp(method, "HEAD") == 0 || strcasecmp(method, "GET") == 0))
    {
        printf("Entered the error strcmp \n");
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }
    if (strcasecmp(method, "HEAD") == 0)
    { //line:netp:doit:beginrequesterr
        isHead = 1;
    }                       //line:netp:doit:endrequesterr
    read_requesthdrs(&rio); //line:netp:doit:readrequesthdrs

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs); //line:netp:doit:staticcheck
    char a[6] = {'a', 'd', 'd', 'e', 'r', '\0'};
    printf("this is unedited filename %s\n", filename);

    if (stat(filename, &sbuf) < 0)
    { //line:netp:doit:beginnotfound
        clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
        return;
    } //line:netp:doit:endnotfound

    if (is_static)
    { /* Serve static content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
        { //line:netp:doit:readable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size, method); //line:netp:doit:servestatic
    }
    else
    { /* Serve dynamic content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        { //line:netp:doit:executable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs, method); //line:netp:doit:servedynamic
    }
}
/* $end doit */

/*
 * read_requesthdrs - read and parse HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    { //line:netp:readhdrs:checkterm
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;
    printf("this is the uri received %s\n", uri);
    if (!strstr(uri, "/dir"))
    { /* Static content */ //line:netp:parseuri:isstatic
        printf("Entered undyna ic contex\n");
        strcpy(cgiargs, "");               //line:netp:parseuri:clearcgi
        strcpy(filename, ".");             //line:netp:parseuri:beginconvert1
        strcat(filename, uri);             //line:netp:parseuri:endconvert1
        if (uri[strlen(uri) - 1] == '/')   //line:netp:parseuri:slashcheck
            strcat(filename, "home.html"); //line:netp:parseuri:appenddefault
        return 1;
    }
    else
    { /* Dynamic content */ //line:netp:parseuri:isdynamic
        printf("Entered the ydnamic context");
        ptr = index(uri, '?'); //line:netp:parseuri:beginextract
        if (ptr)
        {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, ""); //line:netp:parseuri:endextract
        strcpy(filename, "..");  //line:netp:parseuri:beginconvert2
        strcat(filename, uri);   //line:netp:parseuri:endconvert2

        return 0;
    }
}
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize, char *method)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    if (strcasecmp(method, "HEAD") == 0)
    {
        return;
    }

    /* Send response headers to client */
    get_filetype(filename, filetype); //line:netp:servestatic:getfiletype

    sprintf(buf, "HTTP/1.0 200 OK\r\n"); //line:netp:servestatic:beginserve
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf)); //line:netp:servestatic:endserve

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0); //line:netp:servestatic:open
    if (srcfd < 0)
    {
        printf("Theere is a null srcfd\n");
    }
    srcp = (char *)malloc(filesize);
    Rio_readn(srcfd, srcp, filesize);
    Rio_writen(fd, srcp, filesize);
    return;
    srcp = Mmap(0, filesize, PROT_READ, MAP_SHARED, srcfd, 0); //line:netp:servestatic:mmap
    if (*((int *)srcp) == -1)
    {
        printf("The mmap failed\n");
    }
    Close(srcfd);                   //line:netp:servestatic:close
    Rio_writen(fd, srcp, filesize); //line:netp:servestatic:write
    Munmap(srcp, filesize);         //line:netp:servestatic:munmap
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else if (strstr(filename, "./mpg"))
        strcpy(filetype, "video/mpeg");
    else
        strcpy(filetype, "text/plain");
}
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs, char *method)
{
    char buf[MAXLINE], *emptylist[] = {NULL};
    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0)
    { /* child */ //line:netp:servedynamic:fork
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1); //line:netp:servedynamic:setenv
        setenv("REQUEST_ENV", method, 1);
        Dup2(fd, STDOUT_FILENO); /* Redirect stdout to client */    //line:netp:servedynamic:dup2
        Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    }
    Wait(NULL); /* Parent waits for and reaps child */ //line:netp:servedynamic:wait
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor="
                  "ffffff"
                  ">\r\n",
            body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
/* $end clienterror */