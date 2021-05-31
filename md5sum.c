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
#include <unistd.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <string.h>
#include <sys/statfs.h>
#include <openssl/md5.h> 

#define FOTA_LEN_16 (16)
#define FOTA_LEN_1024 (1024)

//gcc md5sum.c -lcrypto

int main()
{
    int times = 10;

    while(times--)
    {
        int i = 0;
        int len = 0;
        MD5_CTX ctx;
        char buffer[FOTA_LEN_1024] = {0};
        unsigned char outmd5[FOTA_LEN_16];

        char *fota_path = "./11.zip";

        /* 计算FOTA包MD5值 */
        FILE *fota_fp = NULL;
        fota_fp = fopen(fota_path, "rb");

        if(NULL == fota_fp)
        {
            printf("Can't open file\n");
            return -1;
        }

        MD5_Init(&ctx);
        while((len = fread(buffer, 1, FOTA_LEN_1024, fota_fp)) > 0)
        {
            MD5_Update(&ctx, buffer, len);
            memset(buffer, 0, sizeof(buffer));
        }
        MD5_Final(outmd5, &ctx);
        
        fclose(fota_fp);
        fota_fp = NULL;

        for(i=0; i<16; i++)
        {
            sprintf(buffer+(i*2), "%02X", outmd5[i]);
        }
        
        printf("buffer is %s\n", buffer);
        sleep(2);
    }

}