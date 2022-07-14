int main(void)
{
    rt_thread_t dht11_thread, led_thread;
    
    dht11_thread = rt_thread_create("dht11tem", read_temp_entry, "temp_dht11",
                                      640, RT_THREAD_PRIORITY_MAX / 2, 20);
    if (dht11_thread != RT_NULL)
    {
        rt_thread_startup(dht11_thread);
    }
    
    led_thread = rt_thread_create("ledshine", led_shine_entry, RT_NULL,
                                  192, RT_THREAD_PRIORITY_MAX / 2, 20);
    if (led_thread != RT_NULL)
    {
        rt_thread_startup(led_thread);
    }
    
    return RT_EOK;
}

#define DHT11_DATA_PIN GET_PIN(B, 12)

static void read_temp_entry(void *parameter)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;

    dev = rt_device_find(parameter);
    if (dev == RT_NULL)
    {
        rt_kprintf("Can't find device:%s\n", parameter);
        return;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return;
    }
    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)100);

    while (1)
    {
        res = rt_device_read(dev, 0, &sensor_data, 1);
        if (res != 1)
        {
            rt_kprintf("read data failed!size is %d\n", res);
            rt_device_close(dev);
            return;
        }
        else
        {
            if (sensor_data.data.temp >= 0)
            {

							uint8_t temp = (sensor_data.data.temp & 0xffff) >> 0;      // get temp
              uint8_t humi = (sensor_data.data.temp & 0xffff0000) >> 16; // get humi
							rt_kprintf("temp:%d, humi:%d\n" ,temp, humi);
            }
        }
/* defined the LED0 pin: PB5 */
#define LED0_PIN    GET_PIN(B, 5)
/* defined the LED1 pin: PE5 */
#define LED1_PIN    GET_PIN(E, 5)

static void led_shine_entry(void *parameter)
{
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    
    while(1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
static int rt_hw_dht11_port(void)
{
    struct rt_sensor_config cfg;
    
    cfg.intf.user_data = (void *)DHT11_DATA_PIN;
    rt_hw_dht11_init("dht11", &cfg);
    
    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_dht11_port);

        rt_thread_mdelay(100);
    }
}
