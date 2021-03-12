//---------------------------------------------------------------------------
#ifndef TGeraPathH
#define TGeraPathH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTGeraPath.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTLigacao;
class VTArvore;

//---------------------------------------------------------------------------
class TGeraPath : public VTGeraPath
   {
	public:

					__fastcall TGeraPath(VTApl* apl);
					__fastcall ~TGeraPath(void);
		  void   __fastcall AddCarga(VTEqpto* eqpto);
		  void   __fastcall AddFonte(VTEqpto* eqpto);
		  void   __fastcall AddLigaExc(VTLigacao* pliga, bool flag_12);
		  void   __fastcall DefLiga(TList* lisLiga);
		  bool   __fastcall GeraArvore(TList* lisArv, TList* lisSUP, bool flag_inv);
  VTArvores*   __fastcall GeraArvore(VTEqpto* eqpto, bool flag_inv);
  VTArvores*   __fastcall GeraCopia(VTArvores* arvs, VTEqpto* eqpto);
		  bool   __fastcall LigaIsolada(VTLigacao* pliga);
		  bool   __fastcall LisFontesConect(VTEqpto* eqpto, TList* lisConect);
		TList*   __fastcall LisTipoProt(void);
		  bool   __fastcall VerConect(VTEqpto* eqpto);

	private: //métodos
	  VTArvore* __fastcall BckTrk(VTArvore* parv);
		  void   __fastcall GeraCaminhos(TList* lisArv, VTArvores* arvs);
		  void   __fastcall GeraEqPath1(TList* lisArv);
        void   __fastcall GeraLisArv(TList* lisArv);
		  void   __fastcall GeraPathFonte1(TList* lisArvS, TList* lisArv);
		  void   __fastcall GeraPathFonte2(TList* lisArvS, TList* lisArv);
		  void   __fastcall GeraProtPath1(TList* lisArvS);
	 VTArvores* __fastcall GeraArvoreIni(VTEqpto* eqpto, bool flag_inv);
		  bool   __fastcall GeraRamos1(VTArvore* parv, bool flag_NA, bool flag_inv = false);

	private: //dados externos
		  VTApl* apl;

	private: //dados locais

		  TList* lisFontes;
		  TList* lisCargas;
		  TList* lisLiga;
		  TList* lisBarraLig;  //Lista de barras da rede
		  TList* lisLigBarra;  //Lista de listas de ligacoes por barra
		  TList* lisL12;//Ligacoes com exclusão de pbarra1 para pbarra2
		  TList* lisL21;//Ligacoes com exclusao de pbarra2 para pbarra1
		  TList* lisTipoProt;//Define tipos de chaves que atuam como protecao

		  bool   flagBarra;// inclui barras em Equipamentos-Path
	};
#endif
//---------------------------------------------------------------------------
//eof

