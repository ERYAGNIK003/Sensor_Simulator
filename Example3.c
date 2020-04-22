#include "EEPROM.h"
#include<stdio.h>
#include<string.h>


//EEPROM Config
#define MAX_FILES 5
//From this address start read to address by jumping to 6 , add if you find it ZERO means it is free , then find allocated memory and free 
//address
#define START_ADD 15
#define DE_size 3
#define File_size 781
#define MAX_DE 256
#define FH_size 13
#define uint16_t unsigned short
#define uint8_t unsigned char 
#define int8_t short

void Cache_Init(void);
unsigned char lowByte(unsigned short);
unsigned char highByte(unsigned short);
void EEPROM_FS_Init(void);
void ReadFAT(void);
uint16_t GetFreeFileAdd(void);
uint16_t GetFreeMem(void);
uint8_t GetNoFiles(void);
uint16_t GetHeaderAdd(uint8_t);

unsigned char tagID[9]={'\0'};//stores one tag ID
long long time_sen;//Stores time getting from simulator
unsigned char f_name[4]={'\0'};

typedef struct
{
unsigned char file_name[3];//only 3 bytes for file name 
unsigned char tag_id[8];//UID of RFID tag
unsigned char  freeoffset;//For next entry valid offset from current header address.It varies from 0-255
unsigned char  data_size;//size of data_entry element =3B
}File_Header;//13B

typedef struct
{
unsigned char HH;
unsigned char MM;
unsigned char SS;
}Data_Entry;//3B

typedef struct
{
//*You can add here one START_ADD , which is first freeMemAdd(in all cases)
unsigned short FreeMemAdd;//Used to get Next Free Address, For First time it is Default, after it you have to find new valid free address
//After each file is created new free address for next file is calculated (by adding (no.of MAX entries* size of each entry + Header size of 
///previous file
unsigned short FreeMemSize;//Remaining Memory Size
unsigned char no_files;//Current no. of files/Tags in system
}FAT_Header;//5B

typedef struct
{
FAT_Header header;//5B
unsigned short tag_header_add[MAX_FILES];//10B
}FAT;//15B==> 0-14 address space for FAT filesystem



FAT file_table;
File_Header fh;
Data_Entry DE;
unsigned short file_add,free_mem;
unsigned char  num;
unsigned char  h=1,m=1,s=0;
unsigned char  fd,fd1;//current file descriptor , index of tag_header_add.
unsigned char  cache_mem[MAX_FILES][9]; 
unsigned char  cur_no_files;
unsigned char  file_name[4];//name with null , but write only chars.

//You can use cache , for checking for repeating tagID, it's easy to take decision for creating new file 
void Cache_Init()
{
for(unsigned char i=0;i<MAX_FILES;i++)
strcpy(cache_mem[i],"00000000\0");
}

unsigned char lowByte(unsigned short val)
{
return (val & 0XFF);
}
unsigned char highByte(unsigned short val)
{
return ((val & 0XFF00)>>8);
}
//Filesystem function
void EEPROM_FS_Init()
{
  
file_table.header.FreeMemAdd=START_ADD;
file_table.header.FreeMemSize=4081;//len(eeprom)-FAT
file_table.header.no_files=0;
for(unsigned char i=0;i<MAX_FILES;i++)
file_table.tag_header_add[i]=0;
EEPROM_Write(0,lowByte(file_table.header.FreeMemAdd));
EEPROM_Write(1,highByte(file_table.header.FreeMemAdd));
EEPROM_Write(2,lowByte(file_table.header.FreeMemSize));
EEPROM_Write(3,highByte(file_table.header.FreeMemSize));
EEPROM_Write(4,file_table.header.no_files);
for(unsigned char i=0;i<5;i++)
{
  
EEPROM_Write(5+(2*i),lowByte(file_table.tag_header_add[i]));
EEPROM_Write(6+(2*i),highByte(file_table.tag_header_add[i]));  
}

}
uint16_t GetFreeFileAdd()
{
  uint16_t temp=0;
  temp = ((EEPROM_Read(0)) | ((EEPROM_Read(1))<<8));
  return temp;
}
uint16_t GetFreeMem()
{
  uint16_t temp=0;
  temp = ((EEPROM_Read(2)) | ((EEPROM_Read(3))<<8));
  return temp;
}
uint8_t GetNoFiles()
{
  return EEPROM_Read(4);
}
uint16_t GetHeaderAdd(uint8_t file_desc) //
{
  uint16_t temp=0;
  file_desc=2*file_desc;
  temp = ( (EEPROM_Read(5+file_desc)) | (EEPROM_Read((6+file_desc))<<8) );
  return temp;
}
void ReadFAT()
{
file_table.header.FreeMemAdd=GetFreeFileAdd();
file_table.header.FreeMemSize=GetFreeMem();
file_table.header.no_files=GetNoFiles();
for(uint8_t i=0;i<MAX_FILES;i++)
file_table.tag_header_add[i]=GetHeaderAdd(i);
}

  

int main() 
{
  EEPROM_FS_Init();
  printf("FAT filesystem is Implemented, Check log files for debug OR use readFAT to read back\n");
  printf("Contents\n\n");
  ReadFAT();
  printf("FreeMemAdd===>%d\n",file_table.header.FreeMemAdd);
  printf("FreeMemSize==>%d\n",file_table.header.FreeMemSize);
  printf("Total Files==>%d\n",file_table.header.no_files);
  for(int temp=0;temp<MAX_FILES;temp++)
  printf("Address of %d file is %d\n",temp,file_table.tag_header_add[temp]);  
}
