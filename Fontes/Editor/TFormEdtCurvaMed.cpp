//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TFormEdtCurvaMed.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtCurvaMed::TFormEdtCurvaMed(TForm* form_owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   //insere em seu Parent
   if (parent)
      {
      Parent = parent;
      Align  = alClient;
      }
   //cria objeto Patamares e define sua escala
   patamares  = DLL_NewObjPatamares(apl);
   legenda    = DLL_NewObjLegenda(apl);
   lisVETVAL  = new TList();
   //pré-seleciona TabSheetPonto
   PageControl->ActivePage = TabSheetPonto;
   //inicia flag que não permite pot.ativa negativa
   pot_neg_enabled = false;
	//exibe todas as colunas //DVK 2015.10.30 retirado
//   Exibe.ColunaP = true;
//   Exibe.ColunaQ = true;
//	Exibe.ColunaV = true;
//	Exibe.ColunaF = true;
	}

//---------------------------------------------------------------------------
__fastcall TFormEdtCurvaMed::~TFormEdtCurvaMed(void)
   {
   //destrói objetos criados
   if (legenda)   {delete legenda;   legenda   = NULL;}
   if (patamares) {delete patamares; patamares = NULL;}
   //destrói listas e seus obejtos
   if (lisVETVAL) {LimpaTList(lisVETVAL); delete lisVETVAL; lisVETVAL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::ChartExibeCurva(VTVetval *vetval)
   {
   //variáveis locais
   int         ind_cor;
   double      val, val_max, val_min, increment;
   AnsiString  titulo;
   TColor      cor = clBlue;

   //reinicia série
   Series1->Clear();
   if (chart->LeftAxis->Maximum >= 0)
      {
      chart->LeftAxis->Minimum  = 0.;
      chart->LeftAxis->Maximum  = 0.;
      }
   else
      {
      chart->LeftAxis->Maximum  = 0.;
      chart->LeftAxis->Minimum  = 0.;
      }
   //preenche a Série c/ os valores em todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina valor no patamar
      val = vetval->Valor[np];
      //insere ponto na Serie
      //Series1->AddXY(np+1, val, IntToStr(np+1), cor);
      Series1->AddXY(np,   val, IntToStr(np+1), cor);
      Series1->AddXY(np+1, val, IntToStr(np+1), cor);
      //determina valores máximo e mínimo
      if (np == 0) val_max = val_min = val;
      val_max = max(val_max, val);
      val_min = min(val_min, val);
      }
   //inclui uma margem adicional nos valores máximo e mínimo
   (val_max > 0) ? (val_max += 0.1 * val_max) : (val_max -= 0.1 * val_max);
   (val_min > 0) ? (val_min -= 0.1 * val_min) : (val_min += 0.1 * val_min);
   //define incremento
   increment = (val_max - val_min) / 10.;
   if      (increment > 10000) increment = 10000.;
   else if (increment > 1000)  increment = 1000.;
   else if (increment > 100)   increment = 100.;
   else if (increment > 10)    increment = 10.;
   else if (increment > 1.)    increment = 1.;
   else if (increment > 0.1)   increment = 0.1;
   else if (increment > 0.01)  increment = 0.01;
   else                        increment = 0.001;
   //redefine valores mínimo e máximo do eixo y
   chart->LeftAxis->Visible   = true;
   chart->LeftAxis->Automatic = false;
   if (val_max >= 0)
      {
      chart->LeftAxis->Maximum   = val_max;
      chart->LeftAxis->Minimum   = val_min;
      }
   else
      {
      chart->LeftAxis->Minimum   = val_min;
      chart->LeftAxis->Maximum   = val_max;
      }
   chart->LeftAxis->Increment = increment;
   //reinicia título
   chart->Title->Text->Clear();
   chart->Title->Text->Add(vetval->DescUnid());
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::ChartExibeCurva(void)
	{//DVK 2015.10.30 curva com apenas 1 grandeza
	//variáveis locais
	strHM			hm;
	int         ind_cor;
	double      valor, val_max, val_min, increment;
   AnsiString  titulo;
	TColor      cor = clBlue;
	VTPatamar   *patamar;
	TList       *lisPAT = patamares->LisPatamar();

   //reinicia série
	Series1->Clear();
   if (chart->LeftAxis->Maximum >= 0)
		{
      chart->LeftAxis->Minimum  = 0.;
      chart->LeftAxis->Maximum  = 0.;
      }
   else
      {
      chart->LeftAxis->Maximum  = 0.;
      chart->LeftAxis->Minimum  = 0.;
		}
	//reconfigura curva conforme escala do Patamares
	canal->Curva->Reconfigura(patamares->Escala);
	//preenche a Série c/ os valores em todos patamares
	for (int np = 0; np < lisPAT->Count; np++)
		{//determina valor no patamar
		patamar   = (VTPatamar*)lisPAT->Items[np];
		hm.hora   = patamar->HoraIni;
		hm.minuto = patamar->MinutoIni;
		//reinicia valor
		valor = 0;
		//atualiza valor conforme a grandeza do canal
		if(! canal->Curva->GetValor(hm, &valor, sizeof(valor)/sizeof(double))) break;
		//insere ponto na Serie
		Series1->AddXY(np,   valor, IntToStr(np+1), cor);
		Series1->AddXY(np+1, valor, IntToStr(np+1), cor);
		//determina valores máximo e mínimo
		if (np == 0) val_max = val_min = valor;
		val_max = max(val_max, valor);
      val_min = min(val_min, valor);
      }
   //inclui uma margem adicional nos valores máximo e mínimo
   (val_max > 0) ? (val_max += 0.1 * val_max) : (val_max -= 0.1 * val_max);
   (val_min > 0) ? (val_min -= 0.1 * val_min) : (val_min += 0.1 * val_min);
   //define incremento
   increment = (val_max - val_min) / 10.;
   if      (increment > 10000) increment = 10000.;
   else if (increment > 1000)  increment = 1000.;
   else if (increment > 100)   increment = 100.;
   else if (increment > 10)    increment = 10.;
   else if (increment > 1.)    increment = 1.;
   else if (increment > 0.1)   increment = 0.1;
   else if (increment > 0.01)  increment = 0.01;
   else                        increment = 0.001;
   //redefine valores mínimo e máximo do eixo y
   chart->LeftAxis->Visible   = true;
   chart->LeftAxis->Automatic = false;
   if (val_max >= 0)
      {
      chart->LeftAxis->Maximum   = val_max;
      chart->LeftAxis->Minimum   = val_min;
      }
   else
      {
      chart->LeftAxis->Minimum   = val_min;
      chart->LeftAxis->Maximum   = val_max;
      }
	chart->LeftAxis->Increment = increment;
   //reinicia título
   chart->Title->Text->Clear();
	chart->Title->Text->Add(canal->TipoCanalAsString);
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::CBoxNumPtoInicia(void)
	{
	//verifica o número de pontos da Curva definido pela Curva
	switch(curva->Escala)
		{
		case pat24HORA:   CBoxNumPto->ItemIndex =  0; break;
		case pat12HORA:   CBoxNumPto->ItemIndex =  1; break;
		case pat08HORA:   CBoxNumPto->ItemIndex =  2; break;
		case pat06HORA:   CBoxNumPto->ItemIndex =  3; break;
		case pat04HORA:   CBoxNumPto->ItemIndex =  4; break;
		case pat03HORA:   CBoxNumPto->ItemIndex =  5; break;
		case pat02HORA:   CBoxNumPto->ItemIndex =  6; break;
		case pat01HORA:   CBoxNumPto->ItemIndex =  7; break;
		case pat15MIN:    CBoxNumPto->ItemIndex =  8; break;
      case pat10MIN:    CBoxNumPto->ItemIndex =  9; break;
		case pat05MIN:    CBoxNumPto->ItemIndex = 10; break;
		case patEDITAVEL: CBoxNumPto->ItemIndex = 11; break;
		default: //curva c/ escala indefinida
			curva->Escala = pat01HORA;
         //chamada recursiva c/ Escala definida
			CBoxNumPtoInicia();
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::CBoxNumPtoChange(TObject *Sender)
   {
   //redefine número de Patamares
   switch(CBoxNumPto->ItemIndex)
      {
      case  0: patamares->Escala = pat24HORA;   break;
      case  1: patamares->Escala = pat12HORA;   break;
      case  2: patamares->Escala = pat08HORA;   break;
      case  3: patamares->Escala = pat06HORA;   break;
      case  4: patamares->Escala = pat04HORA;   break;
      case  5: patamares->Escala = pat03HORA;   break;
      case  6: patamares->Escala = pat02HORA;   break;
      case  7: patamares->Escala = pat01HORA;   break;
      case  8: patamares->Escala = pat15MIN;    break;
      case  9: patamares->Escala = pat10MIN;    break;
      case 10: patamares->Escala = pat05MIN;    break;
      default: patamares->Escala = patEDITAVEL; break;
      }
   //reconfigura a Curva (muda número de pontos, salvando seus valores originais)
   curva->Reconfigura(patamares->Escala);
   //reapresenta pontos da Curva
   SGridInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::FormShow(TObject *Sender)
   {
   //apresenta pontos da Curva (comentado: outras funções chamam SGridInicia)
   //SGridInicia();
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeDadosCanal(void)
   {
   //variáveis locais
   double tolerancia;

   //proteção
   if (canal == NULL) return(false);
   //verifica se o código do Canal está definido
   if (EditCodigo->Text.IsEmpty())
      {
		Aviso("Defina o código do Canal: " + canal->Eqpto->TipoAsString() + " " + canal->Eqpto->Codigo);
		return(false);
		}
   //lê erro do Canal
   try{
      tolerancia = StrToDouble(EditTolerancia->Text);
      }catch(Exception &e)
         {
			Erro("Canal " + EditCodigo->Text + ": tolerância com valor inválido");
         return(false);
         }
   //atualiza dados do Canal
   canal->Codigo = EditCodigo->Text.Trim();
   canal->Tolerancia_perc = tolerancia;
   //lê e valida dados de medição
   if (! LeValidaPontosCurva()) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LePontosCurva(void)
   {
   //proteção
   if (curva == NULL) return(false);
   //verifica se Curva c/ patamares de período variável
	if (CBoxNumPto->ItemIndex == 11) return(LeDadosPatamarVariavel());
   else                             return(LeDadosPatamarFixo());
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeDadosPatamarFixo(void)
	{
	//variáveis locais
	strHM      hm_ini, hm_fim;
	unionVALOR valor;
	VTPatamar *patamar;

	//proteção
	if (curva == NULL) return(false);
	//garante unidade da Curva em MVA
	curva->Unidade = unidMVA;
	try{//atualiza pontos da Curva
		for(int np = 0, nr = 1; np < patamares->NumPatamar(); np++, nr++)
			{
			patamar       = patamares->Patamar[np];
			//define hora inicial e final do patamar
			hm_ini.hora   = patamar->HoraIni;
			hm_ini.minuto = patamar->MinutoIni;
			hm_fim.hora   = patamar->HoraFim;
			hm_fim.minuto = patamar->MinutoFim;
			//assume valores nulos p/ P,Q,V,I,T
			valor.var.p = 0;
			valor.var.q = 0;
			valor.var.v = 0;
			valor.var.i = 0;
			valor.var.t = 0;
			//lê valor somente o dado estiver habilitado
			if (Exibe.ColunaP) valor.var.p = StrToDouble(SGrid->Cells[2][nr]);
			if (Exibe.ColunaQ) valor.var.q = StrToDouble(SGrid->Cells[3][nr]);
			if (Exibe.ColunaV) valor.var.v = StrToDouble(SGrid->Cells[4][nr]);
			if (Exibe.ColunaI) valor.var.i = StrToDouble(SGrid->Cells[5][nr]);
			if (Exibe.ColunaF) valor.var.t = StrToDouble(SGrid->Cells[6][nr]);
			//valida valor somente o dado estiver habilitado
			if ((Exibe.ColunaP)&&(! ValidaP_mw(np, valor.var.p)))   return(false);
			if ((Exibe.ColunaQ)&&(! ValidaQ_mvar(np, valor.var.q))) return(false);
			if ((Exibe.ColunaV)&&(! ValidaV_pu(np, valor.var.v)))   return(false);
			if ((Exibe.ColunaI)&&(! ValidaI_amp(np, valor.var.i)))  return(false);
			if ((Exibe.ColunaF)&&(! ValidaT_grau(np, valor.var.t))) return(false);
			//insere patamar na Curva
			if (! curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double))) return (false);
			}
		}catch(Exception &e)
			{
			return(false);
			}
	//verifica se a Curva cobre o período de 24 horas
	if (! curva->ValidaPeriodo24Horas()) return(false);
	return(true);
	}
*/ //DVK 2015.10.29 reescrito, considerando que a curva do canal possui 1 grandeza
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeDadosPatamarFixo(void)
	{
	//variáveis locais
	strHM      hm_ini, hm_fim;
	double     valor;
	VTPatamar *patamar;

	//proteção
	if (curva == NULL) return(false);
	//garante unidade da Curva em MVA
	curva->Unidade = unidMVA;
	try{//atualiza pontos da Curva
		for(int np = 0, nr = 1; np < patamares->NumPatamar(); np++, nr++)
			{
			patamar       = patamares->Patamar[np];
			//define hora inicial e final do patamar
			hm_ini.hora   = patamar->HoraIni;
			hm_ini.minuto = patamar->MinutoIni;
			hm_fim.hora   = patamar->HoraFim;
			hm_fim.minuto = patamar->MinutoFim;
			//assume valor nulo
			valor = 0;
			//lê valor
			if(! StrToDouble(SGrid->Cells[2][nr], valor))
				{
				Aviso("Valor inválido: " + SGrid->Cells[2][nr]);
				return(false);
				}
			//valida valor
			if ((canal->TipoCanal == canalP)&&(! ValidaP_mw(np, valor)))   return(false);
			if ((canal->TipoCanal == canalQ)&&(! ValidaQ_mvar(np, valor))) return(false);
			if ((canal->TipoCanal == canalV)&&(! ValidaV_pu(np, valor)))   return(false);
			if ((canal->TipoCanal == canalI)&&(! ValidaI_amp(np, valor)))  return(false);
			if ((canal->TipoCanal == canalS)&&(! ValidaS_MVA(np, valor)))  return(false);
			//insere patamar na Curva
			if (! curva->SetPonto(hm_ini, hm_fim, &valor, sizeof(valor)/sizeof(double))) return (false);
			}
		}catch(Exception &e)
			{
			return(false);
			}
	//verifica se a Curva cobre o período de 24 horas
	if (! curva->ValidaPeriodo24Horas()) return(false);
	return(true);
	}
*/ //DVK 2015.12.18 reescrito considerando valor medido
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeDadosPatamarFixo(void)
	{
	//variáveis locais
	double     valor;
	VTPatamar *patamar;

	//proteção
	if (curva == NULL) return(false);
	//garante unidade da Curva em MVA
	curva->Unidade = unidMVA;
	try{//atualiza pontos da Curva
		for(int np = 0, nr = 1; np < patamares->NumPatamar(); np++, nr++)
			{
			patamar       = patamares->Patamar[np];
			//assume valor nulo
			valor = 0;
			//lê valor
			if(! StrToDouble(SGrid->Cells[2][nr], valor))
				{
				Aviso("Valor inválido: " + SGrid->Cells[2][nr]);
				return(false);
				}
			//valida valor
			if ((canal->TipoCanal == canalP)&&(! ValidaP_mw(np, valor)))   return(false);
			if ((canal->TipoCanal == canalQ)&&(! ValidaQ_mvar(np, valor))) return(false);
			if ((canal->TipoCanal == canalV)&&(! ValidaV_pu(np, valor)))   return(false);
			if ((canal->TipoCanal == canalI)&&(! ValidaI_amp(np, valor)))  return(false);
			if ((canal->TipoCanal == canalS)&&(! ValidaS_MVA(np, valor)))  return(false);
			//insere valor medido no patamar
			canal->ValorMedido[patamar] = valor;
			}
		}catch(Exception &e)
			{
			return(false);
			}
	//verifica se a Curva cobre o período de 24 horas
	if (! curva->ValidaPeriodo24Horas()) return(false);
	return(true);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeDadosPatamarVariavel(void)
	{
	//variáveis locais
	strHM      hm_ini, hm_fim;
	unionVALOR valor;

	//proteção
	if (curva == NULL) return(false);
   try{//redefine escala da Curva
      curva->Reinicia();
      curva->Escala = patEDITAVEL;
      //garante unidade da Curva em MVA
      curva->Unidade = unidMVA;
      //atualiza pontos da Curva
      for(int nr = 1, np = 0 ; nr < SGrid->RowCount ; nr++, np++)
         {
			if (! SGridExistePeriodo(nr)) break;
			//lê hora inicia e final do patamar
         hm_ini.hora   = StrToInteger(SGrid->Cells[0][nr]);
         hm_ini.minuto = 0;
         hm_fim.hora   = StrToInteger(SGrid->Cells[1][nr]);
         //hm_fim.minuto = 59;
         hm_fim.minuto = 00;  //FKM 2015.09.25 (o curva agora trabalha com: [a,b[ )
         //assume valores nulos p/ P,Q,V,T
			valor.var.p = 0;
			valor.var.q = 0;
			valor.var.v = 0;
			valor.var.i = 0;
			valor.var.t = 0;
         //lê valor somente se a coluna estiver sendo exibida
			if (Exibe.ColunaP) valor.var.p = StrToDouble(SGrid->Cells[2][nr]);
			if (Exibe.ColunaQ) valor.var.q = StrToDouble(SGrid->Cells[3][nr]);
			if (Exibe.ColunaV) valor.var.v = StrToDouble(SGrid->Cells[4][nr]);
			if (Exibe.ColunaI) valor.var.i = StrToDouble(SGrid->Cells[5][nr]);
			if (Exibe.ColunaF) valor.var.t = StrToDouble(SGrid->Cells[6][nr]);
			//valida hora inicial e final do patamar
			if (! ValidaHora(np, hm_ini.hora)) return(false);
			if (! ValidaHora(np, hm_fim.hora)) return(false);
			//valida valor somente o dado estiver habilitado
			if ((Exibe.ColunaP)&&(! ValidaP_mw(np, valor.var.p)))   return(false);
			if ((Exibe.ColunaQ)&&(! ValidaQ_mvar(np, valor.var.q))) return(false);
			if ((Exibe.ColunaV)&&(! ValidaV_pu(np, valor.var.v)))   return(false);
			if ((Exibe.ColunaI)&&(! ValidaI_amp(np, valor.var.i)))  return(false);
			if ((Exibe.ColunaF)&&(! ValidaT_grau(np, valor.var.t))) return(false);
			//insere patamar na Curva
			if (! curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double))) return (false);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
	}
*/  //DVK 2015.10.29 reescrito, considerando que a curva do canal possui 1 grandeza
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeDadosPatamarVariavel(void)
	{
	//variáveis locais
	strHM      hm_ini, hm_fim;
	double     valor;

	//proteção
	if (curva == NULL) return(false);
   try{//redefine escala da Curva
      curva->Reinicia();
      curva->Escala = patEDITAVEL;
      //garante unidade da Curva em MVA
      curva->Unidade = unidMVA;
      //atualiza pontos da Curva
      for(int nr = 1, np = 0 ; nr < SGrid->RowCount ; nr++, np++)
         {
			if (! SGridExistePeriodo(nr)) break;
			//lê hora inicia e final do patamar
         hm_ini.hora   = StrToInteger(SGrid->Cells[0][nr]);
         hm_ini.minuto = 0;
         hm_fim.hora   = StrToInteger(SGrid->Cells[1][nr]);
         //hm_fim.minuto = 59;
         hm_fim.minuto = 00;  //FKM 2015.09.25 (o curva agora trabalha com: [a,b[ )
			//assume valor nulo
			valor = 0;
			//lê valor
			if(! StrToDouble(SGrid->Cells[2][nr], valor))
				{
				Aviso("Valor inválido: " + SGrid->Cells[2][nr]);
				return(false);
				}
			//valida valor
			if ((canal->TipoCanal == canalP)&&(! ValidaP_mw(np, valor)))   return(false);
			if ((canal->TipoCanal == canalQ)&&(! ValidaQ_mvar(np, valor))) return(false);
			if ((canal->TipoCanal == canalV)&&(! ValidaV_pu(np, valor)))   return(false);
			if ((canal->TipoCanal == canalI)&&(! ValidaI_amp(np, valor)))  return(false);
			if ((canal->TipoCanal == canalS)&&(! ValidaS_MVA(np, valor)))  return(false);
			//valida hora inicial e final do patamar
			if (! ValidaHora(np, hm_ini.hora)) return(false);
			if (! ValidaHora(np, hm_fim.hora)) return(false);
			//insere patamar na Curva
			if (! curva->SetPonto(hm_ini, hm_fim, &valor, sizeof(valor)/sizeof(double))) return (false);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::LeValidaPontosCurva(void)
   {
   //lê pontos da curva
   if (! LePontosCurva())
      {
      Erro("Canal " + EditCodigo->Text + ": valores de medição inválidos");
      return(false);
      }
   //verifica se a curva cobre as 24 horas do dia
   if (! canal->Curva->ValidaPeriodo24Horas())
      {
      Erro("Canal " + EditCodigo->Text + ": medição não cobre todo período de 24 horas");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::PageControlChanging(TObject *Sender, bool &AllowChange)
   {
   //verifica o TabSheet atual
   if (PageControl->ActivePage == TabSheetChart)
      {//está exibindo o gráfico e vai voltar p/ edição dos pontos
      AllowChange = true;
      }
   else
      {//está editando os pontos e vai exibir o gráfico:
      //permite exibir o gráfico somente se os dados da Curva forem válidos
      AllowChange = LeValidaPontosCurva();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::PageControlChange(TObject *Sender)
   {
   //variáveis locais
   int       ind_atrib;
   VTVetval  *vetval;

   //verifica se está exibindo o gráfico
	if (PageControl->ActivePage != TabSheetChart) return;
	//mostra curva no gráfico //DVK 2015.10.29
	ChartExibeCurva();
/*
//JCG - Bloqueado temporariamente
   //obtém lista de valores de tensão em pu definida pela Curva
   switch (canal->Eqpto->Tipo())
      {
      case eqptoGERADOR:      ind_atrib = ger_PQVF;         break;
      case eqptoSUPRIMENTO:   ind_atrib = sup_PQVF;         break;
      case eqptoCARGA:        ind_atrib = car_P;            break;
      case eqptoTRECHO:       ind_atrib = lig_S;            break;
      case eqptoTRAFO:        ind_atrib = lig_S;            break;
      case eqptoCHAVE:        ind_atrib = lig_S;            break;
		case eqptoBARRA:        ind_atrib = bar_VFF_PU_MIN;   break;
		default:    return;
      }
	if (legenda->LisVetValCanal(canal, lisVETVAL, ind_atrib))
      {
      vetval = (VTVetval*)lisVETVAL->First();
		ChartExibeCurva(vetval);
		}
*/
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TFormEdtCurvaMed::PM_GetCanal(void)
   {
   return(canal);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::PM_SetCanal(VTCanal *canal)
   {
   //salva ponteiro p/ objetos
   this->canal = canal;
   this->curva = canal->Curva;
   //exibe dados do Canal
   EditCodigo->Text     = canal->Codigo;
   EditTolerancia->Text = DoubleToStr("%3.2f", canal->Tolerancia_perc);
   //atualiza CBoxNumPto p/ exibir número de pontos da Curva
	CBoxNumPtoInicia();
	//redefine número de Patamares
   switch(curva->Escala)
      {
      case pat24HORA: patamares->Escala  = pat24HORA; break;
      case pat12HORA: patamares->Escala  = pat12HORA; break;
      case pat08HORA: patamares->Escala  = pat08HORA; break;
      case pat06HORA: patamares->Escala  = pat06HORA; break;
      case pat04HORA: patamares->Escala  = pat04HORA; break;
      case pat03HORA: patamares->Escala  = pat03HORA; break;
      case pat02HORA: patamares->Escala  = pat02HORA; break;
      case pat01HORA: patamares->Escala  = pat01HORA; break;
      case pat15MIN:  patamares->Escala  = pat15MIN;  break;
      case pat10MIN:  patamares->Escala  = pat10MIN;  break;
      case pat05MIN:  patamares->Escala  = pat05MIN;  break;
      default:        patamares->Escala  = pat01HORA; break;
      }
   //atualiza exibição das colunas
	SGridAcertaColunas();
   //inicia dados da Curva
   SGridInicia();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::SGridAcertaColunas(void)
	{
	//verifica se a Curva está definida com número variável de patamares
	if (curva->Escala == patEDITAVEL)
		{
		Exibe.ColunaHoraIni = true;
		Exibe.ColunaHoraFim = true;
		}
	else
		{
		Exibe.ColunaHoraIni = true;
		Exibe.ColunaHoraFim = false;
		}
	//define colunas que devem ser exibidas
	switch (canal->TipoCanal)
		{
		case canalV:
			Exibe.ColunaP = false;
			Exibe.ColunaQ = false;
			Exibe.ColunaV = true;
			Exibe.ColunaI = false;
			Exibe.ColunaF = false;
			break;
		case canalI:
			Exibe.ColunaP = false;
			Exibe.ColunaQ = false;
			Exibe.ColunaV = false;
			Exibe.ColunaI = true;
			Exibe.ColunaF = false;
			break;
		case canalP:
			Exibe.ColunaP = true;
			Exibe.ColunaQ = false;
			Exibe.ColunaV = false;
			Exibe.ColunaI = false;
			Exibe.ColunaF = false;
			break;
		case canalQ:
			Exibe.ColunaP = false;
			Exibe.ColunaQ = true;
			Exibe.ColunaV = false;
			Exibe.ColunaI = false;
			Exibe.ColunaF = false;
			break;
//		case canalVF:
//			Exibe.ColunaP = false;
//			Exibe.ColunaQ = false;
//         Exibe.ColunaV = true;
//			Exibe.ColunaI = false;
//			Exibe.ColunaF = true;
//         break;
//		case canalPQ:
//         Exibe.ColunaP = true;
//         Exibe.ColunaQ = true;
//			Exibe.ColunaV = false;
//			Exibe.ColunaI = false;
//			Exibe.ColunaF = false;
//			break;
//		case canalIF:
//			Exibe.ColunaP = false;
//			Exibe.ColunaQ = false;
//			Exibe.ColunaV = false;
//			Exibe.ColunaI = true;
//			Exibe.ColunaF = true;
//			break;
//		case canalPV:
//			Exibe.ColunaP = true;
//         Exibe.ColunaQ = false;
//			Exibe.ColunaV = true;
//			Exibe.ColunaI = false;
//			Exibe.ColunaF = false;
//			break;
      }
	//define dimensão das colunas
	SGrid->ColWidths[0] = (Exibe.ColunaHoraIni) ? 65 : 0;
	SGrid->ColWidths[1] = (Exibe.ColunaHoraFim) ? 65 : 0;
	SGrid->ColWidths[2] = (Exibe.ColunaP)       ? 65 : 0;
	SGrid->ColWidths[3] = (Exibe.ColunaQ)       ? 65 : 0;
	SGrid->ColWidths[4] = (Exibe.ColunaV)       ? 65 : 0;
	SGrid->ColWidths[5] = (Exibe.ColunaI)       ? 65 : 0;
	SGrid->ColWidths[6] = (Exibe.ColunaF)       ? 65 : 0;
	}
*/ //DVK 2015.10.30 reescrito, considerando apenas 1 grandeza
//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::SGridAcertaColunas(void)
	{
	//verifica se a Curva está definida com número variável de patamares
	if (curva->Escala == patEDITAVEL)
		{//mostra hora/min inicial, hora/min final e valor
		SGrid->ColWidths[0] = 65;
		SGrid->ColWidths[1] = 65;
		SGrid->ColWidths[2] = 65;
		}
	else
		{//mostra hora/min e valor
		SGrid->ColWidths[0] = 75;
		SGrid->ColWidths[1] = 0;
		SGrid->ColWidths[2] = 65;
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::SGridExistePeriodo(int nr)
	{
	//verifica se a linha está preenchida
	for (int nc = 0; nc < 2; nc++)
		{
		if (SGrid->Cells[nc][nr].IsEmpty())return(false);
		}
	return(true);
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::SGridInicia(void)
	{
	//variáveis locais
	AnsiString txt;
	char       *tit1[] = {"Hora/min",    "",            "P(MW)", "Q(MVAr)", "V(pu)", "I(A)", "teta(graus)"};
	char       *tit2[] = {"Hora inicial", "Hora final", "P(MW)", "Q(MVAr)", "V(pu)", "I(A)", "teta(graus)"};
	strHM      hm_ini, hm_fim;
	unionVALOR valor;
	VTPatamar *patamar;
	TList     *lisPAT = patamares->LisPatamar();

	//limpa SGrid
	for (int nr = 1; nr < SGrid->RowCount; nr++) SGrid->Rows[nr]->Clear();
	//acerta largura de colunas
	SGridAcertaColunas();
	//verifica a Curva é do tipo patEDITAVEL
	if (curva->Escala == patEDITAVEL)
		{//reinicia dimensão do SGrid
		SGrid->RowCount  = 24 + 1;
		//SGrid->ColCount  = sizeof(tit2)/sizeof(char*);
		SGrid->FixedCols = 0;
		//preenche títulos das colunas
		for (int nc = 0; nc < SGrid->ColCount; nc++) SGrid->Cells[nc][0] = tit2[nc];
		//preenche dados dos pontos da Curva
		for (int nr = 1, np = 0; nr < SGrid->RowCount; nr++, np++)
			{//obtém ponto da Curva p/ o Patamar
			if (! curva->GetPonto(np, hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double))) break;
			//preenche SGrid
			SGrid->Cells[0][nr] = txt.sprintf("%02d", hm_ini.hora);
			SGrid->Cells[1][nr] = txt.sprintf("%02d", hm_fim.hora);
			SGrid->Cells[2][nr] = DoubleToStr("%7.6f", valor.var.p);
			SGrid->Cells[3][nr] = DoubleToStr("%7.6f", valor.var.q);
			SGrid->Cells[4][nr] = DoubleToStr("%7.6f", valor.var.v);
			SGrid->Cells[5][nr] = DoubleToStr("%7.6f", valor.var.i);
			SGrid->Cells[6][nr] = DoubleToStr("%7.6f", valor.var.t);
			}
		}
	else
		{//reinicia dimensão do SGrid
		SGrid->RowCount  = patamares->NumPatamar() + 1;
		//SGrid->ColCount  = sizeof(tit1)/sizeof(char*);
		SGrid->FixedCols = 1;
		//preenche títulos das colunas
		for (int nc = 0; nc < SGrid->ColCount; nc++) SGrid->Cells[nc][0] = tit1[nc];
		//preenche dados dos pontos da Curva
		for(int np = 0, nr = 1; np < lisPAT->Count ; np++, nr++)
			{//preenche hora/minuto do Patamar
			patamar = (VTPatamar*)lisPAT->Items[np];
			SGrid->Cells[0][nr] = patamar->Nome;
			SGrid->Cells[1][nr] = "";
			//obtém ponto da Curva p/ o Patamar
			if (curva->GetPonto(np, hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double)))
				{//preenche dados dos pontos
				SGrid->Cells[2][nr] = DoubleToStr("%7.6f", valor.var.p);
				SGrid->Cells[3][nr] = DoubleToStr("%7.6f", valor.var.q);
				SGrid->Cells[4][nr] = DoubleToStr("%7.6f", valor.var.v);
				SGrid->Cells[5][nr] = DoubleToStr("%7.6f", valor.var.i);
				SGrid->Cells[6][nr] = DoubleToStr("%7.6f", valor.var.t);
				}
			}
		}
	}
*/  //DVK 2015.10.29 reescrito, considerando apenas 1 grandeza do canal
//---------------------------------------------------------------------------
void __fastcall TFormEdtCurvaMed::SGridInicia(void)
	{
	//variáveis locais
	AnsiString txt;
	char       *unid[] = {"", "V(pu)", "I(A)",  "P(MW)", "Q(MVAr)", "S(MVA)"};
	strHM      hm_ini, hm_fim;
	double     valor;
	VTPatamar *patamar;
	TList     *lisPAT = patamares->LisPatamar();

	//limpa SGrid
	for (int nr = 1; nr < SGrid->RowCount; nr++) SGrid->Rows[nr]->Clear();
	//acerta largura de colunas
	SGridAcertaColunas();
	//proteção para canal indefinido ou de fluxo harmônico
	if(canal->TipoCanal == canalINDEF)  return;
	if(canal->TipoCanal > sizeof(unid)) return;
	//verifica a Curva é do tipo patEDITAVEL
	if (curva->Escala == patEDITAVEL)
		{//reinicia dimensão do SGrid
		SGrid->RowCount  = 24 + 1;
		SGrid->FixedCols = 0;
		//preenche títulos das colunas
		SGrid->Cells[0][0] = "Hora inicial";
		SGrid->Cells[1][0] = "Hora final";
		SGrid->Cells[2][0] = unid[canal->TipoCanal];
		//preenche dados dos pontos da Curva
		for (int nr = 1, np = 0; nr < SGrid->RowCount; nr++, np++)
			{//obtém ponto da Curva p/ o Patamar
			if (! curva->GetPonto(np, hm_ini, hm_fim, &valor, sizeof(valor)/sizeof(double))) break;
			//preenche SGrid
			SGrid->Cells[0][nr] = txt.sprintf("%02d", hm_ini.hora);
			SGrid->Cells[1][nr] = txt.sprintf("%02d", hm_fim.hora);
			SGrid->Cells[2][nr] = DoubleToStr("%7.6f", valor);
			}
		}
	else
		{//reinicia dimensão do SGrid
		SGrid->RowCount  = patamares->NumPatamar() + 1;
		//SGrid->ColCount  = sizeof(tit1)/sizeof(char*);
		SGrid->FixedCols = 1;
		//preenche títulos das colunas
		SGrid->Cells[0][0] = "Hora/min";
		SGrid->Cells[2][0] = unid[canal->TipoCanal];
		//preenche dados dos pontos da Curva
		for(int np = 0, nr = 1; np < lisPAT->Count ; np++, nr++)
			{//preenche hora/minuto do Patamar
			patamar = (VTPatamar*)lisPAT->Items[np];
			SGrid->Cells[0][nr] = patamar->Nome;
			SGrid->Cells[1][nr] = "";
			//obtém ponto da Curva p/ o Patamar
			valor = canal->ValorMedido[patamar];
			SGrid->Cells[2][nr] = DoubleToStr("%7.6f", valor);
			}
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaHora(int np, int hora)
	{
	//valida hora
	if ((hora < 0)||(hora > 24)) //FKM - 2015.09.25 - 24:00 em vez de 23:59
	//if ((hora < 0)||(hora > 23))
		{
		Erro("Hora inválida no patamar " + IntToStr(np));
		return(false);
		}
	return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaI_amp(int np, double i_amp)
	{
	//valida tensão em pu
	if (i_amp < 0.)
		{
		Erro("Módulo da corrente inválida no patamar " + IntToStr(np));
		return(false);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaP_mw(int np, double p_mw)
   {
   //valida pot.ativa
   if (pot_neg_enabled) return(true);
   if (p_mw < 0.)
      {//pede confirmação do usuário (OBS: alteração solicitada por NK))
      if (Confirma("Potência ativa com valor negativo no patamar " + IntToStr(np+1),
                   "Valores negativos p/ potência ativa devem ser aceitos ?") != IDYES) return(false);
      //atualiza flag p/ admitir potência ativa negativa
      pot_neg_enabled = true;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaQ_mvar(int np, double q_mvar)
	{
	//valida pot.reativa
//   if (q_mvar <= 0.)
//		{
//		Erro("Potência reativa inválida no patamar " + IntToStr(np));
//		return(false);
//		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaS_MVA(int np, double s_mva)
	{
	//valida pot.aparente
   if (s_mva <= 0.)
		{
		Erro("Potência aparente inválida no patamar " + IntToStr(np));
		return(false);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaV_pu(int np, double v_pu)
	{
	//valida tensão em pu
	if (v_pu < 0.)
		{
		Erro("Módulo da tensão inválida no patamar " + IntToStr(np));
		return(false);
		}
	return(true);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCurvaMed::ValidaT_grau(int np, double t_grau)
   {
   //valida tensão em pu
   if ((t_grau >= 360)||(t_grau <= -360))
      {
		Erro("Ângulo da tensão inválido no patamar " + IntToStr(np));
      return(false);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
//eof


