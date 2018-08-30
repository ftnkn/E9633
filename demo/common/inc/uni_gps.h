#ifndef __UNI_GPS_H__
#define __UNI_GPS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;
typedef long long int int64_t;

typedef enum 
{
    GPS_LOCATION    = 0x0100,   // GPS Location
    GPS_NMEA        = 0x0200,   // GPS NMEA
} UNI_CALLBACK_MSG_ID_T;

typedef enum 
{
    E_UNI_LOCATION_LAT_LONG_VALID   = 0x0001, /**<  GPS location has valid latitude and longitude.  */
    E_UNI_LOCATION_ALTITUDE_VALID   = 0x0002, /**<  GPS location has a valid altitude.  */
    E_UNI_LOCATION_SPEED_VALID      = 0x0004, /**<  GPS location has a valid speed.  */
    E_UNI_LOCATION_BEARING_VALID    = 0x0008, /**<  GPS location has a valid bearing.  */
    E_UNI_LOCATION_ACCURACY_VALID   = 0x0010, /**<  GPS location has valid accuracy.  */
} UNI_LOCATION_VALID_FLAG;

typedef struct 
{
    UNI_LOCATION_VALID_FLAG flags;      /**<   Contains GPS location flags bits. */
    int64_t     timestamp;              /**<   Timestamp for the location fix in UTC million-second base.  */
    double      latitude;               /**<   Latitude in degrees. */
    double      longitude;              /**<   Longitude in degrees. */
    double      altitude;               /**<   Altitude in meters above the WGS 84 reference ellipsoid. */
    float       speed;                  /**<   Speed km/h. */
    float       bearing;                /**<   Heading in degrees. */
    float       accuracy;               /**<   Expected accuracy in meters. */
    int         mode;		            /**<   模式，A=自动，D=差分，E=估测，N=数据无效 */
    int         satellite_number;       /**<   Indicates the satellite number. */
} UNI_LOCATION_INFO; 


typedef void(*UNI_GPS_CallbackFunc)(int type, const byte* pBuf, int length);

/*
 * FullName:	注册消息接收函数和过滤项
 * @param [in]: func : 消息接收回调函数, NULL-取消回调
 * @param [in]: filter : 过滤项，UNI_CALLBACK_MSG_ID_T 里各项的异或值
 * @return:		1-成功 0-失败
 */
int UNI_GPS_SetCallback(UNI_GPS_CallbackFunc func, int filter);

/*
 * FullName:	恢复出厂设置
 * @return:		1-成功 0-失败
 */
int UNI_GPS_Reset();

/*
 * FullName:	冷启动
 * @return:		1-成功 0-失败
 */
int UNI_GPS_ColdStart();

/*
 * FullName:	设置工作模式
 * @param [in]: mode : 0-GPS, 1-GPS+Glonass, 2-GPS+BeiDou
 * @return:		1-成功 0-失败
 */
int UNI_GPS_SetMode(int mode);


#ifdef __cplusplus
}
#endif

#endif  // #ifndef __UNI_GPS_H__

