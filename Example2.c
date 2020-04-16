#include<stdio.h>
#include<unistd.h>
int main()
{
char str[100];
long long sen_time;
for(int temp=0;temp<100;temp++)
{
scanf("%lld",&sen_time);
scanf("%s",str);
printf("%s ID at time %lld\n",str,sen_time);
}
return 0;
}
