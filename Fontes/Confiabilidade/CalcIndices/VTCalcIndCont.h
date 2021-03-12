//---------------------------------------------------------------------------
#ifndef VTCalcIndContH
#define VTCalcIndContH
#include "..\VTConfiab.h"
//---------------------------------------------------------------------------
class VTApl;
class VTArvores;
class VTProgresso;
//---------------------------------------------------------------------------
class VTCalcIndCont: public TObject
   {
   public:
                        __fastcall VTCalcIndCont(void) {};
      virtual           __fastcall ~VTCalcIndCont(void) {};
      virtual   bool    __fastcall Executa(TList *lisCortes, TList *lisArvs, VTProgresso *prog = NULL) = 0;
		virtual   bool    __fastcall AgregaRes(RES_CONF &res_conf, TList* lisArv) = 0;
		//virtual   bool    __fastcall Executa(VTArvores* arv) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTConfiab
//---------------------------------------------------------------------------
VTCalcIndCont* __fastcall NewObjCalcIndice(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 