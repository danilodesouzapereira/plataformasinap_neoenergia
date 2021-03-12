//-----------------------------------------------------------------------------
#ifndef VTMontaH
#define VTMontaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTExtrator.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTMonta : public TObject
   {
   public:
                   __fastcall  VTMonta(void) {};
      virtual      __fastcall ~VTMonta(void) {};
      virtual bool __fastcall  InsereAlternativa(strALTERNATIVA &str) = 0;
	  virtual bool __fastcall  InsereDemandaPotencia(strDEMANDA_POTENCIA &str) = 0;
	  virtual bool __fastcall  InsereElementoPotencia(strELEMENTO_POTENCIA &str) = 0; //DVK 2015.04.28
      virtual bool __fastcall  InsereEquipamento(strEQUIPAMENTO &str) = 0;
      virtual bool __fastcall  InsereEstudo(strESTUDO &str) = 0;
	  virtual bool __fastcall  InsereInformacaoTecnica(strINFORMACAO_TECNICA &str) = 0;
      virtual bool __fastcall  InsereMunicipio(strMUNICIPIO &str) = 0;
      virtual bool __fastcall  InserePotenciaInstalada(strPOTENCIA_INSTALADA &str) = 0;
	  virtual bool __fastcall  InsereObra(strOBRA &str) = 0;
	  virtual bool __fastcall  InsereRede(strREDE &str) = 0;
      virtual bool __fastcall  InsereRedeAlternativa(strREDE_ALTERNATIVA &str) = 0;
      virtual bool __fastcall  InsereSolicitacao(strSOLICITACAO &str) = 0;
      virtual bool __fastcall  InsereValorInformacaoTecnica(strVALOR_INFORMACAO_TECNICA &str) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTMonta* NewObjMonta(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
