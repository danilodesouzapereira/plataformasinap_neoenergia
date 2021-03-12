//---------------------------------------------------------------------------
#ifndef Servidor_H
#define Servidor_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTServidor;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServido(void);
EXPORT VTServidor* __fastcall DLL_NewObjServidorLocal(void);
EXPORT VTServidor* __fastcall DLL_NewObjServidorRemoto(void);

#endif
//---------------------------------------------------------------------------
//eof

 