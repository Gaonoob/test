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

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#define PATH "/mnt/hgfs/Gao/test"
#define PATH_SIZE (19)

static time_t get_file_time(const char *filename)
{
    struct stat statbuf;

    if(-1 == stat (filename, &statbuf))
    {
        printf ("Get stat on %s Error：%s\n", filename, strerror (errno));
        return (-1);
    }

    /* 判断大小,不够10M返回0*/
    if(PATH_SIZE > statbuf.st_size)
    {
        return 0;
    }

    return statbuf.st_mtime;
}

char *get_dir_file(const char *dir_data, char *buffer, int buffer_len, int flag)
{
    DIR *dirp;
    struct dirent *direntp;
    int stats;
    time_t cur_time = 0;
    time_t file_time = 0;

    if((dirp = opendir(dir_data)) == NULL)
    {
        printf ("Open Directory %s Error：%s\n", dir_data, strerror (errno));
        return NULL;
    }

    /* 获取当前时间 */
    if(0 == flag)
    {
        time(&cur_time);
    }
    else
    {
        cur_time = 0;
    }
    //printf ("cur file_time: %ld, cur_time: %ld \n", file_time, cur_time);

    while(1)
    {
        direntp = readdir (dirp);
        if(NULL == direntp)
        {
            break;
        }

        if(NULL == strstr(direntp ->d_name, ".zip"))
        {
            //continue;
        }

        if(direntp->d_type == DT_DIR)
        {
            continue;
        }

        file_time = get_file_time(direntp ->d_name);
        //printf ("%s time:%ld \n", direntp ->d_name, file_time);
        if(0 == file_time)
        {
            continue;
        }

        if(0 == flag)
        {
            if(0 < (difftime(cur_time, file_time)))
            {
                cur_time = file_time;
                //printf ("cur direntp ->d_name:%s\n", direntp ->d_name);
                memset(buffer, 0, buffer_len);
                strcpy(buffer, direntp ->d_name);
            }
        }
        else
        {
            if(0 > (difftime(cur_time, file_time)))
            {
                cur_time = file_time;
                memset(buffer, 0, buffer_len);
                strcpy(buffer, direntp ->d_name);
            }
        }
                //sleep(1);
    }

    closedir (dirp);
    return buffer;
}

typedef struct{
    int hour;
    int min;
    int sec;
    int weekday;
}tieeeem;

void timer_to_str(tieeeem *data)
{
    char buffer[25] = {0};
    sprintf(buffer, "%3d %3d %3d %3d", data->hour, data->min, data->sec, data->weekday);
    printf("buffer : %s\n", buffer);

    memset(data, 0x0,sizeof(data));
    sscanf(buffer, "%3d %3d %3d %3d", &data->hour, &data->min, &data->sec, &data->weekday);
    printf("data->hour : %d\n", data->hour);
    printf("data->min : %d\n", data->min);
    printf("data->sec : %d\n", data->sec);
    printf("data->weekday : %d\n", data->weekday);
}

int main (int argc, char **argv)
{
    tieeeem tip ={0};
    tip.hour = 24;
    tip.min = 3;
    tip.sec = 0;
    tip.weekday = 127 ;
    timer_to_str(&tip);
    #if 0
    int tt = 1439;
    int hour = tt/60;
    int min = tt%60;
    int sec = tt*60%60;
    printf("hour:%d, min:%d, sec:%d \n", hour, min, sec);
    #endif
    return 0;
}