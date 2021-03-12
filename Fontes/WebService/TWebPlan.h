//---------------------------------------------------------------------------
#ifndef TWebPlanH
#define TWebPlanH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <Classes.hpp>
#include "VTWebPlan.h"
#include "TReceberEstudoWebService.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TWebPlan : public VTWebPlan
   {
   public:
					  __fastcall  TWebPlan(VTApl *apl);
                 __fastcall ~TWebPlan(void);
      bool       __fastcall  EnviaEstudo(AnsiString arq_txt);
      AnsiString __fastcall  MsgErroWebService(void);

   private: //métodos
      void __fastcall Aviso(WideString txt);
      void __fastcall EstudoWebDelete(void);
//      int  __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
		void __fastcall GravaRespostaNoRegistro(AnsiString msg = "");
      bool __fastcall LeEnderecoWebService(AnsiString &wsdl);
      bool __fastcall MontaEstudoWeb(AnsiString arq_txt);

   private: //dados externos
      VTApl    *apl;

   private: //objetos locais
      AnsiString                            msg_erro;
      TStrings                              *lines;
      NS_ReceberEstudoWebService::estudo    *estudo_web;
      NS_ReceberEstudoWebService::resposta  *resposta_web;
      _di_ReceberEstudoWebService           ReceberEstudoWebService;
   };

/*
//---------------------------------------------------------------------------
Endereço VIP – para acesso externo
   http://sinapsis-net.dyndns.org:5580/vip/ReceberEstudoWebService?wsdl
   http://sinapsis-net.dyndns.org:5580/viptreina/ReceberEstudoWebService?wsdl
   http://sinapsis-net.dyndns.org:5580/vipdesenv/ReceberEstudoWebService?wsdl
   http://sinapsis-net.dyndns.org:5580/viphomolog/ReceberEstudoWebService?wsdl

Endereço ARIES – para acesso externo
   http://sinapsis-net.dyndns.org:5580/aries/ReceberEstudoWebService
   http://sinapsis-net.dyndns.org:5580/aries/ListarPadraoConstrutivoWebService

//---------------------------------------------------------------------------
*/

#endif
//---------------------------------------------------------------------------
//eof
