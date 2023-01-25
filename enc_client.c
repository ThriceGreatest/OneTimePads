#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent *serverHostInfo;
	char buffer[199999];
	char plaintext[99999];
	char key[99999];

	if (argc < 4)
	{
		fprintf(stderr, "USAGE: %s plaintext key port\n", argv[0]);
		exit(1);
	}

	if (access(argv[1], R_OK) == -1)
	{
		fprintf(stderr, "ERROR: Cannot read plaintext file\n");
		exit(1);
	}
	if (access(argv[2], R_OK) == -1)
	{
		fprintf(stderr, "ERROR: Cannot read key file\n");
		exit(1);
	}

	// Read plaintext and key from files
	FILE *plaintextFile = fopen(argv[1], "r");
	FILE *keyFile = fopen(argv[2], "r");

	// Check for bad characters in plaintext and key files
	int c;
	while ((c = fgetc(plaintextFile)) != EOF)
	{
		if (c == ' ' || c == '\n')
		{
			continue;
		}
		if (!isalpha(c))
		{
			fprintf(stderr, "ERROR: Invalid character in plaintext file\n");
			exit(1);
		}
	}
	while ((c = fgetc(keyFile)) != EOF)
	{
		if (c == ' ' || c == '\n')
		{
			continue;
		}
		if (!isalpha(c))
		{
			fprintf(stderr, "ERROR: Invalid character in key file\n");
			exit(1);
		}
	}

	// Check if key file is shorter than plaintext file
	if (ftell(keyFile) < ftell(plaintextFile))
	{
		fprintf(stderr, "ERROR: Key file is shorter than plaintext file\n");
		exit(1);
	}


	// Check usage & args
	// Set up the server address struct
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]);									 // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET;							 // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber);					 // Store the port number
	serverHostInfo = gethostbyname("localhost");				 // Convert the machine name into a special form of address
	if (serverHostInfo == NULL)
	{
		fprintf(stderr, "CLIENT: ERROR, no such host\n");
		exit(0);
	}

	memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)serverHostInfo->h_addr, serverHostInfo->h_length);
	// Copy in the address
	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0)
		error("CLIENT: ERROR opening socket");
	// Connect to server
	if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to addy
		error("CLIENT: ERROR connecting");


	// Read data from the file and send it to the server in buffered chunks

	fseek(plaintextFile, 0, SEEK_SET);
	fseek(keyFile, 0, SEEK_SET);
	
	//send
	memset(plaintext, '\0', sizeof(plaintext));
	memset(key, '\0', sizeof(key));

	fgets(plaintext, sizeof(plaintext), plaintextFile);
	fgets(key, sizeof(key), keyFile);

	charsWritten = 0;
	memset(buffer, '\0', sizeof(buffer));

	
	sprintf(buffer, "%s\n%s\n", plaintext, key);
	charsWritten += send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	//while(charsWritten < size){
	//	memset(buffer, '\0', sizeof(buffer));	  // Clear out the buffer array
//		fgets(buffer, 9999, plaintextFile);		//Read 256 characters from buffer
//		charsWritten += send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	//}



	if (charsWritten < 0)
		error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(buffer))
		printf("CLIENT: WARNING: Not all data written to socket!\n");
	// Get return message from server
	memset(buffer, '\0', sizeof(buffer));					   // Clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
	if (charsRead < 0)
		error("CLIENT: ERROR reading from socket");

	printf("%s\n", buffer);
	close(socketFD); // Close the socket
	return 0;
}
