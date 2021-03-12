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
      TList* lisL;    //Variáveis contínuas
      TList* lisLBin; //Variáveis binárias
		bool   flag_exe;   //se o indivíduo tem avaliacao possível
		bool   flag_avalia;//se o indivíduo passou pela avaliação
		int    idade;
		double nota, notaEsc;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTDespacho
//---------------------------------------------------------------------------
VTIndividuo* __fastcall NewObjIndividuoDesp(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoRecFalha(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoGD(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoPosIF(VTIndividuo* indiv, bool flag_orig = false);
VTIndividuo* __fastcall NewObjIndividuoDistribIF(VTIndividuo* indiv, bool flag_orig = false);

//---------------------------------------------------------------------------
#endif
//eof




 