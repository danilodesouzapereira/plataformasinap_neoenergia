//---------------------------------------------------------------------------
#ifndef VTItemObraARIESH
#define VTItemObraARIESH

//---------------------------------------------------------------------------
#include <Classes.hpp>
 #include "TItemObra.h"
//---------------------------------------------------------------------------
class VTApl;
class VTPadrao;
//---------------------------------------------------------------------------
class VTItemObraARIES : public TItemObra
{
	public:  //property
		__property double     Custo          = {read=custo,     write=custo};
		__property VTPadrao*  PadraoIns		 = {read=PM_GetPadraoIns, write=PM_SetPadraoIns};
		__property VTPadrao*  PadraoRet      = {read=PM_GetPadraoRet, write=PM_SetPadraoRet};
		__property double  	  QuantidadeIns	 = {read=qntIns, 	write=qntIns};
		__property double	  QuantidadeRet  = {read=qntRet, 	write=qntRet};
	

	public:
						__fastcall  VTItemObraARIES(VTApl *apl) :TItemObra(apl){};
		virtual 		__fastcall ~VTItemObraARIES(void) {};
        virtual VTItemObra* __fastcall  Clone(bool preserveId, bool isDeep) =0;
		virtual bool	__fastcall  ContabilizaCusto(void)=0;
		virtual bool	__fastcall  ContabilizaQuantidadeInsercao(void)=0;
		virtual bool	__fastcall  ContabilizaQuantidadeRetirada(void)=0;
		virtual bool	__fastcall  ContabilizaQuantidades(void)=0;

	protected:
		//Getters
		virtual VTPadrao* __fastcall PM_GetPadraoIns(void) = 0;
		virtual VTPadrao* __fastcall PM_GetPadraoRet(void) = 0;
		//Setters
		virtual void __fastcall PM_SetPadraoIns(VTPadrao *padrao) = 0;
		virtual void __fastcall PM_SetPadraoRet(VTPadrao *padrao) = 0;

	protected:  //
		double      qntIns;
		double      qntRet;
		double      custo;

};

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe VTItemObraARIES
//---------------------------------------------------------------------------
VTItemObra* NewObjItemObraARIES(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof