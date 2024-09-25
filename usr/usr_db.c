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

#include <usr_db.h>

const char* new_usr_table_i = "CREATE TABLE usr ("
                              "usr_name     CHAR(50)    NOT NULL,"
                              "password     CHAR(50)    NOT NULL,"
                              "uid          INT         NOT NULL,"
                              "home         CHAR(50)    NOT NULL,"
                              "shell        CHAR(50)    NOT NULL,"
                              "things       CHAR(200)   NOT NULL,"
                              "account      CHAR(100),"
                              "account_s    CHAR(3)     NOT NULL,"
                              "state        INT         NOT NULL "
                              ");"
                              "CREATE TABLE lia ("
                              "usr_name     CHAR(50)    NOT NULL,"
                              "uid          INT         NOT NULL,"
                              "home         CHAR(50)    NOT NULL,"
                              "shell        CHAR(50)    NOT NULL"
                              ");";
const char* lia_del_all_i = "DELETE FROM lia;";
const char* lia_del_i = "DELETE FROM lia WHERE usr_name = ?;";
const char* del_usr_db_i = "DELETE FROM usr WHERE usr_name = ?;";
const char* lia_check_i = "SELECT 1 FROM lia WHERE usr_name = ? LIMIT 1;";
const char* lock_usr_db_i = "UPDATE usr SET state = ? WHERE usr_name = ?;";
const char* unlock_usr_db_i = "UPDATE usr SET state = ? WHERE usr_name = ?;";
const char* check_usr_db_i = "SELECT 1 FROM usr WHERE usr_name = ? LIMIT 1;";
const char* check_usr_password_i = "SELECT password FROM usr WHERE usr_name = ?;";
const char* lia_add_i = "INSERT INTO lia (usr_name,uid,home,shell) VALUES (?,?,?,?);";
const char* get_usr_info_db_i = "SELECT uid, home, shell FROM usr WHERE usr_name = ?;";
const char* revise_name_db_i = "UPDATE usr SET usr_name = ?, home = ? WHERE usr_name = ?;";
const char* revise_password_db_i = "UPDATE usr SET password = ? WHERE usr_name = ?;";
const char* add_usr_db_i = "INSERT INTO usr (usr_name,password,uid,home,shell,things,account,account_s) VALUES (?,?,?,?,?,?,?,?);";

char *app = "usrcontrol";


int new_usr_table() {
    sqlite3 *db;
    char msg[100];

    if (sqlite3_open(DB, &db) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        Error(app,msg);
    }

    char *errMsg;
    if (sqlite3_exec(db, new_usr_table_i, NULL, NULL, &errMsg) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", errMsg);
        sqlite3_free(errMsg);
        Error(app,msg);
    }

    sqlite3_close(db);
    return DONE;
}

int add_usr_db(const char* name, const char* password, int uid, const char* home, const char* shell, const char* things) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database, please check the system integrity or initialize the database with 'usrcontrol -i'!");
    }

    if (sqlite3_prepare_v2(db, add_usr_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    int result = check_usr_db(name, "");
    if (result == EXIST || result == UNDONE || result == DONE) {
        sqlite3_close(db);
        Error(app, "The user already exists!");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, uid);
    sqlite3_bind_text(stmt, 4, home, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, shell, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, things, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, "", -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, "NO", -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 9, UNLOCK);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return DONE;
}

int del_usr_db(const char*name){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_prepare_v2(db, del_usr_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (check_usr_db(name,"") != UNDONE){
        sqlite3_close(db);
        Error(app,"The target user doesn't exist!");
    }

    if (lia_check(name) == EXIST){
        sqlite3_close(db);
        Error(app,"The user is logged in and can't do it!");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int lock_usr_db(const char* name){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_prepare_v2(db, lock_usr_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (check_usr_db(name,"") != UNDONE){
        sqlite3_close(db);
        Error(app,"The target user doesn't exist!");
    }

    sqlite3_bind_int(stmt,1,LOCK);

    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int unlock_usr_db(const char* name){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_prepare_v2(db, unlock_usr_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (check_usr_db(name,"") != UNDONE){
        sqlite3_close(db);
        Error(app,"The target user doesn't exist!");
    }

    sqlite3_bind_int(stmt,1,UNLOCK);

    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int revise_name_db(const char* name,const char* password,const char* new_name,const char* home){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database, please check the system integrity or initialize the database with 'usrcontrol -i'!");
    }

    if (sqlite3_prepare_v2(db, revise_name_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "Error preparing statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (check_usr_db(name,"") != UNDONE){
        sqlite3_close(db);
        Error(app,"The target user doesn't exist!");
    }

    if (lia_check(name) == EXIST){
        sqlite3_close(db);
        Error(app,"The user is logged in and can't do it!");
    }

    sqlite3_bind_text(stmt, 1, new_name,  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, home, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, name, -1, SQLITE_TRANSIENT);

    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int revise_password_db(const char* name,const char* password,const char* new_password){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database, please check the system integrity or initialize the database with 'usrcontrol -i'!");
    }

    if (sqlite3_prepare_v2(db, revise_password_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "Error preparing statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (check_usr_db(name,"") != UNDONE){
        sqlite3_close(db);
        Error(app,"The target user doesn't exist!");
    }

    if (check_usr_db(name,password) != DONE){
        sqlite3_close(db);
        Error(app,"The original password was entered incorrectly!");
    }

    if (lia_check(name) == EXIST){
        sqlite3_close(db);
        Error(app,"The user is logged in and can't do it!");
    }

    sqlite3_bind_text(stmt, 1, new_password,  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);

    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int check_usr_db(const char* name, const char* password) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database, please check the system integrity or initialize the database with 'usrcontrol -i'!");
    }

    if (sqlite3_prepare_v2(db, check_usr_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "Error preparing statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        return UNEXIST;
    }

    sqlite3_finalize(stmt);

    if (sqlite3_prepare_v2(db, check_usr_password_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "Error preparing statement for password check: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_bind_text(stmt, 1, password, -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        state = DONE;
    } else {
        state = UNDONE;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int get_usr_info_db(const char* name, const char* password, int *uid, char **home, char **shell) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_prepare_v2(db, get_usr_info_db_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (check_usr_db(name, password) != DONE){
       return UNDONE;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *uid = sqlite3_column_int(stmt, 0);
        *home = strdup((const char *)sqlite3_column_text(stmt, 1));
        *shell = strdup((const char *)sqlite3_column_text(stmt, 2));
        state = DONE;
    } else {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return UNDONE;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int lia_check(const char* name){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNEXIST;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database, please check the system integrity or initialize the database with 'usrcontrol -i'!");
    }

    if (sqlite3_prepare_v2(db, lia_check_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "Error preparing statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        state = UNEXIST;
    } else {
        state = EXIST;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int lia_add(const char* name, const char* password){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int uid = 0;
    char *home = NULL;
    char *shell = NULL;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_prepare_v2(db, lia_add_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (get_usr_info_db(name, password, &uid, &home, &shell) != DONE) {
        return UNDONE;
    }

    if (lia_check(name) == EXIST){
        sqlite3_close(db);
        Error(app,"The user is logged in and can't do it!");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, uid);
    sqlite3_bind_text(stmt, 3, home, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, shell, -1, SQLITE_TRANSIENT);
    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    free(home);
    free(shell);
    return state;
}

int lia_del(const char* name){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];
    int state = UNDONE;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_prepare_v2(db, lia_del_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    if (lia_check(name) != EXIST){
        sqlite3_close(db);
        Error(app,"The user is not logged in!");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    state = DONE;

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return state;
}

int lia_del_all(){
    sqlite3 *db;
    char msg[100];
    char *errMsg = 0;

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error(app,"Unable to open the database for reading user info!");
    }

    if (sqlite3_exec(db, lia_del_all_i, 0, 0, &errMsg) != SQLITE_OK){
        snprintf(msg, sizeof(msg), "%s", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        Error(app,msg);
    }
    sqlite3_close(db);
    return DONE;
}
