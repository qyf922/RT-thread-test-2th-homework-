#include <rtthread.h>
#include <rtdevice.h>

#define THREAD_PRIORITY  25
#define THREAD_STACK_SIZE  512
#define THREAD_TIMESLICE  5
static rt_thread_t tid1 = RT_NULL;

/*�߳� 1 ����ں���,500ms��ӡһ�μ���ֵ*/
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0;//��������0

    while (1)
    {
        /* �߳�1���õ����ȼ����У�һֱ��ӡ����ֵ */
        rt_kprintf("thread1 count: %d\n", count ++);//�ں��ṩһ�����������־��Ϣ�Ľӿ�
        rt_thread_mdelay(500);
    }
}

ALIGN(RT_ALIGN_SIZE)   //ջ����
static char thread2_stack[1024];   
static struct rt_thread thread2;


/* �߳�2��ڣ���ӡ����10 �κ��˳�*/
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;//��ӡ����

    for (count = 0; count < 10 ; count++)
    {
        /* �߳�2��ӡ����ֵ */
        rt_kprintf("thread2 count: %d\n", count);
    }
    rt_kprintf("thread2 exit\n");
    /* �߳�2���н�����Ҳ���Զ���ϵͳɾ�� */
}


/*�û���������*/
int main(void)
{
	/* �����߳�1��������thread1�������thread1_entry*/
	/*��̬�߳�thread1*/
    tid1 = rt_thread_create("thread1",//����
                            thread1_entry, RT_NULL,//��ں���������
                            THREAD_STACK_SIZE,//ջ��С
                            THREAD_PRIORITY, THREAD_TIMESLICE);//ʱ��Ƭ��С
    
    /* �������߳̿��ƿ飬��������߳� */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
		//�����������ȼ�

		
    /* ��ʼ���߳�2��������thread2�������thread2_entry */
		/*��̬�߳�thread2*/
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
									 /* �߳�2���ȼ��� */
    rt_thread_startup(&thread2);
		/* �̼߳�����У�ִ�е���*/
									 
    return 0;
}

/* ������ msh �����б��� */
MSH_CMD_EXPORT(main, experiment1);
