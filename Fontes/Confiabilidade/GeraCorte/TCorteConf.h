//---------------------------------------------------------------------------
#ifndef TCorteConfH
#define TCorteConfH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\VTConfiab.h"
//---------------------------------------------------------------------------
class VTEqpto;
//---------------------------------------------------------------------------
class TCorteConf : public TObject
   {
   public:
             __fastcall TCorteConf(void);
				 __fastcall TCorteConf(VTEqpto* eqpto, TCorteConf* pcorte0 = NULL);
				 __fastcall ~TCorteConf(void);
  AnsiString __fastcall Codigo(void);

   private: //métodos

   private: //dados externos

   private: //dados locais
   public:
       TList* lisEqpto;  //Lista de equipamentos pertencentes ao corte
		 TList* lisChvSoc; //Lista de possíveis socorros
		 TList* lisFontes; //Lista de fontes interrompidas NOVO
		 TList* lisFontesP; //Lista de fontes interrompidas por atuação da protecao
		 int    icorte;//0: total 1:parcial
		 TList* lisCortes;
		 VTEqpto* eqpto;
       TCorteConf* pcorte0;

		 RES_MC  res_conf;
		 //struct {
		 //       double dic, fic;
		 //       double tx_ff, r_ff;
		 //       double tx_fm, r_fm;
		 //		  }res_conf;
   };

#endif
//---------------------------------------------------------------------------
//eof
