#include "zerg_sdio.h"

//char *name = "TEST.TXT";			 // Имя будущего файла
extern name;                         // Данные берем из файла main.c
//char *folder_name = "FOLDER";		 // Имя будущего папки
extern folder_name;					 // Данные берем из файла main.c
//extern char path[20] = "0:";		 // Путь к SD карте после форматирования
extern char path;					 // Данные берем из файла main.c
FRESULT res;						 /* FatFs function common result code */
uint32_t byteswritten, bytesread;	 /* File write/read counts */
uint8_t wtext[] = "Hi Zerg ;)";		 // Массив данных который будем записывать в файл на SD.
uint8_t addtext[] = "Keep going!!!\r\n"; // Массив данных который будем добавлять в конец файла на SD.
uint8_t rtext[100];					 // Массив в который будем сохранять данные из файла при чтении.
static uint8_t buffer[_MAX_SS];		 /* a work buffer for the f_mkfs() */

//char buffer[100];	   // to store strings...   BUFFER_SIZE
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS;  /* File system object for SD logical drive */
extern FIL SDFile;	   /* File object for SD */
UINT br, bw;		   // File read/write count

DIR dir;
UINT i;
FILINFO fileinfo;
DWORD fre_clust;
FATFS *pfs;
uint32_t total, free_space;

/*--------------------------------------------------------------------------------------------------------------*/
// Очищаем буфер. / Clear buffer
void clear_buffer(void)
{
	for (int i = 0; i < 100; i++)
		buffer[i] = '\0'; // BUFFER_SIZE
}
/*--------------------------------------------------------------------------------------------------------------*/
// Монтируем SD-карту / Mount SD card
void zerg_mount(void)
{
	if (f_mount(&SDFatFS, (TCHAR const *)SDPath, 0) != FR_OK)
	{
		printf("Error Mounting SD Card\r\n");
	}
	else
	{
		printf("SD card mounted successfully!\r\n");
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Форматируем SD-карту / Format SD card
void zerg_format(void)
{
	// fr = f_mkfs((TCHAR const *)SDPath, FM_ANY, 0, buffer, sizeof(buffer));
	// fr = f_mkfs("0:", FM_ANY, 0, buffer, sizeof(buffer));
	FRESULT fr;
	fr = f_mkfs(&path, FM_ANY, 0, buffer, sizeof(buffer));
	if (fr != FR_OK)
	{
		printf("Error to format SD-card!\r\n");
		Error_Handler();
	}
	else
	{
		printf("SD-card formated successfully!\r\n");
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Создаем файл для записи если не существует то, создаем его! / Open file for writing (Create)
void zerg_open(void)
{
	if (f_open(&SDFile, (char*)name, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		printf("Error openning file\r\n");
	}
	else
	{
		printf("File %s Openned Successfully!\r\n",name);
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Запись данных в файл / Write data to the text file
void zerg_write(void)
{
	res = f_write(&SDFile, wtext, strlen((char *)wtext),
				  (void *)&byteswritten);
	if ((byteswritten == 0) || (res != FR_OK))
	{
		printf("Failed to write file!\r\n");
		f_close(&SDFile);
		clear_buffer();
	}
	else
	{
		printf("File written successfully\r\n");
		f_close(&SDFile);
		clear_buffer();
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Считываем данные из файла / Reading data from file
void zerg_read(void)
{
	f_open(&SDFile, (char*)name, FA_READ);
	memset(rtext, 0, sizeof(rtext));
	res = f_read(&SDFile, rtext, sizeof(rtext), (UINT *)&bytesread);
	if ((bytesread == 0) || (res != FR_OK))
	{
		printf("Failed to read file!\r\n");
	}
	else
	{
		//printf("File *%s* readed successfully! \n", name);
		printf("File *%s* has content it is; %s\r\n",name, (char *)rtext);
	}
	f_close(&SDFile);
	clear_buffer();
}

//	int bufsize(char *buf) {
//		int i = 0;
//		while (*buf++ != '\0')
//			i++;
//		return i;
//	}
/*--------------------------------------------------------------------------------------------------------------*/
// Дописываем данные в существующий файл / Adding data to an existing file
void zerg_add_data(void)
{
	res = f_open(&SDFile, (char*)name, FA_OPEN_APPEND | FA_WRITE);
	if (res != FR_OK)
	{
		printf("Error no %d in opening file *%s*\n", res, name);
	}
	else
	{
		printf("File *%s* is opened. Now we will add same information! \n", name);
	}
	int len = f_size(&SDFile);
	if (len != 0)
		len += 2;
	f_lseek(&SDFile, len);
	res = f_write(&SDFile, addtext, sizeof(addtext), (void *)&byteswritten);
	if ((byteswritten > 0) && (res == FR_OK))

		clear_buffer();
	{
		printf("Information was added successfully in %s\n", name);
	}
	/* Close file */
	res = f_close(&SDFile);
	if (res != FR_OK)
	{
		printf("Error no %d in closing file *%s*\n", res, name);
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Создаем папку / Creating a folder
void zerg_creat_folder(char *folder_name)
{
	res = f_mkdir((char*)folder_name);
	if (res == FR_OK)
	{
		printf("Folder %s has been created successfully \n", folder_name);
		// send_uart (buffer);
	}
	else
	{
		printf("Eerror no %d in creating directory, perhaps it exists!\n", res);
		// send_uart(buffer);
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Смотрим содержимое SD карты / Watch the contents of the SD card
//FRESULT zerg_scan_files(char *path)
//{
//	res = f_opendir(&dir, path); /* Open the directory */
//	if (res == FR_OK)
//		//printf("Content of SD-card: \r\n");
//	{
//		for (;;)
//		{
//			res = f_readdir(&dir, &fileinfo); /* Read a directory item */
//			if (res != FR_OK || fileinfo.fname[0] == 0)break;  /* Break on error or end of dir */
//			if (fileinfo.fattrib & AM_DIR) /* It is a directory */
//			{
//				if (!(strcmp("SYSTEM~1", fileinfo.fname)))continue;
//				printf("Directory(s): %s\r\n", fileinfo.fname);
//				// send_uart(buffer);
//				i = strlen(path);
//				sprintf(&path[i], "/%s", fileinfo.fname);
//				res = zerg_scan_files(path); /* Enter the directory */
//				if (res != FR_OK)break;
//				path[i] = 0;
//			}
//			else
//			{ /* It is a file. */
//				printf("File(s): %s/%s\n", path, fileinfo.fname);
//				// send_uart(buffer);
//			}
//		}
//		//f_closedir(&dir);
//	}
//	return res;
//}

FRESULT zerg_scan_files (char *pat){
    char* path;       /* Start node to be scanned (also used as work area) */

    //FRESULT res;
    //FILINFO fno;
    //DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fileinfo.lfname = lfn;
    fileinfo.lfsize = sizeof lfn;
#endif


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fileinfo);                   /* Read a directory item */
            if (res != FR_OK || fileinfo.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fileinfo.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
            fn = fileinfo.fname;
#endif
            if (fileinfo.fattrib & AM_DIR) {                    /* It is a directory */
                sprintf(&path[i], "/%s", fn);
                res = zerg_scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fn);
            }
        }
    }

    return res;
}



/*--------------------------------------------------------------------------------------------------------------*/
// Отмантируем SD карту / Ummount the SD card
void zerg_ummount(void)
{
	res = f_mount(&SDFatFS, (TCHAR const *)NULL, 0);
	// if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
	if (res == FR_OK)
		printf("SD CARD UNMOUNTED successfully...\n");
	else
		printf("Error!!! in UNMOUNTING SD CARD\n");
}

/*--------------------------------------------------------------------------------------------------------------*/
// Удалить файл/папку / Delite file/folder
void zerg_delete(char *name)
{
	/**** check whether the file exists or not ****/
	res = f_stat(name, &fileinfo);
	if (res != FR_OK)
	{
		printf("%s does not exists\n", name);
		// send_uart(buffer);
	}

	else
	{
		res = f_unlink(name);
		if (res == FR_OK)
		{
			printf("%s has been removed successfully\n", name);
			// send_uart(buffer);
		}

		else
		{
			printf("Error in removing *%s*\n", name);
			// send_uart(buffer);
		}
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Узнать количество свободного места на SD карте / Check free space
void zerg_get_free_space(void)
{
	/* Check free space */
	f_getfree("", &fre_clust, &pfs);

	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	printf("SD CARD Total Size: \t%lu\n", total);
	// send_uart(buffer);
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	printf("SD CARD Free Space: \t%lu\n", free_space);
	// send_uart(buffer);
}
/*--------------------------------------------------------------------------------------------------------------*/
// Получить сведения о файле / Get file Information
void zerg_get_file_info(char *name)
{
	res = f_stat(name, &fileinfo);
	switch (res)
	{
	case FR_OK:

		printf("Below are the details of the *%s* \nSize: %lu\n", name, fileinfo.fsize);
		// send_uart(buffer);
		printf("Timestamp: %u/%02u/%02u, %02u:%02u\n",
			   (fileinfo.fdate >> 9) + 1980, fileinfo.fdate >> 5 & 15, fileinfo.fdate & 31,
			   fileinfo.ftime >> 11, fileinfo.ftime >> 5 & 63);
		// end_uart(buffer);
		printf("Attributes: %c%c%c%c%c\n",
			   (fileinfo.fattrib & AM_DIR) ? 'D' : '-',
			   (fileinfo.fattrib & AM_RDO) ? 'R' : '-',
			   (fileinfo.fattrib & AM_HID) ? 'H' : '-',
			   (fileinfo.fattrib & AM_SYS) ? 'S' : '-',
			   (fileinfo.fattrib & AM_ARC) ? 'A' : '-');
		// send_uart(buffer);
		break;

	case FR_NO_FILE:
		printf("*%s* does not exist.\n", name);
		// send_uart(buffer);
		break;

	default:
		printf("An error occurred. (%d)\n", res);
		// send_uart(buffer);
	}
}
/*--------------------------------------------------------------------------*/

/*##-11- Unlink the SD disk I/O driver ####################################*/
//	FATFS_UnLinkDriver(SDPath);
//	printf("\r\n");
//	printf("CardType is : %lu\r\n", hsd.SdCard.CardType);	// CardType is : 0
//	printf("CardCapacity is : %lu\r\n", hsd.SdCard.BlockNbr);// CardCapacity is : 1961984
//	printf("CardBlockSize is : %lu\r\n", hsd.SdCard.BlockSize);	// CardBlockSize is : 512
//	printf("RCA is : %lu\r\n", hsd.SdCard.RelCardAdd);		// RCA is : 45928
//	printf("CardVersion is : %lu\r\n", hsd.SdCard.CardVersion);	// CardVersion is : 1
//	printf("Class is : %lu\r\n", hsd.SdCard.Class);			// Class is : 1525
//	printf("LogBlockNbr is : %lu\r\n", hsd.SdCard.LogBlockNbr);	// LogBlockNbr is : 1961984
//	printf("LogBlockSize is : %lu\r\n", hsd.SdCard.LogBlockSize); // LogBlockSize is : 512
