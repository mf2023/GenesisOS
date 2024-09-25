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

#ifndef GENESISOS_SPACE_H
#define GENESISOS_SPACE_H

#define _GNU_SOURCE
#include <stdio.h>      // for perror, printf
#include <stdlib.h>     // for malloc, free, exit
#include <string.h>     // for memset
#include <unistd.h>     // for execvp
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>   // for waitpid
#include <sys/mount.h>  // for mount, MS_PRIVATE, MS_REC
#include <sched.h>      // for clone, unshare, CLONE_*
#include <sys/syscall.h> // for syscall

#define STACK_SIZE (1024 * 1024)

#endif //GENESISOS_SPACE_H
