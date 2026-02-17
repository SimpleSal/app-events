/* -- SimpleSal: ssTEA, ssUI, and ssIO | Copyright (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * api_ssTEA_dcl.h      data and software declarations: application API (software interface) to ssTEA
 *
 **************************************************************************************************/
#ifndef __API_SSTEA_DCL_H
#define __API_SSTEA_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Given a properly defined Event, perform the steps required by the ssTEA FSMs to make it active.
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

#endif // __API_SSTEA_DCL_H

