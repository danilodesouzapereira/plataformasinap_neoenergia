// ---------------------------------------------------------------------------
#ifndef TGerenciaGruposH
#define TGerenciaGruposH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>
// #include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\AjusteDemanda\Grupo\VTGerenciaGrupos.h>

// ---------------------------------------------------------------------------
class TGerenciaGrupos : public VTGerenciaGrupos
{
	public:
	 __fastcall TGerenciaGrupos(void);
	 __fastcall ~TGerenciaGrupos(void);
	 void 	   __fastcall Clear(void);
	 void      __fastcall CriaGrupos(TList *lisMEDICAO_VALIDAS);
	 VTGrupos* __fastcall ExisteGrupos(VTMedicao *medicao);
	 void      __fastcall Inicia(void);
	 VTGrupos* __fastcall InsereGrupos(VTMedicao *medicao);
	 void      __fastcall PreencheLisGrupos(TList *lisEXT);
	 void 	   __fastcall SetCargasAjustavelGrupo(bool ajustavel, int categoria);
	 void      __fastcall SetCargasAjustavelCategoria(bool ajustavel, TList *lisCARGA, int categoria);
	 void 	   __fastcall SetPesoGrupo(double peso, int categoria);
	//funcao de testes/debug
	 void      __fastcall DEBUG_CategoriaAleatoria(TList *lisCARGAS);

	private:
	 void __fastcall InsereGrupos(VTGrupos *grupos);

	private:
	 TList *lisGRUPOS;
	 std::map<VTMedicao*,VTGrupos*> mapGRUPOS;
};

// ---------------------------------------------------------------------------
#endif
// eof
