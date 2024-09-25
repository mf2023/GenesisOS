#ifndef GENESISOS_DHCP_H
#define GENESISOS_DHCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
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

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DHCP_DISCOVER 1
#define DHCP_OFFER    2
#define DHCP_REQUEST  3
#define DHCP_ACK      4
#define DHCP_NAK      5
#define DHCP_RELEASE  6
#define DHCP_INFORM   7
#define DHCP_DECLINE  8
#define DHCP_MESSAGE_SIZE 512

const uint8_t dhcp_magic_cookie[] = {0x63,0x82,0x53,0x63};

struct dhcp_header {
    uint8_t op;       // 操作码
    uint8_t htype;    // 硬件类型
    uint8_t hlen;     // 硬件地址长度
    uint8_t hops;     // 跳数
    uint32_t xid;     // 事务 ID
    uint16_t secs;    // 已经过的时间
    uint16_t flags;   // 标志
    uint32_t ciaddr;  // 客户端 IP 地址
    uint32_t yiaddr;  // 服务器分配的 IP 地址
    uint32_t siaddr;  // 服务器 IP 地址
    uint32_t giaddr;  // 中继代理 IP 地址
    uint8_t chaddr[16]; // 客户端硬件地址
    uint8_t sname[64]; // 服务器主机名
    uint8_t file[128]; // 引导文件名
    uint8_t options[312]; // DHCP 选项
};
#endif //GENESISOS_DHCP_H
