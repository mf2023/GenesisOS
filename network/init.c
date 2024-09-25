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

#include "init.h"

typedef struct {
    char name[IFNAMSIZ];
} NetworkInterface;


// 初始化所有网络接口
void init_network() {
    int sockfd;
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    NetworkInterface interfaces[MAX_INTERFACES];
    int interface_count = 0;

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // 获取网络接口信息
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;

    if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
        perror("ioctl");
        close(sockfd);
        return;
    }

    interface_count = ifc.ifc_len / sizeof(struct ifreq);
    for (int i = 0; i < interface_count; i++) {
        strncpy(interfaces[i].name, ifc.ifc_req[i].ifr_name, IFNAMSIZ);
        printf("Found interface: %s\n", interfaces[i].name);

        // 启用接口
        strncpy(ifr.ifr_name, interfaces[i].name, IFNAMSIZ - 1);
        if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
            perror("ioctl");
            continue; // 继续下一个接口
        }

        ifr.ifr_flags |= IFF_UP; // 设置为 UP
        if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
            perror("ioctl");
            continue; // 继续下一个接口
        }

        printf("Interface %s is now UP\n", interfaces[i].name);
    }
    close(sockfd);
}
