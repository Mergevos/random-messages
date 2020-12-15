// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_random_messages
    #undef _inc_random_messages
#endif
// custom include-guard to ensure we don't duplicate
#if defined _rndm_included
    #endinput
#endif
#define _rndm_included

#if !defined MAX_MESSAGE_GET_STRING
    const MAX_MESSAGE_GET_STRING = 144;
#endif

#if !defined MAX_RECEIVERS
    #define MAX_RECEIVERS MAX_PLAYERS
#endif

#include <a_samp>
#include <PawnPlus>

#if defined RM_ALIAS_FUNCTIONS
    #define RandomMessage_Add AddRandomMessage
    #define RandomMessage_Get GetRandomMessage
    #define RandomMessage_SendClient SendClientRandomMessage
    #define RandomMessage_SendToAll SendClientRandomMessageToAll
    #define RandomMessage_GetNumber GetNumberOfRandomMessages
    #define RandomMessage_SendCliSpecific SendClientSpecificRandomMessage
    #define RandomMessage_SendToAllSpecific SendSpecificRandomMessageToAll
    #define RandomMessage_SetPlayerStatus SetPlayerRandomMessageStatus
    #define RandomMessage_GetPlayerStatus GetPlayerRandomMessageStatus
#endif

static 
    Pool: g_MessageList;

//----------------------------------------------------

forward _pp@on_init@rnd_msgs();
public _pp@on_init@rnd_msgs()
{
    g_MessageList = pool_new();
    print("Random Messages by Mergevos loaded.");
}

//--------------------------------------------------------

forward _pp@on_exit@rnd_msgs();
public _pp@on_exit@rnd_msgs()
{
    pool_delete(g_MessageList);
    print("Random Messages by Mergevos unloaded!");
}

//--------------------------------------------------------

/**
 * <summary> Sets player random message status. </summary>
 * <param name="playerid"> Player ID to get message.</param>
 * <param name="status"> Status to set. True or false.</param>
 */

stock RandomMessage_SetPlayerStatus(playerid, bool:status) {
    SetPVarInt(playerid, "_RndMessageStatus_", status ? 1 : 0);
}

/**
 * <summary> Gets player random message status. </summary>
 * <param name="playerid"> Player ID to get message.</param>
 * <returns> Returns player's random message status. True or false.</returns>
 */

stock bool:RandomMessage_GetPlayerStatus(playerid) {
    return GetPVarInt(playerid, "_RndMessageStatus_") == 1 ? true : false;
}


/**
 * <summary> Sends specific random message to a client </summary>
 * <param name="message">Message text to add. </param>
 */

stock RandomMessage_Add(const message[]) {
    pool_add_str(g_MessageList, message);
}

/**
 * <summary> Removes specific random message. </summary>
 * <param name="messageid">Message to delete. </param>
 */

stock RandomMessage_Delete(messageid) {
    new const ret = pool_remove(Pool:pool, messageid);
    if(!ret) {
        printf("random-messages: Messageid %d doesn't exist.", messageid)
    }
}
/**
 * <summary> Sends specific random message to a client </summary>
 * <param name="messageid">Message ID to send. </param>
 * <param name="message"> Variable to store message output.</param>
 * <returns> SendClientMessage return, 1 on succes, 0 on failure. </returns>
 */

stock RandomMessage_Get(messageid, message[], len = sizeof(message)) {
    if(pool_has(g_MessageList, messageid)) {
        pool_get_str(g_MessageList, messageid, message, len);
        return 1;
    } 
    print("random-messages: Invalid message ID");
    return -1;
}

/**
 * <summary> Sends a random message to all </summary>
 * <param name="time"> Time after messages being sent. </param>
 * <param name="times"> How many times the message will be sent.</param>
 * <returns> SendClientMessageToAll return, 1 on succes, 0 on failure. </returns>
 */

stock RandomMessage_SendToAll(time = 0, times = 1) {
    new 
        msg_str[MAX_MESSAGE_GET_STRING],
        ret;
    
    if(time == 0 && times == 1) {
        RandomMessage_Get(random(pool_size(g_MessageList)), msg_str);
        ret = SendClientMessageToAll(-1, msg_str);
    } else {
        while(times != 0) {
            wait_ms(time);
            RandomMessage_Get(random(pool_size(g_MessageList)), msg_str);
            ret = SendClientMessageToAll(-1, msg_str);
            times--;
        }
    }
    return ret;
} 

/**
 * <summary> Sends a random message to a client </summary>
 * <param name="playerid"> Player ID to get message.</param>
 * <param name="time"> Time after messages being sent. </param>
 * <param name="times"> How many times the message will be sent.</param>
 * <returns> SendClientMessage return, 1 on succes, 0 on failure. </returns>
 */


stock RandomMessage_SendClient(playerid, time = 0, times = 1) {
    new 
        msg_str[MAX_MESSAGE_GET_STRING],
        ret;
    
    if(RandomMessage_GetPlayerStatus(playerid) == false) {
        return 0;
    }

    if(time == 0 && times == 1) {
        RandomMessage_Get(random(pool_size(g_MessageList)), msg_str);
        ret = SendClientMessage(playerid, -1, msg_str);
    } else {
        while(times != 0) {
            wait_ms(time);
            RandomMessage_Get(random(pool_size(g_MessageList)), msg_str);
            ret = SendClientMessage(playerid, -1, msg_str);
            times--;
        }
    }
    return ret;
} 

/**
 * <summary> Returns the number of created random messages </summary>
 * <returns> Messages' pool size. </returns>
 */

stock RandomMessage_GetNumber()  {
    return pool_size(g_MessageList);
}

/**
 * <summary> Sends specific random message to a client </summary>
 * <param name="messageid">Message ID to send. </param>
 * <param name="playerid"> Player ID to get message.</param>
 * <param name="time"> Time after messages being sent. </param>
 * <param name="times"> How many times the message will be sent.</param>
 * <returns> SendClientMessage return, 1 on succes, 0 on failure. </returns>
 */

stock RandomMessage_SendCliSpecific(messageid, playerid, time = 0, times = 1) {
    new 
        msg_str[MAX_MESSAGE_GET_STRING],
        ret;
    if(RandomMessage_GetPlayerStatus(playerid) == false) {
        return 0;
    }
    if(time == 0 && times == 1) {
        RandomMessage_Get(messageid, msg_str);
        ret = SendClientMessage(playerid, -1, msg_str);
    } else {
        while(times != 0) {
            wait_ms(time);
            RandomMessage_Get(messageid, msg_str);
            ret = SendClientMessage(playerid, -1, msg_str);
            times--;
        }
    }
    return ret;
}

/**
 * <summary> Sends specific random message to all. </summary>
 * <param name="messageid">Message ID to send. </param>
 * <param name="time"> Time after messages being sent. </param>
 * <param name="times"> How many times the message will be sent.</param>
 * <returns> SendClientMessageToAll return, 1 on succes, 0 on failure. </returns>
 */

stock RandomMessage_SendToAllSpecific(messageid, time = 0, times = 1) {
    new 
        msg_str[MAX_MESSAGE_GET_STRING],
        ret;

    if(time == 0 && times == 1) {
        RandomMessage_Get(messageid, msg_str);
        ret = SendClientMessageToAll(-1, msg_str);
    } else {
        while(times != 0) {
            wait_ms(time);
            RandomMessage_Get(messageid, msg_str);
            ret = SendClientMessageToAll(-1, msg_str);
            times--;
        }
    }
    return ret;
} 