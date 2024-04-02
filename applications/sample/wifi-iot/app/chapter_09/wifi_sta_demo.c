#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"

#define SSID "Y530"
#define SSID_PASSWORD "12345678"

static void WifiStaTask(void *arg)
{
    (void)arg;
    WifiErrorCode errCode;
    WifiDeviceConfig apConfig = {0};
    int netId = -1;

    unsigned int size = WIFI_SCAN_HOTSPOT_LIMIT;
    WifiScanInfo *InfoResult = malloc(sizeof(WifiScanInfo) * WIFI_SCAN_HOTSPOT_LIMIT);
    ;

    // 设置将要连接的AP属性（wifi名称、密码、加密方式），
    strcpy(apConfig.ssid, SSID);
    strcpy(apConfig.preSharedKey, SSID_PASSWORD);
    apConfig.securityType = WIFI_SEC_TYPE_PSK;

    errCode = EnableWifi(); // 启用STA模式
    printf("EnableWifi: %d\r\n", errCode);

    errCode = IsWifiActive();
    printf("IsWifiActive: %d\r\n", errCode);

    errCode = Scan();
    printf("Scan: %d\r\n", errCode);

    sleep(30);
    while (1)
    {
        errCode = GetScanInfoList(InfoResult, &size);
        printf("GetScanInfoList: %d\r\n", errCode);
        printf("size: %d\r\n", size);
        if (size)
        {
            break;
        }
    }
    if (!errCode)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            printf("ssid:%-30s, rssi:%5d\r\n", InfoResult[i].ssid, InfoResult[i].rssi / 100);
        }
    }

    errCode = AddDeviceConfig(&apConfig, &netId); // 配置热点信息,生成netId
    printf("AddDeviceConfig: %d\r\n", errCode);

    errCode = ConnectTo(netId); // 接到指定的热点
    printf("ConnectTo(%d): %d\r\n", netId, errCode);
    usleep(3000 * 1000);
    // 联网业务开始
    // 这里是网络业务代码...
    struct netif *iface = netifapi_netif_find("wlan0"); // 获取网络接口用于IP操作
    printf("%d\r\n", iface);
    if (iface)
    {
        err_t ret = netifapi_dhcp_start(iface); // 启动DHCP客户端，获取IP地址
        printf("netifapi_dhcp_start: %d\r\n", ret);

        usleep(2000 * 1000);
        ;                                                                 // wait DHCP server give me IP
        ret = netifapi_netif_common(iface, dhcp_clients_info_show, NULL); // dhcp_clients_info_show为shell API，展示dhcp客户端信息
        printf("netifapi_netif_common: %d\r\n", ret);
    }
    usleep(5000 * 1000);
    // 联网业务结束
    Disconnect();        // disconnect with your AP
    RemoveDevice(netId); // remove AP config
    errCode = DisableWifi();
    printf("DisableWifi: %d\r\n", errCode);
}

static void WifiStaDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "WifiStaTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;

    if (osThreadNew(WifiStaTask, NULL, &attr) == NULL)
    {
        printf("[WifiConnectDemo] Falied to create WifiConnectTask!\n");
    }
}

APP_FEATURE_INIT(WifiStaDemo);
