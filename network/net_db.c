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

#include "net_db.h"

const char *new_net_table_i =
        "CREATE TABLE devices (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 设备唯一标识符\n"
        "    name VARCHAR(100) NOT NULL,             -- 设备名称\n"
        "    ip VARCHAR(15) NOT NULL UNIQUE,         -- 设备的IP地址\n"
        "    mac VARCHAR(17) NOT NULL,                -- 设备的MAC地址\n"
        "    type VARCHAR(50),                        -- 设备类型（路由器、交换机等）\n"
        "    location VARCHAR(100),                   -- 设备物理位置\n"
        "    manufacturer VARCHAR(100),               -- 设备制造商\n"
        "    model VARCHAR(100),                      -- 设备型号\n"
        "    status VARCHAR(10) NOT NULL,             -- 设备状态（在线/离线等）\n"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP, -- 创建时间\n"
        "    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP -- 更新时间\n"
        ");\n\n"

        "CREATE TABLE dhcp (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- DHCP 记录唯一标识符\n"
        "    ip VARCHAR(15) NOT NULL UNIQUE,         -- DHCP 分配的IP地址\n"
        "    mac VARCHAR(17) NOT NULL,                -- 设备的MAC地址\n"
        "    lease_start DATETIME,                    -- 租约开始时间\n"
        "    lease_end DATETIME                       -- 租约结束时间\n"
        ");\n\n"

        "CREATE TABLE static_ips (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 静态IP记录唯一标识符\n"
        "    ip VARCHAR(15) NOT NULL UNIQUE,         -- 静态IP地址\n"
        "    mac VARCHAR(17) NOT NULL,                -- 设备的MAC地址\n"
        "    status VARCHAR(10) NOT NULL              -- 静态IP状态\n"
        ");\n\n"

        "CREATE TABLE status_history (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 状态历史记录唯一标识符\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    status VARCHAR(10) NOT NULL,            -- 设备状态\n"
        "    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, -- 状态记录时间\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n\n"

        "CREATE TABLE performance_metrics (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 性能指标记录唯一标识符\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    metric_name VARCHAR(50) NOT NULL,      -- 性能指标名称\n"
        "    metric_value FLOAT NOT NULL,            -- 性能指标值\n"
        "    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, -- 性能记录时间\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n\n"

        "CREATE TABLE maintenance_records (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 维护记录唯一标识符\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    maintenance_date DATETIME NOT NULL,     -- 维护日期\n"
        "    description TEXT,                       -- 维护内容描述\n"
        "    maintainer VARCHAR(100),                -- 维护人员\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n\n"

        "CREATE TABLE tags (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 标签唯一标识符\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    tag_name VARCHAR(50) NOT NULL,         -- 标签名称\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n\n"

        "CREATE TABLE device_configurations (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 配置记录唯一标识符\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    config_data TEXT NOT NULL,              -- 配置数据\n"
        "    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, -- 配置记录时间\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n\n"

        "CREATE TABLE event_logs (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 事件日志唯一标识符\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    event_type VARCHAR(50) NOT NULL,       -- 事件类型\n"
        "    event_description TEXT,                 -- 事件描述\n"
        "    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, -- 事件记录时间\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n\n"

        "CREATE TABLE user_access (\n"
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 用户访问记录唯一标识符\n"
        "    user_id INTEGER NOT NULL,               -- 用户ID\n"
        "    device_id INTEGER NOT NULL,             -- 关联的设备ID\n"
        "    access_level VARCHAR(20) NOT NULL,     -- 访问级别（如管理员、用户等）\n"
        "    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, -- 访问记录时间\n"
        "    FOREIGN KEY (device_id) REFERENCES devices(id)\n"
        ");\n";

int new_net_table() {
    sqlite3 *db;
    char msg[100];

    if (sqlite3_open(DB, &db) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", sqlite3_errmsg(db));
        Error(net_app,msg);
    }

    char *errMsg;
    if (sqlite3_exec(db, new_net_table_i, NULL, NULL, &errMsg) != SQLITE_OK) {
        snprintf(msg, sizeof(msg), "%s", errMsg);
        sqlite3_free(errMsg);
        Error(net_app,msg);
    }

    sqlite3_close(db);
    return DONE;
}

int main() {
    new_net_table();
    return 0;
}