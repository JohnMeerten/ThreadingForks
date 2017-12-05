#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         bytesRcvd;              /* Bytes read in single recv() */
	int 		mode = 0;
    int			quit =0;

	parse_args (argc, argv);
	
    sock = CreateTCPClientSocket (argv_ip, argv_port);
        
    while(quit ==0)
    {
        switch(mode)
        {
			case 0:
			{
		        fgets(echoBuffer,RCVBUFSIZE,stdin);
		        //fgets always leaves 1 empty space in array.

		        delaying();
		        
		        // parameters are socket, string, lenght, flags.
		        
		        if (send (sock, echoBuffer, RCVBUFSIZE, 0) != RCVBUFSIZE)
		        {
		            DieWithError ("send() failed");
		        }
		        mode++;
		        break;
			}
			case 1:
			{
			    bytesRcvd = recv (sock, echoBuffer, RCVBUFSIZE-1, 0);
				if (bytesRcvd < 0)
				{
					DieWithError ("recv() failed");
				}
		        if (bytesRcvd > 1)
		        {	
					if(strncmp(echoBuffer, "quit", strlen("quit")) ==0)
					{
						quit = 1;
						break;
					}
					else
					{
						echoBuffer[bytesRcvd] = '\0';
						printf("Received string: %s \n", echoBuffer);	
						mode--;
					}
				}
				break;
			}
		}
    }

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}
