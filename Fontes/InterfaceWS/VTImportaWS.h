//-----------------------------------------------------------------------------
#ifndef VTImportaWSH
#define VTImportaWSH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaWS : public TObject
   {                    
   public:
                         __fastcall  VTImportaWS(void) {};
      virtual            __fastcall ~VTImportaWS(void) {};
      virtual bool       __fastcall  Executa(AnsiString arq_txt)=0;
      virtual AnsiString __fastcall  MsgErro(void)=0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTImportaWS* NewObjImportaWS(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
