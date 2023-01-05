/*******************************************************************************
** 版权所有@2021, gao noob. 保留所有版权.
** 
** 文件名称 : 
** 文件描述 : 
** 作者       :gao noob
** 创建日期 : 2021/5/20
** 其它       :
** 修改历史 : <作者>    <时间>             <版本>     <描述>
**             gao noob
**
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define READ_MAX_SIZE_SET                   1024
#define HTTP_SEND_SIZE                      2048
#define HTTP_RECV_SIZE                      1024*10
#define HTTP_PORT                           "80"
#define HTTP_CONTENTLENGTH_SIZE             100
#define HTTP_CONTENTBODYHEAR_SIZE           200
#define HTTP_CONTENTBODYTAIL_SIZE           50

#define HTTP_UPLOAD_HEADER   "POST /logs/ HTTP/1.1\r\n" \
    "Host: 10.43.61.19:8080\r\n" \
    "User-Agent: chun\r\n" \
    "Accept: */*\r\n" \
    "Content-Length: %ld\r\n" \
    "Expect: 100-continue\r\n" \
    "Content-Type: multipart/form-data; boundary=------------------------a35a7e9e6905df0d\r\n\r\n" 

int contentBodyHearLen = 0;
int contentBodyTailLen = 0;
int contentHearLen = 0;
int httpSocket = -1;
char contentLengthBuf[HTTP_CONTENTLENGTH_SIZE] = {0};
char contentBodyHearBuff[HTTP_CONTENTBODYHEAR_SIZE] = {0};
char contentBodyTailBuff[HTTP_CONTENTBODYTAIL_SIZE] = {0};

void http_send_body(void)
{
    memset(contentBodyHearBuff, 0, HTTP_CONTENTBODYHEAR_SIZE);
    memset(contentBodyTailBuff, 0, HTTP_CONTENTBODYTAIL_SIZE);

    /* body - head*/
    strcat(contentBodyHearBuff, "------------------------a35a7e9e6905df0d\r\n");
    sprintf(contentBodyHearBuff + strlen(contentBodyHearBuff), "Content-Disposition: form-data; name=\"filename\"; filename=\"HUMGR0_log.txt\"\r\n");
    strcat(contentBodyHearBuff, "Content-Type: text/plain\r\n\r\n");
    contentBodyHearLen = strlen(contentBodyHearBuff);

    /* body-tail */
    strcpy(contentBodyTailBuff, "\r\n");
    strcat(contentBodyTailBuff, "------------------------a35a7e9e6905df0d--\r\n");
    contentBodyTailLen = strlen(contentBodyTailBuff);
}

int http_socket_connect(const char * host, const char * port)
{
    struct addrinfo hints, *clientSaddr = NULL, *cur = NULL;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    httpSocket= (int) socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (httpSocket < 0) {
        printf( "http_socket_connect socket creat  error httpSocket = %d \n",httpSocket);
        httpSocket = -1;
        return -1;;
    }
    
    if (0 != getaddrinfo(host, port, &hints, &clientSaddr))
    {
        printf( "getaddrinfo error");
        return -1;
    }
    
    for( cur = clientSaddr; cur != NULL; cur = cur->ai_next )
    {
        if(connect(httpSocket, cur->ai_addr, cur->ai_addrlen ) == 0 )
        {
            //printf( "http_socket_connect connect sucess, ip: %s \n", cur->ai_addr->sa_data);
            freeaddrinfo(clientSaddr);
            return 0;
        }
    }

    printf( "http_socket_connect connect fail \n");
    freeaddrinfo(clientSaddr);
    return -1;
}

int http_socket_send(char *buffer, int len)
{
    printf("buffer is %s\n", buffer);
    return send(httpSocket, buffer, len, 0);
}

#define SOCKET_SEND_RETRT 2

int http_socket_send_data(char * data, int len)
{
    int result = 0;
    int retryCnt = 0;

    if (-1 == httpSocket)
    {
        if (-1 == http_socket_connect("10.43.61.19", "8080"))
        {
            printf( " this two creat socket fail,to not creat \n");
            httpSocket = -1;
        }
    }

    result = http_socket_send(data, len);
    if (result < 0)
    {
        printf( " send error, ret: %d \n", result);
        close(httpSocket);
        httpSocket = -1;
    }

    return result;
}

int linksci_upload_file(const char *path, long data_len)
{
    int ret = 0;
    int file_fd = -1;
    long cur_file_size = 0;
    char recv_buffer[1024] = {0};

    file_fd = open(path, O_RDONLY);
    if(0 >= file_fd)
    {
        printf( " read %s failed! \n", path);
        return -1;
    }

     //printf( " read %s ok! \n", path);

    /* 组装头 */
    char *send_buffer = (char*)malloc(HTTP_SEND_SIZE+1);
    if (NULL == send_buffer)
    {
        printf( " send_buffer malloc fail \n");
        return -1;
    }
    memset(send_buffer, 0, HTTP_SEND_SIZE+1);
    memset(contentLengthBuf, 0, HTTP_CONTENTLENGTH_SIZE);

    /* hear */
    http_send_body();
    sprintf(send_buffer, HTTP_UPLOAD_HEADER"%s", \
        data_len + contentBodyHearLen + contentBodyTailLen, contentBodyHearBuff);
    ret = strlen(send_buffer);

    if(0 > http_socket_send_data(send_buffer, ret))
    {
        free(send_buffer);
        send_buffer = NULL;
        return -1;
    }

    //printf( " start send_buffer is \n %s \n", send_buffer);
#if 1
    while(1)
    {
        if(cur_file_size == data_len)
        {
            printf( " already path:%s cur_file_size: %ld \n", path, cur_file_size);
            ret = 0;
            break;
        }

        memset(send_buffer, 0, HTTP_SEND_SIZE+1);
        if((ret = read(file_fd, send_buffer, HTTP_SEND_SIZE)) != HTTP_SEND_SIZE)
        {
            if(ret + cur_file_size == data_len)
            {
                ret = http_socket_send_data(send_buffer, ret);
                //printf( "ret: %d \n", ret);
                if(0 <= ret)
                {
                    ret = 0;
                    break;
                }
            }
            ret = -1;
            break;
        }
        else
        {
            if(0 > http_socket_send_data(send_buffer, ret))
            {
                ret = -1;
                break;
            }
            cur_file_size += ret;
            memset(send_buffer, 0, HTTP_SEND_SIZE);
        }
    }

    if(0 < file_fd)
    {
        close(file_fd);
        file_fd = -1;
    }

    /* 发送完成 */
    memset(send_buffer, 0, HTTP_SEND_SIZE);
    strcpy(send_buffer, contentBodyTailBuff);
    http_socket_send_data(send_buffer, strlen(send_buffer));
    free(send_buffer);
    send_buffer = NULL;

    if(0 == ret)
    {
        ret = recv(httpSocket, recv_buffer, sizeof(recv_buffer), 0);
        if(0 >= ret)
        {
            printf( "http recv error, ret: %d \n", ret);
            close(httpSocket);
            httpSocket = -1;
            ret = -1;
        }
        else
        {
            printf( "ret %d \n", ret);
            printf( "recv \n http_rsp: %s \n", recv_buffer);
            /* 判断结果 */
            if(NULL != strstr(recv_buffer, "success"))
            {
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
    }
#endif
    return ret;
}


int main(int argc, char *argv[])
{
    /* 上传 */
    //linksci_upload_file("filename", file_size);
    return 0;
}