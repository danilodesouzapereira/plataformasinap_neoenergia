//---------------------------------------------------------------------------
#ifndef VTMultaH
#define VTMultaH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include "..\Reclamacao\VTPerfilBarra.h"

//---------------------------------------------------------------------------
class VTMulta : public TObject
   {
   public:  //property

		__property VTCarga*    	  Carga			 = {read=PM_GetCarga,         write=PM_SetCarga 		 };
		__property double      	  PiorDRC   	 = {read=PM_GetPiorDRC                              	 };
		__property double      	  PiorDRP       = {read=PM_GetPiorDRP                              	 };
		__property VTRede*    	  RedeSec		 = {read=PM_GetRedeSec,			write=PM_SetRedeSec  	 };
		__property VTRegulador*	  Regulador		 = {read=PM_GetReg,				write=PM_SetReg      	 };
		__property VTPerfilBarra* PerfilBarraPE = {read=PM_GetPerfilBarraPE, write=PM_SetPerfilBarraPE};
		__property VTSuprimento*  Suprimento	 = {read=PM_GetSupr,				write=PM_SetSupr     	 };
		__property double      	  ValorMultaCA	 = {read=PM_GetValorMultaCA                         	 };
		__property double      	  ValorMultaPE	 = {read=PM_GetValorMultaPE                         	 };

	public:
							__fastcall  VTMulta(void) {};
							__fastcall ~VTMulta(void) {};
		virtual bool 	__fastcall  Executa(void) = 0;
		virtual void 	__fastcall  Reinicia(void) = 0;

	protected:  //dados acessados vias property
		virtual VTCarga*		   __fastcall PM_GetCarga(void) = 0;
      virtual VTPerfilBarra*  __fastcall PM_GetPerfilBarraPE(void) = 0;
		virtual double			   __fastcall PM_GetPiorDRC(void) = 0;
		virtual double			   __fastcall PM_GetPiorDRP(void) = 0;
		virtual VTRede*		   __fastcall PM_GetRedeSec(void) = 0;
		virtual VTRegulador*	   __fastcall PM_GetReg(void) = 0;
		virtual VTSuprimento*   __fastcall PM_GetSupr(void) = 0;
		virtual double			   __fastcall PM_GetValorMultaCA(void) = 0;
		virtual double			   __fastcall PM_GetValorMultaPE(void) = 0;
		virtual void			   __fastcall PM_SetCarga(VTCarga* carga) = 0;
		virtual void			   __fastcall PM_SetRedeSec(VTRede* rede_sec) = 0;
		virtual void			   __fastcall PM_SetReg(VTRegulador* reg) = 0;
		virtual void			   __fastcall PM_SetSupr(VTSuprimento* supr) = 0;
		virtual void			   __fastcall PM_SetPerfilBarraPE(VTPerfilBarra*	perfil_barra) = 0;

		struct   {
					double			piorDRC;       //percentual
					double			piorDRP;       //percentual
					double 			multa_CA;      //CA: carga analisada
					double 			multa_PE;      //PE: ponto de entrega
					VTCarga* 		carga_PE;
					VTPerfilBarra*	perfil_barra_PE;
					VTRede*			rede_sec;
					VTRegulador*	reg;
					VTSuprimento*	supr;
					}PD;
   };

//---------------------------------------------------------------------------
VTMulta*  __fastcall NewObjMulta(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


