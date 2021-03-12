//---------------------------------------------------------------------------
#ifndef VTArvoreH
#define VTArvoreH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;
class VTLigacao;

//---------------------------------------------------------------------------
class VTArvore : public TObject
   {
	public:  //property
	  __property VTEqpto*        Eqpto      =  {read=PM_GetEqpto};
	  __property VTArvore*       ArvOri     =  {read=PM_GetArvOri, write=PM_SetArvOri};
	  __property VTArvore*       ArvProt    =  {read=PM_GetArvProt, write=PM_SetArvProt};
	  __property VTArvore*       ArvRaiz    =  {read=PM_GetArvRaiz, write=PM_SetArvRaiz};
	  __property VTLigacao*      Ligacao    =  {read=PM_GetLigacao, write=PM_SetLigacao};

	public:
							  __fastcall VTArvore(void) {};
	 virtual 			  __fastcall ~VTArvore(void) {};
	 virtual VTArvore*  __fastcall AddEqpto(VTEqpto* eqpto_aux, VTLigacao* pliga) = 0;
	 virtual void       __fastcall EqptoNA(VTEqpto* eqpto) = 0;
	 virtual void       __fastcall IniciaArvore(VTEqpto* eqpto) = 0;
	 virtual TList*     __fastcall ListaArvores(void) = 0;
	 virtual TList*     __fastcall ListaArvoresAct(void) = 0;
	 virtual TList*     __fastcall ListaNA(void) = 0;
	 virtual bool       __fastcall NA(void) = 0;
	 virtual VTArvore*  __fastcall ProximoRamo(void) = 0;

	public:	 //dados
      int    iarv;

	protected:  //métodos acessados via property
		virtual VTArvore*  __fastcall PM_GetArvOri(void) = 0;
		virtual void       __fastcall PM_SetArvOri(VTArvore* arv_ori) = 0;
		virtual VTArvore*  __fastcall PM_GetArvProt(void) = 0;
		virtual void       __fastcall PM_SetArvProt(VTArvore* arv_prot) = 0;
		virtual VTArvore*  __fastcall PM_GetArvRaiz(void) = 0;
		virtual void       __fastcall PM_SetArvRaiz(VTArvore* arv_raiz) = 0;
		virtual VTEqpto*   __fastcall PM_GetEqpto(void) = 0;
		virtual VTLigacao* __fastcall PM_GetLigacao(void) = 0;
		virtual void       __fastcall PM_SetLigacao(VTLigacao* pliga) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTArvore* __fastcall NewObjArvore(void);
VTArvore* __fastcall NewObjArvore(VTEqpto* eqpto, VTArvore* parv);

#endif
//---------------------------------------------------------------------------
//eof
	 //virtual bool       __fastcall AdicionaRamos(VTArvore* arv_ini, TList* lisPath, TList* lisFontes) = 0;
	 //virtual VTArvore*  __fastcall ArvOri(void) = 0;
	 //virtual void       __fastcall ArvOriDef(VTArvore* arv_ori) = 0;
	 //virtual VTArvore*  __fastcall ArvProt(void) = 0;
	 //virtual void       __fastcall ArvProtDef(VTArvore* arv_prot) = 0;
	 //virtual VTArvore*  __fastcall ArvRaiz(void) = 0;
	 //virtual void       __fastcall ArvRaizDef(VTArvore* arv_raiz) = 0;
	 //virtual bool       __fastcall ArvRemove(VTEqpto* eqpto) = 0;
	 //virtual bool       __fastcall ChecaRamo(void) = 0;
	 //virtual void       __fastcall DefEqpto(VTEqpto* eqpto) = 0;
	 //virtual void       __fastcall DefLigacao(VTLigacao* pliga) = 0;
	 //virtual VTEqpto*   __fastcall Eqpto1(void) = 0;
	 //virtual VTLigacao* __fastcall Ligacao(void) = 0;

