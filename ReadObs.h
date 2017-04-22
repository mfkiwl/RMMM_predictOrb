/****************************************************************************
目的：    从Rinex格式的数据中读取GPS观测值,以后可修改为从星载接收机中实时获取

编写时间：2008.12.3
版本:     V1.1
版权：    武汉大学
****************************************************************************/
#pragma once 
#ifndef _READ_OBSDATA_H_
#define _READ_OBSDATA_H_

#include <stdio.h>
#include "GPSTime.h"


/***************************************************************************
//
// EmptyEpochObsDataStruct
//
// 目的: 初始化EpochObsData结构体, 在每次读数据之前调用
//
// 参数:
//
//  Epoch    待清空的观测数据

***************************************************************************/

void EmptyEpochObsDataStruct( EpochObsData* Epoch );

/***************************************************************************
//
// EmptyPSPRangeStruct
//
// 目的: 初始化PSPRange结构体, 在每次读数据之前调用
//
// 参数:
//
//  Epoch    待清空的观测数据

***************************************************************************/

void EmptyPSPRangeStruct( PSPRange PSPR[] );

/***************************************************************************
//
// ReadObsHead
//
// 目的: 读取Rinex文件头信息, 对于星载接收机的观测数据, 不使用文件头中的
任何信息.
//
// 参数:
//
//  fin   观测数据文件指针
//  ObsTypeList   观测数据类型的顺序列表
//
// 返回值:
// 
// 如果文件读取有问题, 返回0, 否则返回1
***************************************************************************/

int ReadObsHead( FILE* fin, OBSTYPELIST* ObsTypeList );

/***************************************************************************
//
// ReadEpochObs
//
// 目的: 读取一个历元的观测数据
//
// 参数:
//
//  fin           观测数据文件指针
//  ObsTypeList   观测数据类型的顺序列表
//  Epoch         一个历元的观测数据
//
// 返回值:
// 
// 如果文件读取有问题, 返回0, 否则返回1
***************************************************************************/

int ReadEpochObs( FILE* fin, OBSTYPELIST* ObsTypeList, EpochObsData* Epoch );

/***************************************************************************
//
// SmoothPRangeWithPhase
//
// 目的: 用载波相位观测值平滑伪距观测值
//
// 参数:
//
//  PSPR  相位平滑伪距的中间结果
//  Epoch 一个历元的观测数据
//
// 注释:
// 
//   即使有双频观测数据，也采用单频伪距的平滑方法。考虑到电离层折射的影响，
//   平滑采用较短的时间段。超过时间限制，重新启动平滑器。

//    平滑的观测值覆盖历元观测数据的所有伪距，更新PSPR。
//    使用简单的周跳探测方法，检验相位数据和伪距数据的一致性。
***************************************************************************/

void SmoothPRangeWithPhase( PSPRange PSPR[], EpochObsData* Epoch );
void ResetPhaseSmoothor( SatObsData* data, PSPRange* PSPR );

/***************************************************************************
//
// CreateDopplerObs
//
// 目的: 用载波相位中心差分近似法计算Doppler观测值
//
// 参数:
//
//  Epoch[3]         三个历元的观测数据
//
// 返回值:
// 
// 如果文件读取有问题, 返回0, 否则返回1
***************************************************************************/

int CreateDopplerObs( EpochObsData Epoch[3] );

bool GetEpochSatL1( GNSSSys sys, short prn, EpochObsData* data, double* obs );

/***************************************************************************
//
// GetOneSatPseudoRange
//
// 目的: 获取某颗卫星的伪距观测值，若是双频，使用组合观测值（C1+P2），单频返回C1
//
// 参数:

Sys        卫星导航系统
data       卫星的观测数据
pr         伪距观测值
Ion        双频伪距组合计算的电离层改正值

//
// 返回值:
// 
// 如果返回1, 单频C1码；返回2，双频组合伪距，返回0，表示有粗差
***************************************************************************/

int GetOneSatPseudoRange(  GNSSSys Sys, SatObsData* data, double* pr, double *Ion );

/***************************************************************************
//
// GetOneSatPhase
//
// 目的: 获取某颗卫星的载波观测值，若是双频，使用组合观测值（L1+L2），单频返回L1
//
// 参数:

data       卫星的观测数据
ps         载波观测值

//
// 返回值:
// 
// 如果返回1, 单频L1；返回2，双频组合载波，返回0，表示两个频率观测值都不存在
***************************************************************************/
int GetOneSatPhase( GNSSSys Sys, SatObsData* data, double* ps);
//读取周跳与粗差探测后的观测文件，其中LLI为1表示发生周跳，LLI为9表示为粗差
int ReadEpochObsQC( FILE* fin, OBSTYPELIST* ObsTypeList, EpochObsData* Epoch );
//读取QC探测后的观测文件头
int ReadObsHeadQC( FILE* fin, OBSTYPELIST* ObsTypeList );
#endif