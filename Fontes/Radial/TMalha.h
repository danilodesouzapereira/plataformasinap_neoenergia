//---------------------------------------------------------------------------
#ifndef TMalhaH
#define TMalhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMalha.h"

//---------------------------------------------------------------------------
//class

//---------------------------------------------------------------------------
class TMalha : public VTMalha
   {
   private: //dados locais
      TList  *lisBAR;
		TList  *lisLIG;
		TList  *lisREDE;

   public:  //métodos
				  __fastcall  TMalha(void);
				  __fastcall ~TMalha(void);
		bool    __fastcall  ExisteLigacaoComum(VTMalha *malha);
		VTRede* __fastcall  FirstRede(void);
		void    __fastcall  IncorporaMalha(VTMalha *malha);
		bool    __fastcall  InsereLigacao(TList *lisEXT);
		TList*  __fastcall  LisBarra(void);
		TList*  __fastcall  LisLigacao(void);
		TList*  __fastcall  LisRede(void);
		int     __fastcall  NumeroDeLigacoes(VTRede *rede=NULL, int tipo=-1);
		int     __fastcall  NumeroDeRedes(void);

   private: //métodos
		void __fastcall IniciaLisBarra(void);
		void __fastcall IniciaLisRede(void);
	};

//---------------------------------------------------------------------------
#endif
//eof
