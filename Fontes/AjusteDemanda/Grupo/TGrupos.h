//---------------------------------------------------------------------------
#ifndef TGruposH
#define TGruposH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>
//#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\AjusteDemanda\Grupo\VTGrupos.h>
//---------------------------------------------------------------------------
class TGrupos : public VTGrupos
{
   public:
		   __fastcall  TGrupos(VTMedicao *medicao);
		   __fastcall ~TGrupos(void);
	void     __fastcall AtualizaProporcao(int fase);
    void     __fastcall ClassificaMedicao(void);
	VTGrupo* __fastcall ExisteGrupo(int categoria);
    void     __fastcall Inicia(void);
	VTGrupo* __fastcall InsereGrupo(int categoria);
	void __fastcall PreencheLisGrupo(TList *lisEXT);

   protected:
	VTMedicao* __fastcall PM_GetMedicao(void);

   private:
    void __fastcall InsereGrupo(VTGrupo *grupo);

   private:
	TList *lisGRUPO;
	VTMedicao *medicao;
	std::map<int,VTGrupo*> mapGRUPO;
};

//---------------------------------------------------------------------------
#endif
//eof
