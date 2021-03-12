//-----------------------------------------------------------------------------
#ifndef VTPreMonta_H
#define VTPreMonta_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTPreMonta : public TObject
   {
   public:
                   __fastcall VTPreMonta(void) {};
      virtual      __fastcall ~VTPreMonta(void) {};
      virtual bool __fastcall InsereDBAR(int num_seq, TStringList *stList) = 0;
      virtual bool __fastcall InsereDCIR(int num_seq, TStringList *stList) = 0;
      virtual bool __fastcall InsereDMUT(int num_seq, TStringList *stList) = 0;
      virtual int  __fastcall NumeroErros(void) = 0;
      virtual void __fastcall Reinicia(void) = 0;
      virtual bool __fastcall Sucesso(void) = 0;
      virtual bool __fastcall TrataFimDados(void) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTPreMonta
//-----------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
