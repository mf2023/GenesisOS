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

#ifndef GENESISOS_LOGGER_H
#define GENESISOS_LOGGER_H

#include <code.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GError.h>
#include <sqlite3.h>

#define BATCH_SIZE 10
#define DB "./log.db"

const char* app = "suroot";
extern const char* log_init_app;
extern const char* log_init_kernel;
extern const char* log_init_system;
extern const char* system_log_add_i;

int log_init();
int sys_log(const char* name,const char* value);

#endif //GENESISOS_LOGGER_H
