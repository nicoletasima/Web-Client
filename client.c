#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

/* functia extrage cookie-ul din raspuns */
char *get_cookie_from_response(char *response) {	

	char *token = strtok(response, "\n");
	while (strncmp(token, "Set-Cookie", 10) != 0) {
		token = strtok(NULL, "\n");
	}

	char *token1 = strtok(token, " ");
	token1 = strtok(NULL, " ");
	char *cookie = strtok(token1, ";");

	return cookie;
}

/* functia extrage token-ul din raspuns */
char *get_authorization_token(char *response) {
	char *token = strtok(response, "\n");

	while (strncmp(token, "{", 1) != 0) {
		token = strtok(NULL, "\n");
	}

	char *auth_token = strtok(token, "\"");
	auth_token = strtok(NULL, "\"");
	auth_token = strtok(NULL, "\"");
	auth_token = strtok(NULL, "\"");

	return auth_token;
}

int main(int argc, char **argv)	
{
	int sockfd;
	char *command = malloc(30 * sizeof(char));	// comanda primita de la tastatura
	char *message;
	char *response;
	char *cookie = NULL;
	char *auth_token = NULL;

	while (strcmp(fgets(command, 30, stdin), "exit\n") != 0) {	// exit inchide programul

		if (strcmp(command, "register\n") == 0) {
			char *username = malloc(100 * sizeof(char));	// citire de la tastatura username si parola
			char *password = malloc(100 * sizeof(char));
			printf("username=");
			fgets(username, 100, stdin);
			printf("password=");
			fgets(password, 100, stdin);

			char *serialized_string = NULL;			// parsare payload de tip json
			JSON_Value *root_value = json_value_init_object();
			JSON_Object *root_object = json_value_get_object(root_value);
			json_object_set_string(root_object, "username", username);
			json_object_set_string(root_object, "password", password);
			serialized_string = json_serialize_to_string_pretty(root_value);
			// compunere cerere pentru trimitere la server
			message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/register", "application/json",
			 serialized_string, NULL, 0); 
			
			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);	// deschidere conexiune 
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);			// trimite cerere la server
			response = receive_from_server(sockfd);		// ia raspunsul primit de la server
			printf("%s\n", response);

			if (strncmp(response, "HTTP/1.1 201 Created", 20) == 0) {	// mesaje pentru diferite cazuri intalnite
				printf("Account created!\n");
			} else {
				printf("Something went wrong!\n");
			}

			close(sockfd);
		}

		if (strcmp(command, "login\n") == 0) {
			char *username = malloc(100 * sizeof(char));
			char *password = malloc(100 * sizeof(char));
			printf("username=");
			fgets(username, 100, stdin);
			printf("password=");
			fgets(password, 100, stdin);

			char *serialized_string = NULL;		// creare obiect json
			JSON_Value *root_value = json_value_init_object();
			JSON_Object *root_object = json_value_get_object(root_value);
			json_object_set_string(root_object, "username", username);
			json_object_set_string(root_object, "password", password);
			serialized_string = json_serialize_to_string_pretty(root_value);
			
			message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/login", "application/json", 
				serialized_string, NULL, 0);
			
			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			
			close(sockfd);
			
			if (strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {	// tratare cazuri de succes sau eroare
				printf("%s\n", response);
				printf("You are logged in!\n");
				cookie = get_cookie_from_response(response);
			} else if (strncmp(response, "HTTP/1.1 400 Bad Request", 24) == 0) {
				printf("The user doesn't exist!\n");
			} else {
				printf("Something went wrong!");
			}
		}

		if (strcmp(command, "enter_library\n") == 0) {
			message = compute_get_request("3.8.116.10", "/api/v1/tema/library/access", NULL, cookie);

			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close(sockfd);
			if (strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
				auth_token = get_authorization_token(response);
				printf("You entered the library!\n");
			}
			
		}

		if (strcmp(command, "get_books\n") == 0) {
			message = auth_compute_get_request("3.8.116.10", "/api/v1/tema/library/books", NULL, auth_token);

			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close(sockfd);
		}

		if (strcmp(command, "get_book\n") == 0) {
			char *id = malloc(10 * sizeof(char));
			printf("id=");
			fgets(id, 10, stdin);
			int check_number = 1;
	
			for (int i = 0; i < strlen(id) - 1; i++) {	    // verificare daca ce s-a introdus de la tastatura
				if ((id[i] < '0' || id[i] > '9')) {			// este un numar valid
					printf("id must be a number!\n");
					check_number = 0;
					break;
				}
			}
			if (check_number == 0) {
				continue;
			}
			char *line = malloc(100 * sizeof(char));		// concatenare id la ruta
			sprintf(line, "/api/v1/tema/library/books/%d", atoi(id));
			message = auth_compute_get_request("3.8.116.10", line, NULL, auth_token);
			
			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			close(sockfd);
		}

		if (strcmp(command, "add_book\n") == 0) {
			char *title = malloc(100 * sizeof(char));
			char *author = malloc(100 * sizeof(char));
			char *genre = malloc(100 * sizeof(char));
			char *publisher = malloc(100 * sizeof(char));
			char *page_count = malloc(10 * sizeof(char));
			printf("title=");
			fgets(title, 100, stdin);
			printf("author=");
			fgets(author, 100, stdin);
			printf("genre=");
			fgets(genre, 100, stdin);
			printf("publisher=");
			fgets(publisher, 100, stdin);
			printf("page_count=");
			fgets(page_count, 10, stdin);

			int check_number = 1;
	
			for (int i = 0; i < strlen(page_count) - 1; i++) {	// verificare daca numarul de pagini este valid
				if ((page_count[i] < '0' || page_count[i] > '9')) {
					printf("page_count must be a number!\n");
					check_number = 0;
					break;
				}
			}
			if (check_number == 0) {
				continue;
			}
			char *serialized_string = NULL;		// creare payload json
			JSON_Value *root_value = json_value_init_object();
			JSON_Object *root_object = json_value_get_object(root_value);
			json_object_set_string(root_object, "title", title);
			json_object_set_string(root_object, "author", author);
			json_object_set_string(root_object, "genre", genre);
			json_object_set_string(root_object, "publisher", publisher);
			json_object_set_number(root_object, "page_count", atoi(page_count));
			serialized_string = json_serialize_to_string_pretty(root_value);

			message = auth_compute_post_request("3.8.116.10", "/api/v1/tema/library/books", "application/json",
					serialized_string, auth_token);

			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			
			if (strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
				printf("Book added successfully!\n");
			}

			close(sockfd);
			continue;
		}

		if (strcmp(command, "delete_book\n") == 0) {
			char *id = malloc(10 * sizeof(char));
			printf("id=");
			fgets(id, 10, stdin);
			int check_number = 1;
	
			for (int i = 0; i < strlen(id) - 1; i++) {	// verificare validitate id
				if (id[i] < '0' || id[i] > '9') {
					printf("id must be a number!\n");
					check_number = 0;
					break;
				}
			}

			if (check_number == 0) {
				continue;
			}

			char *line = malloc(100 * sizeof(char));
			sprintf(line, "/api/v1/tema/library/books/%d", atoi(id));
			message = compute_delete_request("3.8.116.10", line, NULL, auth_token);

			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			
			if (strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
				printf("%s\n", response);
				printf("Book with id = %d deleted successfully!\n", atoi(id));
			} else {
				printf("%s\n", response);
			}

			close(sockfd);
		}

		if (strcmp(command, "logout\n") == 0) {
			message = compute_get_request("3.8.116.10", "/api/v1/tema/auth/logout", NULL, cookie);

			sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
			DIE(sockfd < 0, "ERROR OPEN CONENCTION");
			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			printf("%s\n", response);
			if (strncmp(response, "HTTP/1.1 200 OK", 15) == 0) {
				printf("You are logged out now!\n");
			}
			close(sockfd);
		}

		// verificare cazuri de comanda invalida
		if (strcmp(command, "register\n") && strcmp(command, "login\n") && strcmp(command, "enter_library\n")
			&& (strcmp(command, "add_book\n") !=0) && strcmp(command, "get_book\n") && strcmp(command, "get_books\n")
			&& strcmp(command, "delete_book\n") && strcmp(command, "logout\n") && strcmp(command, "exit\n")) {
				printf("Invalid command! Try again!\n");
		}
	}

	return 0;
}