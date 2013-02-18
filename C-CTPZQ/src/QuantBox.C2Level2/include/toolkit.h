#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "Level2\level2_ns.h"
#include "Level2\ThostFtdcLevel2UserApiStruct.h"

#include <vector>
#include <set>
#include <string>

using namespace std;

_USING_LEVEL2_NS_

//输入路径，生成多级目录
void makedirs(const char* dir);

//将字符串按指定字符分割
void GetSetFromString(const char* szString,const char* seps,vector<string>& vct,set<string>& st);

//根据OnFrontDisconnected(int nReason)的值填上错误消息
void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

#endif