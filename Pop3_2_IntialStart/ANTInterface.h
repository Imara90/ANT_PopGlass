/** \file ANTInterface.h
*
* Acts as a intermediate between the application layer and the low level
* serial port drivers
*
* $Name:  $
*/
/*******************************************************************************
* Copyright (c) 2006 Dynastream Innovations Inc.
* THE FOLLOWING EXAMPLE CODE IS INTENDED FOR LIMITED CIRCULATION ONLY.
* 
* Please forward all questions regarding this code to ANT Technical Support.
* 
* Dynastream Innovations Inc.
* 228 River Avenue
* Cochrane, Alberta, Canada
* T4C 2C1
* 
* (P) (403) 932-9292
* (F) (403) 932-6521
* (TF) 1-866-932-9292
* (E) support@thisisant.com
* 
* www.thisisant.com
*
*******************************************************************************/

#ifndef ANTINTERFACE_H
#define ANTINTERFACE_H

#include "types.h"

void  ANTInterface_Init(void);
void  ANTInterface_MesgProcess(UCHAR *pucTxData);
UCHAR *ANTInterface_GetPendingEvent(void);
BOOL  ANTInterface_ChannelConfig(USHORT *appucChannelConfig, UCHAR *pucConfigSize, UCHAR ucNumChannels);

#endif
