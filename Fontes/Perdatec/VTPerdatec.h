//---------------------------------------------------------------------------
#ifndef VTPerdatecH
#define VTPerdatecH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTPerdatec : public TObject
   {
   public:
                    __fastcall  VTPerdatec(void) {};
      virtual       __fastcall ~VTPerdatec(void) {};
      virtual void  __fastcall  Clear(void) = 0;
      virtual bool  __fastcall  Executa(TDateTime data, int dia_semana) = 0;
      virtual bool  __fastcall  PreparaRede(TDateTime data) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPerdatec
//---------------------------------------------------------------------------
VTPerdatec* __fastcall NewObjPerdatec(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


