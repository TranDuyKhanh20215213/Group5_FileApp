

#include "Resources.h"
#include "DataIO.h"
#include "Account.h"
#include "Group.h"
#include "File.h"

int nClients = 0;

HANDLE completionPort;
HANDLE emptyQueue, fullQueue, mutex;

CRITICAL_SECTION cs;
CRITICAL_SECTION perClient[MAX_MEMBER];

LPPER_HANDLE_DATA perHandleData[MAX_MEMBER];
LPPER_IO_OPERATION_DATA perIoData[MAX_MEMBER];

unsigned __stdcall serverWorkerThread(LPVOID CompletionPortID);
void splitPath(const char *path, char *parent, char *last)
{
	const char *lastSlash = strrchr(path, '/'); // Find the last occurrence of '/'

	if (lastSlash != nullptr)
	{											// If '/' is found
		size_t parentLength = lastSlash - path; // Calculate length of parent part
		strncpy(parent, path, parentLength);	// Copy the parent part
		parent[parentLength] = '\0';			// Null-terminate the parent string
		strcpy(last, lastSlash + 1);			// Copy the part after the last '/'
	}
	else
	{
		strcpy(parent, ""); // If no '/', parent is empty
		strcpy(last, path); // The whole path is the last element
	}
}
void extractLastSegment(const char *path, char *lastSegment, size_t size)
{
	const char *lastSlash = strrchr(path, '/'); // Find the last occurrence of '/'

	if (lastSlash != nullptr)
	{
		// Copy the substring after the last '/'
		strncpy(lastSegment, lastSlash + 1, size - 1);
		lastSegment[size - 1] = '\0'; // Ensure null-termination
	}
	else
	{
		// If no '/' is found, copy the whole string
		strncpy(lastSegment, path, size - 1);
		lastSegment[size - 1] = '\0'; // Ensure null-termination
	}
}
int main(int argc, char *argv[])
{

	SOCKADDR_IN serverAddr;
	SOCKET listenSock, acceptSock;
	SYSTEM_INFO systemInfo;
	DWORD transferredBytes;
	WSADATA wsaData;
	DWORD Flags = 0;

	// initialize Winsock
	if (WSAStartup((2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup() failed with error " << GetLastError() << endl;
		return 1;
	}

	if ((completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		cout << "CreateIoCompletionPort() failed with error " << GetLastError() << endl;
		return 1;
	}

	// get systemInfo

	GetSystemInfo(&systemInfo);
	// create multiple threads (twice as many as the number of processors) handle server tasks in parallel
	for (int i = 0; i < (int)systemInfo.dwNumberOfProcessors * 2; i++)
	{
		if (_beginthreadex(0, 0, serverWorkerThread, (void *)completionPort, 0, 0) == 0)
		{
			cout << "Create thread failed with error " << GetLastError() << endl;
			return 1;
		}
	}

	// Initialize some important thing in system
	InitializeCriticalSection(&cs);
	mutex = CreateMutex(NULL, NULL, NULL);
	fullQueue = CreateSemaphore(NULL, 0, BACKLOG, NULL);
	emptyQueue = CreateSemaphore(NULL, BACKLOG, BACKLOG, NULL);
	loadAccountTxt(listAccount);
	loadGroupTxt(listGroup);
	createFolder(SERVER_FOLDER);
	for (int i = 0; i < listGroup.size(); i++)
	{
		createSubFolder(SERVER_FOLDER, listGroup[i].nameGroup);
		createTempFolder(listGroup[i].nameGroup);
		// createLogFile(listGroup[i].nameGroup);
		createRequestFile(listGroup[i].nameGroup);
	}

	if ((listenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		cout << "WSASocket() failed with error " << WSAGetLastError() << endl;
		return 1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons((unsigned short)atoi((char *)argv[1]));

	if (bind(listenSock, (PSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cout << "bind() failed with error " << WSAGetLastError() << endl;
		return 1;
	}

	if (listen(listenSock, BACKLOG) == SOCKET_ERROR)
	{
		cout << "listen() failed with error " << WSAGetLastError() << endl;
		return 1;
	}

	cout << "Server started !" << endl;

	while (true)
	{

		if ((acceptSock = WSAAccept(listenSock, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			cout << "WSAAccept() failed with error " << WSAGetLastError() << endl;
			return 1;
		}

		WaitForSingleObject(emptyQueue, INFINITE);
		WaitForSingleObject(mutex, INFINITE);

		if ((perHandleData[nClients] = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
		{
			cout << "GlobalAlloc() failed with error " << GetLastError() << endl;
			return 1;
		}
		perHandleData[nClients]->socket = acceptSock;
		cout << "Socket number " << perHandleData[nClients]->socket << " got connected..." << endl;

		if (CreateIoCompletionPort((HANDLE)perHandleData[nClients]->socket, completionPort, (ULONG_PTR)perHandleData[nClients], 0) == NULL)
		{
			cout << "CreateIoCompletionPort() failed with error " << GetLastError() << endl;
			return 1;
		}

		nClients++;
		cout << nClients << " Clients" << endl;
		ReleaseSemaphore(mutex, 1, NULL);
		ReleaseSemaphore(fullQueue, 1, NULL);

		if ((perIoData[nClients - 1] = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d\n", GetLastError());
			return 1;
		}

		ZeroMemory(&(perIoData[nClients - 1]->overlapped), sizeof(OVERLAPPED));
		perIoData[nClients - 1]->sentBytes = 0;
		perIoData[nClients - 1]->recvBytes = 0;
		perIoData[nClients - 1]->dataBuff.len = MESSAGE_SIZE;
		perIoData[nClients - 1]->dataBuff.buf = perIoData[nClients - 1]->buffer;
		perIoData[nClients - 1]->operation = RECEIVE;

		if (WSARecv(perHandleData[nClients - 1]->socket, &(perIoData[nClients - 1]->dataBuff), 1, &transferredBytes, &Flags, &(perIoData[nClients - 1]->overlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				cout << "WSARecv() failed with error " << WSAGetLastError() << endl;
				return 1;
			}
		}
	}
	updateGroupTxt(listGroup);
	updateAccountTxt(listAccount);

	DeleteCriticalSection(&cs);
	CloseHandle(mutex);
	CloseHandle(emptyQueue);
	CloseHandle(fullQueue);
	_getch();
	return 0;
}

unsigned __stdcall serverWorkerThread(LPVOID completionPortID)
{

	HANDLE completionPort = (HANDLE)completionPortID;
	DWORD transferredBytes;
	LPPER_HANDLE_DATA pHD;
	LPPER_IO_OPERATION_DATA pID;
	DWORD Flags = 0;

	int status;
	int i;
	Message msg = {NULL};
	// Account acc = { NULL };
	char *curPath = (char *)malloc(sizeof(char) * 1024);
	char *fileName = (char *)malloc(sizeof(char) * 1024);
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);

	while (TRUE)
	{

		status = GetQueuedCompletionStatus(completionPort, &transferredBytes, (PULONG_PTR)&pHD, (LPOVERLAPPED *)&pID, INFINITE);
		// cout<<"Transfer " << transferredBytes << " Status " << status <<endl;
		if (transferredBytes == 0 || status == 0)
		{
			// cout << "Close" <<endl;
			WaitForSingleObject(fullQueue, INFINITE);
			WaitForSingleObject(mutex, INFINITE);

			logout(nClients, listAccount, pHD, pID, transferredBytes);

			cout << "Closing socket " << pHD->socket << endl;
			for (i = 0; i < nClients; i++)
			{
				if (perHandleData[i]->socket == pHD->socket)
				{
					break;
				}
			}

			if (closesocket(pHD->socket) == SOCKET_ERROR)
			{
				cout << "closesocket() failed with error " << WSAGetLastError() << endl;
			}
			int j;
			for (j = i; j < nClients - 1; j++)
			{
				perHandleData[j] = perHandleData[j + 1];
			}
			GlobalFree(pHD);
			GlobalFree(pID);

			nClients--;

			ReleaseSemaphore(mutex, 1, NULL);
			ReleaseSemaphore(emptyQueue, 1, NULL);
			continue;
		}

		if (pID->operation == RECEIVE)
		{
			pID->recvBytes += transferredBytes;
			pID->sentBytes = 0;
		}
		else if (pID->operation == SEND)
		{
			pID->sentBytes += transferredBytes;
			pID->recvBytes = 0;
		}

		if (pID->sentBytes < pID->recvBytes)
		{
			if (pID->recvBytes < MESSAGE_SIZE)
			{
				recvMessage(pHD, pID, transferredBytes, MISSING);
			}
			else
			{
				msg = getMessage(pID->buffer);

				switch (msg.opcode)
				{
				case LOGIN:
					login(msg, listAccount, pHD, pID, transferredBytes);
					break;
				case LOGOUT:
					EnterCriticalSection(&cs);
					logout(nClients, listAccount, pHD, pID, transferredBytes);
					LeaveCriticalSection(&cs);
					break;
				case REGISTER:
					registerAcc(msg, listAccount, pHD, pID, transferredBytes);
					break;

				case SHOW_MY_GROUP:
					showMyGroup(msg, listGroup);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case SHOW_OTHER_GROUP:
					showOtherGroup(msg, listGroup);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case CREATE_GROUP:
					createGroup(msg, listGroup);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case ENTER_GROUP:
					enterGroup(msg, listGroup, nClients, pHD->socket, perHandleData);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case JOIN_GROUP:
					joinGroup(msg, listGroup);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case LEAVE_GROUP:
					leaveGroup(msg, listGroup);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;

				case UPLOAD:
					for (i = 0; i < nClients; i++)
					{
						if (perHandleData[i]->socket == pHD->socket)
						{
							break;
						}
					}
					upload(msg, perHandleData[i], pHD->socket);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case DATA_UP:
					for (i = 0; i < nClients; i++)
					{
						if (perHandleData[i]->socket == pHD->socket)
						{
							break;
						}
					}
					// If file transfer not complete
					if (recvDataUpload(msg, perHandleData[i], listAccount) != 0)
					{
						recvMessage(pHD, pID, transferredBytes, ALL);
					}
					// File transfer completed
					else
					{
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;
				case DOWNLOAD:
					for (i = 0; i < nClients; i++)
					{
						if (perHandleData[i]->socket == pHD->socket)
						{
							break;
						}
					}
					download(msg, perHandleData[i]);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case LIST_FILE:
					listFileToString(listGroup, msg.payload);
					craftMessage(msg, LIST_FILE, 0, strlen(msg.payload) + 1, msg.payload);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case LIST_MEMBER:
					listMemberToString(listGroup, listAccount, msg.payload);
					craftMessage(msg, LIST_MEMBER, 0, strlen(msg.payload) + 1, msg.payload);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case DELETE_MEMBER:
					removeMember(msg, listGroup, listAccount);
					cout << msg.payload;
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case CREATE_FOLDER:
					char oldPayload[PAYLOAD_SIZE];

					// Copy the old payload to a temporary variable
					strncpy(oldPayload, msg.payload, PAYLOAD_SIZE);

					// Set the original payload to an empty string
					memset(msg.payload, 0, PAYLOAD_SIZE);

					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload);
					if (createFolder(msg.payload) != -1)
					{
						craftMessage(msg, CREATE_FOLDER_SUCCESS, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					else
					{
						craftMessage(msg, FOLDER_ALREADY_EXIST, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;
				case RENAME_FOLDER:
					char oldPayload_rename[PAYLOAD_SIZE]; // Temporary buffer for received payload
					char *oldPath;						  // Pointer to the first part (curDir/nameFolder)
					char *newName;						  // Pointer to the second part (rename)
					char newFullName[PAYLOAD_SIZE];
					// Copy the original payload to a temporary buffer
					strncpy(oldPayload_rename, msg.payload, PAYLOAD_SIZE);

					// Split the payload using '|' as the delimiter
					oldPath = strtok(oldPayload_rename, "|"); // Extract the first part (curDir/nameFolder)
					newName = strtok(NULL, "|");			  // Extract the second part (rename)
					// printf("%s %s ", oldPath, newName);
					// Set the original payload to an empty string
					memset(msg.payload, 0, PAYLOAD_SIZE);
					// Construct the new payload
					char renamed_name[PAYLOAD_SIZE];
					char renamed_path[PAYLOAD_SIZE];
					splitPath(oldPath, renamed_path, renamed_name);
					char renamed_path2[PAYLOAD_SIZE];
					sprintf(renamed_path2, "%s/%s", SERVER_FOLDER, renamed_path);
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload_rename);
					sprintf(newFullName, "%s/%s", SERVER_FOLDER, newName);
					printf("%s \n", oldPath);
					printf("%s      %s", renamed_path2, renamed_name);
					if (checkFolder(renamed_path2, renamed_name) == 0)
					{
						if (renameFolder(msg.payload, newFullName) != -1)
						{
							craftMessage(msg, RENAME_FOLDER_SUCCESS, 0, 0, NULL);
							memcpy(pID->buffer, &msg, MESSAGE_SIZE);
							sendMessage(pHD, pID, transferredBytes, ALL);
						}
						else
						{
							craftMessage(msg, FOLDER_ALREADY_EXIST, 0, 0, NULL);
							memcpy(pID->buffer, &msg, MESSAGE_SIZE);
							sendMessage(pHD, pID, transferredBytes, ALL);
						}
					}
					else
					{
						craftMessage(msg, FOLDER_NOT_FOUND, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;

				case MOVE_FOLDER:
					char oldPayload_move[PAYLOAD_SIZE]; // Temporary buffer for received payload
					char *oldPath2;						// Pointer to the first part (curDir/nameFolder)
					char *newName2;
					char newFullName2[PAYLOAD_SIZE];
					// Copy the original payload to a temporary buffer
					strncpy(oldPayload_move, msg.payload, PAYLOAD_SIZE);

					// Split the payload using '|' as the delimiter
					oldPath2 = strtok(oldPayload_move, "|"); // Extract the first part (curDir/nameFolder)
					newName2 = strtok(NULL, "|");
					char moved_name[PAYLOAD_SIZE];
					extractLastSegment(oldPath2, moved_name, sizeof(moved_name));
					memset(msg.payload, 0, PAYLOAD_SIZE);
					printf("%s %s ", msg.payload, moved_name, newFullName2);
					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload_move);
					sprintf(newFullName2, "%s/%s/%s", SERVER_FOLDER, newName2, moved_name);
					if (renameFolder(msg.payload, newFullName2) != -1)
					{
						craftMessage(msg, MOVE_FOLDER_SUCCESS, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					else
					{
						craftMessage(msg, MOVE_FOLDER_FAILED, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;
				case DELETE_FOLDER:
					char oldPayload2[PAYLOAD_SIZE];

					// Copy the old payload to a temporary variable
					strncpy(oldPayload2, msg.payload, PAYLOAD_SIZE);

					// Set the original payload to an empty string
					memset(msg.payload, 0, PAYLOAD_SIZE);

					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload2);
					if (removeFolder(msg.payload) != -1)
					{
						craftMessage(msg, DELETE_FOLDER_SUCCESS, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					else
					{
						craftMessage(msg, FOLDER_NOT_FOUND, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;
				case DELETE_FILE:
					if (deleteFile(msg.payload) != -1)
					{
						craftMessage(msg, DELETE_FILE_SUCCESS, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					else
					{
						craftMessage(msg, FILE_NOT_FOUND, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;
				case RENAME_FILE:
					// Copy the original payload to a temporary buffer
					strncpy(oldPayload_rename, msg.payload, PAYLOAD_SIZE);

					// Split the payload using '|' as the delimiter
					oldPath = strtok(oldPayload_rename, "|"); // Extract the first part (curDir/nameFolder)
					newName = strtok(NULL, "|");			  // Extract the second part (rename)
					printf("%s %s ", oldPath, newName);
					// Set the original payload to an empty string
					memset(msg.payload, 0, PAYLOAD_SIZE);
					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload_rename);
					sprintf(newFullName, "%s/%s", SERVER_FOLDER, newName);
					if (checkFile(msg.payload) == 0)
					{
						if (renameFile(msg.payload, newFullName) != -1)
						{
							craftMessage(msg, RENAME_FILE_SUCCESS, 0, 0, NULL);
							memcpy(pID->buffer, &msg, MESSAGE_SIZE);
							sendMessage(pHD, pID, transferredBytes, ALL);
						}
						else
						{
							craftMessage(msg, FILE_ALREADY_EXIST, 0, 0, NULL);
							memcpy(pID->buffer, &msg, MESSAGE_SIZE);
							sendMessage(pHD, pID, transferredBytes, ALL);
						}
					}
					else
					{
						craftMessage(msg, FILE_NOT_FOUND, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}

					break;
				case COPY_FILE:
					char oldPayload_copy[PAYLOAD_SIZE];
					char newFullName5[PAYLOAD_SIZE];
					// Copy the original payload to a temporary buffer
					strncpy(oldPayload_copy, msg.payload, PAYLOAD_SIZE);

					// Split the payload using '|' as the delimiter
					oldPath2 = strtok(oldPayload_copy, "|"); // Extract the first part (curDir/nameFolder)
					newName2 = strtok(NULL, "|");
					// Set the original payload to an empty string
					memset(msg.payload, 0, PAYLOAD_SIZE);
					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload_copy);
					sprintf(newFullName5, "%s/%s", SERVER_FOLDER, newName2);
					if (checkFile(msg.payload) == 0)
					{
						if (copyFile(msg.payload, newFullName5) != -1)
						{
							craftMessage(msg, COPY_FILE_SUCCESS, 0, 0, NULL);
							memcpy(pID->buffer, &msg, MESSAGE_SIZE);
							sendMessage(pHD, pID, transferredBytes, ALL);
						}
					}
					else
					{
						craftMessage(msg, FILE_NOT_FOUND, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}

					break;
				case MOVE_FILE:
					char oldPayload_move3[PAYLOAD_SIZE]; // Temporary buffer for received payload
					char *oldPath3;						 // Pointer to the first part (curDir/nameFolder)
					char *newName3;
					char newFullName3[PAYLOAD_SIZE];
					// Copy the original payload to a temporary buffer
					strncpy(oldPayload_move3, msg.payload, PAYLOAD_SIZE);

					// Split the payload using '|' as the delimiter
					oldPath3 = strtok(oldPayload_move3, "|"); // Extract the first part (curDir/nameFolder)
					newName3 = strtok(NULL, "|");
					char moved_name3[PAYLOAD_SIZE];
					extractLastSegment(oldPath3, moved_name3, sizeof(moved_name3));
					memset(msg.payload, 0, PAYLOAD_SIZE);
					printf("%s %s ", msg.payload, moved_name3, newFullName3);
					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload_move3);
					sprintf(newFullName3, "%s/%s/%s", SERVER_FOLDER, newName3, moved_name3);
					if (renameFolder(msg.payload, newFullName3) != -1)
					{
						craftMessage(msg, MOVE_FILE_SUCCESS, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					else
					{
						craftMessage(msg, MOVE_FILE_FAILED, 0, 0, NULL);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
					}
					break;
				case CHANGE_DIRECTORY:
					char oldPayload3[PAYLOAD_SIZE];

					// Copy the old payload to a temporary variable
					strncpy(oldPayload3, msg.payload, PAYLOAD_SIZE);

					// Set the original payload to an empty string
					memset(msg.payload, 0, PAYLOAD_SIZE);

					// Construct the new payload
					sprintf(msg.payload, "%s/%s", SERVER_FOLDER, oldPayload3);
					changeDirectory(msg);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case SHOW_REQUEST:
					requestToString(listGroup, msg.payload);
					craftMessage(msg, SHOW_REQUEST, 0, strlen(msg.payload) + 1, msg.payload);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case ACCEPT_REQUEST:
					acceptRequest(msg, listGroup, listAccount);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				case SHOW_LOG:
					showLog(msg);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					break;
				}
			}
		}
		else
		{
			if (pID->sentBytes < MESSAGE_SIZE)
			{
				sendMessage(pHD, pID, transferredBytes, MISSING);
			}
			else
			{
				msg = getMessage(pID->buffer);
				EnterCriticalSection(&cs);
				if (msg.opcode == ACCEPT_DOWNLOAD)
				{
					int i;
					for (i = 0; i < nClients; i++)
					{
						if (perHandleData[i]->socket == pHD->socket)
						{
							break;
						}
					}
					int idx = perHandleData[i]->size - perHandleData[i]->nLeft;
					int offset = (int)(idx / PAYLOAD_SIZE);
					char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);

					fseek(perHandleData[i]->f, idx, SEEK_SET);
					int size = (perHandleData[i]->nLeft > PAYLOAD_SIZE) ? PAYLOAD_SIZE : perHandleData[i]->nLeft;
					fread(buff, size, 1, perHandleData[i]->f);
					craftMessage(msg, DATA_DOWN, offset, size, buff);
					memcpy(pID->buffer, &msg, MESSAGE_SIZE);
					sendMessage(pHD, pID, transferredBytes, ALL);
					perHandleData[i]->nLeft -= size;
				}
				else if (msg.opcode == DATA_DOWN)
				{
					int i;
					for (i = 0; i < nClients; i++)
					{
						if (perHandleData[i]->socket == pHD->socket)
						{
							break;
						}
					}
					if (msg.length != 0)
					{
						int idx = perHandleData[i]->size - perHandleData[i]->nLeft;
						int offset = (int)(idx / PAYLOAD_SIZE);
						char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);

						fseek(perHandleData[i]->f, idx, SEEK_SET);
						int size = (perHandleData[i]->nLeft > PAYLOAD_SIZE) ? PAYLOAD_SIZE : perHandleData[i]->nLeft;
						fread(buff, size, 1, perHandleData[i]->f);
						craftMessage(msg, DATA_DOWN, offset, size, buff);
						memcpy(pID->buffer, &msg, MESSAGE_SIZE);
						sendMessage(pHD, pID, transferredBytes, ALL);
						perHandleData[i]->nLeft -= size;
						if (perHandleData[i]->nLeft == 0)
						{
							fclose(perHandleData[i]->f);
						}
					}
					else
					{
						recvMessage(pHD, pID, transferredBytes, ALL);
					}
				}
				else
				{
					recvMessage(pHD, pID, transferredBytes, ALL);
				}
				LeaveCriticalSection(&cs);
			}
		}
	}
}
