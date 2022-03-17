/*
 * zerg_sdio.h
 *
 *  Created on: Jan 28, 2022
 *      Author: Denis
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
int zerg_get_data_from_sd(void);
void zerg_write_data_to_sd(int); // !!!!!!!!!!!!!!!!!!
//void print_structure(struct table T);
FRESULT zerg_scan_files(char *pat);
void zerg_delete();
void zerg_creat_folder(char *folder_name);
void zerg_get_free_space();
void zerg_get_file_info();
void zerg_ummount();

#endif /* INC_ZERG_SDIO_H_ */
