#include "bci.h"

BCI::BCI()
{
    isConnect = false;
    isRunning = false;
    connectId = 0;
    attenlvl = 0;
    errcode = 0;
    DataDraw = 0;
    DataValue = 0;
    DataRec = 0;


}


QString BCI::OpenBCIconnection(void)
{
    QString cr;
    connectId = TG_GetNewConnectionId();
    const char* comPortName = "\\\\.\\COM4";

    if( connectId < 0 )
    {
        cr.sprintf("ERROR: TG_GetNewConnectionId() returned %d",connectId);
        return cr;
    }

    errcode = TG_Connect(connectId, comPortName,TG_BAUD_57600,TG_STREAM_PACKETS);

    if(errcode < 0)
    {
        cr.sprintf("ERROR: TG_Connect() returned %d",errcode);
        return cr;
    }

    cr.sprintf("BCI is Connected.  Reading Packets ");
    return cr;
}


QString BCI::CloseBCIconnection(void)
{
    QString cr;
    TG_FreeConnection(connectId);
    cr.sprintf("BCI is disconnected.");
    return cr;
}


void BCI::ReadBCItest(void)
{
    int data_type_EEG = 4;
    int data_type_att = 2;
    if(TG_ReadPackets(connectId,1)==1)
    {
        if( TG_GetValueStatus(connectId, data_type_EEG) != 0 )
        {
            rawEEG = TG_GetValue(connectId, data_type_EEG);
            //emit EEGvalue(rawEEG);
        }

        if( TG_GetValueStatus(connectId, data_type_att) != 0 )
        {
            attenlvl = TG_GetValue(connectId, data_type_att);
        }
    }
}



void BCI::ReadBCI(void)
{
    int data_type_EEG = 4;
    int data_type_att = 2;
    int datalength = 512*10+1;


    if(TG_ReadPackets(connectId,1)==1)
    {
        DataDraw = DataDraw + 1;
        DataValue = DataValue + 1;
        DataRec = DataRec + 1;


        if(DataRec == 513)
        {
            if( TG_GetValueStatus(connectId, data_type_att) != 0 )
            {
                attenlvl = TG_GetValue(connectId, data_type_att);
            }

            DataRec = 0;
        }
        else
        {
            if( TG_GetValueStatus(connectId, data_type_EEG) != 0 )
            {
                rawEEG = TG_GetValue(connectId, data_type_EEG);
            }
        }
    }


}


