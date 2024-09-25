# Copyright (c) 2024 Wenze Wei
#
# This file is part of GenesisOS.
#
# This Source Code Form is subject to the terms of the GNU General Public License, v. 2.0.
# You can obtain a copy of the GPL at
# https://www.gnu.org/licenses/old-licenses/gpl-2.0.html.
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
# the specific language governing rights and limitations under the License.

CC = gcc
CFLAGS = -Wall -I./ -I./../sys_api
LDFLAGS = -lsqlite3

SRCS = logger.c ../sys_api/GError.c ../sys_api/GError.c
OBJS = $(SRCS:.c=.o)

TARGET = suroot

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean