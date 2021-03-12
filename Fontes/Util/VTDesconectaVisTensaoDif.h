//---------------------------------------------------------------------------
#ifndef VTDesconectaVisTensaoDifH
#define VTDesconectaVisTensaoDifH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTDesconectaVisTensaoDif : public TObject
   {
   public:
                   __fastcall  VTDesconectaVisTensaoDif(void) {};
      virtual      __fastcall ~VTDesconectaVisTensaoDif(void) {};
      virtual bool __fastcall  Executa(void) = 0;
   };

//---------------------------------------------------------------------------
VTDesconectaVisTensaoDif* __fastcall NewObjDesconectaVisTensaoDif(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof