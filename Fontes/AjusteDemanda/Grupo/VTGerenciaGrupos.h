// ---------------------------------------------------------------------------
#ifndef VTGerenciaGruposH
#define VTGerenciaGruposH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <PlataformaSinap\Fontes\Constante\Fases.h>
//#include <complex>
// ---------------------------------------------------------------------------
class VTMedicao;
class VTMedicoes;
class VTGrupos;
// ---------------------------------------------------------------------------
class VTGerenciaGrupos : public TObject
{
	public:
	 __fastcall VTGerenciaGrupos(void){};
	 __fastcall ~VTGerenciaGrupos(void){};

	 virtual void      __fastcall Clear(void) = 0;
	 virtual void      __fastcall CriaGrupos(TList *lisMEDICAO_VALIDAS) = 0;
	 virtual VTGrupos* __fastcall ExisteGrupos(VTMedicao *medicao) = 0;
     virtual void      __fastcall Inicia(void) = 0;
	 virtual VTGrupos* __fastcall InsereGrupos(VTMedicao *medicao) = 0;
	 virtual void      __fastcall PreencheLisGrupos(TList *lisEXT) = 0;
	 virtual void      __fastcall SetCargasAjustavelGrupo(bool ajustavel, int categoria) = 0;
	 virtual void      __fastcall SetCargasAjustavelCategoria(bool ajustavel, TList *lisCARGA, int categoria) = 0;
	 virtual void 	   __fastcall SetPesoGrupo(double peso, int categoria) = 0;
	 //funcao de testes/debug
	 virtual void      __fastcall DEBUG_CategoriaAleatoria(TList *lisCARGAS) = 0;

};

// ---------------------------------------------------------------------------
VTGerenciaGrupos* __fastcall NewObjGerenciaGrupos(void);
// ---------------------------------------------------------------------------
#endif
// eof
