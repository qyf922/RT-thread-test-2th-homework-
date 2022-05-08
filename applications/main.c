#include <rtthread.h>
#include <rtdevice.h>

#define THREAD_PRIORITY  25
#define THREAD_STACK_SIZE  512
#define THREAD_TIMESLICE  5
static rt_thread_t tid1 = RT_NULL;

/*线程 1 的入口函数,500ms打印一次计数值*/
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0;//计数器归0

    while (1)
    {
        /* 线程1采用低优先级运行，一直打印计数值 */
        rt_kprintf("thread1 count: %d\n", count ++);//内核提供一个用于输出日志信息的接口
        rt_thread_mdelay(500);
    }
}

ALIGN(RT_ALIGN_SIZE)   //栈对齐
static char thread2_stack[1024];   
static struct rt_thread thread2;


/* 线程2入口，打印计数10 次后退出*/
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;//打印计数

    for (count = 0; count < 10 ; count++)
    {
        /* 线程2打印计数值 */
        rt_kprintf("thread2 count: %d\n", count);
    }
    rt_kprintf("thread2 exit\n");
    /* 线程2运行结束后也将自动被系统删除 */
}


/*用户代码的入口*/
int main(void)
{
	/* 创建线程1，名称是thread1，入口是thread1_entry*/
	/*动态线程thread1*/
    tid1 = rt_thread_create("thread1",//名称
                            thread1_entry, RT_NULL,//入口函数及参数
                            THREAD_STACK_SIZE,//栈大小
                            THREAD_PRIORITY, THREAD_TIMESLICE);//时间片大小
    
    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
		//就绪，按优先级

		
    /* 初始化线程2，名称是thread2，入口是thread2_entry */
		/*静态线程thread2*/
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
									 /* 线程2优先级高 */
    rt_thread_startup(&thread2);
		/* 线程加入队列，执行调度*/
									 
    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(main, experiment1);
