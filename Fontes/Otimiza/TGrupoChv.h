//---------------------------------------------------------------------------
#ifndef TGrupoChvH
#define TGrupoChvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTGrupo.h"
//---------------------------------------------------------------------------
class VTEqpto;
//---------------------------------------------------------------------------
class TGrupoChv : public VTGrupo
	{
	public:
		__fastcall TGrupoChv(VTEqpto* eqpto);
		__fastcall ~TGrupoChv(void);
		void       __fastcall AddEqpto(VTEqpto* eqpto);
		void       __fastcall AtivaEqpto(VTEqpto* eqpto){return;};
		AnsiString __fastcall Codigo(void);
		void       __fastcall DesatEqpto(VTEqpto* eqpto) {return;};
		VTEqpto*   __fastcall Eqpto(int neq);
		TList*     __fastcall LisEqpto(void);
		int        __fastcall NEq(void);
		void       __fastcall RemEqpto(VTEqpto* eqpto);
		bool       __fastcall VerEqpto(VTEqpto* eqpto);
		//Particulares de TGrupoChv
		int        __fastcall Estado(void);
		void       __fastcall Abre(void);
		void       __fastcall Fecha(void);

	 private://métodos
		TList* lisCHV;//Lista de Chaves do grupo
	 public:
		bool flag_conec;
		bool flag_vis;
		int  estado_ini;
		int  itipoAtua; //tipo de atuação Auto/Man
		TObject* p1;
		TObject* p2;
		unsigned estado;

	};
//---------------------------------------------------------------------------
#endif

