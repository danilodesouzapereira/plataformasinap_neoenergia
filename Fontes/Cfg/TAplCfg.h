// ---------------------------------------------------------------------------
#ifndef TAplCfgH
#define TAplCfgH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include<stdexcept>
#include "..\Apl\TApl.h"

// --------------------------------------------------------------------------------------- namespace
class VTCurva;

using namespace std;

// ---------------------------------------------------------------------------
class TAplCfg : public TApl
{
public:
	__fastcall TAplCfg(TForm *form_owner, VTApl *apl_owner);
	__fastcall ~TAplCfg(void);
	void __fastcall LeBaseArranjo(void);
	void __fastcall LeBaseCurvaTipica(void);
	void __fastcall LeBaseTipoChave(void);
	void __fastcall LeBaseTipoRede(void);
	void __fastcall SalvaBaseArranjo(void);
	void __fastcall SalvaBaseClasseConsumidor(void);
	// void __fastcall  SalvaBaseCurvaTipica(void);
	void __fastcall SalvaBaseCurvaTipica(VTCurva *curva);
	void __fastcall SalvaBaseTipoChave(void);
	void __fastcall SalvaBaseTipoRede(void);

	/**
	 * Este metodo acessa a base da empresa(\SINAPgrid4.3\Dat\RedesPadrao\RedeEmpresa.mdb) e retorna
	 * a lista de curvas típicas(Demanda de cosumidor e Iluminação pública) da empresa.
	 * @return uma lista <code>TList</code> de curvas típicas <code>VTCurva</code>
	 * @throws uma exceção do tipo <code>runtime_error</code> caso ocorra algum erro na troca/acesso
	 * das bases (redeUsuario -> RedeEmpresa.mdb -> redeUsuario)
	 * @see VTCurva
	 */
	TList* __fastcall GetCurvasTipicasEmpresa() throw(runtime_error);


private:
	bool __fastcall ExisteCurva(TList *lisCURVA, VTCurva *curva);

};

#endif
// ---------------------------------------------------------------------------
// eof
