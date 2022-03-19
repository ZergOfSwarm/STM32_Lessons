/*
 * zerg_sdio.h
 *
 *  Created on: Mar 19, 2022
 *      Author: denis
 */

#include <string.h>
#include <stdio.h>
#include "fatfs.h"

#ifndef INC_ZERG_SDIO_H_
#define INC_ZERG_SDIO_H_

void zerg_mount();
void zerg_format();
void zerg_open();
void zerg_add_data();
void zerg_write();
void zerg_read();
FRESULT zerg_scan_files(char *pat);
void zerg_delete();
void zerg_creat_folder(char *folder_name);
void zerg_get_free_space();
void zerg_get_file_info();
void zerg_ummount();

#endif /* INC_ZERG_SDIO_H_ */
