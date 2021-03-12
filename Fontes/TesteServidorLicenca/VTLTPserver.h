//-----------------------------------------------------------------------------
#ifndef VTLTPserver_H
#define VTLTPserver_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTPacote;

//-----------------------------------------------------------------------------
class VTLTPserver  : public TObject
   {
   public:
                   __fastcall VTLTPserver(void)  {};
      virtual      __fastcall ~VTLTPserver(void) {};
      virtual void __fastcall Conecta(AnsiString arq_db) = 0;
      virtual bool __fastcall TrataPacote(VTPacote *pacote)  = 0 ;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTLTPserver
//-----------------------------------------------------------------------------
VTLTPserver* __fastcall NewObjLTPserver(void);

#endif
//---------------------------------------------------------------------------
//eof
