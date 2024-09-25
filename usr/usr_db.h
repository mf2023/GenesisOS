/*
 * Copyright (c) 2024 Wenze Wei
 *
 * This file is part of GenesisOS.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License, v. 2.0.
 * You can obtain a copy of the GPL at
 * https://www.gnu.org/licenses/old-licenses/gpl-2.0.html.
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
 * the specific language governing rights and limitations under the License.
 */

#ifndef GENESISOS_USR_DB_H
#define GENESISOS_USR_DB_H

#include <code.h>
#include <GError.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DB "./usr.db"

extern char *app;
extern const char* lia_add_i;
extern const char* lia_del_i;
extern const char* lia_check_i;
extern const char* del_usr_db_i;
extern const char* add_usr_db_i;
extern const char* lia_del_all_i;
extern const char* check_usr_db_i;
extern const char* new_usr_table_i;
extern const char* revise_name_db_i;
extern const char* get_usr_info_db_i;
extern const char* revise_password_db_i;
extern const char* check_usr_password_i;

int lia_del_all();
int new_usr_table();
int lia_del(const char* name);
int lia_check(const char* name);
int lock_usr_db(const char* name)
int unlock_usr_db(const char* name);
int lia_add(const char* name, const char* password);
int del_usr_db(const char*name,const char* password);
int check_usr_db(const char* name,const char* password);
int revise_password_db(const char* name,const char* password,const char* new_password);
int revise_name_db(const char* name,const char* password,const char* new_name,const char* home);
int get_usr_info_db(const char* name, const char* password, int *uid, char **home, char **shell);
int add_usr_db(const char* name,const char* password,int uid,const char* home,const char* shell,const char* things);

#endif //GENESISOS_USR_DB_H
