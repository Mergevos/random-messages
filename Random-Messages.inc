// RandomMessages.inc by Emmet

/*
	native AddRandomMessage(const message[], color = -1);
	native SetMessageInterval(time);
	native IsValidRandomMessage(messageid);
	native SetRandomMessagesForPlayer(playerid, bool:status);
	native bool:GetRandomMessagesForPlayer(playerid);
	native GetRandomMessageID();
	native SendRandomMessage(playerid, messageid);
	native GetRandomMessageOutput(playerid, messageid, dest[], size = sizeof(dest));
	native GetTotalRandomMessages();
*/

#if !defined MAX_MESSAGE_QUEUE
	#define MAX_MESSAGE_QUEUE (12)
#endif

#if !defined MAX_MESSAGE_LENGTH
	#define MAX_MESSAGE_LENGTH (128)
#endif

#if !defined INVALID_MESSAGE_ID
	#define INVALID_MESSAGE_ID (-1)
#endif

#define Replace->$%0(%1) \
	forward rm_%0(%1); public rm_%0(%1)

#define SetMessageInterval(%0) \
	(SetTimer(!"_RM_Process", (%0), true))

static
    bool:g_bRandomMessageException[MAX_PLAYERS char],
	g_sRandomMessageQueue[MAX_MESSAGE_QUEUE][MAX_MESSAGE_LENGTH char],
	g_iRandomMessageColor[MAX_MESSAGE_QUEUE],
	g_iCurrentQueueIndex,
	g_iNextQueueIndex
;

static const
	g_iMessageLength = MAX_MESSAGE_LENGTH
;

forward OnRandomInterval();

stock AddRandomMessage(const message[], color = -1)
{
	if (!(0 <= g_iNextQueueIndex < MAX_MESSAGE_QUEUE))
	    return 0;

	strpack(g_sRandomMessageQueue[g_iNextQueueIndex], message, MAX_MESSAGE_LENGTH char);

	g_iRandomMessageColor[g_iNextQueueIndex++] = color;

	return 1;
}

stock SetRandomMessagesForPlayer(playerid, bool:status)
{
    return g_bRandomMessageException{playerid} = !status;
}

stock bool:GetRandomMessagesForPlayer(playerid)
{
	return g_bRandomMessageException{playerid};
}

stock GetRandomMessageOutput(playerid, messageid, dest[], size = sizeof(dest))
{
    if (!IsValidRandomMessage(messageid))
        return 0;

	new
		result[MAX_MESSAGE_LENGTH],
		function[32],
		address,
		index;

	strunpack(dest, g_sRandomMessageQueue[messageid], size);

	for (new i = 0, l = strlen(dest); i != l; i ++)
	{
	    if (dest[i] == '[' && dest[i + 1] == '$')
		{
		    strmid(function, dest, i + 2, strfind(dest, "]", true, i + 2));

		    strins(function, "rm_", 0);

			if ((index = funcidx(function)) != -1)
			{
			    // In this case, we have to call the function manually.
		    	// This is because we can't pass referenced strings to CallLocalFunction.
				#emit LCTRL 1
				#emit STOR.S.pri address
				#emit NEG
				#emit ADD.C 32
				#emit STOR.S.pri address
				#emit LREF.S.alt address
				#emit LCTRL 1
			 	#emit NEG
				#emit ADD
				#emit LOAD.S.alt index
				#emit SHL.C.alt 3
				#emit ADD
				#emit STOR.S.pri address
				#emit LREF.S.pri address
				#emit STOR.S.pri address

				// Push the parameters onto the stack.
				#emit PUSH.C g_iMessageLength
				#emit PUSH.adr result
				#emit PUSH.S playerid
				#emit PUSH.C 12

				// Call the function manually.
				#emit LCTRL 6
				#emit ADD.C 28
				#emit PUSH.pri
				#emit LOAD.S.pri address
				#emit SCTRL 6

				strdel(dest, i, strfind(dest, "]", true, i + 2) + 1);

				strins(dest, result, i, size);
			}
		}
	}
	return 1;
}

stock SendRandomMessage(playerid, messageid)
{
    if (!IsValidRandomMessage(messageid))
        return 0;

	new
	    string[MAX_MESSAGE_LENGTH * 2];

	GetRandomMessageOutput(playerid, messageid, string, sizeof(string));

	return SendClientMessage(playerid, g_iRandomMessageColor[messageid], string);
}

stock GetRandomMessageID()
{
	new index = INVALID_MESSAGE_ID;

	if (g_iNextQueueIndex)
		index = random(g_iNextQueueIndex);

	return index;
}

stock IsValidRandomMessage(messageid)
{
	return (0 <= messageid < MAX_MESSAGE_QUEUE) && (g_sRandomMessageQueue[messageid]{0});
}

stock GetTotalMessages()
{
	return g_iNextQueueIndex;
}

forward _RM_Process();
public _RM_Process()
{
	if (g_iNextQueueIndex)
	{
	 	if (g_iCurrentQueueIndex >= g_iNextQueueIndex)
	 		g_iCurrentQueueIndex = 0;

		for (new i = 0, l = GetMaxPlayers(); i != l; i ++)
		{
	    	if (IsPlayerConnected(i) && !g_bRandomMessageException{i})
		    {
		        SendRandomMessage(i, g_iCurrentQueueIndex);
			}
		}
		g_iCurrentQueueIndex++;

		CallLocalFunction("OnRandomInterval", "");
	}
}
