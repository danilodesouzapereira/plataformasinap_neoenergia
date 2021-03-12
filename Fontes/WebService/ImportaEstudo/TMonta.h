//---------------------------------------------------------------------------
#ifndef TMontaH
#define TMontaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"
#include "..\WebService\TReceberEstudoWebService.h"

//---------------------------------------------------------------------------
class TMonta : public VTMonta
   {
   public:
           __fastcall  TMonta(VTApl *apl_owner);
           __fastcall ~TMonta(void);
      bool __fastcall  InsereAlternativa(strALTERNATIVA &str);
	  bool __fastcall  InsereDemandaPotencia(strDEMANDA_POTENCIA &str);
	  bool __fastcall  InsereElementoPotencia(strELEMENTO_POTENCIA &str); //DVK 2015.04.28
      bool __fastcall  InsereEquipamento(strEQUIPAMENTO &str);
      bool __fastcall  InsereEstudo(strESTUDO &str);
	  bool __fastcall  InsereInformacaoTecnica(strINFORMACAO_TECNICA &str);
	  bool __fastcall  InsereMunicipio(strMUNICIPIO &str);
      bool __fastcall  InserePotenciaInstalada(strPOTENCIA_INSTALADA &str);
	  bool __fastcall  InsereObra(strOBRA &str);
	  bool __fastcall  InsereRede(strREDE &str);
      bool __fastcall  InsereRedeAlternativa(strREDE_ALTERNATIVA &str);
      bool __fastcall  InsereSolicitacao(strSOLICITACAO &str);
      bool __fastcall  InsereValorInformacaoTecnica(strVALOR_INFORMACAO_TECNICA &str);

   private: //objetos externos
      VTApl       *apl;
      struct{
			NS_ReceberEstudoWebService::estudo                  	*estudo;
			NS_ReceberEstudoWebService::alternativa             	*alternativa;
			NS_ReceberEstudoWebService::redeAlternativa         	*rede_alternativa;
			NS_ReceberEstudoWebService::obra                    	*obra;
			NS_ReceberEstudoWebService::informacaoTecnicaComercial  *informacao_tecnica;
			NS_ReceberEstudoWebService::elementoPotencia            *elemento_potencia;
			NS_ReceberEstudoWebService::equipamento                 *equipamento;
			NS_ReceberEstudoWebService::municipio                   *municipio;
			}web;

   private: //dados locais
   };

#endif
//-----------------------------------------------------------------------------
// eof



