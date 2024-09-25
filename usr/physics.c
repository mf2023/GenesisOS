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

#include "physics.h"
#include <errno.h>

char *appt = "usrcontrol";

int add(const char* name) {
    char dir_path[512];
    snprintf(dir_path, sizeof(dir_path), "/home/%s", name);

    if (mkdir(dir_path, 0700) == -1) {
        Error(appt,"Failed to create directory!");
    }

    const char* subdirs[] = { "Public", "Desktop", "Videos","Pictures","Docs","Downloads","Musics"};
    size_t num_subdirs = sizeof(subdirs) / sizeof(subdirs[0]);

    for (size_t i = 0; i < num_subdirs; i++) {
        char subdir_path[512];
        if (snprintf(subdir_path, sizeof(subdir_path), "%s/%s", dir_path, subdirs[i]) >= sizeof(subdir_path)) {
            Error(appt, "Subdirectory path truncated!");
        }

        if (mkdir(subdir_path, 0700) == -1) {
            Error(appt,"Failed to create directory!");
        }
    }

    return DONE;
}

int rename_home(const char* name, const char* new_name) {
    char dir_path[512];
    snprintf(dir_path, sizeof(dir_path), "/home/%s", name);

    char n_dir_path[512];
    snprintf(n_dir_path, sizeof(n_dir_path), "/home/%s", new_name);

    if (lia_check(name) != UNEXIST){
        Error(appt,"The user is logged in and can't do it!");
    }

    if (rename(dir_path, n_dir_path) == -1) {
        Error(appt,"User directory modification failed!");
    }
    return DONE;
}

int remove_dir(const char *dir) {
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[512];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    if (0 != access(dir, F_OK)) {
        return 0;
    }

    if (stat(dir, &dir_stat) < 0) {
        Error(appt,"Get directory stat error!");
    }

    if (S_ISREG(dir_stat.st_mode)) {
        remove(dir);
    } else if (S_ISDIR(dir_stat.st_mode)) {
        dirp = opendir(dir);
        if (dirp == NULL) {
            perror("opendir");
            return -1;
        }

        while ((dp = readdir(dirp)) != NULL) {
            if (strcmp(cur_dir, dp->d_name) == 0 || strcmp(up_dir, dp->d_name) == 0) {
                continue;
            }

            snprintf(dir_name, sizeof(dir_name), "%s/%s", dir, dp->d_name);
            remove_dir(dir_name);
        }
        closedir(dirp);

        rmdir(dir);
    } else {
        Error(appt,"unknown file type!");
    }

    return 0;
}

int del(const char* name, const char* password) {
    char dir_path[512];
    snprintf(dir_path, sizeof(dir_path), "/home/%s", name);

    if (lia_check(name) != UNEXIST) {
        Error(appt, "The user is logged in and can't do it!");
    }

    if (remove_dir(dir_path) != 0) {
        Error(appt, strerror(errno));
    }

    return DONE;
}

int del_root(const char* name) {
    char dir_path[512];
    snprintf(dir_path, sizeof(dir_path), "/home/%s", name);

    if (remove_dir(dir_path) != 0) {
        Error(appt, strerror(errno));
    }

    return DONE;
}