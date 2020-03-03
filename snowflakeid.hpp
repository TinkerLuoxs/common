// 分布式自增ID算法SnowFlake，整体上按照时间自增排序，并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分)，并且效率较高。
#pragma once

#include <chrono>

/*
* SnowFlake的结构说明:
* 0 - 0000000000 0000000000 0000000000 0000000000 0 - 00000 - 00000 - 000000000000
* 1位整型符号位，许多数据库中不能存储unsigned型，故置为0
* 41位时间截(毫秒级)，存储时间截的差值（当前时间截 - 开始时间截)，可以使用69年，年T = (1L << 41) / (1000L * 60 * 60 * 24 * 365) = 69
* 10位的数据机器位，可以部署在1024个节点，包括5位datacenterID和5位machineID
* 12位序列，毫秒内的计数，12位的计数顺序号支持每个节点每毫秒(同一机器，同一时间截)产生4096个ID序号
*/

class snowflake
{
public:
    long long id_generate(unsigned char machine_id, unsigned char datacenter_id)
    {
        if (machine_id > 0x1f) // 机器ID（0-31）
            machine_id &= 0x1f;
        if (datacenter_id > 0x1f) // 数据中心ID（0-31）
            datacenter_id &= 0x1f;

        auto timestamp = current_timestamp();
        // 如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过
        if (timestamp < last_timestamp_)
            return 0;

        // 如果是同一时间生成的，则使用毫秒序列
        if (timestamp == last_timestamp_) {
            milliseconds_seq_ = (milliseconds_seq_ + 1) & 0xfff; //生成序列的掩码，这里为4095
            // 序列溢出，则阻塞到下一个毫秒
            if (milliseconds_seq_ == 0) {
                timestamp = current_timestamp();
                while (timestamp <= last_timestamp_)
                    timestamp = current_timestamp();
            }
        }
        else
            milliseconds_seq_ = 0; //时间戳改变，毫秒序列重置

        last_timestamp_ = timestamp;

        return ((timestamp - start_timestamp_) << 22)
            | (datacenter_id << 17)
            | (machine_id << 12)
            | milliseconds_seq_;
    }

private:
    long long current_timestamp() const 
    {
        const auto time_now = std::chrono::system_clock::now();
        const auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
        return duration_in_ms.count();
    }

    long long last_timestamp_ = 0;  //上一次生成的时间戳
    long long start_timestamp_ = 0; //开始时间戳，默认为0从1970年1月1日算起
    unsigned short milliseconds_seq_ = 0; // 毫秒序列（0-4095）
};