#ifndef NETCOM_H_ 
#define NETCOM_H_ 

int initCommunication();

int receiveBytes(int SOCKET, char* address, char* buffer);

#endif
