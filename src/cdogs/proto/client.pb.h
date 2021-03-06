/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.0 at Fri May 29 20:00:55 2015. */

#ifndef PB_CLIENT_PB_H_INCLUDED
#define PB_CLIENT_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
/* Struct definitions */
typedef struct _NetMsgRequestPlayers {
    uint8_t dummy_field;
} NetMsgRequestPlayers;

typedef struct _NetMsgCmd {
    uint32_t Cmd;
} NetMsgCmd;

typedef struct _NetMsgNewPlayers {
    int32_t ClientId;
    int32_t NumPlayers;
} NetMsgNewPlayers;

/* Default values for struct fields */

/* Initializer values for message structs */
#define NetMsgRequestPlayers_init_default        {0}
#define NetMsgCmd_init_default                   {0}
#define NetMsgNewPlayers_init_default            {0, 0}
#define NetMsgRequestPlayers_init_zero           {0}
#define NetMsgCmd_init_zero                      {0}
#define NetMsgNewPlayers_init_zero               {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define NetMsgCmd_Cmd_tag                        1
#define NetMsgNewPlayers_ClientId_tag            1
#define NetMsgNewPlayers_NumPlayers_tag          2

/* Struct field encoding specification for nanopb */
extern const pb_field_t NetMsgRequestPlayers_fields[1];
extern const pb_field_t NetMsgCmd_fields[2];
extern const pb_field_t NetMsgNewPlayers_fields[3];

/* Maximum encoded size of messages (where known) */
#define NetMsgRequestPlayers_size                0
#define NetMsgCmd_size                           6
#define NetMsgNewPlayers_size                    22

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
