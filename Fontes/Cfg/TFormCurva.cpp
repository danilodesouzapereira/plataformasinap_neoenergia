//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCurva.h"
#include "..\Constante\Const.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCurva::TFormCurva(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //pré-seleciona curva c/ de 24 pontos de 1 hora
	CBoxNumPto->ItemIndex =  0;
	}

//---------------------------------------------------------------------------
__fastcall TFormCurva::~TFormCurva(void)
   {
   //destrói curva criada localmente
   if (curva_nova) {delete curva_nova; curva_nova = NULL;}
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormCurva::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::ActionConfirmaExecute(TObject *Sender)
   {
	//verifica se o código foi definido
   if (EditCodigo->Text.IsEmpty())
      {
      Aviso("Defina a identifcação da curva");
      return;
      }
   //verifica número de pontos da curva
   if (CBoxNumPto->ItemIndex < 0)
      {
      Aviso("Selecione o número de pontos da curva");
      return;
      }
	//verifica tipo da curva
	if (CBoxTipoCurva->ItemIndex < 0)
		{
		Aviso("Selecione o conjunto de dados da curva");
		return;
		}
	//verifica unidade da curva
	if (CBoxUnidade->ItemIndex < 0)
		{
		Aviso("Selecione a unidade da curva");
		return;
		}
	//verifica dia da medição
   //define dia da medição
   switch(CBoxDiaMedicao->ItemIndex)
      {
      case 0: curva->DiaMedicao = SABADO;          break;
      case 1: curva->DiaMedicao = DOMINGO;         break;
      case 2: curva->DiaMedicao = DIA_UTIL;        break;
      case 3: curva->DiaMedicao = SEMANA_COMPLETA; break;
      default:
		   Aviso("Selecione o tipo da curva");
		   return;
      }
   //lê faixa de consumo
   if (! FaixaConsumoLe()) return;
   //lê pontos da Curva
	if (! SGridLeCurva()) return;
	//salva código
   curva->Codigo = EditCodigo->Text;
	//curva->Tipo   = int(CBoxTipoCurva->Items->Objects[CBoxTipoCurva->ItemIndex]);
	//curva->Unidade = int(CBoxUnidade->Items->Objects[CBoxUnidade->ItemIndex]);
	curva->Tipo    = tipo_curva;
	curva->Unidade = unidade;
	ModalResult    = mrOk;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::CBoxNumPtoChange(TObject *Sender)
   {
	//variáveis locais
	int escala;

	//redefine número de Patamares
	switch(CBoxNumPto->ItemIndex)
		{
		case  0: escala = pat24HORA;   break;
		case  1: escala = pat12HORA;   break;
		case  2: escala = pat08HORA;   break;
		case  3: escala = pat06HORA;   break;
		case  4: escala = pat04HORA;   break;
      case  5: escala = pat03HORA;   break;
		case  6: escala = pat02HORA;   break;
		case  7: escala = pat01HORA;   break;
		case  8: escala = pat15MIN;    break;
		case  9: escala = pat10MIN;    break;
      case 10: escala = pat05MIN;    break;
		default: escala = patEDITAVEL; break;
		}
	//reconfigura a Curva (muda número de pontos, salvando seus valores originais)
	curva->Reconfigura(escala);
   //reapresenta pontos da Curva
	SGridInicia();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::CBoxMedicaoInicia(void)
   {
   //seleciona opção
   switch(curva->DiaMedicao)
      {
      case SABADO:          CBoxDiaMedicao->ItemIndex = 0; break;
      case DOMINGO:         CBoxDiaMedicao->ItemIndex = 1; break;
      case DIA_UTIL:        CBoxDiaMedicao->ItemIndex = 2; break;
      case SEMANA_COMPLETA: CBoxDiaMedicao->ItemIndex = 3; break;
      default:              CBoxDiaMedicao->ItemIndex = 3; break;
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormCurva::CBoxNumPtoInicia(void)
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
void __fastcall TFormCurva::CBoxTipoCurvaInicia(void)
	{
	//reinicia
	CBoxTipoCurva->Clear();
   //verifica a classe associada à curva
   if (curva->Classe != NULL)
      {//curva de demanda associada a classe de consumidor
		//CBoxTipoCurva->Items->AddObject("P(pu)/Q(pu)/V(pu)/F(grau)",          			(TObject*)curvaPQVT);
		CBoxTipoCurva->Items->AddObject("P(pu)/Q(pu) - Média+Desvio padrão",          (TObject*)curvaPQ_MDP);
		CBoxTipoCurva->Items->AddObject("P(pu)/Fator Potência - Média+Desvio padrão", (TObject*)curvaPFP_MDP);
      if (curva->Classe->Tag == consIP)
         {//inclui opção de curva Ligada/Desligada p/ IP
         CBoxTipoCurva->Items->AddObject("Ligada/Desligada", (TObject*)curvaLD);
		   }
      //pré-seleciona item
		switch(curva->Tipo)
			{
			case  curvaPQ_MDP:   CBoxTipoCurva->ItemIndex = 0; break;
			case  curvaPFP_MDP:  CBoxTipoCurva->ItemIndex = 1; break;
			case  curvaLD:       CBoxTipoCurva->ItemIndex = 2; break;
			default:             CBoxTipoCurva->ItemIndex = 0; break;
			}
      }
   else if ((curva->Tipo == curvaLD) || (curva->Tipo == curvaPQ)) //DVK 2014.08.01
      {
      CBoxTipoCurva->Items->AddObject("Ligada/Desligada", (TObject*)curvaLD);
      //pré-seleciona item
      CBoxTipoCurva->ItemIndex = 0;
		}
   else if (curva->Tipo == curvaPQVT)
      {
		CBoxTipoCurva->Items->AddObject("P(pu)/Q(pu)/V(pu)/F(grau) ", (TObject*)curvaPQVT);
      //pré-seleciona item
      CBoxTipoCurva->ItemIndex = 0;
		}
	//efetiva seleção do tipo de curva  //DVK 2015.09.08
	CBoxTipoCurvaChange(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::CBoxTipoCurvaChange(TObject *Sender)
	{
   //proteção
   if (CBoxTipoCurva->ItemIndex < 0) return;
   //determina tipo curva
   tipo_curva = (int)(CBoxTipoCurva->Items->Objects[CBoxTipoCurva->ItemIndex]);
   //reapresenta pontos da Curva
	SGridInicia();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::CBoxUnidadeChange(TObject *Sender)
	{
	//verifica a unidade definida pela Curva
	switch(CBoxUnidade->ItemIndex)
		{
      case 0:   unidade = unidMVA; break;
		case 1:   unidade = unidPU_MED; break;
		case 2:   unidade = unidPU_MAX; break;
		default:  unidade = unidPU_MED; break;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::CBoxUnidadeInicia(void)
	{
	//verifica a unidade definida pela Curva
	switch(curva->Unidade)
		{
      case unidMVA:   CBoxUnidade->ItemIndex =  0; break;
		case unidPU_MED:   CBoxUnidade->ItemIndex =  1; break;
		case unidPU_MAX:   CBoxUnidade->ItemIndex =  2; break;
		default: 			 CBoxUnidade->ItemIndex =  1; break;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurva::EditaCurva(VTCurva *curva)
   {
   //salva ponteiro p/ Curva
   this->curva = curva;
   //inicia nome da Curva
	EditCodigo->Text = curva->Codigo;
	//inicia tipo_curva
	tipo_curva = curva->Tipo;
	unidade = curva->Unidade;
   //inicia CBoxTipoCurva, CBoxNumPto e CBoxUnidade
	CBoxTipoCurvaInicia();
	CBoxNumPtoInicia();
	CBoxUnidadeInicia();
   //seleciona opção de CBoxDiaMedicao
   CBoxMedicaoInicia();
   //configura faixa de consumo
   FaixaConsumoInicia();
   //inicia SGrid
   SGridInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurva::FaixaConsumoInicia(void)
   {
   //verifica se curva típica de carga diferente de IP
   if ((curva->Classe == NULL)||(curva->Classe->Tag == consIP))
      {
      EditLimiteInf->Text = "0.";
      EditLimiteSup->Text = "0.";
      GBoxFaixaConsumo->Visible = false;
      }
   else
      {
      EditLimiteInf->Text = DoubleToStr("%3.f", curva->LimiteInferior);
      EditLimiteSup->Text = DoubleToStr("%3.f", curva->LimiteSuperior);
		if((curva->Classe->Tag == consA2)||(curva->Classe->Tag == consA4))
			{//altera texto para faixas A2 e A4     //DVK 2012.12.18
			GBoxFaixaConsumo->Caption = "Faixa de demanda (kW)";
         }
		else
         {
         GBoxFaixaConsumo->Caption = "Faixa de consumo de Energia (kWh/mês)";
         }
		GBoxFaixaConsumo->Visible = true; 
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::FaixaConsumoLe(void)
   {
   //verifica se curva típica de carga
   if (curva->Classe == NULL)
      {
      curva->LimiteInferior = 0.;
      curva->LimiteSuperior = 0.;
      }
   else
      {
      try{
         curva->LimiteInferior = StrToDouble(EditLimiteInf->Text);
         curva->LimiteSuperior = StrToDouble(EditLimiteSup->Text);
         }catch(Exception &e)
            {
            Erro("Erro na leitura dos valores da faixa de consumo");
            return(false);
            }
      }
   //verifica valores
   if ((curva->LimiteSuperior > 0)&&(curva->LimiteInferior > curva->LimiteSuperior))
      {
      Erro("Limite inferior de consumo tem valor maior que o limite superior");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridExistePeriodo(int nr)
   {
   //verifica se a linha está preenchida
   for (int nc = 0; nc < 2; nc++)
      {
      if (SGrid->Cells[nc][nr].IsEmpty())return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridInicia(void)
   {
   //variáveis locais
   int num_pto;

   if (curva->Escala == patEDITAVEL)
      {//define número de colunas fixas
      SGrid->FixedCols = 0;
      //define número de linhas de SGrid
      num_pto = 24;
      }
   else
      {//define número de colunas fixas
      SGrid->FixedCols = 2;
      //define número de pontos da Curva
      num_pto = curva->NumeroPontos();
      }
	switch(tipo_curva)
      {
      case curvaPQVT:    SGridIniciaCurvaPQVT(num_pto);    break;
      case curvaPQ_MDP:  SGridIniciaCurvaPQ_MDP(num_pto);  break;
      case curvaPFP_MDP: SGridIniciaCurvaPFP_MDP(num_pto); break;
      case curvaLD:      SGridIniciaCurvaLD(num_pto);      break;
      case curvaPQ:      SGridIniciaCurvaLD(num_pto);      break;  //DVK 2014.08.01
      default:                                             break;
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridIniciaCurvaPQ(int num_pto)
	{
   //variáveis locais
   AnsiString   txt;
	char         *tit_col[] = {"Início", "Fim", "P", "Q"};
   strHM         hm_ini, hm_fim ;
	double        valor[nvCURVA_PQVT];

   //reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++) {SGrid->Rows[nr]->Clear();}
   //redimensiona SGrid
   SGrid->ColCount = sizeof(tit_col)/sizeof(char*);
   SGrid->RowCount = num_pto + 1;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGrid->ColCount; nc++) {SGrid->Cells[nc][0] = tit_col[nc];}
   //preenche valores da Curva
   for (int np = 0, nr = 1; np < num_pto; np++, nr++)
      {//obtém ponto da curva
		if (! curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor)/sizeof(double))) break;
      //preenche hora inicial
		if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_ini.hora);
      else                       txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      //preenche hora final
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_fim.hora);
      else                       txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
      SGrid->Cells[1][nr] = txt;
      //preenche valores de P e Q
      SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
      SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
		}
	}  
*/

//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridIniciaCurvaPQVT(int num_pto)
   {
   //variáveis locais
   AnsiString   txt;
   char         *tit_col[] = {"Início", "Fim", "P", "Q", "V", "F"};
   strHM         hm_ini, hm_fim ;
   double        valor[32];

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) {SGrid->Rows[nr]->Clear();}
   //redimensiona SGrid
   SGrid->ColCount = sizeof(tit_col)/sizeof(char*);
   SGrid->RowCount = num_pto + 1;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGrid->ColCount; nc++) {SGrid->Cells[nc][0] = tit_col[nc];}
   //preenche valores da Curva
   for (int np = 0, nr = 1; np < num_pto; np++, nr++)
      {//inicia vetor de valores a serem lidos
      for (int n = 0; n < 4; n++) {valor[n] = 0.;}
      //obtém ponto da curva
      if (! curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores)) break;
      //preenche hora inicial
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_ini.hora);
      else                       txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      //preenche hora final
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_fim.hora);
      else                       txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
      SGrid->Cells[1][nr] = txt;
      //preenche valores de P,Q,V,T
      SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
      SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
      SGrid->Cells[4][nr] = DoubleToStr("%4.3f", valor[2]);
      SGrid->Cells[5][nr] = DoubleToStr("%4.3f", valor[3]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridIniciaCurvaPQ_MDP(int num_pto)
   {
   //variáveis locais
   AnsiString    txt;
   char         *tit_col[] = {"Início", "Fim", "P", "Q", "dp(P)", "dp(Q)"};
   strHM         hm_ini, hm_fim ;
   double        valor[32];

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) {SGrid->Rows[nr]->Clear();}
   //redimensiona SGrid
   SGrid->ColCount = sizeof(tit_col)/sizeof(char*);
   SGrid->RowCount = num_pto + 1;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGrid->ColCount; nc++) {SGrid->Cells[nc][0] = tit_col[nc];}
   //preenche valores da Curva
   for (int np = 0, nr = 1; np < num_pto; np++, nr++)
      {//inicia vetor de valores a serem lidos
      for (int n = 0; n < 4; n++) {valor[n] = 0.;}
      //obtém ponto da curva
      if (! curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores)) break;
      //preenche hora inicial
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_ini.hora);
      else                       txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      //preenche hora final
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_fim.hora);
      else                       txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
      SGrid->Cells[1][nr] = txt;
      //preenche valores de P, Q, dp[P], dp(Q)
      SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
      SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
      SGrid->Cells[4][nr] = DoubleToStr("%4.3f", valor[2]);
      SGrid->Cells[5][nr] = DoubleToStr("%4.3f", valor[3]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridIniciaCurvaPFP(int num_pto)
	{  /*
   //variáveis locais
   AnsiString    txt;
   char         *tit_col[] = {"Início", "Fim", "P", "FP"};
   strHM         hm_ini, hm_fim ;
   double        valor[32];

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) {SGrid->Rows[nr]->Clear();}
   //redimensiona SGrid
   SGrid->ColCount = sizeof(tit_col)/sizeof(char*);
   SGrid->RowCount = num_pto + 1;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGrid->ColCount; nc++) {SGrid->Cells[nc][0] = tit_col[nc];}
   //preenche valores da Curva
   for (int np = 0, nr = 1; np < num_pto; np++, nr++)
      {//inicia vetor de valores a serem lidos
      for (int n = 0; n < 4; n++) {valor[n] = 0.;}
      //obtém ponto da curva
      if (! curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores)) break;
      //preenche hora inicial
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_ini.hora);
      else                       txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      //preenche hora final
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_fim.hora);
      else                       txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
      SGrid->Cells[1][nr] = txt;
      //preenche valores de P, FP
      SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
      SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
		} */
	}    

//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridIniciaCurvaPFP_MDP(int num_pto)
   {
   //variáveis locais
   AnsiString    txt;
   char         *tit_col[] = {"Início", "Fim", "P", "FP", "dp(P)", "dp(FP)"};
   strHM         hm_ini, hm_fim ;
   double        valor[32];

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) {SGrid->Rows[nr]->Clear();}
   //redimensiona SGrid
   SGrid->ColCount = sizeof(tit_col)/sizeof(char*);
   SGrid->RowCount = num_pto + 1;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGrid->ColCount; nc++) {SGrid->Cells[nc][0] = tit_col[nc];}
   //preenche valores da Curva
   for (int np = 0, nr = 1; np < num_pto; np++, nr++)
      {//inicia vetor de valores a serem lidos
      for (int n = 0; n < 4; n++) {valor[n] = 0.;}
      //obtém ponto da curva
      if (! curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores)) break;
      //preenche hora inicial
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_ini.hora);
      else                       txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      //preenche hora final
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_fim.hora);
      else                       txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
      SGrid->Cells[1][nr] = txt;
      //preenche valores de P, FP, dp[P], dp(FP)
      SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
      SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
      SGrid->Cells[4][nr] = DoubleToStr("%4.3f", valor[2]);
      SGrid->Cells[5][nr] = DoubleToStr("%4.3f", valor[3]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurva::SGridIniciaCurvaLD(int num_pto)
   {
   //variáveis locais
   AnsiString txt;
   strHM      hm_ini, hm_fim ;
   double     valor[32];
   char      *tit_col[] = {"Início", "Fim", "Ligado"};

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) {SGrid->Rows[nr]->Clear();}
   //redimensiona SGrid
   SGrid->ColCount = sizeof(tit_col)/sizeof(char*);
   SGrid->RowCount = num_pto + 1;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGrid->ColCount; nc++) {SGrid->Cells[nc][0] = tit_col[nc];}
   //preenche valores da Curva
   for (int np = 0, nr = 1; np < num_pto; np++, nr++)
      {//inicia vetor de valores a serem lidos
      for (int n = 0; n < 4; n++) {valor[n] = 0.;}
      //obtém ponto da curva
      if (! curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores)) break;
      //preenche hora inicial
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_ini.hora);
      else                       txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      //preenche hora final
      if (SGrid->FixedCols == 0) txt.sprintf("%02d", hm_fim.hora);
      else                       txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
      SGrid->Cells[1][nr] = txt;
      //preenche valor de Ligado
      SGrid->Cells[2][nr] = IntToStr(int(valor[0]));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurva(void)
   {
	switch(tipo_curva)
      {
      case curvaPQVT:    return(SGridLeCurvaPQVT());
      case curvaPQ_MDP:  return(SGridLeCurvaPQ_MDP());
      case curvaPFP_MDP: return(SGridLeCurvaPFP_MDP());
      case curvaLD:      return(SGridLeCurvaLD());
		}
	//DVK 2015.09.08
	Aviso("Erro na leitura dos pontos da curva");
   return(false);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPQ(void)
   {
   //variáveis locais
   double valor[nvCURVA_PQ];

   //verifica se Curva com patamar variável
   if (curva->Escala == patEDITAVEL)
      {//lê hora inicial, hora final e valores de P e Q dos pontos
      return(SGridLeCurvaPatamarVariavel(valor, sizeof(valor)/sizeof(double)));
      }
   else
      {//lê valores de P e Q
      return(SGridLeCurvaPatamarFixo(valor, sizeof(valor)/sizeof(double)));
      }
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPQVT(void)
   {
   //variáveis locais
   double valor[nvCURVA_PQVT];

   //verifica se Curva com patamar variável
   if (curva->Escala == patEDITAVEL)
      {//lê hora inicial, hora final e valores de P e Q dos pontos
      return(SGridLeCurvaPatamarVariavel(valor, sizeof(valor)/sizeof(double)));
      }
   else
      {//lê valores de P,Q,V,T
      return(SGridLeCurvaPatamarFixo(valor, sizeof(valor)/sizeof(double)));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPQ_MDP(void)
   {
   //variáveis locais
   double valor[nvCURVA_PQ_MDP];

   //verifica se Curva com patamar variável
   if (curva->Escala == patEDITAVEL)
      {//lê hora inicial, hora final e valores de P e Q dos pontos
      return(SGridLeCurvaPatamarVariavel(valor, sizeof(valor)/sizeof(double)));
      }
   else
      {//lê valores de P e Q
      return(SGridLeCurvaPatamarFixo(valor, sizeof(valor)/sizeof(double)));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPFP(void)
   {
   //variáveis locais
   double valor[nvCURVA_PFP];

   //verifica se Curva com patamar variável
   if (curva->Escala == patEDITAVEL)
		{//lê hora inicial, hora final e valores de P e Fp dos pontos
      return(SGridLeCurvaPatamarVariavel(valor, sizeof(valor)/sizeof(double)));
      }
   else
		{//lê valores de P e Fp
      return(SGridLeCurvaPatamarFixo(valor, sizeof(valor)/sizeof(double)));
      }
	}

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPFP_MDP(void)
   {
   //variáveis locais
   double valor[nvCURVA_PFP_MDP];

   //verifica se Curva com patamar variável
   if (curva->Escala == patEDITAVEL)
		{//lê hora inicial, hora final e valores de P e Fp dos pontos
      return(SGridLeCurvaPatamarVariavel(valor, sizeof(valor)/sizeof(double)));
      }
   else
		{//lê valores de P e Fp
      return(SGridLeCurvaPatamarFixo(valor, sizeof(valor)/sizeof(double)));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaLD(void)
   {
   //variáveis locais
   double     valor[nvCURVA_LD];

   //verifica se Curva com patamar variável
   if (curva->Escala == patEDITAVEL)
		{//lê hora inicial, hora final e valores dos pontos
      return(SGridLeCurvaPatamarVariavel(valor, sizeof(valor)/sizeof(double)));
      }
   else
		{//lê valores
      return(SGridLeCurvaPatamarFixo(valor, sizeof(valor)/sizeof(double)));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPatamarFixo(double *valor, int dim_valor)
   {
   //variáveis locais
   strHM hm_ini, hm_fim;
	int   num_pto = curva->NumeroPontos();

    try{//reinicia pontos da curva e define número de valores
      //curva->Reinicia();
      curva->NumeroValores = dim_valor;
      curva->Reconfigura(curva->Escala);
      //lê valores da Curva
      for (int np = 0, row = 1; np < num_pto; np++, row++)
         {//obtém ponto da curva
			if (! curva->GetPonto(np, hm_ini, hm_fim, valor, dim_valor)) return(false);
			//lê valores do ponto
         for (int ind = 0, col = 2; ind < dim_valor; ind++, col++)
            {
            valor[ind] = StrToDouble(SGrid->Cells[col][row]);
            }
         //atualiza Curva
			if (! curva->SetPonto(hm_ini, hm_fim, valor, dim_valor)) return(false);
         }
		}catch(Exception &e)
			{//DVK 2015.09.08 mensagem de erro comentada: transferida para SGridLeCurva
//         Erro("Erro na leitura dos pontos da curva");
			return(false);
         }
   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormCurva::SGridLeCurvaPatamarVariavel(double *valor, int dim_valor)
   {
   //variáveis locais
   strHM hm_ini, hm_fim;

   try{//reinicia a Curva
      //curva->Reinicia();
      curva->NumeroValores = dim_valor;
      curva->Reconfigura(patEDITAVEL);
      //loop p/ todas as linhas do SGrid
      for (int row = 1; row < SGrid->RowCount; row++)
         {//verifica se a linha foi preenchida
         if (! SGridExistePeriodo(row)) break;
         //lê valor da hora inicial e define minuto inicial
         hm_ini.hora   = StrToInteger(SGrid->Cells[0][row]);
         hm_ini.minuto = 0;
         //lê valor da hora final e define minujto final
         hm_fim.hora   = StrToInteger(SGrid->Cells[1][row]);
         hm_fim.minuto = 0;
         //lê valores do ponto
         for (int ind = 0, col = 2; ind < dim_valor; ind++, col++)
            {
            valor[ind] = StrToDouble(SGrid->Cells[col][row]);
            }
         //atualiza Curva
         if (! curva->SetPonto(hm_ini, hm_fim, valor, dim_valor)) return(false);
         }
      }catch(Exception &e)
         {//DVK 2015.09.08 mensagem de erro comentada: transferida para SGridLeCurva
//         Erro("Erro na leitura dos pontos da curva");
			return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

