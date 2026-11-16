#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdio.h>
int main()
{
    int nbSent;
    int retour;
    char Msg[] = "DIST";
    char Buffer[100];
    //Spécifique à WINDOWS
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN addr;
    SOCKADDR addr2;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    //Fin spécificité WINDOWS
    sock=socket(AF_INET, SOCK_DGRAM,0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.184.160"); //IP Serveur destinataire de la demande de connexion
    addr.sin_port = htons(8001); //Port d'écoute du serveur destinataire

    /*if (connect(sock,(SOCKADDR *)&addr,sizeof(addr))==-1)
    {
        fprintf(stderr,"bind problem");
        closesocket(sock);
        exit(-1);
    }
*/

    nbSent=sendto(sock,Msg,strlen(Msg),0,(SOCKADDR *)&addr,sizeof(addr));

    printf("sent %d\n",nbSent);

    retour = recvfrom(sock,Buffer,sizeof(Buffer),0,NULL,NULL);
    if (retour == SOCKET_ERROR) {
        // traitement de l'erreur
    }
    printf(Buffer);

    closesocket(sock);
    //Spécifique à WINDOWS
    WSACleanup();
    //Fin spécificité WINDOWS
    exit(-1);
}
