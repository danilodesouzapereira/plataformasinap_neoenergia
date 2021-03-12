//---------------------------------------------------------------------------
#ifndef TFaixaTensaoH
#define TFaixaTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TFaixaTensao : public TObject
{
    public:
					__fastcall  TFaixaTensao(VTApl *apl);
					__fastcall ~TFaixaTensao(void);
		void    	__fastcall  InsereBarra(VTBarra* barra);
		void    	__fastcall  InsereBarra(TList* lisBarra);
		TList*	  	__fastcall  LisBarra(void);
		AnsiString 	__fastcall  Texto(void);

	public:
	   double valor_inf;
	   double valor_sup;
	   TColor cor;

	private: //dados locais
	   TList *lisBARRA;

	private: //dados externos
	   VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
