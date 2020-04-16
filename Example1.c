#include<stdio.h>
#include"EEPROM.h"

int main()
{

int add,add1;char name;
for(int temp=0;temp<10;temp++)
EEPROM_Write(temp,temp+10);


printf("Enter Single Character\n");
scanf("%c",&name);
EEPROM_Write(1001,name);
name='\0';
name=EEPROM_Read(1001);
printf("Value at 1001 is %c \n",name);

printf("Enter Number as address\n");
scanf("%d",&add);
EEPROM_Write(1000,add);
add1=EEPROM_Read(1000);
printf("Value at 1000 is %d \n",add1);

EEPROM_Close();
}
