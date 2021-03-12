//---------------------------------------------------------------------------
#ifndef TEdtRegiaoH
#define TEdtRegiaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"
#include "..\Rede\Estrutura.h"
//---------------------------------------------------------------------------
#define TOL 4 //tolerancia de pixels (quadrado de lado 2TOL centrado nos pontos)
//---------------------------------------------------------------------------
class VTRedegraf;
class VTRegiao;
class VTBarra;
class VTTrecho;
//class TFormEdtRegiao;
//---------------------------------------------------------------------------
class TEdtRegiao : public TEdt
   {
	public:
				__fastcall  TEdtRegiao(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone, TWinControl *parent);
				__fastcall ~TEdtRegiao(void);
		void	__fastcall  Inicia(VTRegiao *regiao);
		void	__fastcall	Redraw(void);

	private:  //métodos
		void __fastcall DestroiLista(TList *lista);
      void __fastcall EvtEnter(void);
      void __fastcall EvtEscape(void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseMove(void);
		void __fastcall EvtMouseUp(void);
      void __fastcall LisEqptoNovoApaga(void);
		void __fastcall LisEqptoNovoExibe(void);
		void __fastcall LimpaLista(TList *lista);
		bool __fastcall VerificaPontoInicial(int x, int y);
		bool __fastcall VerificaUltimoPonto(int x, int y);

	private:  //objetos externos
		VTRedegraf 		*redegraf;
		VTRegiao   		*regiao;
		//TFormEdtRegiao *formEdtRegiao;

   private:  //dados locais
		bool  	evt_mouse_enabled;
		TPoint 	*pontoInicial_pixels;
		TPoint	*ultimoPonto_pixels;
		TList		*lisBARRastro;
		TList		*lisTRECHORastro;
		struct{
				bool     enabled;
				VTBarra  *barraInicial, *barraMovel, *barraUltima;
				VTTrecho *trecho1 , *trecho2;
				}rastro;
   };

#endif
//---------------------------------------------------------------------------
//eof
