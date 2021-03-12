// ---------------------------------------------------------------------------
#ifndef TConverteBaseH
#define TConverteBaseH

//---------------------------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTApl;

// ---------------------------------------------------------------------------
class TConverteBase : public TObject
{
public:
	__fastcall TConverteBase(VTApl *apl_owner);
	__fastcall ~TConverteBase(void);
	bool __fastcall Executa(void);

private:
	void __fastcall TrocaArqRede(AnsiString arq_rede);
	void __fastcall TrocaEstudo(int estudo_id);

private: // objeto externo
	VTApl *apl;

private: //obj. locais
	AnsiString base_original, base_convertida;

};

// ---------------------------------------------------------------------------
#endif
