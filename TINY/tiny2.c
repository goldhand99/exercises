#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *fieltype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd,
                 char *cause,
                 char *errnum,
                 char *shortsmsg,
                 char *longmsg);
int Open_listenfd(char *port);

int main(int argc, char **argv)
{

    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2)
    {
        return ~1;
    }

    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        getnameinfo((SA *)&clientaddr,
                    clientlen,
                    hostname,
                    MAXLINE,
                    port,
                    MAXLINE, 0);

        printf("Client %s connected\n", hostname);
        doit(connfd);
        close(connfd);
    }
    return 1;
}

void echo(int fd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, fd);

    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) > 0)
    {
        if (strcmp(buf, "\r\n") == 0)
        {
            break;
        }
        Rio_writen(fd, buf, n);
    }
}

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
    if (strcasecmp(method, "GET"))
    { //line:netp:doit:beginrequesterr
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }                       //line:netp:doit:endrequesterr
    read_requesthdrs(&rio); //line:netp:doit:readrequesthdrs

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs); //line:netp:doit:staticcheck
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
        serve_static(fd, filename, sbuf.st_size); //line:netp:doit:servestatic
    }
    else
    { /* Serve dynamic content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        { //line:netp:doit:executable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs); //line:netp:doit:servedynamic
    }
}

void clienterror(int fd,
                 char *cause,
                 char *errnum,
                 char *shortmsg,
                 char *longmsg)
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

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin"))
    { /* Static content */                 //line:netp:parseuri:isstatic
        strcpy(cgiargs, "");               //line:netp:parseuri:clearcgi
        strcpy(filename, ".");             //line:netp:parseuri:beginconvert1
        strcat(filename, uri);             //line:netp:parseuri:endconvert1
        if (uri[strlen(uri) - 1] == '/')   //line:netp:parseuri:slashcheck
            strcat(filename, "home.html"); //line:netp:parseuri:appenddefault
        return 1;
    }
    else
    { /* Dynamic content */    //line:netp:parseuri:isdynamic
        ptr = index(uri, '?'); //line:netp:parseuri:beginextract
        if (ptr)
        {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, ""); //line:netp:parseuri:endextract
        strcpy(filename, ".");   //line:netp:parseuri:beginconvert2
        strcat(filename, uri);   //line:netp:parseuri:endconvert2
        return 0;
    }
}
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
    //Send response headers
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-Length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));

    // Send response to client
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize);
}

void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = {NULL};

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0)
    { /* child */ //line:netp:servedynamic:fork
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);                         //line:netp:servedynamic:setenv
        Dup2(fd, STDOUT_FILENO); /* Redirect stdout to client */    //line:netp:servedynamic:dup2
        Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    }
    Wait(NULL);
}
