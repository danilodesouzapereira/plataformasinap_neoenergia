//---------------------------------------------------------------------------
#ifndef TVerifCjtoH
#define TVerifCjtoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTVerifCjto.h"

//---------------------------------------------------------------------------
class TVerifCjto : public VTVerifCjto
	{
	public:
					  __fastcall TVerifCjto(void);
					  __fastcall ~TVerifCjto(void);

	private: //métodos

	protected:  //métodos acessados via property
		AnsiString __fastcall PM_GetCodigo(void);
		double     __fastcall PM_GetDecAjust(void);
		double     __fastcall PM_GetDecVerif(void);
		double     __fastcall PM_GetFecAjust(void);
		double     __fastcall PM_GetFecVerif(void);
		TList*     __fastcall PM_GetLisVerifSe(void);
		void       __fastcall PM_SetCodigo(AnsiString codigo);
		void       __fastcall PM_SetDecAjust(double decAjust);
		void       __fastcall PM_SetDecVerif(double decVeric);
		void       __fastcall PM_SetFecAjust(double fecAjust);
		void       __fastcall PM_SetFecVerif(double fecVerif);

	private: //dados acessados via property
		struct   {
					AnsiString codigo;
					double decAjust;
					double decVerif;
					double fecAjust;
					double fecVerif;
					}PD;

	private: //dados locais
		TList  *lisVerifSe;
	};

#endif
//---------------------------------------------------------------------------
