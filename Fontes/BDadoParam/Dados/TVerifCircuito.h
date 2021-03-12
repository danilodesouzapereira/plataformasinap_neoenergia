//---------------------------------------------------------------------------
#ifndef TVerifCircuitoH
#define TVerifCircuitoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTVerifCircuito.h"

//---------------------------------------------------------------------------
class TVerifCircuito : public VTVerifCircuito
	{
	public:
					  __fastcall TVerifCircuito(void);
					  __fastcall ~TVerifCircuito(void);

	private: //métodos

	protected:  //métodos acessados via property
		AnsiString   __fastcall PM_GetCodigo(void);
		double       __fastcall PM_GetDecAjust(void);
		double       __fastcall PM_GetDecVerif(void);
		double       __fastcall PM_GetFecAjust(void);
		double       __fastcall PM_GetFecVerif(void);
		VTRede*      __fastcall PM_GetRede(void);
		void         __fastcall PM_SetCodigo(AnsiString codigo);
		void         __fastcall PM_SetDecAjust(double decAjust);
		void         __fastcall PM_SetDecVerif(double decVeric);
		void         __fastcall PM_SetFecAjust(double fecAjust);
		void         __fastcall PM_SetFecVerif(double fecVerif);
		void         __fastcall PM_SetRede(VTRede* rede);

	private: //dados acessados via property
		struct   {
					AnsiString codigo;
					AnsiString cjto;
					bool selecionado;
					double decAjust;
					double decVerif;
					double fecAjust;
					double fecVerif;
					}PD;

		VTRede *rede; //rede Sinap

	private: //dados locais
	};

#endif
//---------------------------------------------------------------------------
