//---------------------------------------------------------------------------
#ifndef TWebPlanPadraoH
#define TWebPlanPadraoH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <Classes.hpp>
#include "VTWebPlanPadrao.h"

class VTApl;
//---------------------------------------------------------------------------
class TWebPlanPadrao : public VTWebPlanPadrao
   {
   public:
			  __fastcall  TWebPlanPadrao(VTApl *apl);
           __fastcall ~TWebPlanPadrao(void);
      bool __fastcall  ImportaPadrao(AnsiString arq_txt);

   private: //métodos
      void __fastcall Aviso(WideString txt);
//      int  __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
//      bool __fastcall GravaArquivoPadrao(AnsiString arq_txt);
	  bool __fastcall LeDominio(int &dominio_id);
      bool __fastcall LeEnderecoWebService(AnsiString &wsdl);

   private: //objetos locais
		TStrings *lines;

	private: //obj externo
		VTApl *apl;
   };
/*
//---------------------------------------------------------------------------
Endereço do WSDL – para acesso externo

   http://sinapsis-net.dyndns.org:5580/vip/ListarPadraoConstrutivoWebService?wsdl
   http://sinapsis-net.dyndns.org:5580/vipdesenv/ListarPadraoConstrutivoWebService?wsdl
   http://sinapsis-net.dyndns.org:5580/viptreina/ListarPadraoConstrutivoWebService?wsdl
   http://sinapsis-net.dyndns.org:5580/viphomolog/ListarPadraoConstrutivoWebService?wsdl
*/

#endif
//---------------------------------------------------------------------------
//eof
