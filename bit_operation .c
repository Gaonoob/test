#include <stdio.h>	/* for stdout */
#include <stdlib.h>	/* for malloc() */
#include <assert.h>	/* for run-time control */
#include <string.h>
#include <stdio.h>

#define RSV_BIT_SWITCH  7
#define RSV_BIT_CW  0
#define RSV_BIT_BW  1
#define RSV_BIT_SH  2

#define RSV_BIT0(a,b) (a & (~(1 << b)))
#define RSV_BIT1(a,b) (a |(1 << b))



#define GET_BIT(x, bit)     ((x & (1 << bit)) >> bit)   /* 获取第bit位 */

int main()
{
    int num = 32;
    //num = RSV_BIT1(num, RSV_BIT_SWITCH);
    //num = RSV_BIT1(num, RSV_BIT_CW);
    printf("num is %d\n", num);
    //num = RSV_BIT0(num, RSV_BIT_SWITCH);
    //printf("num is %d\n", num);
    printf("num is %d\n", GET_BIT(num, 5));
}