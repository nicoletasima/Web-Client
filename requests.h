#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, char *url, char *query_params,
							char *cookies);
// add authorization token
char *auth_compute_get_request(char *host, char *url, char *query_params, char *authorization);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
							char** cookies, int cookies_count);
// post with authorization token
char *auth_compute_post_request(char *host, char *url, char *content_type, char *body_data, char *authorization);

// delete book
char *compute_delete_request(char *host, char *url, char *query_params,
                            char *authorization);

#endif
