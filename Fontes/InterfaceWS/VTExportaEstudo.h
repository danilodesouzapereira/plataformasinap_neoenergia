//-----------------------------------------------------------------------------
#ifndef VTExportaEstudoH
#define VTExportaEstudoH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTEstudo;

//-----------------------------------------------------------------------------
class VTExportaEstudo : public TObject
   {                    
   public:
                         __fastcall  VTExportaEstudo(void) {};
      virtual            __fastcall ~VTExportaEstudo(void) {};
      virtual bool       __fastcall  Executa(AnsiString estudo_codigo, AnsiString arq_txt)=0;
      virtual AnsiString __fastcall  MsgErro(void)=0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTExportaEstudo* NewObjExportaEstudo(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
