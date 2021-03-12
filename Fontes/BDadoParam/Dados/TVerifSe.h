//---------------------------------------------------------------------------

#ifndef TVerifSeH
#define TVerifSeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTVerifSe.h"

//---------------------------------------------------------------------------
class TVerifSe : public VTVerifSe
	{
	public:
					  __fastcall TVerifSe(void);
					  __fastcall ~TVerifSe(void);

	private: //métodos

	protected:  //métodos acessados via property
		AnsiString   __fastcall PM_GetCodigo(void);
		AnsiString   __fastcall PM_GetCodigoCjto(void);
		VTVerifCjto* __fastcall PM_GetConjunto(void);
		double       __fastcall PM_GetDecAjust(void);
		double       __fastcall PM_GetDecVerif(void);
		double       __fastcall PM_GetFecAjust(void);
		double       __fastcall PM_GetFecVerif(void);
		//VTRede*      __fastcall PM_GetRede(void);
		bool         __fastcall PM_GetSelecionada(void);
		void         __fastcall PM_SetCodigo(AnsiString codigo);
		void         __fastcall PM_SetCodigoCjto(AnsiString codigo);
		void         __fastcall PM_SetConjunto(VTVerifCjto* cjto);
		void         __fastcall PM_SetDecAjust(double decAjust);
		void         __fastcall PM_SetDecVerif(double decVeric);
		void         __fastcall PM_SetFecAjust(double fecAjust);
		void         __fastcall PM_SetFecVerif(double fecVerif);
		//void         __fastcall PM_SetRede(VTRede* rede);
		void         __fastcall PM_SetSelecionada(bool selecionada);

	private: //dados acessados via property
		struct   {
					AnsiString codigo;
					AnsiString cjto;
					bool selecionada;
					double decAjust;
					double decVerif;
					double fecAjust;
					double fecVerif;
					}PD;

		VTVerifCjto *cjto;
		//VTRede      *rede; //rede Sinap

	private: //dados locais
	};

#endif
//---------------------------------------------------------------------------
