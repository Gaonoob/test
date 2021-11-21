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
#include "time.h"

//https://xxxxx.cn
//http://xxxx.cn/

/* 打印宏 */
#define MSG_DEBUG   0x01
#define MSG_INFO    0x02
#define MSG_ERROR   0x04

static int print_level = /*MSG_DEBUG |*/ MSG_INFO | MSG_ERROR;

#define log_printf(level, format, argv...) do{     \
    if(level & print_level)     \
        printf("[%s][%s(%d)]:"format, #level, __FUNCTION__, __LINE__, ##argv);  \
}while(0)

#define MIN(x, y) ((x) > (y) ? (y) : (x))

#define IP_MAX_LEN          64
#define HOST_NAME_LEN       256

typedef enum
{
    IP_TYPE_MIX = -1,

    IP_TYPE_V4 = 0,
    IP_TYPE_V6,
    IP_TYPE_DOMIN,

    IP_TYPE_MAX
}url_ip_e_type;

typedef struct
{
    int port;                           /* 端口 */
    url_ip_e_type ip_type;              /* v4/v6 */
    char ip_str[IP_MAX_LEN];
    char host_name[HOST_NAME_LEN];
}parse_url_info_s_type;

/* 不区分大小写的strstr */
char *strncasestr(char *str, char *sub)
{
    if(!str || !sub)
        return NULL;

    int len = strlen(sub);
    if (len == 0)
    {
        return NULL;
    }

    while (*str)
    {
        if (strncasecmp(str, sub, len) == 0)
        {
            return str;
        }
        ++str;
    }
    return NULL;
}

int url_parse_info(char *url, parse_url_info_s_type *info)
{
    int len = 0;
    char **pp;
    char buffer[8] = {0};
    struct in_addr addr;
    struct hostent* host;
    char *tmp = url, *start = NULL, *end = NULL;

    /* 跳过http:// */
    if(strncasestr(tmp, "http://"))
    {
        tmp += strlen("http://");
        info->port = 80;
    }
    else if(strncasestr(tmp, "https://"))
    {
        tmp += strlen("https://");
        info->port = 443;
    }
    else
    {
        /* 如果无http头,则默认domin */
        log_printf(MSG_INFO, "cur don't have http head, url : %s \n", url);
        start = strchr(tmp, ':');
        if(NULL == start)
        {
            return -1;
        }
        strncpy(buffer, start+1, strlen(start+1));
        info->port = atoi(buffer);
    }

    start = tmp;
    if(!(tmp = strchr(start, '/')))
    {
        strncpy(info->host_name, start, strlen(start)-3);
        info->host_name[strlen(start)] = '\0';
    }
    else
    {
        end = tmp;
        len = MIN(end - start, HOST_NAME_LEN - 1);
        strncpy(info->host_name, start, len);
        info->host_name[len] = '\0';
    }

    log_printf(MSG_INFO, "host_name:%s, port:%d \n", info->host_name, info->port);
    host = gethostbyname(info->host_name);
    if (host == NULL)
    {
        log_printf(MSG_ERROR, "gethostbyname %s failed \n", info->host_name);
        return -1;
    }

    memset(info->host_name, 0, sizeof(info->host_name));
    if(!(tmp = strrchr(start, '/')))
    {
        strncpy(info->host_name, start, strlen(start)-3);
        info->host_name[strlen(start)] = '\0';
    }
    else
    {
        end = tmp;
        len = MIN(end - start, HOST_NAME_LEN - 1);
        strncpy(info->host_name, start, len);
        info->host_name[len] = '\0';
    }
    log_printf(MSG_INFO,"h_addrtype type: %d \n", host->h_addrtype);

    if(AF_INET == host->h_addrtype)
    {
        info->ip_type = 0;
    }
    else if(AF_INET6 == host->h_addrtype)
    {
        info->ip_type = 1;
    }
    else
    {
        info->ip_type = IP_TYPE_DOMIN;
        return 0;
    }

    pp = host->h_addr_list;
    if (*pp != NULL)
    {
        addr.s_addr = *((unsigned int *)*pp);
        strcpy(info->ip_str, inet_ntoa(addr));
        pp++;
        return 0;
    }

    return -1;
}

int main()
{
    parse_url_info_s_type body = {0};
    url_parse_info("http://xxxxx/upload/:1440", &body);
    log_printf(MSG_INFO, "host_name:%s, ip is %s, ip_type is %d, port : %d \n", body.host_name, body.ip_str, body.ip_type, body.port);
}