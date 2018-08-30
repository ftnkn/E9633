#ifndef __UNI_API_H__
#define __UNI_API_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;

enum UNI_API_REG_ITEM {
    ACC             = 0x0001,   // ACC
    COLLIDE         = 0x0002,   // 碰撞
    ROLLOVER        = 0x0004,   // 侧翻
    DATA_TRANSFER   = 0x0008,   // 数据透传
    CAN_READ        = 0x0010,   // CAN
    OBD_READ        = 0x0020,   // OBD
    LTE_DIAL_STATE  = 0x0040,   // LTE拨号状态
    IO_INPUT        = 0x0080,   // IO INPUT

    RAPIDLY_SPEED   = 0x0400,   // 急加速/急减速
    SHARP_CURVE     = 0x0800,   // 急转弯
    PULL_PLUG       = 0x1000,   // 拔插
    ALARM           = 0x2000,   // 报警
};

typedef void(*UNI_API_CallbackFunc)(int type, const byte* pBuf, int length);

/*
 * FullName:	注册消息接收函数和过滤项
 * @param [in]: func : 消息接收回调函数, NULL-取消回调
 * @param [in]: filter : 过滤项，API_REG_ITEM里各项的异或值
 * @return:		1-成功 0-失败
 */
int UNI_API_SetCallback(UNI_API_CallbackFunc func, int filter);

/*
 * FullName:	获取build.prop值
 * @param [out]: buf : 保存读到的value
 * @param [in]: length : buf的大小
 * @param [in]: key : key
 * @return:		读到的长度
 */
int UNI_API_GetProperty(char* buf, int length, const char* key);

/*
 * FullName:	获取MCU软件版本号
 * @param [out]: buf : 保存版本号字符串
 * @param [in]: length : buf的大小
 * @return:		读到的长度
 */
int UNI_API_GetMcuVersion(char* buf, int length);

/*
 * FullName:	获取APP版本号
 * @param [out]: buf : 保存版本号字符串
 * @param [in]: length : buf的大小
 * @return:		读到的长度
 */
int UNI_API_GetAppVersion(char* buf, int length);

/*
 * FullName:	获取SN号
 * @param [out]: buf : 保存SN号字符串
 * @param [in]: length : buf的大小
 * @return:		读到的长度
 */
int UNI_API_GetSN(char* buf, int length);

/*
 * FullName:	写入SN号
 * @param [in]: sn : SN号字符串
 * @return:		1-写入成功 0-写入失败
 */
int UNI_API_SetSN(const char* sn);

/*
 * FullName:	获取ACC状态
 * @return:		1-ACC ON, 0-ACC OFF, -1-UNKNOWN
 */
int UNI_API_GetAccStatus();

/*
 * FullName:	获取电池充电状态
 * @return:		1-正在充电, 0-未充电, 2-已充满
 */
int UNI_API_GetChargeStatus();

/*
 * FullName:	获取外部电源电压
 * @param [out]: buf : 保存外部电源电压字符串
 * @param [in]: length : buf的大小
 * @return:		读到的长度
 */
int UNI_API_GetPowerVoltage(char* buf, int length);

/*
 * FullName:	获取内置电池电压
 * @param [out]: buf : 保存内置电池电压字符串
 * @param [in]: length : buf的大小
 * @return:		读到的长度
 */
int UNI_API_GetBatteryVoltage(char* buf, int length);

/*
 * FullName:	获取G-sensor值
 * @param [out]: buf : 保存G-sensor的XYZ三个值
 * @return:		1-成功， 0-失败
 */
int UNI_API_GetGsensor(short buf[3]);

/*
 * FullName:	设置碰撞阀值
 * @param [in]: value : 0-4
 * @return:		1-成功， 0-失败
 */
int UNI_API_SetCollideThreshold(int value);

/*
 * FullName:	升级MCU程序
 * @param [in]: filepath : 升级文件路径
 * @return:		1-成功， 0-失败
 */
int UNI_API_UpdateMcu(const char* filepath);

/*
 * FullName:	升级APP
 * @param [in]: filepath : 升级文件路径
 * @return:		1-成功， 0-失败
 */
int UNI_API_UpdateApp(const char* filepath);

/*
 * FullName:	设置数据透传串口波特率
 * @param [in]: id : 串口编号
 * @param [in]: baud : 波特率
 * @return:		1-成功， 0-失败
 */
int UNI_API_SetDataTransferParam(int id, int baud);

/*
 * FullName:	获取CAN速度
 * @param [in]: channel : 通道号
 * @return:		1-250K， 2-500K
 */
int UNI_API_GetCanSpeed(int channel);

/*
 * FullName:	设置CAN速度
 * @param [in]: channel : 通道号
 * @param [in]: value : 0-125K, 1-250K， 2-500K
 * @return:		1-成功， 0-失败
 */
int UNI_API_SetCanSpeed(int channel, int value);

/*
 * FullName:	获取输入IO状态
 * @param [in]: id : 1-4
 * @return:		1/0
 */
int UNI_API_GetIoInput(int id);

/*
 * FullName:	设置输出IO状态
 * @param [in]: id : 1-2
 * @param [in]: value : 0/1
 * @return:		1-成功， 0-失败
 */
int UNI_API_SetIoOutput(int id, int value);

/*
 * FullName:	数据透传
 * @param [in]: id : 设备编号
 * @param [in]: pBuf : 数据
 * @param [in]: length : pBuf的大小
 * @return:		1-成功， 0-失败
 */
int UNI_API_DataTransfer(int id, const byte* pBuf, int length);

/*
 * FullName:	写CAN
 * @param [in]: buf : 数据,长度必须是14
 * @return:		1-成功， 0-失败
 */
int UNI_API_CanWrite(const byte buf[14]);

/*
 * FullName:	写OBD
 * @param [in]: pBuf : 数据
 * @param [in]: length : pBuf的大小
 * @return:		1-成功， 0-失败
 */
int UNI_API_ObdWrite(const byte* pBuf, int length);

/*
 * FullName:	控制LED灯 打开/关闭
 * @param [in]: id : 灯编号 1-GPS 2-LTE
 * @param [in]: state : 1-打开 0-关闭
 * @return:		1-成功， 0-失败
 */
int UNI_API_LedCtrl(int id, int state);

/*
 * FullName:	设置灯的所有权
 * @param [in]: id : 灯编号 1-GPS 2-LTE
 * @param [in]: grab : 1-由用户控制灯 0-由系统控制灯
 * @return:		1-成功， 0-失败
 */
int UNI_API_LedGrab(int id, int grab);

/*
 * FullName:	获取当前拨号状态
 * @return:		1-拨号成功， 0-拨号失败或正在拨号
 */
int UNI_API_GetLteDialState();

/*
 * FullName:	获取线束拔插状态
 * @return:		1-已插入， 0-已拔出
 */
int UNI_API_GetPlugState();

/*
 * FullName:	使设备进入深度睡眠
 * @return:		1-成功， 0-失败
 */
int UNI_API_DeepSleep();

/*
 * FullName:	设置MCU时间
 * @param [in]: timestr : 20180806142830
 * @return:		1-写入成功 0-写入失败
 */
int UNI_API_SetTime(const char* timestr);

/*
 * FullName:	获取SN号
 * @param [out]: buf : 保存SN号字符串
 * @param [in]: length : buf的大小,不能小于15
 * @return:		读到的长度
 */
int UNI_API_GetTime(char* buf, int length);




#ifdef __cplusplus
}
#endif

#endif  // #ifndef __UNI_API_H__

