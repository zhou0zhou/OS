// ��ҵ����.cpp : �������̨Ӧ�ó������ڵ㡣
/*
//����������
#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <list>
#include <string>
//list�������Խ����ݽ�����ʽ�洢,�кܶ෽��ķ���
//list<node>��node��ʽ������ʽ�洢
//list<node>&��һ�����ô���

using namespace std;

//ʱ��ṹ��

typedef struct data{

    int hour;

    int min;

}Time;

typedef struct link_node{
    string name;			//��ҵ����
    Time arrive;			//��ҵ�뾮ʱ��
    int zx;					//��ҵִ��ʱ��
    Time JobTime;			//��ҵ����ʱ��
    int JobWait;			//��ҵ���ȵȴ�ʱ��
    Time ProcessTime;		//���̵���ʱ��
    int ProcessWait;		//���̵��ȵȴ�ʱ��
    Time finish;			//��ҵ���ʱ��
    int zz;					//��ҵ��תʱ��=��ҵ���ʱ��-��ҵ�뾮ʱ��
    float zzxs;				//��ҵ��Ȩ��תʱ��=��ҵ��תʱ��/��ҵִ��ʱ��
    int finished;			//�����ҵ���
    float response;			//��ҵ��Ӧ��
}node;

/*
����ʾ��
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
    std::cout << "---------------��ҵ����---------------\n";
    std::cout << "\t\t*\t\t1.FCFS\t\t*\n";
    std::cout << "\t\t*\t\t2.SJF\t\t*\n";
    std::cout << "\t\t*\t\t3.HRRF\t\t*\n";
    std::cout << "\t\t*\t\t0.�˳�\t\t*\n";
    std::cout << "������Ҫ���еĲ�����:";
}


//ȫ�ֱ���
Time nowTime;
float avgZZ;//ϵͳƽ����תʱ��
float avgZZXS;//ϵͳƽ����Ȩ��תʱ��

//�ȽϺ���

//��ʱ��ת��Ϊ����
int toMinute(Time t){
    return t.hour*60 + t.min;
}

//������ת��Ϊʱ��
Time toTime(int t){
    Time s;
    s.hour = t/60;
    s.min = t % 60;
    return s;
}

//���ݵ���ʱ��Ƚ�
bool timeCompare(node a, node b){
    return toMinute(a.arrive) < toMinute(b.arrive);
}

//��������ʱ����бȽ�
bool runtimeCompare(node a, node b){
    return a.zx < b.zx;
}

//������Ӧ�Ƚ��бȽ�
bool respsonseCompare(node a, node b){
    return a.response > b.response;
}

void printFinished(list<node>& finishedJobs, int choose) {
    if (choose == 1) {
        cout << "ģ����ҵFCFS���ȹ��̵���������" << endl;
    }
    else if (choose == 2) {
        cout << "ģ����ҵSJF���ȹ��̵���������" << endl;
    }
    else if (choose == 3) {
        cout << "ģ����ҵHRRF���ȹ��̵���������" << endl;
    }

    printf("����\t�뾮ʱ��\t����ʱ��\t��ҵ����ʱ��\t��ҵ���ȵȴ�ʱ��\t���̵���ʱ��\t���̵��ȵȴ�ʱ��\t���ʱ��\t��תʱ��\t��Ȩ��תϵ��\n");
    for (auto it = finishedJobs.begin(); it != finishedJobs.end(); it++) {
        printf("%s\t%d:%02d\t\t%d(����)\t%d:%02d\t\t%d(����)  \t\t%d:%02d\t\t%d(����)\t\t\t%d:%02d\t\t%d(����)\t%.4f\n",
               it->name.c_str(), it->arrive.hour, it->arrive.min, it->zx, it->JobTime.hour, it->JobTime.min, it->JobWait,
               it->ProcessTime.hour, it->ProcessTime.min, it->ProcessWait, it->finish.hour, it->finish.min, it->zz, it->zzxs);
    }
    printf("ϵͳƽ����תʱ��Ϊ��%.2f\n", avgZZ);
    printf("ϵͳƽ����Ȩ��תʱ��Ϊ��%.4f\n", avgZZXS);
}

void FCFS(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while(!readyJobs.empty()){
        //�����������е�һ����ҵ����ֹ������
        //�������ʱ��
        //���½�����Ϣ�͵�ǰʱ��
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);//��ҵ���ȵȴ�ʱ��
        t.ProcessWait = 0;//���̵��ȵȴ�ʱ��
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz * 1.0 / t.zx;
        //���뵽��ɶ���
        finishedJobs.push_back(t);
        //����ƽ����תʱ���ϵ��
        avgZZ += t.zz * 1.0 / n;
        avgZZXS += t.zzxs / n;


        //���¾�������
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
    while (!readyJobs.empty()) {	//�жϾ��������Ƿ�Ϊ��
        //���½�����Ϣ�͵�ǰʱ��
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);
        t.ProcessWait = 0;
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz * 1.0 / t.zx;
        //���뵽��ɶ���
        finishedJobs.push_back(t);
        //����ƽ����תʱ���ϵ��
        avgZZ += t.zz * 1.0 / n;
        avgZZXS += t.zzxs / n;

        //�Ժ󱸶����е���ʱ��ȵ�ǰʱ��С����ҵ���ٰ���Ӧ������
        list<node> s;
        //�ҳ�����ʱ��ȵ�ǰʱ��С����ҵ
        while (!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)) {
            s.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
        //���������������е���Ӧ��
        //�����ǰ���Ӧ�ȴӴ�С����
        //�����Ǽ��뵽��������
        readyJobs.insert(readyJobs.end(), s.begin(), s.end());
        for (auto it = readyJobs.begin(); it != readyJobs.end(); it++) {
            //��Ӧ�� = 1 + ��ҵ�ȴ�ʱ�� / ��ҵ����ʱ��
            it->response = 1 + (toMinute(nowTime) - toMinute(it->arrive)) * 1.0 / it->zx;
        }
        readyJobs.sort(respsonseCompare);
    }

}

int main()
{
    int operate,num,option;
    char c;		//��ȡ�ַ�
    cout << "���������(1:��ʼ��ҵ����; 0:������ҵ):";
    cin >> option;
    while(option!=0){
        avgZZ = avgZZXS = 0;
        cout << "����������Ҫ��������ҵ��:";
        cin >> num;
        list<node> readyJobs;	//��������
        list<node> backupJobs;	//�󱸶���
        list<node> finishedJobs;//����ɶ���
        cout << "����������" << endl;
        cout << "��ҵ��\t�뾮ʱ��\t����ʱ��(����)" << endl;
        for(int i = 0; i < num; i++){
            node job;
            cin >> job.name >> job.arrive.hour >> c >> job.arrive.min >> job.zx;
            backupJobs.push_back(job);
        }
        //�ȸ��ݵ���ʱ���������
        backupJobs.sort(timeCompare);
        nowTime = backupJobs.front().arrive;
        //��ʼ����������
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
        cout << "���������(1:��ʼ��ҵ����; 0:������ҵ):";
        cin >> option;
    }
//    std::cout << "Hello, World!" << std::endl;
	getchar();
    return 0;
}
*/

//����������ϵͳ �α�P99 6
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
	int jid;	//��ҵid
	char name[10];	//��ҵ��
	int arriveTime;	//����ʱ��
	int needrunTime;	//Ԥ������ʱ��
	int needMemory;		//��Ҫ�ڴ�
	int needTypeDrive;	//��Ҫ�Ŵ�������
	int alreadyRunTime;	//������ʱ��
	int beginTime;		//��ʼ����ʱ��
	int finishTime;		//���ʱ��
	int startMemary;	//�ڴ�ռ�ÿ�ʼλ��
	int useTypeDriveid[4];	//ʹ�ôŴ���id
	struct jcb *next;
};
typedef struct jcb JCB;
JCB *InputWell = NULL;//���뾮
JCB *arriveJobList = NULL;//������ҵ
JCB *allocatedJobList = NULL;//�ѷ�����ҵ��
JCB *run = NULL;
JCB *finish = NULL;

struct memary
{
	int mid;	//����id
	int begin;	//��ʼλ��
	int length;	//��������
	char status[5];	//״̬��FREE �� BUSY
	struct memary *pre;
	struct memary *next;
};
typedef struct memary Memary;
Memary *memaryHead = NULL;

struct typedrive
{
	int did;	//�Ŵ���id
	char status[5];	//״̬��FREE �� BUSY
	struct typedrive *next;
};
typedef struct typedrive TypeDrive;
TypeDrive *driveHead=NULL;//�����б�

int jobNum = 0;
int time = 0;//ʱ��

int readJobList(); //��ȡ��ҵ��
void initJobList();//��ʼ����ҵ��
void initMemary(); //��ʼ���ڴ�
void initTypeDrive(); //��ʼ���Ŵ���
int arrive();//����
int allocate(JCB *target);//�����ڴ�ʹ���
JCB* deleteJob(JCB *list,JCB *target);//��������ɾ����ҵ
int addJobToList(JCB *list, JCB *target);//����ҵ��ӽ�����
int FirstPartition(JCB *target);//�ײ����ȷ���
int recycle(JCB *target);//�������պʹŴ�����
int FCFSJS();//��ҵ���������ȷ���
int SJSJS(); //��ҵ������С��ҵ����
int FCFSPS(); //���̵��������ȷ���
int SJFPS(); //���̵��������ҵ����
void showJobList(JCB *list);
void showMemary();
void showTypeDrive();

int readJobList()//�ļ���ȡ��ȡ��ҵ�б�
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
	arriveJobList = (JCB*)getpch(JCB);//������ҵ��
	arriveJobList->next = NULL;
	allocatedJobList = (JCB*)getpch(JCB);//�ѷ�����ҵ��
	allocatedJobList->next = NULL;
	run = (JCB*)getpch(JCB);//���б�
	run->next = NULL;
	finish = (JCB*)getpch(JCB);//��ɱ�
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
			job = deleteJob(InputWell, job);//�����뾮��ɾ�� ���뵽���������
			addJobToList(arriveJobList, job);
			job = NULL;
			free(job);
			return 1;
		}
		job = job->next;
	}
	return 0;
}

int allocate(JCB * target)//Ϊ��ҵ������Դ
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
			isMemaryEnough = 1;//�ڴ��㹻����
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
		if (strcmp(q->status,FREE)==0)//�ж������м�̨�Ŵ����ǿ��е�
		{
			countDrive++;
		}
		q = q->next;
	}
	if (countDrive < needdrive)
		return 0;
	JCB *job = deleteJob(arriveJobList, target);//�ڴ�ʹŴ������� ����ҵ���뵽�ѷ�����Դ�Ķ�����
	addJobToList(allocatedJobList, job);
	FirstPartition(job);//�ײ����ȷ���
	q = driveHead;
	for (int i = 0; i < 4; i++)//����Ŵ���
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

int FirstPartition(JCB *job)//�ڴ� �ײ����ȷ���
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
			printf("�ڴ����ɹ�!");
			return 1;
		}
		p = p->next;
	}
	printf("�ڴ����ʧ��!");
	return 0;
}

int recycle(JCB *target)//�������պ��ڴ����
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
		printf("``````````�ڴ�ʹŴ������ճɹ�!``````````");
		return 1;
	}
	else
	{
		printf("``````````�ڴ�ʹŴ�������ʧ��!``````````");
		return 0;
	}
}

int FCFSJS()//�����ȷ�����ҵ���� �Ե�����ҵ����
{
	JCB *first = arriveJobList->next;
	if (first==NULL)
		return 0;
	else return allocate(first);
}

int SJFJS()//����ҵ������ҵ���� 
{
	JCB *temp = arriveJobList->next;
	if (temp == NULL)
		return 0;
	int minNum = 999;//����ʼ�����ҵʱ����Ϊһ���ܴ��ֵ ��������Ƚ�
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

int FCFSPS()//���̵��� �����ȷ��� ���ѷ�����Դ�Ķ���
{
	if (run->next!=NULL)
		return 0;
	JCB *first = allocatedJobList->next;
	if (first == NULL)
		return 0;
	JCB *target=deleteJob(allocatedJobList, first);
	addJobToList(run, target);
	target->beginTime = time;
	printf("\n����%s��ʼ����\n", target->name);
	return 1;
}

int SJFPS()//���̵��� ����ҵ���� ���ѷ�����Դ�Ķ���
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
	if (run->next==NULL)//�����ж�����ֻ��һ��ʱ ֱ������
	{
		JCB *target = deleteJob(allocatedJobList, minJob);
		addJobToList(run, target);
		if (target->beginTime==-1)
		{
			target->beginTime = time;
		}
		printf("\n����%s��ʼ����\n", target->name);
		return 1;
	}
	else if (minJob->needrunTime < run->next->needrunTime)//�����ж���������ʱ ��ռʽ ����ҵ����
	{
		JCB *target = deleteJob(allocatedJobList, minJob);
		JCB *pre = deleteJob(run, run->next);
		addJobToList(run, target);
		addJobToList(allocatedJobList,pre);
		if (target->beginTime == -1)
		{
			target->beginTime = time;
		}
		printf("\n����%s��ʼ����\n", target->name);
		printf("\n����%s��ͣ����\n", pre->name);
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
	printf("** ��ҵ�� | ����ʱ�� | ��������ʱ�� | �ڴ���Ҫ | �Ŵ�����Ҫ |  ��ʼ����ʱ�� | ������ʱ�� |���ʱ�� ");
	while (temp!=NULL)
	{
		printf("\n**-----------------------------------------------------------------------------------------------\n");
		printf("**");
		printf("  %s  |    %3d   |     %3d      |    %3d       |    %3d    |",temp->name,temp->arriveTime,temp->needrunTime,temp->needMemory,temp->needTypeDrive);
		if (temp->beginTime==-1)
		{
			printf("   δ��ʼ���� |   δ��ʼ���� |");
		}
		else
		{
			printf("  %3d   | %3d      |", temp->beginTime,temp->alreadyRunTime);
		}
		if (temp->finishTime==-1)
		{
			printf("δ�������� ");
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
		printf("\nƽ����תʱ��Ϊ��%lf", 1.0*TurnaroundTime / jobNum);
	printf("\n````````````````````````````````````````````````````````\n");
}

void showMemary()
{
	printf("\n````````````````````````````````````````````````````````\n");
	printf("``````````��ǰ�ڴ�����������``````````\n");
	printf("````` ��ʼ��ַ | �ռ��С | ״̬  `````\n");
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
	printf("``````````````````��ǰ�Ŵ���ռ���������``````````````````\n");
	printf("`` �Ŵ���id | ״̬  ``\n");
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
	printf("\n```````````````��ǰ���뾮��ҵ�������``````````````````\n");
	showJobList(InputWell);
	while (1)
	{
		printf("-------------��ѡ��Ҫģ�����ҵ���ȷ����㷨:\n");
		printf("\t\t          0--�����ȷ����㷨  \n");
		printf("\t\t          1--��С��ҵ�����㷨  \n");
		printf("������ţ�\n");
		scanf("%d", &JS);
		if (JS == 0)
		{
			printf("----------��ѡ���������ȷ����㷨\n");
		}
		else if (JS == 1)
		{
			printf("----------��ѡ������С��ҵ�����㷨\n");		
		}
		else
		{
			printf("����:���� 0|1\n");
			break;
		}
		printf("-------------ѡ��Ҫģ��Ľ��̵��ȷ����㷨:\n");
		printf("\t\t          0--�����ȷ����㷨  \n");
		printf("\t\t          1--��������㷨  \n");
		printf("������ţ�\n");
		scanf("%d", &PS);
		if (JS == 0)
		{
			printf("----------��ѡ���������ȷ����㷨\n");
		}
		else if (JS == 1)
		{
			printf("----------��ѡ������������㷨\n");
		}
		else
		{
			printf("����:���� 0|1\n");
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
			printf("\n``````````````��ǰ�ڴ�����ҵ�������`````````````````\n");
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
			printf("\n```````````````��ǰ���н����������``````````````````\n");
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

	printf("\n```````````````�����ҵ�������`````````````````````\n");
	showJobList(finish);

	system("pause");
	return 0;
}