// 作业调度.cpp : 定义控制台应用程序的入口点。
/*
//单道批处理
#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <list>
#include <string>
//list容器可以将数据进行链式存储,有很多方便的方法
//list<node>将node形式进行链式存储
//list<node>&是一种引用传递

using namespace std;

//时间结构体

typedef struct data{

    int hour;

    int min;

}Time;

typedef struct link_node{
    string name;			//作业名称
    Time arrive;			//作业入井时间
    int zx;					//作业执行时间
    Time JobTime;			//作业调度时间
    int JobWait;			//作业调度等待时间
    Time ProcessTime;		//进程调度时间
    int ProcessWait;		//进程调度等待时间
    Time finish;			//作业完成时间
    int zz;					//作业周转时间=作业完成时间-作业入井时间
    float zzxs;				//作业带权周转时间=作业周转时间/作业执行时间
    int finished;			//标记作业完成
    float response;			//作业响应化
}node;

/*
输入示例
1
4
JOB1 8:00 120
JOB2 8:50 50
JOB3 9:00 10
JOB4 9:50 20
1
1
4
JOB1 8:00 120
JOB2 8:50 50
JOB3 9:00 10
JOB4 9:50 20
2
1
4
JOB1 8:00 120
JOB2 8:50 50
JOB3 9:00 10
JOB4 9:50 20
3
0
*/
/*
A 10:00 40
B 10:20 30
C 10:30 50
D 10:50 20
*/
/*
void printfoption(){
    std::cout << "---------------作业管理---------------\n";
    std::cout << "\t\t*\t\t1.FCFS\t\t*\n";
    std::cout << "\t\t*\t\t2.SJF\t\t*\n";
    std::cout << "\t\t*\t\t3.HRRF\t\t*\n";
    std::cout << "\t\t*\t\t0.退出\t\t*\n";
    std::cout << "请输入要进行的操作号:";
}


//全局变量
Time nowTime;
float avgZZ;//系统平均周转时间
float avgZZXS;//系统平均带权周转时间

//比较函数

//将时间转换为分钟
int toMinute(Time t){
    return t.hour*60 + t.min;
}

//将分钟转换为时间
Time toTime(int t){
    Time s;
    s.hour = t/60;
    s.min = t % 60;
    return s;
}

//根据到达时间比较
bool timeCompare(node a, node b){
    return toMinute(a.arrive) < toMinute(b.arrive);
}

//根据运行时间进行比较
bool runtimeCompare(node a, node b){
    return a.zx < b.zx;
}

//根据响应比进行比较
bool respsonseCompare(node a, node b){
    return a.response > b.response;
}

void printFinished(list<node>& finishedJobs, int choose) {
    if (choose == 1) {
        cout << "模拟作业FCFS调度过程的输出结果：" << endl;
    }
    else if (choose == 2) {
        cout << "模拟作业SJF调度过程的输出结果：" << endl;
    }
    else if (choose == 3) {
        cout << "模拟作业HRRF调度过程的输出结果：" << endl;
    }

    printf("名字\t入井时间\t运行时间\t作业调度时间\t作业调度等待时间\t进程调度时间\t进程调度等待时间\t完成时间\t周转时间\t带权周转系数\n");
    for (auto it = finishedJobs.begin(); it != finishedJobs.end(); it++) {
        printf("%s\t%d:%02d\t\t%d(分钟)\t%d:%02d\t\t%d(分钟)  \t\t%d:%02d\t\t%d(分钟)\t\t\t%d:%02d\t\t%d(分钟)\t%.4f\n",
               it->name.c_str(), it->arrive.hour, it->arrive.min, it->zx, it->JobTime.hour, it->JobTime.min, it->JobWait,
               it->ProcessTime.hour, it->ProcessTime.min, it->ProcessWait, it->finish.hour, it->finish.min, it->zz, it->zzxs);
    }
    printf("系统平均周转时间为：%.2f\n", avgZZ);
    printf("系统平均带权周转时间为：%.4f\n", avgZZXS);
}

void FCFS(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while(!readyJobs.empty()){
        //弹出就绪队列第一个作业到终止队列中
        //计算相关时间
        //更新进程信息和当前时间
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);//作业调度等待时间
        t.ProcessWait = 0;//进程调度等待时间
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz * 1.0 / t.zx;
        //加入到完成队列
        finishedJobs.push_back(t);
        //更新平均周转时间和系数
        avgZZ += t.zz * 1.0 / n;
        avgZZXS += t.zzxs / n;


        //更新就绪队列
        while (!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)) {
            readyJobs.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
    }
}

void SJF(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while(!readyJobs.empty()){
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);
		t.ProcessWait = 0;
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz*1.0/t.zx;
        finishedJobs.push_back(t);
        avgZZ += t.zz*1.0/n;
        avgZZXS += t.zzxs*1.0/n;
        list<node> s;
        while(!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)){
            s.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
        s.sort(runtimeCompare);
        readyJobs.insert(readyJobs.end(), s.begin(), s.end());
    }

}

void HRRF(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while (!readyJobs.empty()) {	//判断就绪队列是否为空
        //更新进程信息和当前时间
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);
        t.ProcessWait = 0;
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz * 1.0 / t.zx;
        //加入到完成队列
        finishedJobs.push_back(t);
        //更新平均周转时间和系数
        avgZZ += t.zz * 1.0 / n;
        avgZZXS += t.zzxs / n;

        //对后备队列中到达时间比当前时间小的作业，再按响应比排序
        list<node> s;
        //找出到达时间比当前时间小的作业
        while (!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)) {
            s.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
        //计算出加入就绪队列的响应比
        //将他们按响应比从大到小排序
        //将他们加入到就绪队列
        readyJobs.insert(readyJobs.end(), s.begin(), s.end());
        for (auto it = readyJobs.begin(); it != readyJobs.end(); it++) {
            //响应比 = 1 + 作业等待时间 / 作业处理时间
            it->response = 1 + (toMinute(nowTime) - toMinute(it->arrive)) * 1.0 / it->zx;
        }
        readyJobs.sort(respsonseCompare);
    }

}

int main()
{
    int operate,num,option;
    char c;		//读取字符
    cout << "请输入操作(1:开始作业调度; 0:结束作业):";
    cin >> option;
    while(option!=0){
        avgZZ = avgZZXS = 0;
        cout << "请输入你需要创建的作业数:";
        cin >> num;
        list<node> readyJobs;	//就绪队列
        list<node> backupJobs;	//后备队列
        list<node> finishedJobs;//已完成队列
        cout << "请依次输入" << endl;
        cout << "作业名\t入井时间\t运行时间(分钟)" << endl;
        for(int i = 0; i < num; i++){
            node job;
            cin >> job.name >> job.arrive.hour >> c >> job.arrive.min >> job.zx;
            backupJobs.push_back(job);
        }
        //先根据到达时间进行排序
        backupJobs.sort(timeCompare);
        nowTime = backupJobs.front().arrive;
        //初始化就绪队列
        readyJobs.push_back(backupJobs.front());
        backupJobs.pop_front();
        printfoption();
        cin >> operate;
        if(operate == 1){
            FCFS(num, readyJobs, backupJobs, finishedJobs);
            printFinished(finishedJobs, operate);
        }else if(operate == 2){
            SJF(num, readyJobs, backupJobs, finishedJobs);
            printFinished(finishedJobs, operate);
        }else if(operate == 3){
            HRRF(num, readyJobs, backupJobs, finishedJobs);
            printFinished(finishedJobs, operate);
        }else if(operate == 0){
            return 0;
        }
        cout << "请输入操作(1:开始作业调度; 0:结束作业):";
        cin >> option;
    }
//    std::cout << "Hello, World!" << std::endl;
	getchar();
    return 0;
}
*/

//两道批处理系统 课本P99 6
#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define getpch(type) (type*)malloc(sizeof(type)) 
#define WAIT "Wait"
#define READY "Ready"
#define RUN "Run"
#define FINISH "Finish"

#define FREE "free"
#define BUSY "BUSY"

struct jcb
{
	int jid;	//作业id
	char name[10];	//作业名
	int arriveTime;	//到达时间
	int needrunTime;	//预计运行时间
	int needMemory;		//需要内存
	int needTypeDrive;	//需要磁带机数量
	int alreadyRunTime;	//已运行时间
	int beginTime;		//开始运行时间
	int finishTime;		//完成时间
	int startMemary;	//内存占用开始位置
	int useTypeDriveid[4];	//使用磁带机id
	struct jcb *next;
};
typedef struct jcb JCB;
JCB *InputWell = NULL;//输入井
JCB *arriveJobList = NULL;//到达作业
JCB *allocatedJobList = NULL;//已分配作业表
JCB *run = NULL;
JCB *finish = NULL;

struct memary
{
	int mid;	//分区id
	int begin;	//开始位置
	int length;	//分区长度
	char status[5];	//状态：FREE 或 BUSY
	struct memary *pre;
	struct memary *next;
};
typedef struct memary Memary;
Memary *memaryHead = NULL;

struct typedrive
{
	int did;	//磁带机id
	char status[5];	//状态：FREE 或 BUSY
	struct typedrive *next;
};
typedef struct typedrive TypeDrive;
TypeDrive *driveHead=NULL;//磁盘列表

int jobNum = 0;
int time = 0;//时间

int readJobList(); //读取作业表
void initJobList();//初始化作业表
void initMemary(); //初始化内存
void initTypeDrive(); //初始化磁带机
int arrive();//到达
int allocate(JCB *target);//分配内存和磁盘
JCB* deleteJob(JCB *list,JCB *target);//从链表中删除作业
int addJobToList(JCB *list, JCB *target);//将作业添加进链表
int FirstPartition(JCB *target);//首部优先分区
int recycle(JCB *target);//分区回收和磁带回收
int FCFSJS();//作业调度先来先服务
int SJSJS(); //作业调度最小作业优先
int FCFSPS(); //进程调度先来先服务
int SJFPS(); //进程调度最短作业优先
void showJobList(JCB *list);
void showMemary();
void showTypeDrive();

int readJobList()//文件读取获取作业列表
{
	int  count = 0;
	char c;
	FILE *fp;
	if ((fp = fopen(".\\joblist.txt", "r")) == NULL)
		return -1;
	while (!feof(fp))
	{
		c = fgetc(fp);
		if (c == '\n')
			count++;
	}
	jobNum = count + 1;
	fclose(fp);
	fp = fopen(".\\joblist.txt", "r");
	InputWell = (JCB*)getpch(JCB);
	InputWell->next = NULL;
	for (int i = 0; i < jobNum; i++) 
	{
		JCB *p;
		p = (JCB*)getpch(JCB);
		p->jid = i + 1;
		fscanf(fp, "%s %d %d %d %d", p->name,&p->arriveTime,&p->needrunTime,&p->needMemory,&p->needTypeDrive);
		p->alreadyRunTime = 0;
		p->beginTime = -1;
		p->finishTime = -1;
		p->startMemary = -1;
		p->next = NULL;
		for (int j = 0; j < 4; j++)
		{
			p->useTypeDriveid[j] = -1;
		}
			JCB *q = InputWell;
			while (q->next!=NULL)
			{
				q = q->next;
			}
			q->next = p;
	}
	return 0;
}

void initJobList()
{
	arriveJobList = (JCB*)getpch(JCB);//到达作业表
	arriveJobList->next = NULL;
	allocatedJobList = (JCB*)getpch(JCB);//已分配作业表
	allocatedJobList->next = NULL;
	run = (JCB*)getpch(JCB);//运行表
	run->next = NULL;
	finish = (JCB*)getpch(JCB);//完成表
	finish->next = NULL;
}

void initMemary()
{
	Memary* p = (Memary*)getpch(Memary);
	memaryHead= (Memary*)getpch(Memary);
	p->mid = 0;
	p->begin = 0;
	p->length = 100;
	strcpy(p->status, FREE);
	p->next = NULL;
	p->pre = memaryHead;
	memaryHead->pre = NULL;
	memaryHead->next = p;
}

void initTypeDrive()
{
	for (int i = 0; i < 4; i++)
	{
		TypeDrive *p = (TypeDrive*)getpch(TypeDrive);
		p->did = i;
		p->next = NULL;
		strcpy(p->status, FREE);
		if (NULL==driveHead)
		{
			driveHead = p;
		}
		else
		{
			TypeDrive *q = driveHead;
			while (q->next!=NULL)
			{
				q = q->next;
			}
			q->next = p;
		}
	}
}

int arrive()
{
	JCB *job = InputWell->next;
	while (job!=NULL)
	{
		if (job->arriveTime==time)
		{
			job = deleteJob(InputWell, job);//从输入井中删除 加入到到达队列中
			addJobToList(arriveJobList, job);
			job = NULL;
			free(job);
			return 1;
		}
		job = job->next;
	}
	return 0;
}

int allocate(JCB * target)//为作业分配资源
{
	if (target==NULL)
	{
		return 0;
	}
	int needmemary = target->needMemory;
	int needdrive = target->needTypeDrive;
	int countDrive = 0;
	int isMemaryEnough = 0;
	Memary *temp = memaryHead->next;
	while (temp!=NULL)
	{
		if (temp->length >= needmemary)
		{
			isMemaryEnough = 1;//内存足够分配
		}
		temp = temp->next;
	}
	temp = NULL;
	free(temp);
	if (isMemaryEnough==0)
	return 0;
	TypeDrive *q = driveHead;
	while (q!=NULL)
	{
		if (strcmp(q->status,FREE)==0)//判断现在有几台磁带机是空闲的
		{
			countDrive++;
		}
		q = q->next;
	}
	if (countDrive < needdrive)
		return 0;
	JCB *job = deleteJob(arriveJobList, target);//内存和磁带机都够 将作业加入到已分配资源的队列中
	addJobToList(allocatedJobList, job);
	FirstPartition(job);//首部优先分区
	q = driveHead;
	for (int i = 0; i < 4; i++)//分配磁带机
	{
		if (strcmp(q->status,FREE)==0&&needdrive!=0)
		{
			strcpy(q->status, BUSY);
			needdrive--;
			job->useTypeDriveid[i] = 1;
		}
		q = q->next;
	}
	return 1;
}

JCB * deleteJob(JCB * list, JCB * target)
{
	JCB *q = list;
	while (q!=NULL)
	{
		if (q->next==NULL)
		{
			return NULL;
		}
		if (q->next==target&&q->next->next!=NULL)
		{
			q->next = q->next->next;
			target->next = NULL;
			return target;
		}
		else if (q->next==target&&q->next->next==NULL)
		{
			q->next = NULL;
			return target;
		}
		q = q->next;
	}
	return NULL;
}

int addJobToList(JCB *list, JCB *target)
{
	if (list==NULL||target==NULL)
		return 0;
	JCB *q = list;
	while (q->next!=NULL)
	{
		q = q->next;
	}
	q->next = target;
	return 1;
}

int FirstPartition(JCB *job)//内存 首部优先分区
{
	int size = job->needMemory;
	Memary *p = memaryHead->next;
	while (p != NULL)
	{
		if (strcmp(p->status, FREE) == 0 && p->length >= size)
		{
			if (p->length == size)
			{
				strcpy(p->status, BUSY);
			}
			else
			{
				Memary *node = (Memary*)malloc(sizeof(Memary));
				node->begin = p->begin + size;
				node->length = p->length - size;
				strcpy(node->status, FREE);
				node->pre = p;
				node->next = p->next;
				if (p->next != NULL)
				{
					p->next->pre = node;
				}
				p->next = node;
				p->length = size;
				job->startMemary = p->begin;
				strcpy(p->status, BUSY);
			}
			printf("内存分配成功!");
			return 1;
		}
		p = p->next;
	}
	printf("内存分配失败!");
	return 0;
}

int recycle(JCB *target)//分区回收和内存回收
{
	int start = target->startMemary;
	int flag = 0;
	Memary *p = memaryHead->next;
	Memary *q = NULL;
	while (p != NULL)
	{
		if (strcmp(p->status, BUSY) == 0 && p->begin == start)
		{
			flag = 1;
			if ((p->pre != memaryHead && strcmp(p->pre->status, FREE) == 0) && (p->next != NULL) && strcmp(p->next->status, FREE) == 0)
			{
				//merge pre
				q = p;
				p = p->pre;
				p->length += q->length;
				p->next = q->next;
				q->next->pre = p;
				free(q);
				//merge next
				q = p->next;
				p->length += q->length;
				p->next = q->next;
				if (q->next != NULL)
				{
					q->next->pre = p;
				}
				free(q);
			}
			else if ((p->pre == memaryHead || strcmp(p->pre->status, BUSY) == 0) && (p->next != NULL && strcmp(p->next->status, FREE) == 0))
			{
				//only merge next 
				q = p->next;
				p->length += q->length;
				strcpy(p->status, FREE);
				p->next = q->next;
				if (q->next != NULL)
				{
					q->next->pre = p;
				}
				free(q);
			}
			else if ((p->pre != memaryHead && strcmp(p->pre->status, FREE) == 0) && (p->next == NULL || strcmp(p->next->status, BUSY) == 0))
			{
				//only merge pre
				q = p;
				p = p->pre;
				p->length += q->length;
				p->next = q->next;
				if (q->next != NULL)
				{
					q->next->pre = p;
				}
				free(q);
			}
			else
			{
				strcpy(p->status, FREE);
			}
		}
		p = p->next;
	}
	TypeDrive *temp = driveHead;
	int driveNum = target->needTypeDrive;
	for (int i = 0; i < 4; i++)
	{
		if (target->useTypeDriveid[i]==1)
		{
			target->useTypeDriveid[i] = -1;
			strcpy(temp->status, FREE);
		}
		temp = temp->next;
	}
	target->finishTime = time;
	JCB *job = deleteJob(run, target);
	addJobToList(finish, job);
	if (flag == 1)
	{
		printf("``````````内存和磁带机回收成功!``````````");
		return 1;
	}
	else
	{
		printf("``````````内存和磁带机回收失败!``````````");
		return 0;
	}
}

int FCFSJS()//先来先服务作业调度 对到达作业队列
{
	JCB *first = arriveJobList->next;
	if (first==NULL)
		return 0;
	else return allocate(first);
}

int SJFJS()//短作业优先作业调度 
{
	JCB *temp = arriveJobList->next;
	if (temp == NULL)
		return 0;
	int minNum = 999;//将初始最短作业时间设为一个很大的值 方便后续比较
	JCB *minJob = NULL;
	while (temp!=NULL)
	{
		if (temp->needrunTime <minNum)
		{
			minJob = temp;
			minNum = temp->needrunTime;
		}
		temp = temp->next;
	}
	return allocate(minJob);
}

int FCFSPS()//进程调度 先来先服务 对已分配资源的队列
{
	if (run->next!=NULL)
		return 0;
	JCB *first = allocatedJobList->next;
	if (first == NULL)
		return 0;
	JCB *target=deleteJob(allocatedJobList, first);
	addJobToList(run, target);
	target->beginTime = time;
	printf("\n进程%s开始运行\n", target->name);
	return 1;
}

int SJFPS()//进程调度 短作业优先 对已分配资源的队列
{
	JCB *temp=allocatedJobList->next;
	int minNum = 999;
	JCB *minJob = NULL;
	while (temp != NULL)
	{
		if (temp->needrunTime < minNum)
		{
			minJob = temp;
			minNum = temp->needrunTime;
		}
		temp = temp->next;
	}
	if (minJob==NULL)
		return 0;
	if (run->next==NULL)//当运行队列中只有一条时 直接运行
	{
		JCB *target = deleteJob(allocatedJobList, minJob);
		addJobToList(run, target);
		if (target->beginTime==-1)
		{
			target->beginTime = time;
		}
		printf("\n进程%s开始运行\n", target->name);
		return 1;
	}
	else if (minJob->needrunTime < run->next->needrunTime)//当运行队列有两条时 抢占式 短作业优先
	{
		JCB *target = deleteJob(allocatedJobList, minJob);
		JCB *pre = deleteJob(run, run->next);
		addJobToList(run, target);
		addJobToList(allocatedJobList,pre);
		if (target->beginTime == -1)
		{
			target->beginTime = time;
		}
		printf("\n进程%s开始运行\n", target->name);
		printf("\n进程%s暂停运行\n", pre->name);
		return 1;
	}
	else
	{
		return 0;
	}
}

void showJobList(JCB * list)
{
	JCB *temp = list->next;
	int TurnaroundTime = 0;
	printf("** 作业名 | 到达时间 | 估计运行时间 | 内存需要 | 磁带机需要 |  开始运行时间 | 已运行时间 |完成时间 ");
	while (temp!=NULL)
	{
		printf("\n**-----------------------------------------------------------------------------------------------\n");
		printf("**");
		printf("  %s  |    %3d   |     %3d      |    %3d       |    %3d    |",temp->name,temp->arriveTime,temp->needrunTime,temp->needMemory,temp->needTypeDrive);
		if (temp->beginTime==-1)
		{
			printf("   未开始运行 |   未开始运行 |");
		}
		else
		{
			printf("  %3d   | %3d      |", temp->beginTime,temp->alreadyRunTime);
		}
		if (temp->finishTime==-1)
		{
			printf("未结束运行 ");
		}
		else
		{
			printf(" %3d ", temp->finishTime+1);
		}
		if (list == finish)
		{
			TurnaroundTime += temp->finishTime - temp->arriveTime;
		}
		temp = temp->next;
	}

	if (list == finish)
		printf("\n平均周转时间为：%lf", 1.0*TurnaroundTime / jobNum);
	printf("\n````````````````````````````````````````````````````````\n");
}

void showMemary()
{
	printf("\n````````````````````````````````````````````````````````\n");
	printf("``````````当前内存分配情况如下``````````\n");
	printf("````` 起始地址 | 空间大小 | 状态  `````\n");
	Memary *memary = memaryHead->next;
	while (memary!=NULL)
	{
			printf("\n``----------------------------------------------\n");
			printf("``");
			printf("   %3d k  |", memary->begin);
			printf("   %3d k  |", memary->length);
			printf("   %s ", memary->status);
			printf("\n");
			memary = memary->next;
	}
	printf("\n````````````````````````````````````````````````````````\n");
}

void showTypeDrive()
{
	TypeDrive *drive = driveHead;
	printf("\n````````````````````````````````````````````````````````\n");
	printf("``````````````````当前磁带机占用情况如下``````````````````\n");
	printf("`` 磁带机id | 状态  ``\n");
	while (drive!=NULL)
	{
		printf("\n``------------------------------\n");
		printf("``");
		printf(" %3d | %s ",drive->did,drive->status);
		printf("\n");
		drive = drive->next;
	}
	printf("\n````````````````````````````````````````````````````````\n");
}

int main(void)
{
	readJobList();
	initJobList();
	initMemary();
	initTypeDrive();
	int JS = 0, PS = 0,jstag=0,pstag=0;
	printf("\n```````````````当前输入井作业情况如下``````````````````\n");
	showJobList(InputWell);
	while (1)
	{
		printf("-------------请选择要模拟的作业调度分配算法:\n");
		printf("\t\t          0--先来先服务算法  \n");
		printf("\t\t          1--最小作业优先算法  \n");
		printf("输入序号：\n");
		scanf("%d", &JS);
		if (JS == 0)
		{
			printf("----------您选择了先来先服务算法\n");
		}
		else if (JS == 1)
		{
			printf("----------您选择了最小作业优先算法\n");		
		}
		else
		{
			printf("错误:输入 0|1\n");
			break;
		}
		printf("-------------选择要模拟的进程调度分配算法:\n");
		printf("\t\t          0--先来先服务算法  \n");
		printf("\t\t          1--最短优先算法  \n");
		printf("输入序号：\n");
		scanf("%d", &PS);
		if (JS == 0)
		{
			printf("----------您选择了先来先服务算法\n");
		}
		else if (JS == 1)
		{
			printf("----------您选择了最短优先算法\n");
		}
		else
		{
			printf("错误:输入 0|1\n");
			break;
		}
		break;
	}

	while (1)
	{
		arrive();
		if (JS==0)
		{
			jstag=FCFSJS();
		}
		else
		{
			jstag=SJFJS();
		}
		if (PS==0)
		{
			pstag=FCFSPS();
		}
		else
		{
			pstag=SJFPS();
		}
		if (jstag==1||pstag==1)
		{
			printf("\n``````````````当前内存中作业情况如下`````````````````\n");
			showJobList(allocatedJobList);
			showJobList(run);
			showMemary();
			showTypeDrive();
		}
		if (run->next != NULL)
		{
			JCB *running = run->next;
			running->alreadyRunTime++;
			running->needrunTime--;
			printf("\n```````````````当前运行进程情况如下``````````````````\n");
			showJobList(run);
			if (0 == running->needrunTime)
			{
				recycle(running);
			}
		}
		if (InputWell->next==NULL&&arriveJobList->next==NULL&&allocatedJobList->next==NULL&&run->next==NULL)
		{
			break;
		}
		jstag = pstag = 0;
		time ++;
	}

	printf("\n```````````````完成作业情况如下`````````````````````\n");
	showJobList(finish);

	system("pause");
	return 0;
}