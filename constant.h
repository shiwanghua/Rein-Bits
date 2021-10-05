//#pragma once
#ifndef _CONST_H
#define _CONST_H
#include<iostream>
using namespace std;

extern string expID;       // 记录实验id
extern bool display;       // 是否打印映射关系
extern const int interval; // 每匹配这么多个事件输出一条日志信息
extern const double subp;  // subp比例的订阅的谓词定义在前cons个维度上
const int subs = 1000000;  // Number of subscriptions.
extern const int pubs;     // Number of publications.
extern int atts;           // Total number of attributes, i.e. dimensions.
extern const int cons;     // Number of constraints(predicates) in one sub.
extern int m;              // Number of constraints in one pub.
extern const int attDis;   // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
extern const int valDis;   // The distribution of values in subs and pubs. 0:uniform
extern const int valDom;   // Cardinality of values.
extern const double alpha; // Parameter for Zipf distribution.
extern double width;       // Width of a predicate.

// Rein
extern const int buks;     // Number of buckets.

// BIOP
extern int be;             // 每个维度上 2 * 2^bits个bits数组, -1表示用桶数做为bits数组组数
extern const int lvls;     // 层数
extern const int gs;       // 一组有多少个维度

// Tama
extern const int level;
#endif
