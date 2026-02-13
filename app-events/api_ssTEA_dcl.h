/**************   license-text-string   copyright-text-string  ***
 * api_ssTEA_dcl.h      data and software declarations: application API (software interface) to ssTEA
 *
 * 2025/11/25   instantiate a minimum application to demonstrate the range of SimpleSal function.
 **************************************************************************************************/
#ifndef __API_SSTEA_DCL_H
#define __API_SSTEA_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Given a properly defined Event, perform the steps required by the ssTEA FSMs to become active.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t      App_Launch_in_ssTEA (ssUI_db_pEvApi_t pEvApi);

// -------------------------------------------------------------------------------------------------
// a utility function that displays the result of an #ifdef-based configuration at build time.
// -------------------------------------------------------------------------------------------------
void                App_BuildOp_Show_Build (void);

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note]       ssE declares what the interface will be
// [.\SimpleSal\ssDocs\ssTEA\Event Databases.note]          ssE or the App instantiates the database
// -------------------------------------------------------------------------------------------------
// This says "NOT declared by the App" because the form is defined by ssTEA; implementations vary.
// -------------------------------------------------------------------------------------------------
// NOT declared by the App          void    AboutEvDB_dbOp_Init_DB (void);

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

#endif // __API_SSTEA_DCL_H

