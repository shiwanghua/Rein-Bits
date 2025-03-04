//#pragma once
#ifndef _REIN_BITS_H
#define _REIN_BITS_H

#include <cstring>
#include "Util.h"
#include "constant.h"
#include <algorithm>
#include <unordered_set>
#include <bitset>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

// 在Rein的基础上加上bitset数组缓存机制
class HEM
{
private:
	int numSub, numDimension, buckStep, numBits, bitStep; // 让前面的bits数组差距都是bitStep，多余的都留给最后一个bits数组
	vector<vector<vector<Combo>>> data[2];  // 0:left parenthesis, 1:right parenthesis
	vector<vector<bitset<subs>>> bits[2];   // 需要提前知道订阅个数...
	vector<bitset<subs>> fullBits;          // 全覆盖的bits单独存，因为只要存一次
	int* endBucket[2],*bitsID[2];           // 落入这个bucket的事件在标记时终止于哪一个bucket、用到的bits数组的下标
	//vector<vector<int>> fix[2];           // 0是low上的后缀和，1是high上的前缀和，可以用于计算任务量
public:
	int numBucket;
	double compareTime = 0.0;               // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;                  // 标记时间
	double orTime = 0.0;                    // 或运算时间
	double bitTime = 0.0;                   // 遍历bits数组得到结果所需的时间
	//vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	HEM();
	~HEM();

	//void insert(Sub sub);
	void insert(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(const Pub& pub, int& matchSubs);
	void match_debug(const Pub& pub, int& matchSubs);

	void initBits();      // 插入完后初始化bits数组
	//void calBucketSize(); // 计算bucketSize
	int calMemory();      // 计算占用内存大小
	void printRelation(); // 打印映射关系
	vector<int> calMarkNumForBuckets(); // 计算事件落到每个属性的同一个桶里时需要标记和比较的谓词个数
};

#endif