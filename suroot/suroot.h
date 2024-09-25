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

#ifndef GENESISOS_SUROOT_H
#define GENESISOS_SUROOT_H

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/syscall.h>

#define SYS_cgroup 317
#define SUROOT_COUNT 56
#define SUSCEPTIBLE_COUNT 27
#define STACK_SIZE (1024 * 1024)
#define CGROUP2_SUPER_MAGIC 0x63677270

typedef enum {
    SYS_REBOOT                  = (1 << 0),   // 重启系统
    SYS_SHUTDOWN                = (1 << 1),   // 关闭系统
    SYS_MODIFY_CONFIG           = (1 << 2),   // 修改系统配置
    USER_CREATE                 = (1 << 3),   // 创建用户账户
    USER_MODIFY                 = (1 << 4),   // 修改用户账户
    USER_DELETE                 = (1 << 5),   // 删除用户账户
    USER_GROUP_MANAGE           = (1 << 6),   // 管理用户组和用户权限
    FS_READ                     = (1 << 7),   // 读取文件系统
    FS_WRITE                    = (1 << 8),   // 写入文件系统
    FS_EXECUTE                  = (1 << 9),   // 执行文件系统内的程序
    FS_MOUNT                    = (1 << 10),  // 挂载文件系统
    FS_UNMOUNT                  = (1 << 11),  // 卸载文件系统
    NET_CONFIG                  = (1 << 12),  // 配置网络设备
    NET_ADMINISTER              = (1 << 13),  // 管理网络服务
    NET_RAW                     = (1 << 14),  // 使用原始套接字
    NET_BIND_SERVICE            = (1 << 15),  // 绑定到低编号的网络服务
    PROC_VIEW                   = (1 << 16),  // 查看进程信息
    PROC_SIGNAL                 = (1 << 17),  // 向进程发送信号
    PROC_KILL                   = (1 << 18),  // 结束进程
    HW_ACCESS                   = (1 << 19),  // 访问硬件设备
    HW_MANAGE                   = (1 << 20),  // 管理硬件设备
    SECURITY_POLICY_READ        = (1 << 21),  // 读取安全策略
    SECURITY_POLICY_WRITE       = (1 << 22),  // 修改安全策略
    AUDIT_LOG_READ              = (1 << 23),  // 读取审计日志
    AUDIT_LOG_WRITE             = (1 << 24),  // 写入审计日志
    RESOURCE_MONITOR            = (1 << 25),  // 监控资源使用情况
    RESOURCE_ALLOCATE           = (1 << 26),  // 分配系统资源
    SERVICE_START               = (1 << 27),  // 启动服务
    SERVICE_STOP                = (1 << 28),  // 停止服务
    SERVICE_ENABLE              = (1 << 29),  // 启用服务自启动
    SERVICE_DISABLE             = (1 << 30),  // 禁用服务自启动
    SOFTWARE_INSTALL            = (1 << 31),  // 安装软件
    SOFTWARE_UPDATE             = (1UL << 32),  // 更新软件
    SOFTWARE_UNINSTALL          = (1UL << 33),  // 卸载软件
    PERMISSION_GRANT            = (1UL << 34),  // 授予权限
    PERMISSION_REVOKE           = (1UL << 35),  // 撤销权限
    SYS_MONITOR                 = (1UL << 36),  // 监控系统状态
    SYS_MAINTENANCE             = (1UL << 37),  // 执行系统维护任务
    CONTAINER_CREATE            = (1UL << 38),  // 创建容器
    CONTAINER_DELETE            = (1UL << 39),  // 删除容器
    CONTAINER_RUN               = (1UL << 40),  // 运行容器
    CONTAINER_MANAGE            = (1UL << 41),  // 管理容器配置
    LOG_VIEW                    = (1UL << 42),  // 查看日志
    LOG_MANAGE                  = (1UL << 43),  // 管理日志
    SYSTEM_UPDATE               = (1UL << 44),  // 更新系统
    BACKUP_CREATE               = (1UL << 45),  // 创建备份
    BACKUP_RESTORE              = (1UL << 46),  // 恢复备份
    HW_DRIVER_LOAD              = (1UL << 47),  // 加载硬件驱动程序
    HW_DRIVER_UNLOAD            = (1UL << 48),  // 卸载硬件驱动程序
    ENV_VAR_READ                = (1UL << 49),  // 读取环境变量
    ENV_VAR_WRITE               = (1UL << 50),  // 设置环境变量
    EMERGENCY_ACCESS            = (1UL << 51),  // 紧急访问系统
    DEBUG_ATTACH                = (1UL << 52),  // 附加到进程进行调试
    DEBUG_DETACH                = (1UL << 53),  // 从进程分离调试器
    SYSTEM_CONFIG_READ          = (1UL << 54),  // 读取系统配置文件权限
    SYSTEM_CONFIG_WRITE         = (1UL << 55),  // 写入系统配置文件权限
    HIDDEN_FILES_ACCESS         = (1UL << 56),  // 访问不可见文件权限

    PERMISSION_COUNT = 100
} Suroot;

typedef enum {
    PHONE_CALL                  = (1 << 0),  // 拨打电话权限
    PHONE_READ                  = (1 << 1),  // 读取电话记录权限
    PHONE_WRITE                 = (1 << 2),  // 写入电话记录权限
    CONTACTS_DELETE             = (1 << 3),  // 删除联系人权限
    CONTACTS_ACCESS             = (1 << 4),  // 访问联系人权限
    GALLERY_READ                = (1 << 5),  // 读取相册权限
    GALLERY_WRITE               = (1 << 6),  // 写入相册权限
    ADDRESS_READ                = (1 << 7),  // 读取地址权限
    ADDRESS_WRITE               = (1 << 8),  // 写入地址权限
    SENSOR_READ                 = (1 << 9),  // 读取传感器数据权限
    LOCATION_READ               = (1 << 10),  // 读取定位信息权限
    LOCATION_WRITE              = (1 << 11),  // 写入定位信息权限
    CAMERA_TAKE_PHOTO           = (1 << 12),  // 使用相机拍照权限
    CAMERA_RECORD_VIDEO         = (1 << 13),  // 使用相机录像权限
    FILE_ACCESS                 = (1 << 14),  // 访问文件权限
    NETWORK_ACCESS              = (1 << 15),  // 网络访问权限
    STORAGE_READ                = (1 << 16),  // 读取存储权限
    STORAGE_WRITE               = (1 << 17),  // 写入存储权限
    MICROPHONE_ACCESS           = (1 << 18),  // 访问麦克风权限
    NOTIFICATION_ACCESS         = (1 << 19),  // 发送通知权限
    INSTALL_PACKAGES            = (1 << 20),  // 安装软件包权限
    UNINSTALL_PACKAGES          = (1 << 21),  // 卸载软件包权限
    READ_SMS                    = (1 << 22),  // 读取短信权限
    SEND_SMS                    = (1 << 23),  // 发送短信权限
    READ_CALENDAR               = (1 << 24),  // 读取日历权限
    WRITE_CALENDAR              = (1 << 25),  // 写入日历权限
    ACCESS_FINE_LOCATION        = (1 << 26),  // 精确定位权限
    ACCESS_COARSE_LOCATION      = (1 << 27),  // 粗略定位权限

}Susceptible;

#endif //GENESISOS_SUROOT_H
