#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <stdio.h>
#include <string.h>     // for strlen()

#define RCVBUFSIZE 32   /* Size of receive buffer */

int main (int argc, char * argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    int quit = 0;
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int 		mode = 0;


    parse_args (argc, argv);
    
    servSock = CreateTCPServerSocket (argv_port);

    clntSock = AcceptTCPConnection (servSock);
    
    while(quit == 0)
	{
		switch(mode)
		{
			case 0:
			{
			    bytesRcvd = recv (clntSock, echoBuffer, RCVBUFSIZE-1, 0);
				if (bytesRcvd < 0)
				{
					DieWithError ("recv() failed");
				}
		        if (bytesRcvd > 1)
		        {
				    echoBuffer[bytesRcvd] = '\0';
					if(strncmp(echoBuffer, "quit", strlen("quit")) ==0)
					{
						quit = 1;
						break;
					}
					else{
					printf("Received string: %s \n", echoBuffer);
					
			        mode++;
					}
				}
				break;
			}
			case 1:
			{  
				fgets(echoBuffer,RCVBUFSIZE,stdin);
		        if (send (clntSock, echoBuffer, RCVBUFSIZE, 0) != RCVBUFSIZE)
		        {
		            DieWithError ("send() failed");
		        }
				mode--;
				break;
			}
		}
	}
   
        
    close (clntSock);    /* Close client socket */
    info ("close");
}
