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

#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int ASCII_TO_STRING(unsigned char *instr, int inlen, unsigned char *outstr)
{
    int i = 0;
    int j = 0;

    while(i < inlen)
    {
        sprintf(outstr+j, "%c", instr[i]);
        j++;
        i++;
    }

    return j;
}

int STRING_TO_ASCII(unsigned char *instr, int inlen, unsigned char *outstr)
{
    int i = 0;
    int j = 0;

    while(i < inlen)
    {
        outstr[j] = toascii(instr[i]);
        i++;
        j++;
    }

    return j;
}

/* 2进制字符串转16进制字符串 */
void Bin2Hex(const unsigned char *sSrc, unsigned char *sDest, int nSrcLen)
{
    int times=nSrcLen/4;
    char temp[times];
    int x=0;
    int i ;
    for(i=0;i<times;i++)
    {
        x=8*(sSrc[i*4]-'0');
        x+=4*(sSrc[i*4+1]-'0');
        x+=2*(sSrc[i*4+2]-'0');
        x+=sSrc[i*4+3]-'0';
        sprintf(temp+i, "%1x", x);
    }
    memcpy(sDest, temp, times);
}

/* 16进制转为2进制 */
//十六进制字符串转二进制字符串
void Hex2Bin(unsigned char *source, unsigned char *dest, int len) 
{
    int i=0;
    unsigned char Dict[17][5] =
    {
        "0000", "0001", "0010", "0011",
        "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111",
    };

    for(i=0;i<len;i++)
    {
        int n = 16;
        if (source[i] >= 'a' && source[i] <= 'f') n = source[i] - 'a' + 10;
        if (source[i] >= 'A' && source[i] <= 'F') n = source[i] - 'A' + 10;
        if (source[i] >= '0' && source[i] <= '9') n = source[i] - '0';
        memcpy(&dest[i*4], Dict[n], 4);
    }
    return;
}


void hex2str(unsigned char *pstr, unsigned char *phex, int len)
{
    unsigned char data[2];
    int i, j;

    for (j = 0; j < len; j++) {
        data[0] = (phex[j] & 0xF0) >> 4;
        data[1] = phex[j] & 0x0F;
        for (i = 0; i < 2; i++) {
            if (data[i] < 10) {
                data[i] += 0x30;
            }else{
                if (data[i] < 16) {
                    data[i] = data[i] - 10 + 'A';
                }
            }
            *pstr++ = data[i];
        }
    }
}

//字节流转换为十六进制字符串
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        highByte += 0x30;

        if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
        else
                dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}

//字节流转换为十六进制字符串的另一种实现方式
void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )
{
    int  i;
    char szTmp[3];

    for( i = 0; i < nSrcLen; i++ )
    {
        sprintf( szTmp, "%02X", (unsigned char) sSrc[i] );
        memcpy( &sDest[i * 2], szTmp, 2 );
    }
    return ;
}

//十六进制字符串转换为字节流
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i += 2)
    {
        highByte = toupper(source[i]);
        lowByte  = toupper(source[i + 1]);

        if (highByte > 0x39)
            highByte -= 0x37;
        else
            highByte -= 0x30;

        if (lowByte > 0x39)
            lowByte -= 0x37;
        else
            lowByte -= 0x30;

        dest[i / 2] = (highByte << 4) | lowByte;
    }
    return ;
}

//16进制字符串转为16进制
void StrToHex(unsigned char* pbDest, unsigned char* pbSrc, unsigned char nLen)
{
    unsigned char h1,h2;
    unsigned char s1,s2;
    unsigned char i;

    for (i = 0; i < nLen; i++)
    {
        h1 = pbSrc[2*i];
        h2 = pbSrc[2*i+1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        pbDest[i] = s1*16 + s2;
    }
}

unsigned char ls_generate_crc8(unsigned char * p_buffer, int buf_size)
{
    unsigned char i = 0;
    unsigned char crc = 0;

    if(buf_size <= 0)
    {
        return crc;
    }

    while(buf_size--)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            if((crc & 0x80) != 0)
            {
                crc *= 2;
                crc ^= 0x07; // 多项式：X8 + X2 + X + 1
            }
            else
            {
                crc *= 2;
            }

            if((*p_buffer & i) != 0)
            {
                crc ^= 0x07;
            }
        }
        p_buffer++;
    }
    return crc;
}

#if 0
char *instr = "helloworld";
unsigned char outstr[10] = {0};
STRING_TO_ASCII(instr, 10, outstr);
int i = 0;
for(i = 0; i < 10; i++)
{
    printf("cur outstr[%d] is %d\n", i, outstr[i]);
}
unsigned char outstr1[10] = {0};

ASCII_TO_STRING(outstr, 10, outstr1);
printf("cur outstr1 is %s\n", (char *)outstr1);

int cmd = 0x0213;
unsigned char outstr1[3] = {0};
int i = 0;

outstr1[0] = cmd & 0xFF;
outstr1[1] = (cmd >> 8) & 0xFF;

for(i = 0; i < 2; i++)
{
printf("cur outstr1[%d] is %x\n", i, outstr1[i]);
}
#endif

int main()
{
    char *instr = "helloworld";
    unsigned char outstr[10] = {0};
    STRING_TO_ASCII(instr, 10, outstr);
    int i = 0;
    for(i = 0; i < 10; i++)
    {
        printf("cur outstr[%d] is %x\n", i, outstr[i]);
    }

    unsigned char ret = ls_generate_crc8(outstr, 10);
    printf("cur ret is %x\n", ret);
}