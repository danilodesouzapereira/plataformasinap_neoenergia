//---------------------------------------------------------------------------
#ifndef TMultaH
#define TMultaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Rede\VTCarga.h>
#include "..\Reclamacao\VTPerfilBarra.h"
#include "VTMulta.h"
//---------------------------------------------------------------------------
class VTApl;
class VTClasse;
//---------------------------------------------------------------------------

class TMulta  : public VTMulta
	{
   public:
			  __fastcall  TMulta(VTApl *apl);
			  __fastcall ~TMulta(void);
      bool __fastcall Executa(void);
		void __fastcall Reinicia(void);

	protected:  //métodos acessados vias property
		VTCarga*   		__fastcall PM_GetCarga(void);
      VTPerfilBarra* __fastcall PM_GetPerfilBarraPE(void);
		double 	  		__fastcall PM_GetPiorDRC(void);
		double 	  		__fastcall PM_GetPiorDRP(void);
		VTRede*			__fastcall PM_GetRedeSec(void);
		VTRegulador*	__fastcall PM_GetReg(void);
		VTSuprimento*	__fastcall PM_GetSupr(void);
		double     		__fastcall PM_GetValorMultaCA(void);
		double     		__fastcall PM_GetValorMultaPE(void);
		void       		__fastcall PM_SetCarga(VTCarga* carga);
		void		  		__fastcall PM_SetRedeSec(VTRede* rede_sec);
		void		  		__fastcall PM_SetReg(VTRegulador* reg);
		void		  		__fastcall PM_SetSupr(VTSuprimento* supr);
		void		  		__fastcall PM_SetPerfilBarraPE(VTPerfilBarra*	perfil_barra);

	private: //métodos
		double 	  __fastcall CalculaMulta(double energia_kwh, VTClasse *classe, bool ca=false);
		double 	  __fastcall CalculaMultaPE(void);
		void 		  __fastcall MaioresDRCeDRP(void);
      double     __fastcall ValorEUSD(void);
		double 	  __fastcall ValorTUSD(AnsiString cod_classe);

	private: //objetos externos
		VTApl       *apl;

	};
//---------------------------------------------------------------------------
#endif
