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

#include "usr_sys_api.h"

char *api = "usrcontrol";

int add_usr(const char* name,const char* password,int uid,const char* shell,const char* things){
    char home[100];

    snprintf(home, sizeof(home), "/home/%s", name);
    if (check_usr_db(name, "") != UNEXIST){
        Error(api, "Target users to exist!");
    }

    if (add(name) != DONE){
        Error(api, "Failed to create directory!");
    }

    if (add_usr_db(name, password, uid, home, shell, things) != DONE){
        del_root(name);
        Error(api, "The target user cannot write to the database!");
    }
    return DONE;
}

int del_usr(const char* name,const char* password){
    if (check_usr_db(name, "") != UNEXIST){
        Error(api, "The user doesn't exist!");
    }

    if (lia_check(name) != UNEXIST){
        Error(api, "The user is logged in and can't do it!");
    }

    del_root(name);

    del_usr_db(name, password);

    return DONE;
}

int lock_usr(const char* name,const char* password){
    if (check_usr_db(name, "") != UNEXIST){
        Error(api, "The user doesn't exist!");
    }

    if (lia_check(name) != UNEXIST){
        Error(api, "The user is logged in and can't do it!");
    }

    lock_usr_db(name,password);

    return DONE;
}

int unlock_usr(const char* name,const char* password){
    if (check_usr_db(name, "") != UNEXIST){
        Error(api, "The user doesn't exist!");
    }

    if (lia_check(name) != UNEXIST){
        Error(api, "The user is logged in and can't do it!");
    }

    unlock_usr_db(name,password);

    return DONE;
}