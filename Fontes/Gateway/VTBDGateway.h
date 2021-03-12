//---------------------------------------------------------------------------
#ifndef VTBDGatewayH
#define VTBDGatewayH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSNPDBConn.h"
#include "SNPSTR.h"

//---------------------------------------------------------------------------
// ENUMERAÇÕES

enum enmDBStatusExec { stsINDEF = -1, stsCARGAINI, stsCARGARUN, stsCARGAEND, stsCARGAERR };

//---------------------------------------------------------------------------
class VTApl;
class VTBDGatewaygeral;
class VTBDGatewayhist;
class VTBDGatewayeqpto;

//---------------------------------------------------------------------------
class VTBDGateway : public TObject
   {
   public:  //property
      __property AnsiString   Revisao           = { read =revisao                                 };
      __property bool         Conectado         = { read=PM_GetConectado                          };
      __property bool         CronometroEnabled = { write=cronometro_enabled                      };
      __property bool         ProgressoEnabled  = { write=progresso_enabled                       };

   public:
                     __fastcall  VTBDGateway(void) {};
      virtual        __fastcall ~VTBDGateway(void) {};
      virtual bool   __fastcall  Clear(void) = 0;
      virtual bool   __fastcall  Commit(void) = 0;
      virtual bool   __fastcall  Conecta(void) = 0;
      virtual bool   __fastcall  Desconecta(void) = 0;
      virtual bool   __fastcall  LeNodesFilho(int node_pai_id) = 0;
      virtual bool   __fastcall  LeGrupoRedes(DynamicArray<int> vet_rede_id) = 0;
      virtual bool   __fastcall  LeTodasRedes(void) = 0;
      virtual bool   __fastcall  Rollback(void) = 0;
      virtual bool   __fastcall  SalvaRedes(void) = 0;
      virtual bool   __fastcall  SalvaTipos(void) = 0;
      virtual SNPSTR __fastcall  UltimoErro(void) = 0;

   protected:  //métodos acessados via property
      virtual bool __fastcall  PM_GetConectado(void) = 0;

   protected:  //dados acessados via property
      AnsiString   revisao;
      bool         cronometro_enabled;
      bool         progresso_enabled;
      bool         base_remota;
   };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTBDGateway* NewObjBDGateway(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof