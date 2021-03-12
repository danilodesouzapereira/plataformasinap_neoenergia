//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResCurto.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTNo.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTResCurto* __fastcall NewObjResCurto(VTApl *apl)
   {
	return(new TResCurto(apl));
   }

//---------------------------------------------------------------------------
__fastcall TResCurto::TResCurto(VTApl *apl)
   {
	//salva ponteiro para objeto
	this->apl    = apl;
	this->redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	}

//---------------------------------------------------------------------------
__fastcall TResCurto::~TResCurto(void)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaIccBarDef(VTEqpto *eqp_no, STR_FASE *ifase, STR_SEQ *iseq, STR_FASE *assim_ifase)
   {
	//vari�veis locais
	VTBarra   *barra;
	VTNo      *no;
	VTBarCC   *barCC;

	//prote��o
	if(eqp_no == NULL)            return(false);
	if(eqp_no->Tipo() != eqptoNO) return(false);
	//determina No e sua Barra pai
	no = (VTNo *) eqp_no;
	if((barra = no->pai) == NULL) return(false);
	//determina BarCC
	if((barCC = (VTBarCC *) barra->Obj) == NULL) return(false);
	//salva dados em RedeCC
	if(redeCC != NULL) redeCC->SalvaIccBarDef(barCC, ifase, iseq, assim_ifase);

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaIccBarSup(VTEqpto *eqp_no, STR_FASE *ifase, STR_SEQ *iseq)
   {
	//vari�veis locais
	VTBarra   *barra;
	VTNo      *no;
	VTBarCC   *barCC;

	//prote��o
	if(eqp_no == NULL)            return(false);
	if(eqp_no->Tipo() != eqptoNO) return(false);
	//determina No e sua Barra pai
	no = (VTNo *) eqp_no;
	for(int nb=0; nb < no->lisBAR->Count; nb++)
		{
		barra = (VTBarra *) no->lisBAR->Items[nb];
		if((barCC = (VTBarCC *) barra->Obj) == NULL) continue;
		//salva correntes da barra
		redeCC->SalvaIccBarSup(barCC, ifase, iseq);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaIccLigacao(VTEqpto *eqp_lig, VTEqpto *eqp_no_ref, STR_FASE ifase[][NBAR_LIG], STR_SEQ iseq[][NBAR_LIG])
	{
	//vari�veis locais
	VTLigacao *ligacao;
	VTBarra   *bar_ref;
	VTNo      *no_ref;
	VTLigCC   *ligCC;
	VTBarCC   *barCC_ref;

	//prote��o: valida Ligacao
	if(eqp_lig == NULL)          return(false);
	if(! eqp_lig->TipoLigacao()) return(false);
	//prote��o: valida No de referencia
	if(eqp_no_ref == NULL)            return(false);
	if(eqp_no_ref->Tipo() != eqptoNO) return(false);
	//determina Ligacao e Barra de refer�ncia
	ligacao = (VTLigacao *) eqp_lig;
	no_ref  = (VTNo *) eqp_no_ref;
	if((bar_ref = no_ref->pai) == NULL) return(false);
	//determina LigCC e BarCC de refer�ncia
	ligCC     = (VTLigCC *) ligacao->Obj;
	barCC_ref = (VTBarCC *) bar_ref->Obj;
	//salva correntes de curto na liga��o
	if(redeCC != NULL) redeCC->SalvaIccLigCC(ligCC, barCC_ref, ifase, iseq);

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaPotenciasCurto(complex<double> s3f_mva,
                                               complex<double> sft_mva)
	{
   if(redeCC != NULL)
      {
	   redeCC->S3f_mva = s3f_mva;
	   redeCC->Sft_mva = sft_mva;
      }

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaVccBarra(VTEqpto *eqp_no, STR_FASE *vfase, STR_SEQ *vseq)
	{
	//vari�veis locais
	VTBarra   *barra;
	VTNo      *no;
	VTBarCC   *barCC;

	//prote��o
	if(eqp_no == NULL)            return(false);
	if(eqp_no->Tipo() != eqptoNO) return(false);
	//determina No e sua Barra pai
	no = (VTNo *) eqp_no;
/*
	if((barra = no->pai) == NULL) return(false);
	//determina BarCC
	if((barCC = (VTBarCC *) barra->Obj) == NULL) return(false);
	//salva tesn�es de curto na Barra
	redeCC->SalvaVccBarCC(barCC, vfase, vseq);
*/
	for(int nb=0; nb < no->lisBAR->Count; nb++)
		{
		barra = (VTBarra *) no->lisBAR->Items[nb];
		if((barCC = (VTBarCC *) barra->Obj) == NULL) continue;
		//salva correntes da barra
		if(redeCC != NULL) redeCC->SalvaVccBarCC(barCC, vfase, vseq);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaZentrada(complex<double> z0_ohm,
                                         complex<double> z1_ohm)
	{
   if(redeCC != NULL)
      {
	   redeCC->Z0entrada_ohm = z0_ohm;
	   redeCC->Z1entrada_ohm = z1_ohm;
      }

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TResCurto::SalvaZentradaSoTrechosRede(complex<double> z0_ohm,
                                                      complex<double> z1_ohm)
	{
   if(redeCC != NULL)
      {
     	redeCC->Z0entradaSoTrechos_ohm = z0_ohm;
	   redeCC->Z1entradaSoTrechos_ohm = z1_ohm;
      }

	return(true);
	}

//---------------------------------------------------------------------------
//eof
