//-----------------------------------------------------------------------------
#ifndef VTImportaPadraoH
#define VTImportaPadraoH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaPadrao : public TObject
   {                    
   public:
                         __fastcall  VTImportaPadrao(void) {};
      virtual            __fastcall ~VTImportaPadrao(void) {};
      virtual bool       __fastcall  Executa(AnsiString arq_txt)=0;
      virtual AnsiString __fastcall  MsgErro(void)=0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTImportaPadrao* NewObjImportaPadrao(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
