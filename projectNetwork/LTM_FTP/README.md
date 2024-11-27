cd /c/Users/pc/Documents/Network\ project/projectNetwork/LTM_FTP/Client
g++ `pkg-config --cflags gtk4` -o client Client.cpp `pkg-config --libs gtk4` -lws2_32