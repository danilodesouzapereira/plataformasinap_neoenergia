//-----------------------------------------------------------------------------
#ifndef VTExportaANEELH
#define VTExportaANEELH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//op��es do usu�rio
struct strOPCAO_ANEEL	{
								bool rede_aneel;
								int  trafoDY;
								int  fmt_codigo;	//{0:Sem coordenadas; 1: Coordenadas utm; 2:Coordenadas utm com numerador sequencial}
								};

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;

//-----------------------------------------------------------------------------
class VTExportaANEEL : public TObject
   {
   public:
                   __fastcall  VTExportaANEEL(void) {};
      virtual      __fastcall ~VTExportaANEEL(void) {};
		virtual bool __fastcall  ExportaRedes(AnsiString filename, strOPCAO_ANEEL &opcao) = 0;
	};

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTExportaANEEL
//-----------------------------------------------------------------------------
VTExportaANEEL* NewObjExportaANEEL(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
