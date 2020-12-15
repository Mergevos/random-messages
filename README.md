# samp-multilanguage

[![Mergevos](https://img.shields.io/badge/Mergevos-samp--random--mesages-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/samp-random-messages)

It is originally created by Emmet_, but I've forked this and modified.

## Installation

Simply install to your project:

```bash
sampctl package install Mergevos/samp-random-messages
```

Include in your code and begin using the library:

```pawn
#include <random-messages>
```

## Usage

Library uses PawnPlus' dynamic containers, to precise, pools. Library has pre-defined library pool in which you add messages, that later should be send via following functions:

```c
RandomMessage_SendToAll(time = 0, times = 1)
RandomMessage_SendClient(playerid, time = 0, times = 1)
RandomMessage_SendCliSpecific(messageid, playerid, time = 0, times = 1)
RandomMessage_SendToAllSpecific(messageid, time = 0, times = 1) 
```

Param names are pretty self-explanatory, but for those who are unable to understand, here's what every one of them means:
`playerid` - Playerid which message will be sent to.
`messageid` - This param is only used in functions which sends specific message to a player/players.
`time` - Obviously, time after the messages being sent.
`times` -  How many messages you will send.

Also these functions are required to either add or remove functions and get output or set player status:

```c
RandomMessage_SetPlayerStatus(playerid, bool:status)
bool:RandomMessage_GetPlayerStatus(playerid)
RandomMessage_Add(const message[]) 
RandomMessage_Delete(messageid)
RandomMessage_Get(messageid, message[], len = sizeof(message))
RandomMessage_GetNumber() // Gets number of added random messages.
```
Param names are pretty self-explanatory, but for those who are unable to understand, here's what every one of them means:
`playerid` - Playerid whoses status will be set.
`messageid` - This param is only used in functions above which gets or deletes a message.
`message` - Used to add a message or to get message output.
`status` - Boolean to set status.


### Example

```
main() {
    new msg[126];
    RandomMessage_Add("Test");
    RandomMessage_Add("Test2");
    RandomMessage_Add("Test3");
    RandomMessage_Add("Test4");
    RandomMessage_Add("Test5");
    RandomMessage_Add("Test6");
    RandomMessage_Get(0, msg);
    
    printf("%s, max msgs: %d", msg, RandomMessage_GetNumber());
}

public OnPlayerSpawn(playerid) {
    RandomMessage_SendCliSpecific(5, playerid, 0, 6); //Shouldn't be sent
    printf("Status %s", RandomMessage_GetPlayerStatus(playerid) ? "true" : "false"); // should print false
    RandomMessage_SetPlayerStatus(playerid, true);
    
    printf("Status %s", RandomMessage_GetPlayerStatus(playerid) ? "true" : "false");
    RandomMessage_SendCliSpecific(5, playerid, 0, 6); //Should be sent
    return 1;
}
```

In the above example, we added 6 messages via `RandomMessage_Add` function and later, gets the output of the message on index 0. Expected output should be `Test, max msgs: 6`. When player spawns, we sent specific client message `RandomMessage_SendCliSpecific` 6 times instantly. It shouldn't be sent because the player status is false by default. Later, with `RandomMessage_SetPlayerStatus` we set status to true, and then, we send 6 messages instantly with `RandomMessage_SendCliSpecific` which should be sent.

## Testing

To test, simply run the package:

```bash
sampctl package run
```
