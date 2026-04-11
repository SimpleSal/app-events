/* -- SimpleSal: ssTEA, ssUI, and ssIO | Copyright (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * api_ssTEA_def.h      data and software definitions: application API (software interface) to ssTEA
 *
 **************************************************************************************************/
#ifndef __API_SSTEA_DEF_H
#define __API_SSTEA_DEF_H

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Launching an event involves 1) defining the event and 2) feeding the definition to ssTEA.
// This function demonstrates that the majority of the work involved is in the Event's definition.
// The sequence of signals moves the FSMs used to manage the Event into the On and Running state.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_Launch_in_ssTEA (ssUI_db_pEvApi_t pEvApi)
{
    ssE_EvResult_t  Launch_Result = ssE_EvResult_OK_go;

    App_Announce_App ("Launching Event in ssTEA");
    App_Announce_App (pEvApi->pEvApiNameTkn_App->pAsciiA);

    if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Validate))
    {
        App_Announce_Event ("Event's AboutEvent data <validate> succeeded");
        if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Run))
        {
            App_Announce_Event ("Agency Api FSM <On Running> succeeded");
            if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Start))
            {
                App_Announce_Event ("Agency Run FSM <Start> succeeded");
            }   // Start succeeded
            else
            {   // Start failed
                App_Announce_Event ("Error: Agency Run FSM failed to start <Agencying>");
                Launch_Result = ssE_EvResult_notOK_state;
            }   // Start failed
        }   // Run succeeded
        else
        {   // Run failed
            App_Announce_Event ("Error: Agency Api FSM failed to enter <On Running>");
            Launch_Result = ssE_EvResult_notOK_state;
        }   // Run failed
    }   // Validate succeeded
    else
    {   // Validate failed
        App_Announce_Event ("Error: Event's AboutEvent data <validate> failed");
        return (ssE_EvResult_notOK_state);
    }   // Validate failed

#ifdef APP_OPTIN_ANNOUNCE_APP
    ss_uiOp_Show_Apifsm_state (pEvApi->pssTEA_ApiSig, pApiSigName (pEvApi));
    ss_uiOp_Show_Runfsm_state (pEvApi->pssTEA_ApiSig, pApiSigName (pEvApi));
#endif // APP_OPTIN_ANNOUNCE_APP

    return (Launch_Result);
}   // App_Launch_in_ssTEA
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The allocation of memory for event management data may be handled by ssTEA or by the App.
// At the end of this file is ssTEA's implementation. Selected as manager? Data exists and is used.
// [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note]       ssE declares what the interface will be
// [.\SimpleSal\ssDocs\ssTEA\Event Databases.note]          ssE or the App instantiates the database
// -------------------------------------------------------------------------------------------------
// App note: the allocation is optional UNLESS the configuration says the App will allocate data.
// App note: the method is arbitrarily different from the ssE implementation just to show it can be.
// -------------------------------------------------------------------------------------------------
#ifdef SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP
ssE_AboutEv_t   AboutEvDB_allocSpace[SSE_ONEOF_ABOUTEVDB_ALLOC_CT];
boolean         AboutEvDB_allocTrack[SSE_ONEOF_ABOUTEVDB_ALLOC_CT];

// -------------------------------------------------------------------------------------------------
// each time initialization is performed, the database is cleared and valid.
// -------------------------------------------------------------------------------------------------
void    AboutEvDB_dbOp_Init_DB (void)
{
    int                 DB_i;

    for (DB_i=0; DB_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; DB_i++)
    {
        AboutEvDB_allocTrack[DB_i] = false;
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
        ss_uiOp_emit_1 (Ascii_EncapL);
        ss_uiOp_emit_Hex_2 (DB_i);
        ss_uiOp_emit_1 (Ascii_EncapR);
        ssE_EvDB_Show_ssE_pAboutEv (S("Event Data : App Init"), &AboutEvDB_allocSpace[DB_i]);
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
        ssE_dbOp_Init_One_AboutEv (&AboutEvDB_allocSpace[DB_i], DEEPINIT);
    }
}   // AboutEvDB_dbOp_Init_DB
// -------------------------------------------------------------------------------------------------
ssE_pAboutEv_t   AboutEvDB_dbOp_Get (void)
{
    ssE_pAboutEv_t      ssE_pAboutEv = ssE_pAboutEvNull;
    int                 DB_i;

    for (DB_i=0; DB_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; DB_i++)
    {
        if (AboutEvDB_allocTrack[DB_i] == false)
        {
            AboutEvDB_allocTrack[DB_i] = true;
            ssE_pAboutEv = &AboutEvDB_allocSpace[DB_i];
            break;      // out of for
        }   // if not in use already
    }   // checking all allocated data
    return (ssE_pAboutEv);         // null if none found or the first one found
}   // AboutEvDB_dbOp_Get
// -------------------------------------------------------------------------------------------------
ssE_pAboutEv_t     AboutEvDB_dbOp_Put (ssE_pAboutEv_t pReleaseThis)
{
    ssE_pAboutEv_t   ssE_pAboutEv = ssE_pAboutEvNull;
    int                     DB_i;

    for (DB_i=0; DB_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; DB_i++)
    {
        if (&AboutEvDB_allocSpace[DB_i] == pReleaseThis)  // only put good values
        {
            AboutEvDB_allocTrack[DB_i] = false;
            return (ssE_pAboutEvNull);
        }   // any match completes the task, whethere allocated or not
    }   // look in each slot

    // couldn't put it back, better to not just pretend we did put it back
    return (ssE_pAboutEv);
}   // AboutEvDB_dbOp_Put
#endif  // SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This is a tool that creates a burden over time: it displays all actual build options selected.
// This model of ifdef-to-select-code makes the same function available to every build; varies here.
// -------------------------------------------------------------------------------------------------
// This function is always referencable, whether it has content or not depends on the build options.
// Moving the SHOW #ifdef outside the function causes the reference in source to require an #ifdef.
// -------------------------------------------------------------------------------------------------
// This is not provided as part of ssTEA or ssUI so that the changes occur in files owned by a user.
// -------------------------------------------------------------------------------------------------
// All build choices must have been made by this time; a change later in the compile is not reported.
// -------------------------------------------------------------------------------------------------
void    App_BuildOp_Show_Build (void)
{
#ifdef SS_OPTIN_SHOW_BUILD_OPTIONS
#ifdef SS_ACCEPT_SIMPLEST_CONFIG
    {   ss_uiOp_emit_qAsciiA ("SS_ACCEPT_SIMPLEST_CONFIG");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_IS_AN_ARROW
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_IS_AN_ARROW");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_IS_AN_FSM
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_IS_AN_FSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    {   ss_uiOp_emit_qAsciiA ("SSE_OPTIN_ABOUTEVDB_TRUSTED_APP");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSE_ONEOF_ABOUTEVDB_ALLOC_CT
    {
        ss_uiOp_emit_qAsciiA ("SSE_ONEOF_ABOUTEVDB_ALLOC_CT ");
        ss_uiOp_emit_Int_99 (SSE_ONEOF_ABOUTEVDB_ALLOC_CT);
        ss_uiOp_emit_newline ();
    }
#endif
#ifdef  SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP
    {   ss_uiOp_emit_qAsciiA ("SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSE_ONEOF_ABOUTEVDB_ALLOC_BY_SSE
    {   ss_uiOp_emit_qAsciiA ("SSE_ONEOF_ABOUTEVDB_ALLOC_BY_SSE");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_STATS_FSM
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_STATS_FSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_STATS_APIFSM
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_STATS_APIFSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_STATS_RUNFSM
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_STATS_RUNFSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
    {   ss_uiOp_emit_qAsciiA ("SST_OPTIN_TIME_COMPRESSED_TUS");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_OPTIN_TIME_NSECS_TU
    {   ss_uiOp_emit_qAsciiA ("SST_OPTIN_TIME_NSECS_TU");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_NSECS_RANDOM
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_NSECS_RANDOM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_NSECS_ZERO
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_NSECS_ZERO");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_NSECS_INCR
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_NSECS_INCR");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_PLANCKS_RANDOM
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_PLANCKS_RANDOM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_PLANCKS_ZERO
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_PLANCKS_ZERO");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_PLANCKS_INCR
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_PLANCKS_INCR");  ss_uiOp_emit_newline (); }
#endif
#ifdef  MESA_OPTIN_RANDOM_NUMBERS
    {   ss_uiOp_emit_qAsciiA ("MESA_OPTIN_RANDOM_NUMBERS");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_AGENCY_DEBUG
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_AGENCY_DEBUG");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP
    {   ss_uiOp_emit_qAsciiA ("SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI
    {   ss_uiOp_emit_qAsciiA ("SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI");  ss_uiOp_emit_newline (); }
#endif
#ifdef SS_OPTIN_SHOW_BUILD_OPTIONS
    {   ss_uiOp_emit_qAsciiA ("SS_OPTIN_SHOW_BUILD_OPTIONS");  ss_uiOp_emit_newline (); }  // a joke
#endif
#endif // SS_OPTIN_SHOW_BUILD_OPTIONS

}   // App_BuildOp_Show_Build

#endif // __API_SSTEA_DEF_H

