//---------------------------------------------------------------------------
#ifndef VTIndividuoH
#define VTIndividuoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTIndividuo: public TObject
   {
   public:
                   __fastcall VTIndividuo(void) {};
                   __fastcall ~VTIndividuo(void) {};
   public:
      TList* lisL;    //Vari�veis cont�nuas
      TList* lisLBin; //Vari�veis bin�rias
		bool   flag_exe;   //se o indiv�duo tem avaliacao poss�vel
		bool   flag_avalia;//se o indiv�duo passou pela avalia��o
		int    idade;
		double nota, notaEsc;
   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTDespacho
//---------------------------------------------------------------------------
VTIndividuo* __fastcall NewObjIndividuoDesp(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoRecFalha(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoGD(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoPosIF(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoDistribIF(VTIndividuo* indiv, bool flag_orig = false);

//---------------------------------------------------------------------------
#endif
//eof




 