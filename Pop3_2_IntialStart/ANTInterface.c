/** \file ANTInterface.c
*
* Acts as a intermediate between the application layer and the low level
* serial port driver 
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

#include "types.h"
///#include "BitSyncSerial.h"
#include "antmessage.h"
#include "antdefines.h"


// Rx Message Buffer
// old ways in c
//__no_init static UCHAR aucRxMesgBuffer[MESG_BUFFER_SIZE];                         ///< Buffer used to hold messages received from the ANT chip 
//__no_init static BOOL  bPendingRxMessage;                                         ///< Flag used to indicate that a message has been received
static UCHAR aucRxMesgBuffer[MESG_BUFFER_SIZE] __attribute__ ((section (".noinit")));
static BOOL bPendingRxMessage __attribute__ ((section (".noinit")));


//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
/// ANTInterface_Init
///
/// This function initialize the ANTInterface module
///
/////////////////////////////////////////////////////////////////////////////
void ANTInterface_Init(void)
{
   // Intialize the serial port driver
   BitSyncSerial_Init();

   bPendingRxMessage = FALSE;                                                     // clear pending receive message flag
}

//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
/// ANTInterface_MesgProcess
///
/// This function is called to send/receive message to/from the ANT chip
///
/// \param pucTxData - pointer to the message to be transmitted, if there is no transmit message this can be set to NULL
/////////////////////////////////////////////////////////////////////////////
void ANTInterface_MesgProcess(UCHAR *pucTxData)
{
   if (BitSyncSerial_Transaction(pucTxData, aucRxMesgBuffer))                     // process receive and transmit messages
      bPendingRxMessage = TRUE;                                                   // we have a new receive message to process
}

//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
/// ANTInterface_GetPendingEvent
///
/// This function is used to retrive messages that were received from the ANT chip
/// 
/// \return The return value is a pointer to the received message, if no message is received the return value will be NULL
/////////////////////////////////////////////////////////////////////////////
UCHAR *ANTInterface_GetPendingEvent(void)
{
   if (bPendingRxMessage)                                                         // if we have a pending messasge
   {
      bPendingRxMessage = FALSE;                                                  // clear the pending message flag
      return aucRxMesgBuffer;                                                     // return message pointer
   }

   return NULL;                                                                   // otherwise return NULL (no message to process)
}

//========================== ANT Interface Channel Config ===================
/////////////////////////////////////////////////////////////////////////////
/// ANTInterface_ChannelConfig
///
/// This function is used to reset and configure the ANT chip with the desired 
/// ANT channels and parameters.  
///
/// \param appucChannelConfig - pointer to an array of pointers to the config command block for each channel
/// \param pucConfigSize - pointer to an array of UCHARs that indicate the size of each of the config command blocks
/// \param ucNumChannels - the number of channels (config command blocks) appucChannelConfig contains
/// \return Boolean value indicating weather or not the entire configureation was set sucessfully
/////////////////////////////////////////////////////////////////////////////
BOOL ANTInterface_ChannelConfig(USHORT *appucChannelConfig, UCHAR *pucConfigSize, UCHAR ucNumChannels)
{
   UCHAR ucCurConfigMesg;
   UCHAR ucCurChannel;
   BOOL  bResponse;

   // Perform sync serial reset
   while(!BitSyncSerial_SyncReset());

   ucCurConfigMesg = 0;
   ucCurChannel    = 0;                  
   // Send the first config command
   bResponse       = BitSyncSerial_Transaction(((UCHAR***)appucChannelConfig)[ucCurChannel][ucCurConfigMesg], aucRxMesgBuffer);  

   // State Machine for performing channel setup
   while (1)
   {                                 
      // wait for a response                
      if (!bResponse)                                                             // if we already have a response then skip
         bResponse = BitSyncSerial_Transaction(NULL, aucRxMesgBuffer);            // otherwise wait for a response

      // check for a correct response  
      if ((bResponse) &&                                                          // if we got a response
          (aucRxMesgBuffer[BUFFER_INDEX_MESG_ID] == MESG_RESPONSE_EVENT_ID) &&    // and it's a response message
          (aucRxMesgBuffer[BUFFER_INDEX_RESPONSE_MESG_ID] == ((UCHAR***)appucChannelConfig)[ucCurChannel][ucCurConfigMesg][BUFFER_INDEX_MESG_ID])) // and if this is the right response message
      {
         if (aucRxMesgBuffer[BUFFER_INDEX_RESPONSE_CODE] != RESPONSE_NO_ERROR)    // check if our config message was successful
            return FALSE;                                                         // our config message failed so restart

         if (++ucCurConfigMesg >= pucConfigSize[ucCurChannel])                    // if we've run all the channel config commands for this channel
         {                                                                      
            if (++ucCurChannel >= ucNumChannels)                                  // go to next channel and check if we are finished
               return TRUE;                                                       // we are finished so exit

            ucCurConfigMesg = 0;                                                  // reset the channel config message number
         }

         // send out the next config command
         bResponse = BitSyncSerial_Transaction(((UCHAR***)appucChannelConfig)[ucCurChannel][ucCurConfigMesg], aucRxMesgBuffer);  
      }
      else
      {
         bResponse = FALSE;                                                       // we either didn't get a response or we got the wrong response, so clear the response flag
      }
   } 
}
