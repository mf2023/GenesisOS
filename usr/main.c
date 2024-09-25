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

#include "main.h"

const char* apt = "usrcontrol";
extern void free_string(char* s);
extern unsigned int uid_generate(void);
extern char* sha256_hash_c_string(const char* input);

int create(const char* name, const char* shell){
    int attempts = 0;
    char home[100];
    char things[512];
    char password_f[100];
    char password_s[100];
    int max_attempts = 3;
    unsigned int uid = uid_generate();


    snprintf(home, sizeof(home), "/home/%s", name);

    if (check_usr_db(name,"") != UNEXIST){
        Error(apt,"The user already exists!");
    }

    printf("You are creating an account:%s\n\n",name);

    printf("Please enter a password: ");
    scanf("%99s", password_f);

    while (getchar() != '\n');

    do {
        printf("Please enter the password again: ");
        scanf("%99s", password_s);

        if (strcmp(password_f, password_s) == 0) {
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
            attempts++;
        }

        while (getchar() != '\n');

    } while (attempts < max_attempts);

    if (attempts == max_attempts) {
        printf("\nYou have reached the maximum number of attempts. Access denied.\n");
        return 1;
    }

    printf("Please add a user note (within 500 words) [press ENTER to skip]:\n");

    if (fgets(things, sizeof(things), stdin) != NULL) {
        size_t len = strlen(things);
        if (len > 0 && things[len - 1] == '\n') {
            things[len - 1] = '\0';
        }

        if (strlen(things) > 0) {
        }
    }

    char* hashed = sha256_hash_c_string(password_s);
    if (shell == NULL){
        add_usr(name,hashed,uid,home,"bin/sh",things);
        Warning(apt,"Default Schell is being used: '/bin/sh'!");
    } else{
        add_usr(name,hashed,uid,home,shell,things);
    }

    printf("\nCreating user directory...\n");
    printf("Configuring database...\n");

    free(hashed);
    return 0;
}

int deld(const char* name){
    int attempts = 0;
    char password_f[100];
    char password_s[100];
    int max_attempts = 3;

    if (check_usr_db(name,"") != UNDONE){
        Error(apt,"The target user doesn't exist!");
    }

    printf("Please enter a password: ");
    scanf("%99s", password_f);

    while (getchar() != '\n');

    do {
        printf("Please enter the password again: ");
        scanf("%99s", password_s);

        if (strcmp(password_f, password_s) == 0) {
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
            attempts++;
        }

        while (getchar() != '\n');

    } while (attempts < max_attempts);

    if (attempts == max_attempts) {
        printf("\nYou have reached the maximum number of attempts. Access denied.\n");
        return 1;
    }

    char* hashed = sha256_hash_c_string(password_s);
    del(name, hashed);
    printf("Deleting user directories...\n");
    del_usr_db(name,hashed);
    printf("Configuring database...\n");

    return 0;
}

int revise_n(const char* name,const char* new_name){
    char home[100];
    int attempts = 0;
    char password_f[100];
    char password_s[100];
    int max_attempts = 3;

    snprintf(home, sizeof(home), "/home/%s", new_name);
    if (check_usr_db(name,"") != UNDONE){
        Error(apt,"The target user doesn't exist!");
    }

    printf("Please enter a password: ");
    scanf("%99s", password_f);

    while (getchar() != '\n');

    do {
        printf("Please enter the password again: ");
        scanf("%99s", password_s);

        if (strcmp(password_f, password_s) == 0) {
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
            attempts++;
        }

        while (getchar() != '\n');

    } while (attempts < max_attempts);

    if (attempts == max_attempts) {
        printf("\nYou have reached the maximum number of attempts. Access denied.\n");
        return 1;
    }

    char* hashed = sha256_hash_c_string(password_s);
    revise_name_db(name,hashed,new_name,home);
    printf("\nModifying the database...");
    rename_home(name,new_name);
    printf("\nModifying User Directory...\n");
    return 0;
}

int revise_p(const char* name,const char* new_password){
    int attempts = 0;
    char password_f[100];
    char password_s[100];
    int max_attempts = 3;

    if (check_usr_db(name,"") != UNDONE){
        Error(apt,"The target user doesn't exist!");
    }

    printf("Please enter your original password: ");
    scanf("%99s", password_f);

    while (getchar() != '\n');

    do {
        printf("Please enter your original password again: ");
        scanf("%99s", password_s);

        if (strcmp(password_f, password_s) == 0) {
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
            attempts++;
        }

        while (getchar() != '\n');

    } while (attempts < max_attempts);

    if (attempts == max_attempts) {
        printf("\nYou have reached the maximum number of attempts. Access denied.\n");
        return 1;
    }

    char* hashed = sha256_hash_c_string(password_s);
    char* nhashed = sha256_hash_c_string(new_password);

    revise_password_db(name,hashed,nhashed);

    printf("\nModifying the database...");
    return 0;
}

int main(int argc, char *argv[]) {
    int index = 0;
    int a_option = UNEXIST;
    int r_option = UNEXIST;
    int p_option = UNEXIST;
    const char* a_arg = NULL;
    const char* s_arg = NULL;
    const char* r_arg = NULL;
    const char* n_arg = NULL;
    const char* p_arg = NULL;

    struct option options[] = {
            {"i", no_argument, NULL, 'i'},
            {"a", required_argument, NULL, 'a'},
            {"s", required_argument, NULL, 's'},
            {"d", required_argument, NULL, 'd'},
            {"r", required_argument, NULL, 'r'},
            {"n", required_argument, NULL, 'n'},
            {"p", required_argument, NULL, 'p'},
            {"L", required_argument, NULL, 'L'},
            {0, 0, 0, 0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "ia:s:d:r:n:p:L:", options, &index)) != -1) {
        switch (c) {
            case 'i':
                new_usr_table();
                break;
            case 'a':
                a_option = EXIST;
                a_arg = optarg;
                break;
            case 's':
                s_arg = optarg;
                break;
            case 'd':
                deld(optarg);
                break;
            case 'r':
                r_option = EXIST;
                r_arg = optarg;
                break;
            case 'n':
                n_arg = optarg;
                break;
            case 'p':
                p_option = EXIST;
                p_arg = optarg;
                break;
            case 'L':

                break;
            default:
                break;
        }
    }

    if (a_option == EXIST) {
        create(a_arg, s_arg);
        a_arg = NULL;
        s_arg = NULL;
    }

    if (r_option == EXIST) {
        if (n_arg != NULL) {
            revise_n(r_arg, n_arg);
            return 0;
        }
        if (p_option == EXIST) {
            revise_p(r_arg, p_arg);
            return 0;
        }
        Error("usrcontrol(Error)", "When [-r] is used, at least one subparameter [-c] [-n] [-L] or [-p] is required!");
    }

    return 0;
}