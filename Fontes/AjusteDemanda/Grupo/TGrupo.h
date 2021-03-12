// ---------------------------------------------------------------------------
#ifndef TGrupoH
#define TGrupoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// #include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\AjusteDemanda\Grupo\VTGrupo.h>

// ---------------------------------------------------------------------------
class TGrupo : public VTGrupo
{
	public:
		__fastcall TGrupo(void);
		__fastcall ~TGrupo(void);
		void __fastcall Inicia(void);
};

// ---------------------------------------------------------------------------
#endif
// eof
