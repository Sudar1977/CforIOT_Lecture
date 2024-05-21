/*!
 * Simple chat program (client side).cpp - http://github.com/hassanyf
 * Version - 2.0.1
 *
 * Copyright (c) 2016 Hassan M. Yousuf
 */

//#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

//using namespace std;

int main()
{
    /* ---------- INITIALIZING VARIABLES ---------- */
    /* ---------- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ КЛИЕНТА---------- */
    
    /*  
       1. client is a file descriptor to store the values 
       returned by the socket system call and the accept 
       system call.
       1. client - файл-дескриптор, хранящий значение сокета,
       которые вернула система при подключении к серверу.

       2. portNum is for storing port number on which
       the accepts connections
       2. portNum нужен для хранения номера порта, на котором происходит соединение.

       3. isExit is bool variable which will be used to 
       end the loop
       3. isExit - булевая переменная, признак завершения программы.

       4. The client reads characters from the socket 
       connection into a dynamic variable (buffer).
       4. Клиент считывает сообщение из сокета соединения в буффер обмена для приема/отправки данных к/от сервера

       5. A sockaddr_in is a structure containing an internet 
       address. This structure is already defined in netinet/in.h, so
       we don't need to declare it again.
       5. sockaddr_in - структура, содержащая интернет адрес, с которым будет установлено соединение.
       Эта структура уже определена в netinet/in.h, поэтому нет необходимости
       заново ее задавать.

        DEFINITION:
        ОПРЕДЕЛЕНИЕ:
        
        struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };

        6. serv_addr will contain the address of the server
        6. serv_addr будет содержать адрес сервера
        * 
    */

    int client;
    int portNum = 1500; // Номер порта один для сервера и клиента!
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    /*
        The socket() function creates a new socket.
        It takes 3 arguments,
        Socket() функция создает новый сокет.
        На вход получает 3 аргумента,

            a. AF_INET: address domain of the socket.
            a. AF_INET: доменный адрес сокета.

            b. SOCK_STREAM: Type of socket. a stream socket in 
            which characters are read in a continuous stream (TCP)
            b. SOCK_STREAM: тип сокета. Потоковый сокет, в котором сообщение 
            читается последовательным потоком(TCP).

            c. Third is a protocol argument: should always be 0. The 
            OS will choose the most appropiate protocol.
            c. Третий это аргумент протокола: всегда должен быть 0. 
            Операционная система выберет самый подходящий протокол.

            This will return a small integer and is used for all 
            references to this socket. If the socket call fails, 
            it returns -1.
            Функция возвращает файл-дескриптор целое число - соединение, которое используется для всех ссылок 
            на этот сокет. Если произошла ошибка соединения, то возвращается -1.
    */

    client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/
    /* ---------- УСТАНОВКА СОКЕТ ПОДКЛЮЧЕНИЯ ----------*/
    /* --------------- socket() функция ------------------*/

    if (client < 0) 
    {
        printf("Error establishing socket...\n");
        exit(1);
    }

    printf("=> Socket client has been created...\n");
    
    /* 
        The variable serv_addr is a structure of sockaddr_in. 
        sin_family contains a code for the address family. 
        It should always be set to AF_INET.
        Переменная serv_addr - это структура sockaddr_in. 
        sin_family содержит код для адресной семьи. 
        Всегда должна быть установлена AF_INET.
         
        htons() converts the port number from host byte order 
        to a port number in network byte order.
        htons() переводит номер порта из порядка байтов хоста 
        в номер порта в порядке байтов сети.
    */

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    // this function returns returns 1 if the IP is valid
	// эта функция возвращает 1, если IP-адрес действительный
    
    // and 0 if invalid
    // и 0, если недействительный
    
    // inet_pton converts IP to packets
    // inet_pton переводит IP-адрес в пакеты
    
    // inet_ntoa converts back packets to IP
    // inet_ntoa переводит обратно из пакета в IP-адрес
    
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        printf("=> Connection to the server %s with port number: %d\n",inet_ntoa(server_addr.sin_addr),portNum);


    /* ---------- CONNECTING THE SOCKET ---------- */
    /* ---------------- connect() ---------------- */
    /* ---------- СОЕДИНЕНИЕ СОКЕТА ---------- */
    /* ---------------- connect() ---------------- */

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        printf("=> Connection to the server port number: %d \n",portNum);

    /* 
        The connect function is called by the client to 
        establish a connection to the server. It takes 
        three arguments, the socket file descriptor, the 
        address of the host to which it wants to connect 
        (including the port number), and the size of this 
        address. 
        Функция connect вызывается клиентом для установки соединения с сервером. 
        Ей нужны 3 аргумента: файловый дескриптор сокета, адрес хоста, к которому
        клиент хочет подключится (включая номер порта), и  размер адреса.

        This function returns 0 on success and -1 
        if it fails.
        Функция возвращает 0 при успехе и 1 при неудаче.

        Note that the client needs to know the port number of
        the server but not its own port number.
        Примечание: клиенту нужно знать номер порта сервера, 
        а не свой номер порта.
    */

    printf("=> Awaiting confirmation from the server...\n");
    recv(client, buffer, bufsize, 0);
    printf("=> Connection confirmed, you are good to go...\n");

    printf("\n\n=> Enter # to end the connection\n");

    // Once it reaches here, the client can send a message first.
    // При достижении соединения, клиент может послать сообщение первым.
    
    do {
		printf("Client: ");
        do {
            scanf("%s",buffer);
            send(client, buffer, bufsize, 0);
            if (*buffer == '#') 
            {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);

        printf("Server: ");
        do {
            recv(client, buffer, bufsize, 0);
            printf("%s",buffer);
            if (*buffer == '#') 
            {
                *buffer = '*';
                isExit = true;
            }

        } while (*buffer != 42);
        printf("\n");

    } while (!isExit);

    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */
    /* ---------------- ЗАВЕРШЕНИЕ СОЕДИНЕНИЯ ------------- */
    /* ----------------- close() --------------- */

    /* 
        Once the server presses # to end the connection,
        the loop will break and it will close the server 
        socket connection and the client connection.
        Как только сервер нажмет # для завершения соединения,
        цикл завершится и закроет серверное сокет соединение
        и клиентсткое соединение.
    */

    printf("\n=> Connection terminated.\n\nGoodbye...\n");

    close(client);
    return 0;
}
