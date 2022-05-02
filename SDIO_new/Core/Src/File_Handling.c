/*
 * File_Handling_RTOS.c
 *
 *  Created on: 14-May-2020
 *      Author: Controllerstech
 */

#include <File_Handling.h>
#include "stm32f4xx_hal.h"


extern UART_HandleTypeDef huart6;
#define UART &huart6



/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;


void Send_Uart (char *string)
{
	HAL_UART_Transmit(UART, (uint8_t *)string, strlen (string), HAL_MAX_DELAY);
}



void Mount_SD (const TCHAR* path)
{
	fresult = f_mount(&fs, path, 1);
	if (fresult != FR_OK) Send_Uart ("ERROR!!! in mounting SD CARD...\n\n");
	else Send_Uart("SD CARD mounted successfully...\n");
}

void Unmount_SD (const TCHAR* path)
{
	fresult = f_mount(NULL, path, 1);
	if (fresult == FR_OK) Send_Uart ("SD CARD UNMOUNTED successfully...\n\n\n");
	else Send_Uart("ERROR!!! in UNMOUNTING SD CARD\n\n\n");
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD (char* pat)
{
    DIR dir;
    UINT i;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;
            	char *buf = malloc(30*sizeof(char));
            	sprintf (buf, "Dir: %s\r\n", fno.fname);
            	Send_Uart(buf);
            	free(buf);
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                fresult = Scan_SD(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
           	   char *buf = malloc(30*sizeof(char));
               sprintf(buf,"File: %s/%s\n", path, fno.fname);
               Send_Uart(buf);
               free(buf);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Don't work!!!!!!!!!!!!!!!!!!! */
FRESULT Scan_files (char* path)/* Start node to be scanned (***also used as work area***) */
{
    DIR dir;
    //char *path = malloc(20*sizeof (char));
    sprintf (path, "%s","/");
    UINT i;

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                fresult = Scan_files(path);                    /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
            	printf("-- It is a file -- %s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_SD (void)
{
	BYTE work[_MAX_SS]; /* Work area (larger is better for processing time) */
	// fr = f_mkfs((TCHAR const *)SDPath, FM_ANY, 0, buffer, sizeof(buffer));
	// fr = f_mkfs("0:", FM_ANY, 0, buffer, sizeof(buffer));
	FRESULT result;
	result = f_mkfs("", FM_ANY, 0, work, sizeof work);
	if (result != FR_OK)
	{
		printf("Error to format SD-card!\r\n");
		Error_Handler();
	}
	else
	{
		printf("SD-card formated successfully!\r\n");
	}
	return result;
}



FRESULT Write_File (char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
	    free(buf);
	    return fresult;
	}

	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	        free(buf);
	        return fresult;
	    }

	    else
	    {
	    	fresult = f_write(&fil, data, strlen(data), &bw);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult, name);
	    		Send_Uart(buf);
	    		free(buf);
	    	}

	    	/* Close file */
	    	fresult = f_close(&fil);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult, name);
	    		Send_Uart(buf);
	    		free(buf);
	    	}
	    	else
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "File *%s* is WRITTEN and CLOSED successfully\n", name);
	    		Send_Uart(buf);
	    		free(buf);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERRROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
	    return fresult;
	}

	else
	{
		/* Open file to read */
		fresult = f_open(&fil, name, FA_READ);

		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
		    Send_Uart(buf);
		    free(buf);
		    return fresult;
		}

		/* Read data from the file
		* see the function details for the arguments */

		char *buffer = malloc(sizeof(f_size(&fil)));
		fresult = f_read (&fil, buffer, f_size(&fil), &br);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			free(buffer);
		 	sprintf (buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
		  	Send_Uart(buffer);
		  	free(buf);
		}

		else
		{
			Send_Uart(buffer);
			free(buffer);

			/* Close file */
			fresult = f_close(&fil);
			if (fresult != FR_OK)
			{
				char *buf = malloc(100*sizeof(char));
				sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
				Send_Uart(buf);
				free(buf);
			}
			else
			{
				char *buf = malloc(100*sizeof(char));
				sprintf (buf, "File *%s* CLOSED successfully\n", name);
				Send_Uart(buf);
				free(buf);
			}
		}
	    return fresult;
	}
}

FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &fno);
	if (fresult == FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",name);
		Send_Uart(buf);
		free(buf);
	    return fresult;
	}
	else
	{
		fresult = f_open(&fil, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult, name);
			Send_Uart(buf);
			free(buf);
		    return fresult;
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
			Send_Uart(buf);
			free(buf);
		}

		fresult = f_close(&fil);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult, name);
			Send_Uart(buf);
			free(buf);
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n", name);
			Send_Uart(buf);
			free(buf);
		}
	}
    return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
	    return fresult;
	}

	else
	{
		 /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_APPEND | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	        free(buf);
	        return fresult;
	    }

	    /* Writing text */
	    fresult = f_write(&fil, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	    	free(buf);
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "*%s* UPDATED successfully\n", name);
	    	Send_Uart(buf);
	    	free(buf);
	    }

	    /* Close file */
	    fresult = f_close(&fil);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	    	free(buf);
	    }
	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "File *%s* CLOSED successfully\n", name);
	    	Send_Uart(buf);
	    	free(buf);
	     }
	}
    return fresult;
}

FRESULT Remove_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
		return fresult;
	}

	else
	{
		fresult = f_unlink (name);
		if (fresult == FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* has been removed successfully\n", name);
			Send_Uart (buf);
			free(buf);
		}

		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in removing *%s*\n\n",fresult, name);
			Send_Uart (buf);
			free(buf);
		}
	}
	return fresult;
}

FRESULT Create_Dir (char *name)
{
    fresult = f_mkdir(name);
    if (fresult == FR_OK)
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "*%s* has been created successfully\n", name);
    	Send_Uart (buf);
    	free(buf);
    }
    else
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "ERROR No. %d in creating directory *%s*\n\n", fresult,name);
    	Send_Uart(buf);
    	free(buf);
    }
    return fresult;
}

void Check_SD_Space (void)
{
    /* Check free space */
    f_getfree("/", &fre_clust, &pfs);

    total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    //char *buf = malloc(30*sizeof(char));
    //sprintf (buf, "SD CARD Total Size: \t%lu\n",total);
    //Send_Uart(buf);
    //free(buf);
    printf("SD CARD Total Size: \t%lu\n", total);
    free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
//    buf = malloc(30*sizeof(char));
//    sprintf (buf, "SD CARD Free Space: \t%lu\n",free_space);
//    Send_Uart(buf);
//    free(buf);
    printf("SD CARD Free Space: \t%lu\n", free_space);
}

void Get_File_Info(char *name)
{
	fresult = f_stat(name, &fno);
	switch (fresult)
	{
	case FR_OK:

		printf("Below are the details of the *%s* \nSize: %lu\n", name, fno.fsize);
		// send_uart(buffer);
		printf("Timestamp: %u/%02u/%02u, %02u:%02u\n",
			   (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
			   fno.ftime >> 11, fno.ftime >> 5 & 63);
		// end_uart(buffer);
		printf("Attributes: %c%c%c%c%c\n",
			   (fno.fattrib & AM_DIR) ? 'D' : '-',
			   (fno.fattrib & AM_RDO) ? 'R' : '-',
			   (fno.fattrib & AM_HID) ? 'H' : '-',
			   (fno.fattrib & AM_SYS) ? 'S' : '-',
			   (fno.fattrib & AM_ARC) ? 'A' : '-');
		// send_uart(buffer);
		break;

	case FR_NO_FILE:
		printf("*%s* does not exist.\n", name);
		// send_uart(buffer);
		break;

	default:
		printf("An error occurred. (%d)\n", fresult);
		// send_uart(buffer);
	}
}
