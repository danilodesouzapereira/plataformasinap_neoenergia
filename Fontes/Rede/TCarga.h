//---------------------------------------------------------------------------
#ifndef TCargaH
#define TCargaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCarga.h"

//---------------------------------------------------------------------------
class TCarga : public VTCarga
   {
   public:
					  __fastcall  TCarga(int num_pat);
                 __fastcall ~TCarga(void);
		VTCarga*   __fastcall  Clone(void);
		bool       __fastcall  ConsumidorTipico(void);
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
		void       __fastcall  DefineClasse(VTClasse *classe);
		void       __fastcall  DefineCrescimento(VTCrescimento *crescimento);
		void       __fastcall  DefineCurva(VTCurva *curva);
		VTCurva*   __fastcall  ExisteCurva(void);
		int        __fastcall  Tipo(void);
		AnsiString __fastcall  TipoAsString(void);

	protected: //métodos acessados via property
		bool   __fastcall PM_GetClasseIP(void);
		bool   __fastcall PM_GetDefinidaPorFase(void);
		double __fastcall PM_GetEnergia_kwhmes(void);
		double __fastcall PM_GetEnergia_mwhmes(void);
		bool   __fastcall PM_GetEquilibrada(void);
      bool   __fastcall PM_GetGrupoA(void);
		int    __fastcall PM_GetMBTfasica(void);
		int    __fastcall PM_GetModeloCarga(void);
		int    __fastcall PM_GetNumConsTotal(void);
		double __fastcall PM_GetFatorAjusteP(int nf);
		double __fastcall PM_GetFatorAjusteQ(int nf);
	  bool   __fastcall PM_GetVeryImportant(void);
	  //
	  void   __fastcall PM_SetEnergia_kwhmes(double energia_kwhmes);
	  void   __fastcall PM_SetEnergia_mwhmes(double energia_mwhmes);
	  void   __fastcall PM_SetGrupoA(bool grupoA);
	  void	 __fastcall PM_SetFatorAjusteP(int nf, double valor);
	  void	 __fastcall PM_SetFatorAjusteQ(int nf, double valor);
      void   __fastcall PM_SetVeryImportant(bool vip);
   };

//---------------------------------------------------------------------------
#endif
//eof

 