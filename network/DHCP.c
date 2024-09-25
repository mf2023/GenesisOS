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

#include "DHCP.h"

void fill_dhcp_header(struct dhcp_header *dhcp) {
    memset(dhcp, 0, sizeof(struct dhcp_header)); // 清空结构体

    dhcp->op = 1; // 请求
    dhcp->htype = 1; // Ethernet
    dhcp->hlen = 6; // MAC 地址长度
    dhcp->hops = 0; // 跳数
    dhcp->xid = htonl(0x12345678); // 事务 ID
    dhcp->secs = 0; // 已经过的时间
    dhcp->flags = 0; // 标志
    dhcp->ciaddr = 0; // 客户端 IP 地址
    dhcp->yiaddr = 0; // 服务器分配的 IP 地址
    dhcp->siaddr = 0; // 服务器 IP 地址
    dhcp->giaddr = 0; // 中继代理 IP 地址

    // 填充客户端硬件地址（假设为 00:11:22:33:44:55）
    uint8_t mac_address[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    memcpy(dhcp->chaddr, mac_address, 6);

    // 选项字段（例如，DHCP Discover 消息）
    dhcp->options[0] = 53; // DHCP消息类型
    dhcp->options[1] = 1;  // 长度
    dhcp->options[2] = 1;  // DHCP Discover
    dhcp->options[3] = 255; // 结束
}