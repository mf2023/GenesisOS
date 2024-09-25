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

#ifndef GENESISOS_USR_API_H
#define GENESISOS_USR_API_H

#include <code.h>
#include <usr_db.h>
#include <GError.h>
#include <physics.h>

int del_usr(const char* name,const char* password);
int lock_usr(const char* name,const char* password);
int unlock_usr(const char* name,const char* password);
int add_usr(const char* name,const char* password,int uid,const char* home,const char* shell,const char* things);

#endif //GENESISOS_USR_API_H
