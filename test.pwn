#include <a_samp>
#include "random-messages.inc"

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
    printf("Status %s", RandomMessage_GetPlayerStatus(playerid) ? "true" : "false");
    RandomMessage_SetPlayerStatus(playerid, true);
    
    printf("Status %s", RandomMessage_GetPlayerStatus(playerid) ? "true" : "false");
    RandomMessage_SendCliSpecific(5, playerid, 0, 6); //Should be sent
    return 1;
}