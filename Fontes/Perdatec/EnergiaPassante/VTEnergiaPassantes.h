//---------------------------------------------------------------------------
#ifndef VTEnergiaPassantesH
#define VTEnergiaPassantesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEnergiaPassante;
class VTRede;

//---------------------------------------------------------------------------
class VTEnergiaPassantes : public TObject
   {
   public:
                         __fastcall VTEnergiaPassantes(void) {};
      virtual            __fastcall ~VTEnergiaPassantes(void) {};
      virtual bool       __fastcall Executa(void) = 0;
      virtual VTEnergiaPassante* __fastcall ExisteEnergiaPassante(VTRede *rede) = 0;
      virtual TList*     __fastcall LisEnergiaPassante(void) = 0;
      virtual void       __fastcall Reinicia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEnergiaPassantes
//---------------------------------------------------------------------------
VTEnergiaPassantes* __fastcall NewObjEnergiaPassantes(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 