#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_delete_request(char *host, char *url, char *query_params,
                            char *authorization)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
   
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }
 
    compute_message(message, line);

  
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (authorization != NULL) {
        sprintf(line, "Authorization: Bearer %s", authorization);
        compute_message(message, line);
    }
   
    compute_message(message, "");
    return message;
}

char *compute_get_request(char *host, char *url, char *query_params,
                            char *cookies)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
 
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL) {
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);
    }
   
    compute_message(message, "");
    return message;
}

char *auth_compute_get_request(char *host, char *url, char *query_params,
                            char *authorization)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
 
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (authorization != NULL) {
        sprintf(line, "Authorization: Bearer %s", authorization);
        compute_message(message, line);
    }
   
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                        char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
   
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);

    char *cookie = calloc(LINELEN, sizeof(char));
    if (cookies != NULL) {
        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(cookie, cookies[i]);
            strcat(cookie, ";");
        }
        strcat(cookie, cookies[cookies_count - 1]);
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }
   
    compute_message(message, "");
    memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}

char *auth_compute_post_request(char *host, char *url, char* content_type, char *body_data,
                        char *authorization)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    if (authorization != NULL) {
        sprintf(line, "Authorization: Bearer %s", authorization);
        compute_message(message, line);
    }

    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);
   
    compute_message(message, "");
    memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}
