//---------------------------------------------------------------------------

#ifndef TVerificaMalhaEdicaoH
#define TVerificaMalhaEdicaoH
//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdita;
class VTLigacao;
class VTMalhas;
class VTRede;

//---------------------------------------------------------------------------
class TVerificaMalhaEdicao : public TObject
{
	public:
                   __fastcall  TVerificaMalhaEdicao(VTApl *apl);
                   __fastcall ~TVerificaMalhaEdicao(void);
        bool       __fastcall  VerificaSeFechaMalhaExterna(VTLigacao *lig);
        bool       __fastcall  ExisteAlertaMalha(VTRede* rede);

    private:
        bool __fastcall FechouMalhaExterna(VTLigacao *lig);

	private: //dados
        VTApl *apl;
        VTMalhas *malhas;

};
//---------------------------------------------------------------------------
#endif
