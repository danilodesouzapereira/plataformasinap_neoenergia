//---------------------------------------------------------------------------
#ifndef TArvoresH
#define TArvoresH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTArvores.h"

//---------------------------------------------------------------------------
class TArvores : public VTArvores
   {
   public:
					 __fastcall TArvores(VTEqpto* eqpto);
					 __fastcall ~TArvores(void);
		bool      __fastcall AddCaminho(VTArvore* arv);
		bool      __fastcall AtribCaminho(VTArvore* arv, VTArvore* arvEqpto);
		bool      __fastcall AtribCaminhoBarra(VTArvore* arv, VTEqpto *eqpto);
		bool      __fastcall AtribCaminhoProt(VTArvore* arv, VTArvore* arvEqpto);
		//bool      __fastcall AtribCaminhoProt(VTArvore* arv, VTEqpto* eqpto);
		VTEqpto*  __fastcall EqRaiz(void);
		VTArvore* __fastcall Arv(void);
		TList*    __fastcall LisArvProt(void);
		TList*    __fastcall LisCortesP(void);
		TList*    __fastcall LisEqpto(void);
		TList*    __fastcall LisEqptoPath(void);
		TList*    __fastcall LisFontesPath(void);
		TList*    __fastcall LisPath(bool flagNF, VTEqpto *eqpto);
		TList*    __fastcall LisPathProt(VTEqpto *eqpto);
		TList*    __fastcall LisEqProt(VTEqpto *eqpto);
		bool      __fastcall NA(VTEqpto *eqpto);
		//TList*    __fastcall LisCortes(int icorte);
		//bool      __fastcall AtribArvore(VTArvore* arv, VTEqpto *eqpto);

	private: //dados locais
		bool   flag_calc;
		VTEqpto* eqpto;//Eqpto raiz da árvore
		VTArvore* arv_raiz;//raiz da árvore
		//TList* lisArvs;//Lista de arvores com os mesmos indicadores de continuidade
		TList* lisArvProt;//Lista de arvore com equipamento de protecao
		TList* lisRaiz;//Lista de árvore raiz
		TList* lisPath; //lista de caminhos - árvores extremas de caminhsos NA + NF
		TList* lisPathNF; //lista de caminhos - árvores extremas de caminhos NF
		//TList* lisCortes1;//lista de cortes mínimos totais de 1a ordem
		//TList* lisCortes2;//lista de cortes mínimos totais de 2a ordem
		//TList* lisCortes3;//lista de cortes mínimos totais de 3a ordem
		TList* lisCortesP;//lista de cortes mínimos parciais
		TList* lisEqpto;//lista de equipamentos
		TList* lisEqptoPath;//lista de caminhos por equipamento
		TList* lisFontes;//lista de fontes (eqptos de nós finais)
		TList* lisFontesPath;//lista de caminhos por fonte
	};

#endif
//---------------------------------------------------------------------------
//eof

