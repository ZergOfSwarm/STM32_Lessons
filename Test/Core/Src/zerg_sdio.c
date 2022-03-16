#include "zerg_sdio.h"
#include "http_ssi.h" // Для работы со 'struct table' и "#define num_lines 70" из https_ssi.c

extern name_sd;                         // Данные берем из файла main.c
extern folder_name;					 // Данные берем из файла main.c
extern char path;					 // Данные берем из файла main.c
FRESULT res; /* FatFs function common result code */
uint32_t byteswritten, bytesread; /* File write/read counts */
uint8_t wtext[] = "Hi Zerg;)";// Массив данных который будем записывать в файл на SD.
uint8_t addtext[] = "Keep going!!!\r\n"; // Массив данных который будем добавлять в конец файла на SD.
char rtext[11000]; // Массив в который будем сохранять данные из файла при чтении.
static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

//char buffer[100];	   // to store strings...   BUFFER_SIZE
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */
UINT br, bw;		   // File read/write count

DIR dir;
UINT i;
FILINFO fileinfo;
DWORD fre_clust;
FATFS *pfs;
uint32_t total, free_space;

extern struct table T[num_lines];		   // доступ из вне к 'struct table'.
/*--------------------------------------------------------------------------------------------------------------*/
// Очищаем буфер. / Clear buffer
void clear_buffer(void) {
	for (int i = 0; i < 100; i++)
		buffer[i] = '\0'; // BUFFER_SIZE
}
/*--------------------------------------------------------------------------------------------------------------*/
// Монтируем SD-карту / Mount SD card
void zerg_mount(void) {
	if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
		printf("Error Mounting SD Card\r\n");
	} else {
		printf("SD card mounted successfully!\r\n");
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Форматируем SD-карту / Format SD card
void zerg_format(void) {
	// fr = f_mkfs((TCHAR const *)SDPath, FM_ANY, 0, buffer, sizeof(buffer));
	// fr = f_mkfs("0:", FM_ANY, 0, buffer, sizeof(buffer));
	FRESULT fr;
	fr = f_mkfs(&path, FM_ANY, 0, buffer, sizeof(buffer));
	if (fr != FR_OK) {
		printf("Error to format SD-card!\r\n");
		Error_Handler();
	} else {
		printf("SD-card formated successfully!\r\n");
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Создаем файл для записи если не существует то, создаем его! / Open file for writing (Create)
void zerg_open(void) {
	if (f_open(&SDFile, (char*) name_sd, FA_CREATE_ALWAYS | FA_WRITE)
	//if (f_open(&SDFile, (char*) "0:name_sd", FA_CREATE_ALWAYS | FA_WRITE) //Create destination file on the drive 0
			!= FR_OK) {
		printf("Error openning file\r\n");
	} else {
		printf("File %s Openned Successfully!\r\n", name_sd);
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Запись данных в файл / Write data to the text file
void zerg_write(void) {
	res = f_write(&SDFile, wtext, strlen((char*) wtext), (void*) &byteswritten);
	if ((byteswritten == 0) || (res != FR_OK)) {
		printf("Failed to write file!\r\n");
		f_close(&SDFile);
		clear_buffer();
	} else {
		printf("File written successfully\r\n");
		f_close(&SDFile);
		clear_buffer();
	}
}

/*--------------------------------------------------------------------------------------------------------------*/
// Считываем данные из файла / Reading data from file
void zerg_read(void) {
	f_open(&SDFile, (char*) name_sd, FA_READ);
	memset(rtext, 0, sizeof(rtext));
	res = f_read(&SDFile, rtext, sizeof(rtext), (UINT*) &bytesread);
	if ((bytesread == 0) || (res != FR_OK)) {
		printf("Failed to read file!\r\n");
	} else {
		//printf("File *%s* readed successfully! \n", name);
		//printf("File *%s* has content it is; %s\r\n",name_sd, (char *)rtext);
		printf("Data from SD card:\r\n");
		printf("%s\r\n", rtext);
//		for(rtext i = 0; ; i++) {
//
//		}
	}
	f_close(&SDFile);
	clear_buffer();
}

int zerg_get_data_from_sd(void) {
	zerg_mount();
	f_open(&SDFile, (char*) name_sd, FA_READ);
	memset(rtext, 0, sizeof(rtext));
	res = f_read(&SDFile, rtext, sizeof(rtext), (UINT*) &bytesread);
	if ((bytesread == 0) || (res != FR_OK)) {
		printf("Failed to read file!\r\n");
		return 0;
	} else {
		//printf("File *%s* readed successfully! \n", name);
		//printf("File *%s* has content it is; %s\r\n",name_sd, (char *)rtext);
		//printf("Data from SD card:\r\n");
		//printf("%s\r\n", rtext);
		char *token1, *token2, *token3;
		char *rest = rtext;
		char *name1; // для записи в структуру
		int id;
		int z = 0;
		int y = 0;

		while ((token1 = strtok_r(rest, "\n", &rest))) {
			//printf("token - %s\n", token1);
			char *rest1 = token1;
			y = 0;
			while ((token2 = strtok_r(rest1, "=", &rest1))) {
				//printf("NAME - %s\n", token2);
				z = 0;
				if (y == 0) {
					// получение id
					char *rest2 = token2;
					while ((token3 = strtok_r(rest2, "_", &rest2))) {
						if (z == 0) {
							name1 = token2;
						}
						if (z == 1) {
							id = atoi(token3);
							//printf("ID %d\n", id);
						}
						z++;
					}
				}
				if (y == 1) {
					//printf("VAL = %s\n", token2);
					// записываем в структуру
					/************************************************/
					if (strcmp(name1, "pulluptype") == 0) {
						strcpy(T[id].pulluptype, token2);
						printf(" pulluptype_%d is: %s \n", id,
								T[id].pulluptype);
					} else if (strcmp(name1, "bounceinterval") == 0) {
						T[id].bounceinterval = atoi(token2);
						printf(" bounceinterval_%d is: %d \n", id,
								T[id].bounceinterval);
					} else if (strcmp(name1, "holdinterval") == 0) {
						T[id].holdinterval = atoi(token2);
						printf(" holdinterval_%d is: %d \n", id,
								T[id].holdinterval);
					} else if (strcmp(name1, "repeat") == 0) {
						T[id].repeat = atoi(token2);
						printf(" repeat_%d is: %d \n", id, T[id].repeat);
					} else if (strcmp(name1, "repeatinterval") == 0) {
						T[id].repeatinterval = atoi(token2);
						printf(" repeatinterval_%d is: %d \n", id,
								T[id].repeatinterval);
					} else if (strcmp(name1, "doubleclickinterval") == 0) {
						T[id].doubleclickinterval = atoi(token2);
						printf(" doubleclickinterval_%d is: %d \n", id,
								T[id].doubleclickinterval);
					} else if (strcmp(name1, "preventclick") == 0) {
						T[id].preventclick = atoi(token2);
						printf(" preventclick_%d is: %d \n", id,
								T[id].preventclick);
					} else if (strcmp(name1, "infofield") == 0) {
						strcpy(T[id].infofield, token2);
						printf(" infofield_%d is: %s \n", id, T[id].infofield);
					} else if (strcmp(name1, "onoff") == 0) {
						T[id].onoff = atoi(token2);
						printf(" onoff_%d is: %d \n", id, T[id].onoff);
					}
					/************************************************/
				}
				y++;
			}
			z++;
		}
	}
	f_close(&SDFile);
	clear_buffer();
	return 0;
}

// Запись структуры на SD-card / Write our struct to the SD-card
void zerg_write_data_to_sd(int num) {
	  zerg_mount();
	  zerg_open();
	char full_data[15000];
	char line[200];

	for (int id = 0; id <= num; id++) {
//		printf(" pulluptype is: %s \n", T[id].pulluptype);
//		printf(" bounceinterval is: %d \n", T[id].bounceinterval);
//		printf(" holdinterval is: %d \n", T[id].holdinterval);
//		printf(" repeat is: %d \n", T[id].repeat);
//		printf(" repeatinterval is: %d \n", T[id].repeatinterval);
//		printf(" doubleclickinterval is: %d \n", T[id].doubleclickinterval);
//		printf(" preventclick is: %d \n", T[id].preventclick);
//		printf(" infofield is: %s \n", T[id].infofield);
//		printf(" onoff is: %d \n", T[id].onoff);

		sprintf(line,"pulluptype_%d=%s\n",id,T[id].pulluptype);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"bounceinterval_%d=%d\n",id,T[id].bounceinterval);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"holdinterval_%d=%d\n",id,T[id].holdinterval);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"repeat_%d=%d\n",id,T[id].repeat);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"repeatinterval_%d=%d\n",id,T[id].repeatinterval);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"doubleclickinterval_%d=%d\n",id,T[id].doubleclickinterval);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"preventclick_%d=%d\n",id,T[id].doubleclickinterval);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"infofield_%d=%s\n",id,T[id].infofield);
		strcat(full_data, line); // объеденяем строки
		sprintf(line,"onoff_%d=%d\n",id,T[id].onoff);
		strcat(full_data, line); // объеденяем строки
	}
		res = f_write(&SDFile, full_data, strlen((char*) full_data), (void*) &byteswritten);
		if ((byteswritten == 0) || (res != FR_OK)) {
			printf("Failed to write file!\r\n");
			f_close(&SDFile);
			clear_buffer();
		} else {
			printf("File written successfully\r\n");
			f_close(&SDFile);
			clear_buffer();
		}
		memset(full_data, 0, sizeof(full_data));
		memset(line, 0, sizeof(line));
		//zerg_read();
		}



// Дописываем данные в существующий файл / Adding data to an existing file
void zerg_add_data(void) {
	res = f_open(&SDFile, (char*) name_sd, FA_OPEN_APPEND | FA_WRITE);
	if (res != FR_OK) {
		printf("Error no %d in opening file *%s*\n", res, name_sd);
	} else {
		printf("File *%s* is opened. Now we will add same information! \n",
				name_sd);
	}
	int len = f_size(&SDFile);
	if (len != 0)
		len += 2;
	f_lseek(&SDFile, len);
	res = f_write(&SDFile, addtext, sizeof(addtext), (void*) &byteswritten);
	if ((byteswritten > 0) && (res == FR_OK))

		clear_buffer();
	{
		printf("Information was added successfully in %s\n", name_sd);
	}
	/* Close file */
	res = f_close(&SDFile);
	if (res != FR_OK) {
		printf("Error #%d in closing file *%s*\n", res, name_sd);
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Создаем папку / Creating a folder
void zerg_creat_folder(char *folder_name) {
	res = f_mkdir((char*) folder_name);
	if (res == FR_OK) {
		printf("Folder %s has been created successfully \n", folder_name);
		// send_uart (buffer);
	} else {
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
FRESULT zerg_scan_files(char *pat) {
	char *path; /* Start node to be scanned (also used as work area) */

//FRESULT res;
//FILINFO fno;
//DIR dir;
	int i;
	char *fn; /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fileinfo.lfname = lfn;
    fileinfo.lfsize = sizeof lfn;
#endif

	res = f_opendir(&dir, path); /* Open the directory */
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = f_readdir(&dir, &fileinfo); /* Read a directory item */
			if (res != FR_OK || fileinfo.fname[0] == 0)
				break; /* Break on error or end of dir */
			if (fileinfo.fname[0] == '.')
				continue; /* Ignore dot entry */
#if _USE_LFN
            fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
			fn = fileinfo.fname;
#endif
			if (fileinfo.fattrib & AM_DIR) { /* It is a directory */
				sprintf(&path[i], "/%s", fn);
				res = zerg_scan_files(path);
				if (res != FR_OK)
					break;
				path[i] = 0;
			} else { /* It is a file. */
				printf("%s/%s\n", path, fn);
			}
		}
	}

	return res;
}

/*--------------------------------------------------------------------------------------------------------------*/
// Отмонтируем SD карту / Ummount the SD card
void zerg_ummount(void) {
	f_close(&SDFile);
	printf("f_close->res = %d\n", res);
	res = f_mount(&SDFatFS, (TCHAR const*) NULL, 0);
// if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
	printf("res = %d\n", res); // FR_INVALID_DRIVE,	/* (11) The logical drive number is invalid */
	if (res == FR_OK)
		printf("SD CARD UNMOUNTED successfully...\n");
	else
		printf("Error!!! in UNMOUNTING SD CARD\n");
}

/*--------------------------------------------------------------------------------------------------------------*/
// Удалить файл/папку / Delite file/folder
void zerg_delete(char *name) {
	/**** check whether the file exists or not ****/
	res = f_stat(name, &fileinfo);
	if (res != FR_OK) {
		printf("%s does not exists\n", name);
		// send_uart(buffer);
	}

	else {
		res = f_unlink(name);
		if (res == FR_OK) {
			printf("%s has been removed successfully\n", name);
			// send_uart(buffer);
		}

		else {
			printf("Error in removing *%s*\n", name);
			// send_uart(buffer);
		}
	}
}
/*--------------------------------------------------------------------------------------------------------------*/
// Узнать количество свободного места на SD карте / Check free space
void zerg_get_free_space(void) {
	/* Check free space */
	f_getfree("", &fre_clust, &pfs);

	total = (uint32_t) ((pfs->n_fatent - 2) * pfs->csize * 0.5);
	printf("SD CARD Total Size: \t%lu\n", total);
// send_uart(buffer);
	free_space = (uint32_t) (fre_clust * pfs->csize * 0.5);
	printf("SD CARD Free Space: \t%lu\n", free_space);
// send_uart(buffer);
}
/*--------------------------------------------------------------------------------------------------------------*/
// Получить сведения о файле / Get file Information
void zerg_get_file_info(char *name) {
	res = f_stat(name, &fileinfo);
	switch (res) {
	case FR_OK:

		printf("Below are the details of the *%s* \nSize: %lu\n", name,
				fileinfo.fsize);
		// send_uart(buffer);
		printf("Timestamp: %u/%02u/%02u, %02u:%02u\n",
				(fileinfo.fdate >> 9) + 1980, fileinfo.fdate >> 5 & 15,
				fileinfo.fdate & 31, fileinfo.ftime >> 11,
				fileinfo.ftime >> 5 & 63);
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
