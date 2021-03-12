//---------------------------------------------------------------------------
#ifndef VTPerdaregH
#define VTPerdaregH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTPerdareg : public TObject
   {
   public:
                    __fastcall  VTPerdareg(void) {};
      virtual       __fastcall ~VTPerdareg(void) {};
      virtual void  __fastcall  ClearMontaRede(void) = 0;
      virtual bool  __fastcall  Executa(TDateTime data, bool batch = false) = 0;
      virtual bool  __fastcall  ImportaMedicao(TDateTime data, TStrings *files) = 0;
      virtual bool  __fastcall  PreparaRede(TDateTime data) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPerdareg
//---------------------------------------------------------------------------
VTPerdareg* __fastcall NewObjPerdareg(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


