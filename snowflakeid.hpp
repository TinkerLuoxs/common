// �ֲ�ʽ����ID�㷨SnowFlake�������ϰ���ʱ���������򣬲��������ֲ�ʽϵͳ�ڲ������ID��ײ(����������ID�ͻ���ID������)������Ч�ʽϸߡ�
#pragma once

#include <chrono>

/*
* SnowFlake�Ľṹ˵��:
* 0 - 0000000000 0000000000 0000000000 0000000000 0 - 00000 - 00000 - 000000000000
* 1λ���ͷ���λ��������ݿ��в��ܴ洢unsigned�ͣ�����Ϊ0
* 41λʱ���(���뼶)���洢ʱ��صĲ�ֵ����ǰʱ��� - ��ʼʱ���)������ʹ��69�꣬��T = (1L << 41) / (1000L * 60 * 60 * 24 * 365) = 69
* 10λ�����ݻ���λ�����Բ�����1024���ڵ㣬����5λdatacenterID��5λmachineID
* 12λ���У������ڵļ�����12λ�ļ���˳���֧��ÿ���ڵ�ÿ����(ͬһ������ͬһʱ���)����4096��ID���
*/

class snowflake
{
public:
    long long id_generate(unsigned char machine_id, unsigned char datacenter_id)
    {
        if (machine_id > 0x1f) // ����ID��0-31��
            machine_id &= 0x1f;
        if (datacenter_id > 0x1f) // ��������ID��0-31��
            datacenter_id &= 0x1f;

        auto timestamp = current_timestamp();
        // �����ǰʱ��С����һ��ID���ɵ�ʱ�����˵��ϵͳʱ�ӻ��˹�
        if (timestamp < last_timestamp_)
            return 0;

        // �����ͬһʱ�����ɵģ���ʹ�ú�������
        if (timestamp == last_timestamp_) {
            milliseconds_seq_ = (milliseconds_seq_ + 1) & 0xfff; //�������е����룬����Ϊ4095
            // �������������������һ������
            if (milliseconds_seq_ == 0) {
                timestamp = current_timestamp();
                while (timestamp <= last_timestamp_)
                    timestamp = current_timestamp();
            }
        }
        else
            milliseconds_seq_ = 0; //ʱ����ı䣬������������

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

    long long last_timestamp_ = 0;  //��һ�����ɵ�ʱ���
    long long start_timestamp_ = 0; //��ʼʱ�����Ĭ��Ϊ0��1970��1��1������
    unsigned short milliseconds_seq_ = 0; // �������У�0-4095��
};