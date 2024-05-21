/*!
 * Simple chat program (server side).cpp - http://github.com/hassanyf
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

//using namespace std;

///bool isExitBuffer(char* buffer) {
/// return *buffer != '*' && *buffer !='#'
///}



int main()
{
    /* ---------- INITIALIZING VARIABLES ---------- */
    /* ---------- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ СЕРВЕРА---------- */

    /*  
       1. client/server are two file descriptors
       These two variables store the values returned 
       by the socket system call and the accept system call.
       1. client/server - два файл-дескриптора.
       Эти две перемнные хранят значение сокетов,
       которые вернула система при подключении.

       2. portNum is for storing port number on which
       the accepts connections
       2. portNum нужен для хранения номера порта, на котором происходит соединение.
       
       3. isExit is bool variable which will be used to 
       end the loop
       3. isExit - булевая переменная, признак завершения программы.
       
       4. The server reads characters from the socket 
       connection into a dynamic variable (buffer).
       4. Сервер считывает сообщение из сокет соединения в буффер обмена для приема/отправки данных к/от сервера.

    */
    int client;//файл-дескриптор сервера 
    int server;//файл-дескриптор клианта 
    int portNum = 1500;//номера порта, на котором происходит соединение (0 до 65535)
    bool isExit = false;//булевая переменная, признак завершения программы.
    int bufsize = 1024;//размер буффера
    char buffer[bufsize]; //буффер обмена для приема/отправки данных к/от сервера
/*    
       5. A sockaddr_in is a structure containing an internet 
       address. This structure is already defined in netinet/in.h, so
       we don't need to declare it again.
       5. A sockaddr_in - структура, содержащая интернет адрес, с которым будет установлено соединение.
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
        
        7. socklen_t  is an intr type of width of at least 32 bits
        7. socklen_t - длиной по крайней мере 32 бита
*/ 
    struct sockaddr_in server_addr;
    socklen_t size;

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/
    /* ---------- УСТАНОВКА СОКЕТ СОЕДИНЕНИЯ ----------*/
    /* --------------- socket() функция ------------------*/
 
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

    if (client < 0) 
    {
        printf("Error establishing socket...\n");
        exit(1);
    }
	
	printf("=> Socket server has been created...\n");

    /* 
        The variable serv_addr is a structure of sockaddr_in. 
        sin_family contains a code for the address family. 
        It should always be set to AF_INET.
        * 
        Переменная serv_addr - структура sockaddr_in. 
        sin_family содержит код для адресной семьи. 
        Всегда должна быть установлена AF_INET.

        INADDR_ANY contains the IP address of the host. For 
        server code, this will always be the IP address of 
        the machine on which the server is running.
        * 
        INADDR_ANY содержит IP-адрес хоста. Для сервера - это 
		IP-адрес компьютера, на котором работает сервер.

        htons() converts the port number from host byte order 
        to a port number in network byte order.
        * 
        htons() функция переводит номер порта из порядка байтов хоста 
        в номер порта в порядке байтов сети.
            
    */

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    /* ---------- BINDING THE SOCKET ---------- */
    /* ---------------- bind() ---------------- */
    /* ---------- ПРИВЯЗКА СОКЕТА ---------- */
    /* ---------------- bind() ---------------- */

    /* 
        The bind() system call binds a socket to an address, 
        in this case the address of the current host and port number 
        on which the server will run. It takes three arguments, 
        the socket file descriptor. The second argument is a pointer 
        to a structure of type sockaddr, this must be cast to
        the correct type.
        * 
        bind() функция привязывает сокет к адресу, то есть в нашем случае 
        к адресу сервера и номеру порта, на котором сервер будет работать.
        Для этого нужны три аргумента: файл дескриптор сокета, указатель на структуру
        типа sockaddr (должен указывать на правильный тип)
        * 
    */

    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        printf("=> Error binding connection, the socket has already been established...\n");
        return -1;
    }



    size = sizeof(server_addr);
    printf("=> Looking for clients...\n");

    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */
    /* ------------- ПРОСЛУШИВАНИЕ СОКЕТА ------------- */
    /* ---------------- listen() ---------------- */

    /* 
        The listen system call allows the process to listen 
        on the socket for connections. 
        Функция listen позволяет прослушивать сокета  для подключения. 
        
        The program will be stay idle here if there are no 
        incomming connections.
        Программа будет в состоянии простоя, если подключений не обнаружится.

        The first argument is the socket file descriptor, 
        and the second is the size for the number of clients 
        i.e the number of connections that the server can 
        handle while the process is handling a particular 
        connection. The maximum size permitted by most 
        systems is 5.
        Первый аргумент - это файл дескриптор сокета, второй - количество клиентов,
        то есть количество подключений, которое сервер может обработать, пока процесс
        обрабатывает определенное подключение. Максимальное число клиентов во многих системах равняется 5.
        
    */
    
    listen(client, 1);



    /* ------------- ACCEPTING CLIENTS  ------------- */
    /* ----------------- listen() ------------------- */
    /* ------------- ПОДКЛЮЧЕНИЕ КЛИЕНТОВ  ------------- */
    /* ----------------- accept() ------------------- */

    /* 
        The accept() system call causes the process to block 
        until a client connects to the server. Thus, it wakes 
        up the process when a connection from a client has been 
        successfully established. It returns a new file descriptor, 
        and all communication on this connection should be done 
        using the new file descriptor. The second argument is a 
        reference pointer to the address of the client on the other 
        end of the connection, and the third argument is the size 
        of this structure.
        Система accept() вызывает процесс блокировки пока клиент подключается к серверу.
        К тому же, она пробуждает процесс, когда подключение с клиентом было успешно установлено.
        Она возвращает новый файл дескриптор, и вся связь по этому подключению должна
        осуществляться, используя новый файл дескриптор. Второй аргумент - указатель на
        адрес клиента, третий - размер структуры.
    */

    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // first check if it is valid or not
    // первая проверка действительности подключения
    if (server < 0) 
        printf("=> Error on accepting...");

//Основной цикл


    while (server > 0) 
    {
        printf("=> Connected with the client %d, you are good to go...\n",clientCount);
        printf("\n=> Enter # to end the connection\n");
//.....        
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);


        /* 
            Note that we would only get to this point after a 
            client has successfully connected to our server. 
            This reads from the socket. Note that the read() 
            will block until there is something for it to read 
            in the socket, i.e. after the client has executed a 
            the send().
            Примечание: мы перейдем в этот момент только после того, как
            клиент успешно подключится к нашему серверу.
            Произойдет чтение сокета. Заметим, что read() будет
            блокировать до момента наличия чего-то для чтения
            
            It will read either the total number of characters 
            in the socket or 1024
            Чтение будет происходить до достижения 1024 символов в пакете
        */

        printf("Client: ");
        do {
            recv(server, buffer, bufsize, 0);
            printf("%s",buffer);
            if (*buffer == '#') 
            {
                *buffer = '*';///???
                isExit = true;
            }
        } while (*buffer != '*');
        ///~ } while (*buffer != '*' && *buffer !='#' );

        do {
            printf("\nServer: ");
            do {
                scanf("%s",buffer);
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') 
                {
                    send(server, buffer, bufsize, 0);///?????
                    *buffer = '*';///?????
                    isExit = true;
                }
            } while (*buffer != '*');

			printf("Client: ");
            do {
                recv(server, buffer, bufsize, 0);
                printf("%s",buffer);
                if (*buffer == '#') 
                {
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

        /* 
            Once a connection has been established, both ends 
            can both read and write to the connection. Naturally, 
            everything written by the client will be read by the 
            server, and everything written by the server will be 
            read by the client.
            Как только соединение будет установлено, оба конца связи могут
            писать и читать соединение. Все, написанное клиентом, будет прочитано сервером,
            а все, написанное сервером, будет прочитано клиентом.
        */

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

        // inet_ntoa converts packet data to IP, which was taken from client
        // inet_ntoa переводит данные пакета в IP-адрес, который был взят у клиента  
        printf("\n\n=> Connection terminated with IP %s\n",inet_ntoa(server_addr.sin_addr));   
        close(server);
        printf("\nGoodbye...");
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;
}
