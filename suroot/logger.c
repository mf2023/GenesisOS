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

#include "logger.h"

const char* log_init_kernel = "CREATE TABLE IF NOT EXISTS kernel ("
                              "log TEXT NOT NULL"
                              ");";
const char* log_init_system = "CREATE TABLE IF NOT EXISTS system ("
                              "name TEXT NOT NULL,"
                              "log TEXT NOT NULL"
                              ");";
const char* log_init_app = "CREATE TABLE IF NOT EXISTS app ("
                           "name TEXT NOT NULL,"
                           "log TEXT NOT NULL"
                           ");";
const char* kernel_log_add_i = "INSERT INTO kernel (log) VALUES (?);";
const char* system_log_add_i = "INSERT INTO system (name,log) VALUES (?,?);";
const char* check_kernel_log_i = "SELECT EXISTS(SELECT 1 FROM kernel WHERE log = ? LIMIT 1);";

int log_init() {
    sqlite3 *db;
    char *errMsg;
    char msg[100];
    
    if (sqlite3_open(DB, &db) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        Error(app, msg);
    }

    if (sqlite3_exec(db, log_init_kernel, NULL, NULL, &errMsg) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        Error(app, msg);
    }

    if (sqlite3_exec(db, log_init_system, NULL, NULL, &errMsg) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        Error(app, msg);
    }

    if (sqlite3_exec(db, log_init_app, NULL, NULL, &errMsg) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        Error(app, msg);
    }

    sqlite3_close(db);
    return DONE;
}

int ker_log() {
    sqlite3 *db;
    sqlite3_stmt *stmt_check, *stmt_add;
    FILE *fp;
    char log[1024];
    const char *tail = "dmesg";
    const char *db_path = DB;

    if (sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        Error("ker_log", "Database open error");
    }

    if (sqlite3_prepare_v2(db, check_kernel_log_i, -1, &stmt_check, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        Error("ker_log", "Prepare check statement error");
    }

    if (sqlite3_prepare_v2(db, kernel_log_add_i, -1, &stmt_add, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt_check);
        sqlite3_close(db);
        Error("ker_log", "Prepare insert statement error");
    }

    fp = popen(tail, "r");
    if (fp == NULL) {
        sqlite3_finalize(stmt_check);
        sqlite3_finalize(stmt_add);
        sqlite3_close(db);
        Error("ker_log", "Failed to execute popen");
    }

    while (fgets(log, sizeof(log), fp) != NULL) {
        log[strcspn(log, "\n")] = 0;

        sqlite3_bind_text(stmt_check, 1, log, -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt_check) != SQLITE_ROW) {
            if (sqlite3_errcode(db) != SQLITE_DONE) {
                sqlite3_reset(stmt_check);
                sqlite3_finalize(stmt_add);
                pclose(fp);
                sqlite3_close(db);
                Error("ker_log", "Check query error");
            }
            sqlite3_reset(stmt_check);

            // 绑定新日志到添加语句
            sqlite3_bind_text(stmt_add, 1, log, -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt_add) != SQLITE_DONE) {
                sqlite3_finalize(stmt_check);
                sqlite3_finalize(stmt_add);
                pclose(fp);
                sqlite3_close(db);
                Error("ker_log", "Insert error");
            }
            sqlite3_reset(stmt_add);
        }
    }

    if (pclose(fp) == -1) {
        sqlite3_finalize(stmt_check);
        sqlite3_finalize(stmt_add);
        sqlite3_close(db);
        Error("ker_log", "Failed to close popen stream");
    }

    sqlite3_finalize(stmt_check);
    sqlite3_finalize(stmt_add);
    sqlite3_close(db);
    return DONE;
}


int sys_log(const char* name,const char* value){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char msg[100];

    if (sqlite3_open_v2(DB, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        return WRONG;
    }

    if (sqlite3_prepare_v2(db, system_log_add_i, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        sqlite3_close(db);
        Error(app,msg);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2,value, -1, SQLITE_TRANSIENT);

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



int main(){
    sys_log("test","This is a test log!");
    return 0;
}
