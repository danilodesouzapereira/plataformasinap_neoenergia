// ---------------------------------------------------------------------------
#ifndef VTGruposH
#define VTGruposH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// #include <PlataformaSinap\Fontes\Constante\Fases.h>
// #include <complex>
// ---------------------------------------------------------------------------
class VTMedicao;
class VTGrupo;
// ---------------------------------------------------------------------------
class VTGrupos : public TObject
{
	public:  //property
	  __property VTMedicao* Medicao = {read=PM_GetMedicao};

	public:
		__fastcall VTGrupos(void){};
		__fastcall ~VTGrupos(void){};
    virtual void     __fastcall AtualizaProporcao(int fase) = 0;
	virtual VTGrupo* __fastcall ExisteGrupo(int categoria) = 0;
	virtual void     __fastcall ClassificaMedicao(void) = 0;
	virtual void     __fastcall Inicia(void) = 0;
	virtual VTGrupo* __fastcall InsereGrupo(int categoria) = 0;
    virtual void 	 __fastcall PreencheLisGrupo(TList *lisEXT) = 0;

	protected: //métodos acessados via property
	  virtual VTMedicao* __fastcall PM_GetMedicao(void) = 0;
};
// ---------------------------------------------------------------------------
VTGrupos* __fastcall NewObjGrupos(VTMedicao *medicao);
// ---------------------------------------------------------------------------
#endif
// eof
