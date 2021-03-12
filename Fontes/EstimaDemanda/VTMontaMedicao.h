//---------------------------------------------------------------------------
#ifndef VTMontaMedicaoH
#define VTMontaMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTMontaMedicao : public TObject
   {
   public:  //métodos
                     __fastcall  VTMontaMedicao(void) {};
                     __fastcall ~VTMontaMedicao(void) {};
      virtual bool   __fastcall  InsereMedidores(void) = 0;
      virtual TList* __fastcall  LisMedidor(void) = 0;
      virtual bool   __fastcall  RefleteMedicaoNasCargas(void) = 0;
      virtual void   __fastcall  RetiraMedidores(void) = 0;
   };

//---------------------------------------------------------------------------
VTMontaMedicao* __fastcall NewObjMontaMedicaoDesequilibrada(VTApl *apl_owner);
VTMontaMedicao* __fastcall NewObjMontaMedicaoEquilibrada(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
