//---------------------------------------------------------------------------
#ifndef TBlocoH
#define TBlocoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBloco.h"

//---------------------------------------------------------------------------
class TBloco : public VTBloco
   {
   public:
             __fastcall TBloco(void);
             __fastcall ~TBloco(void);
		bool   __fastcall ExisteBarra(VTBarra *barra);
		bool   __fastcall ExisteGeracao(void);
      bool   __fastcall ExisteLigacao(VTLigacao *ligacao);
      bool   __fastcall ExisteLigacaoDaRede(VTRede *rede);
      void   __fastcall InsereBarra(VTBarra *barra);
      void   __fastcall InsereLigacao(VTLigacao *ligacao);
		TList* __fastcall LisBarra(void);
		void   __fastcall LisEqbar(TList *lisEXT, int tipo);
		TList* __fastcall LisLigacao(void);
		void   __fastcall LisLigacao(TList *lisEXT, int tipo);
      TList* __fastcall LisRede(void);

	protected:	//métodos acessados via property
	  TColor  __fastcall PM_GetColor(void);
		VTRede* __fastcall PM_GetRede(void);
	  void    __fastcall PM_SetColor(TColor color);
	  TObject*   __fastcall PM_GetObj(void) {return(PD.obj);};
	  void       __fastcall PM_SetObj(TObject* obj) {PD.obj = obj;};

   private: //dados
	  TList *lisBAR;
	  TList *lisLIG;
	  TList *lisREDE;
	  struct{
			TColor color;
			TObject    *obj;  //ponteiro auxiliar
			}PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

