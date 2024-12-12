#pragma once
#include "Resources.h"
#include "DataIO.h"
#include <iomanip>
void handleFileResponse(int msgOpcode)
{
	switch (msgOpcode)
	{
	case ACCEPT_UPLOAD:
		cout << " => SYSTEM : File is allow to upload" << endl;
		break;
	case UPLOAD_SUCCESS:
		cout << " => SYSTEM : Upload file successfully" << endl;
		break;
	case ACCEPT_DOWNLOAD:
		cout << " => SYSTEM : File is allow to download" << endl;
		break;
	case DOWNLOAD_SUCCESS:
		cout << " => SYSTEM : Download file successfully" << endl;
		break;
	case CREATE_FOLDER_SUCCESS:
		cout << " => SYSTEM : Create folder successfully" << endl;
		break;
	case DELETE_FOLDER_SUCCESS:
		cout << " => SYSTEM : Delete folder successfully" << endl;
		break;
	case FOLDER_NOT_FOUND:
		cout << " => SYSTEM : Folder is not found" << endl;
		break;
	case FOLDER_ALREADY_EXIST:
		cout << " => SYSTEM : Folder is already exist" << endl;
		break;
	case DELETE_FILE_SUCCESS:
		cout << " => SYSTEM : Delete file successfully" << endl;
		break;
	case FILE_NOT_FOUND:
		cout << " => SYSTEM : File is not found" << endl;
		break;
	}
}

int upload(SOCKET sock, char *nameGroup, char *nameFile)
{
	int ret;
	Message msg;
	char *message = (char *)malloc(sizeof(char) * BUFF_SIZE);
	char *path = (char *)malloc(sizeof(char) * BUFF_SIZE);
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(path, "%s/%s", CLIENT_FOLDER, nameFile);
	// cout << path <<endl;
	// File exist
	if (checkFile(path) == 0)
	{
		cout << nameGroup << endl;
		sprintf(message, "%s/%s", nameGroup, nameFile);
		ret = sendMessage(sock, message, UPLOAD);
		if (ret != SOCKET_ERROR)
		{
			recvMessage(sock, msg);
			handleFileResponse(msg.opcode);
			// File is allow to upload
			if (msg.opcode == ACCEPT_UPLOAD)
			{
				ret = sendFile(sock, nameFile);
				if (ret != SOCKET_ERROR)
				{
					recvMessage(sock, msg);
					handleFileResponse(msg.opcode);
					// cout << "Opcode " << msg.opcode << endl;
					return msg.opcode;
				}
				else
				{
					cout << "Something wrong" << endl;
					return -1;
				}
			}
			else
			{
				cout << " => Server : Upload file is not accepted" << endl;
				return -1;
			}
		}
		else
		{
			cout << "Something wrong" << endl;
			return -1;
		}
	}
	else
	{
		cout << "File is not exist" << endl;
		return FILE_NOT_FOUND;
	}
	free(message);
}

int download(SOCKET sock, char *nameGroup, char *nameFile)
{
	Message msg;
	char *message = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(message, "%s/%s", nameGroup, nameFile);
	sendMessage(sock, message, DOWNLOAD);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	if (msg.opcode == ACCEPT_DOWNLOAD)
	{
		recvFile(sock, nameFile);
		cout << " => SYSTEM : Download file is completed" << endl;
		free(message);
		return DOWNLOAD_SUCCESS;
	}
	free(message);
	return FILE_NOT_FOUND;
}

void showListFile(SOCKET sock, char *curDir, char *result = NULL)
{
	Message msg;
	char *file = (char *)malloc(sizeof(char) * 256);
	char *message = (char *)malloc(sizeof(char) * BUFF_SIZE);

	sendMessage(sock, curDir, LIST_FILE);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	sprintf(file, "");
	file = strtok_s(msg.payload, DELIMETER, &message);
	while (file != NULL)
	{
		if (result != NULL)
		{
			strcat(result, "\n");
			strcat(result, file);
			strcat(result, "\n");
		}
		// cout << file << "\t";
		file = strtok_s(message, DELIMETER, &message);
	}
	// cout << endl;
	free(file);
}
int renameFile(SOCKET sock, char *curDir, char *nameFile, char *rename)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s|%s/%s", curDir, nameFile, curDir, rename);
	// sprintf(buff, "%s/%s", curDir, nameFolder);
	sendMessage(sock, buff, RENAME_FILE);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}
void showListMember(SOCKET sock, char *nameGroup, char *result = NULL)
{
	Message msg;
	char *member = (char *)malloc(sizeof(char) * 256);
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);

	sendMessage(sock, nameGroup, LIST_MEMBER);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	sprintf(member, "");
	member = strtok_s(msg.payload, DELIMETER, &buff);

	while (member != NULL)
	{
		if (result != NULL)
		{
			strcat(result, member);
			strcat(result, " ");
		}
		// cout << member << "\t";
		member = strtok_s(buff, DELIMETER, &buff);
	}
	// cout << endl;
	free(member);
}
int removeMember(SOCKET sock, char *groupName, char *user)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s %s", groupName, user);
	// cout<< buff <<endl;
	sendMessage(sock, buff, DELETE_MEMBER);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}
int createFolder(SOCKET sock, char *curDir, char *nameFolder)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s", curDir, nameFolder);
	// sprintf(buff, "%s/%s", curDir, nameFolder);
	sendMessage(sock, buff, CREATE_FOLDER);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}

int renameFolder(SOCKET sock, char *curDir, char *nameFolder, char *rename)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s|%s/%s", curDir, nameFolder, curDir, rename);
	// sprintf(buff, "%s/%s", curDir, nameFolder);
	sendMessage(sock, buff, RENAME_FOLDER);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}
int moveFolder(SOCKET sock, char *nameGroup, char *nameFolder, char *name)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s|%s/%s", nameGroup, nameFolder, nameGroup, name);
	// sprintf(buff, "%s/%s", curDir, nameFolder);
	sendMessage(sock, buff, MOVE_FOLDER);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}

int moveFile(SOCKET sock, char *nameGroup, char *file, char *destination)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s|%s/%s", nameGroup, file, nameGroup, destination);
	// sprintf(buff, "%s/%s", curDir, nameFolder);
	sendMessage(sock, buff, MOVE_FILE);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}
int deleteFolder(SOCKET sock, char *curDir, char *nameFolder)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s", curDir, nameFolder);
	sendMessage(sock, buff, DELETE_FOLDER);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}

int deleteFile(SOCKET sock, char *curDir, char *nameFile)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s/%s", SERVER_FOLDER, curDir, nameFile);
	// cout<< buff <<endl;
	sendMessage(sock, buff, DELETE_FILE);
	recvMessage(sock, msg);
	handleFileResponse(msg.opcode);
	free(buff);
	return msg.opcode;
}

int changeDirectory(SOCKET sock, char *curDir, char *nameFolder)
{
	Message msg;
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sprintf(buff, "%s/%s", curDir, nameFolder);
	// cout << "Send directory " << buff <<endl;
	sendMessage(sock, buff, CHANGE_DIRECTORY);
	recvMessage(sock, msg);
	cout << msg.payload << endl;
	handleFileResponse(msg.opcode);
	if (msg.opcode != FOLDER_NOT_FOUND)
	{
		sprintf(curDir, msg.payload);
		cout << "Change folder " << curDir << " successfully" << endl;
	}
	free(buff);
	return msg.opcode;
}

void showRequest(SOCKET sock, char *nameGroup, char *result = NULL)
{
	Message msg;
	char *request = (char *)malloc(sizeof(char) * 255);
	char *buff = (char *)malloc(sizeof(char) * BUFF_SIZE);
	char *message = (char *)malloc(sizeof(char) * BUFF_SIZE);
	sendMessage(sock, nameGroup, SHOW_REQUEST);
	recvMessage(sock, msg);
	sprintf(request, "");
	request = strtok_s(msg.payload, DELIMETER, &buff);
	if (result != NULL)
	{
		strcpy(result, "");
	}
	while (request != NULL)
	{
		if (isalnum(request[0]))
		{
			strcat(result, request);
			strcat(result, " ");
		}
		cout << request << "\t";
		request = strtok_s(buff, DELIMETER, &buff);
		cout << endl;
	}
	// cout << "Enter name to accept ? ";
	// cin.getline(buff, 255);
	// if (strlen(buff) != 0) {
	// 	sprintf(message, "%s %s", nameGroup, buff);
	// 	sendMessage(sock, message, ACCEPT_REQUEST);
	// 	recvMessage(sock, msg);
	// }
}

int acceptRequet(SOCKET sock, char *nameGroup, char *userID)
{
	Message msg;
	char *request = (char *)malloc(sizeof(char) * 255);
	char *message = (char *)malloc(sizeof(char) * BUFF_SIZE);
	if (strlen(userID) != 0)
	{
		sprintf(message, "%s %s", nameGroup, userID);
		sendMessage(sock, message, ACCEPT_REQUEST);
		recvMessage(sock, msg);
		return msg.opcode;
	}
	return -1;
}

void showLog(SOCKET sock, char *nameGroup)
{
	Message msg;
	string date, time, fileName, uploadedBy;
	sendMessage(sock, nameGroup, SHOW_LOG);
	recvMessage(sock, msg);
	cout << left
		 << setw(16) << "Date"
		 << setw(16) << "Time"
		 << setw(40) << "File name"
		 << "Uploaded by" << endl;
	cout << msg.payload;
}
