#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "../../Base.h/Base/base.h"

#define BROADCAST_PORT 5001
#define DATA_PORT 5002

using namespace std;
using namespace Base::Sockets;

TcpListener* broadcastListener;
TcpListener* dataListener;

TcpClient* broadcastClient1 = NULL;
TcpClient* broadcastClient2 = NULL;
TcpClient* dataClient1 = NULL;
TcpClient* dataClient2 = NULL;

void InitBroadcastListener();
void InitDataListener();

void OnBroadcastClientAccepted(TcpClient*);
void OnBroadcastReceive1(char*, int);
void OnBroadcastReceive2(char*, int);

void OnDataClientAccepted(TcpClient*);
void OnDataReceive1(char*, int);
void OnDataReceive2(char*, int);

int main()
{
	InitSocketLibrary();
	
	InitBroadcastListener();
	InitDataListener();

	while(true)
	{
		SleepMilliseconds(1000);
	}
}

void InitBroadcastListener()
{
	broadcastListener = new TcpListener();
	bool success = broadcastListener->Bind(IpAddress.Any, BROADCAST_PORT);
	if(success == true) 
	{
		printf("Broadcast-Listener erfolgreich initialisiert\n");
		success = broadcastListener->StartListening(OnBroadcastClientAccepted);
		if(success == false)
		{
			printf("Fehler beim Listen des Broadcast-Listeners\n");
		}
	}
	else
	{
		printf("Fehler beim initialisieren des Broadcast-Listeners\n");
	}
}

void InitDataListener()
{
	dataListener = new TcpListener();
	bool success = dataListener->Bind(IpAddress.Any, DATA_PORT);
	if(success == true) 
	{
		printf("Data-Listener erfolgreich initialisiert\n");
		success = dataListener->StartListening(OnDataClientAccepted);
		if(success == false)
		{
			printf("Fehler beim Listen des Data-Listeners\n");
		}
	}
	else
	{
		printf("Fehler beim initialisieren des Data-Listeners\n");
	}
}


void OnBroadcastClientAccepted(TcpClient* client)
{
	if(broadcastClient1 == NULL) 
	{
		printf("Broadcast-Client-1 hat sich verbunden\n");
		broadcastClient1 = client;
		client->StartReceiveData(OnBroadcastReceive1);
	}
	else if(broadcastClient2 == NULL)
	{
		printf("Broadcast-Client-2 hat sich verbunden\n");
		broadcastClient2 = client;
		client->StartReceiveData(OnBroadcastReceive2);
	}
}

void OnBroadcastReceive1(char* data, int length)
{
	if(broadcastClient2 != NULL)
	{
		printf("Broadcast von Client-1 erhalten\n");
		broadcastClient2->Send(data, length);
	}

	delete data;
}

void OnBroadcastReceive2(char* data, int length)
{
	printf("Broadcast von Client-2 erhalten\n");
	broadcastClient1->Send(data, length);

	delete data;
}


void OnDataClientAccepted(TcpClient* client)
{
	if(dataClient1 == NULL)
	{
		printf("Data-Client-1 hat sich verbunden\n");
		dataClient1 = client;
		client->StartReceiveData(OnDataReceive1);
	}
	else if(dataClient2 == NULL)
	{
		printf("Data-Client-2 hat sich verbunden\n");
		dataClient2 = client;
		client->StartReceiveData(OnDataReceive2);
	}
}

void OnDataReceive1(char* data, int length)
{
	if(dataClient2 != NULL)
	{
		printf("Daten von Client-1 erhalten\n");
		dataClient2->Send(data, length);
	}

	delete data;
}

void OnDataReceive2(char* data, int length)
{
	printf("Daten von Client-2 erhalten\n");
	dataClient1->Send(data, length);

	delete data;
}