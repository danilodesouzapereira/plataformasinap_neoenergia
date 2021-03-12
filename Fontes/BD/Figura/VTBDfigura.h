// ---------------------------------------------------------------------------
#ifndef VTBDfiguraH
#define VTBDfiguraH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Figura\Estilos\VTEstilos.h"
// ---------------------------------------------------------------------------
class VTApl;
class VTSNPDBConn;
class VTEstudo;

// ---------------------------------------------------------------------------
// Estrutura auxiliar para geração dos identificadores sem consultar a base de dados
//struct sttIndexFigura
//{
//	int ndxFiguraId;
//	int ndxValorFormaAtributoId;
//	int ndxFormaAtributoId;
//	int ndxFormaId;
//	int ndxLinkId;
//	int ndxEstiloId;
//	int ndxEstiloAtributoId;
//	int ndxTextoId;
//	int ndxTipoValorAtributoGraficoId;
//	int ndxAtributoGraficoId;
//};

// ---------------------------------------------------------------------------
class VTBDfigura : public TObject
{
public:
				 __fastcall VTBDfigura(void){};
	virtual 	 __fastcall ~VTBDfigura(void){};
	virtual bool __fastcall LeAuxiliaresFiguras(void)=0;
	virtual bool __fastcall LeFiguras(VTEstudo *estudo, bool estudo_novo, TList* lisREDE_SELE = NULL) = 0;
	virtual bool __fastcall RemoveEstudo(VTEstudo *estudo) = 0;
	virtual bool __fastcall SalvaFiguras(VTEstudo *estudo) = 0;


protected:

//	struct sttIndexFigura index; // Indexação local dos identificadores
};

// ---------------------------------------------------------------------------
VTBDfigura* NewObjBDfigura(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDfigura* NewObjBDfiguraDummy(VTApl *apl_owner, VTSNPDBConn *bd_conn);
#endif
// ---------------------------------------------------------------------------
// eof
