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

#ifndef GENESISOS_GERROR_H
#define GENESISOS_GERROR_H

#include <code.h>
#include <stdio.h>
#include <stdlib.h>

int Error(const char* app, const char* value);
int Warning(const char* app,const char* value);

#endif //GENESISOS_GERROR_H
