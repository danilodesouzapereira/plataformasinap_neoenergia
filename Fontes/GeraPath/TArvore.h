//---------------------------------------------------------------------------
#ifndef TArvoreH
#define TArvoreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTArvore.h"

//---------------------------------------------------------------------------
class TArvore : public VTArvore
	{
	public:
					__fastcall TArvore(void);
					__fastcall TArvore(VTEqpto* eqpto, VTArvore* parv);
					__fastcall ~TArvore(void);
	 VTArvore*  __fastcall AddEqpto(VTEqpto* eqpto_aux, VTLigacao* pliga);
	 void       __fastcall EqptoNA(VTEqpto* eqpto);
	 void       __fastcall IniciaArvore(VTEqpto* eqpto);
	 TList*     __fastcall ListaArvores(void);
	 TList*     __fastcall ListaArvoresAct(void);
	 TList*     __fastcall ListaNA(void);
	 bool       __fastcall NA(void);
	 VTArvore*  __fastcall ProximoRamo(void);

	protected:  //métodos acessados via property
	  VTArvore*  __fastcall PM_GetArvOri(void);
			 void  __fastcall PM_SetArvOri(VTArvore* arv_ori);
	  VTArvore*  __fastcall PM_GetArvProt(void);
			 void  __fastcall PM_SetArvProt(VTArvore* arv_prot);
	  VTArvore*  __fastcall PM_GetArvRaiz(void);
			 void  __fastcall PM_SetArvRaiz(VTArvore* arv_raiz);
		VTEqpto*  __fastcall PM_GetEqpto(void);
	 VTLigacao*  __fastcall PM_GetLigacao(void);
			 void  __fastcall PM_SetLigacao(VTLigacao* pliga);


	private: //métodos

	private: //dados externos

	private: //dados locais
		VTArvore* arv_ori;  //Ramo gerador
		VTArvore* arv_prot;
		VTArvore* arv_raiz; //ponteiro para a arvore raiz
		VTLigacao* pliga;   //ligação do equipamento ao ramo gerador
		TList* lisArv;
		TList* lisArvAct;  //Lista de arvores que apontam a eqptos que acionam a protecao
		VTEqpto* eqpto;
		TList* lisNA;       //número de chaves NA no caminho
   public:
      int    iarv;

   };

#endif
//---------------------------------------------------------------------------
//eof
	 //VTLigacao* __fastcall Ligacao(void);
	 //bool       __fastcall AdicionaRamos(VTArvore* arv_ini, TList* lisPath, TList* lisFontes);
	 //VTArvore*  __fastcall ArvOri(void);
	 //void       __fastcall ArvOriDef(VTArvore* arv_ori);
	 //VTArvore*  __fastcall ArvProt(void);
	 //void       __fastcall ArvProtDef(VTArvore* arv_prot);
	 //VTArvore*  __fastcall ArvRaiz(void);
	 //void       __fastcall ArvRaizDef(VTArvore* arv_raiz);
	 //bool       __fastcall ArvRemove(VTEqpto* eqpto);
	 //bool       __fastcall ChecaRamo(void);
	 //void       __fastcall DefEqpto(VTEqpto* eqpto);
	 //void       __fastcall DefLigacao(VTLigacao* pliga);
	 //VTEqpto*   __fastcall Eqpto1(void);

