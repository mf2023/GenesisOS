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

#ifndef GENESISOS_PHYSICS_H
#define GENESISOS_PHYSICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <GError.h>
#include <usr_db.h>

#define HOME "/home"

int add(const char* name);
int del(const char* name, const char* password);
int del_root(const char* name);
int rename_home(const char* name, const char* new_name);

#endif //GENESISOS_PHYSICS_H
