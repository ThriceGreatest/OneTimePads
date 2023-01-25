#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
void error(const char *msg)
{
	perror(msg);
	exit(1);
} // Error function used for reporting issues

void encrypt(char ptxt[], char key[], char cipher[], int size){

	int i; 
	int intPTXT[size];
	int intKEY[size];
	int intCipher[size];

	for(i = 0; i < size; i++){

		if(ptxt[i] == ' '){
			cipher[i] = ' ';
			continue;
		}
			
		intPTXT[i] = (int)ptxt[i] - 'A';

		intKEY[i] = (int)key[i] - 'A';
		
		intCipher[i] = ((intPTXT[i] + intKEY[i]) % 26);

		cipher[i] = (char)intCipher[i] + 'A';
	//	printf("encrypt, %c -> %d -> %d -> %c\n", ptxt[i], intPTXT[i], intCipher[i], cipher[i]);
	}

	//printf("plaintext: %s\n", ptxt);
	//printf("Cipher: %s\n", cipher);

}

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char buffer[999999];
	char plaintext[99999];
	char cipher[99999];
	char key[99999];
	int size;
	

	struct sockaddr_in serverAddress, clientAddress;
	if (argc < 2)
	{
		fprintf(stderr, "USAGE: %s port\n", argv[0]);
		exit(1);
	} // Check usage & args
	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]);									 // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET;							 // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber);					 // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY;					 // Any address is allowed for connection to this process
	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0)
		error("ERROR opening socket");
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the d

	while (1)
	{
		// Accept a connection, blocking if one is not available until one connects
		sizeOfClientInfo = sizeof(clientAddress);																// Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0)
			error("ERROR on accept");
	//	printf("SERVER: Connected Client at port %d\n", ntohs(clientAddress.sin_port));
		// Get the message from the client and display it

		//Get size
	
		memset(buffer, '\0', strlen(buffer));
	///	recv(establishedConnectionFD, buffer, 255, 0); // Read the client's messageabout size
		//size_sent = atoi(buffer);
	//	printf("Size = %d\n", size_sent);

		charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer), 0); // Read the client's message about size;
		
		
		
		if (charsRead < 0)
			error("ERROR reading from socket");
	
		//Tokenize values into their respective variables
		
		char *token;
		//First is plaintext
		token = strtok(buffer,"\n");
		sprintf(plaintext, "%s", token);
		//printf("SERVER: I received this many from the client: \"%s\"\n", plaintext);
		size = strlen(plaintext);
		//Get key
		token = strtok(NULL, "\n");
		sprintf(key, "%s", token);
		//printf("SERVER: I received this many from the client: \"%s\"\n", key);
		

		pid_t spawnPid = -5;
		int childExitStatus = -5;
		spawnPid = fork();

		switch (spawnPid)
		{
		case -1:
			perror("Hull Breach!\n");
			exit(1);
			break;
		case 0:
			// Send a encrypted message back to the client
			encrypt(plaintext, key, cipher, size);
			charsRead = send(establishedConnectionFD, cipher, strlen(cipher), 0); // Send success back
			if (charsRead < 0)
				error("ERROR writing to socket");
			close(establishedConnectionFD); // Close the existing socket which is connected to the client
			break;
		default:
			fflush(stdout);
			waitpid(spawnPid, &childExitStatus, WNOHANG);
			break;
		}

	}
	close(listenSocketFD); // Close the listening socket
	return 0;
}
