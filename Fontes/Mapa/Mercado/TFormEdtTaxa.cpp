//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtTaxa.h"
#include "..\Rede\VTArea.h"
#include "..\Mercado\VTMercado.h"
#include "..\Mercado\VTSegmento.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtTaxa::TFormEdtTaxa(TComponent* Owner, VTMercado *mercado, VTSegmento *segmento)
   : TForm(Owner)
   {
   //vari�veis locais
   int width;

   //salva ponteiro p/ objetos externos
	this->mercado   = mercado;
	this->segmento = segmento;
   //atualiza Caption
   if (segmento->Area->x1 < 0)
      {
      Caption = Caption + " para rede " + segmento->Codigo;
      }
   else
      {
      Caption = Caption + " para regi�o " + segmento->Codigo;
      }
   //inicia CBoxClasse e CBoxAno
   CBoxCrescimentoInicia();
   CBoxAnoInicia();
   //inicia SGrid
   SGridInicia();
   //ajusta tamanho do Form
   width = 0;
   for (int nc = 0; nc < SGrid->ColCount; nc++) width += SGrid->ColWidths[nc] + 3;
	//tamanho minimo do toolbar FKM 2015.08.12
	if(width > 490){ClientWidth  = width;}
	else{width = 490;}
   ClientHeight = (ToolBar->Height + 3) + (SGrid->RowCount * (SGrid->DefaultRowHeight + 1));
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTaxa::~TFormEdtTaxa(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::ActionConfirmaExecute(TObject *Sender)
   {
   //l� e valida os valores digitados pelo usu�rio
   if (! LeDados())     return;
   if (! Validadados()) return;
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::ActionDefineTaxaExecute(TObject *Sender)
   {
   //vari�veis locais
   int           ano_ini, ano_fim;
   double        taxa;
   TList         *lisCRESCIMENTO;
   VTCrescimento *crescimento;

   //prote��es
   if (CBoxAno->ItemIndex < 0)         return;
   if (CBoxCrescimento->ItemIndex < 0) return;
   //verifica se foi definida uma taxa
   if (! LeTaxa(taxa)) return;
   //determina ano inicial e ano final
   if (CBoxAno->ItemIndex == 0)
      {
      ano_ini = mercado->AnoInicial;
      ano_fim = mercado->AnoFinal;
      }
   else
      {
      ano_ini = ano_fim = StrToInteger(CBoxAno->Text);
      }
   //verifica se foi selecionado um Crescimento espec�fico
   if (CBoxCrescimento->ItemIndex == 0)
      {//todos Crescimentos
      lisCRESCIMENTO = segmento->LisCrescimento();
      for (int n = 0; n < lisCRESCIMENTO->Count; n++)
         {
         crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[n];
         DefineTaxa(crescimento, ano_ini, ano_fim, taxa);
         }
      }
   else
      {//determina Crescimento
      crescimento = (VTCrescimento*)CBoxCrescimento->Items->Objects[CBoxCrescimento->ItemIndex];
      DefineTaxa(crescimento, ano_ini, ano_fim, taxa);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::CBoxAnoInicia(void)
   {
   //reinicia CBoxAno
   CBoxAno->Clear();
   //inclui op��o "Todos anos"
   CBoxAno->Items->Add("Todos anos");
   //inclui op��o p/ cada ano do mercado
   for (int ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++)
      {
      CBoxAno->Items->Add(IntToStr(ano));
      }
   //pr�-seleciona op��o
   CBoxAno->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::CBoxCrescimentoInicia(void)
   {
   //vari�veis locais
   VTCrescimento *crescimento;
   TList         *lisCRESCIMENTO = segmento->LisCrescimento();

   //reinicia CBoxCrescimento
   CBoxCrescimento->Clear();
   //inclui op��o "Todas classes"
	CBoxCrescimento->Items->Add("Todas classes");
   //inclui item p/ cada Crescimento
   for (int n = 0; n < lisCRESCIMENTO->Count; n++)
      {
      crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[n];
      CBoxCrescimento->Items->AddObject(crescimento->Codigo, crescimento);
      }
   //pr�-seleciona op��o
   CBoxCrescimento->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::DefineTaxa(VTCrescimento *crescimento, int ano_ini, int ano_fim, double taxa_perc)
   {
   //prote��o
   if (crescimento == NULL)  return;
   if (ano_ini < mercado->AnoInicial) return;
   if (ano_fim > mercado->AnoFinal)   return;
   //determina linha do SGrid que cont�m o Crescimento
   for (int nr = 1; nr < SGrid->RowCount; nr++)
      {
      if (SGrid->Objects[0][nr] == crescimento)
         {//preenche SGrid p/ os anos indicados
         for (int ano = ano_ini, nc = ano_ini - mercado->AnoInicial + 1; ano <= ano_fim; ano++, nc++)
            {
            SGrid->Cells[nc][nr] = DoubleToStr("%5.4f", taxa_perc);
            }
         return;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTaxa::LeDados(void)
   {
   //vari�veis locais
   VTCrescimento *crescimento;

   try{//l� taxas do SGrid
      for (int nr = 1; nr < SGrid->RowCount; nr++)
         {//obt�m objeto Crescimento associado � linha de SGrid
         crescimento = (VTCrescimento*)SGrid->Objects[0][nr];
         //l� valores das c�lulas
         for (int nc = 1, ano = mercado->AnoInicial;
              (nc < SGrid->ColCount)&&(ano <= mercado->AnoFinal);
              nc++, ano++)
            {
            crescimento->Taxa_perc[ano] = StrToDouble(SGrid->Cells[nc][nr]);
            }
         }
      }catch(Exception &e)
         {
         Aviso("Valores preenchidos com caracteres inv�lidos");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTaxa::LeTaxa(double &taxa)
   {
   //verifica se EditTaxa foi preenchido
   if (EditTaxa->Text.IsEmpty())
      {
      Aviso("Defina a taxa ser preenchida");
      return(false);
      }
   try{//l� EditTaxa
      taxa = StrToDouble(EditTaxa->Text);
      }catch(Exception &e)
         {
         Aviso("Valor da taxa preenchido com caractere inv�lido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTaxa::SGridInicia(void)
   {
   //vari�veis locais
   VTCrescimento *crescimento;
	TList         *lisCRESCIMENTO = segmento->LisCrescimento();

   //dimensiona o SGrid
   SGrid->ColCount = mercado->NumAnos + 1;
   SGrid->RowCount = lisCRESCIMENTO->Count + 1;
   //inicia t�tulos das colunas
   SGrid->Cells[0][0] = "Classe";
   for (int nc = 1, ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++, nc++)
      {
      SGrid->Cells[nc][0] = IntToStr(ano);
      }
   //inicia t�tulos das linhas
   for (int nr = 1, n = 0; n < lisCRESCIMENTO->Count; n++, nr++)
      {
      crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[n];
      SGrid->Cells[0][nr] = crescimento->Codigo;
      }
   //preenche o SGrid c/ as taxas de Crescimento
   for (int nr = 1, n = 0; n < lisCRESCIMENTO->Count; n++, nr++)
      {
      crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[n];
      SGrid->Objects[0][nr] = crescimento;
      for (int nc = 1, ano = mercado->AnoInicial;
           (nc < SGrid->ColCount)&&(ano <= mercado->AnoFinal);
           nc++, ano++)
         {
         SGrid->Cells[nc][nr] = DoubleToStr("%5.4f", crescimento->Taxa_perc[ano]);
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTaxa::Validadados(void)
   {
   //vari�veis locais
   VTCrescimento *crescimento;

   //verifica se todos Crescimentos foram definidos c/ valores s�o nulos
   for (int nr = 1; nr < SGrid->RowCount; nr++)
      {//obt�m objeto Crescimento associado � linha de SGrid
      crescimento = (VTCrescimento*)SGrid->Objects[0][nr];
      if (! crescimento->Nulo()) return(true);
      }
   //todos Crescimentos s�o nulos
   if (Confirma("Todas as taxas de crescimento s�o nulas.",
                "Confirma os valores ?") == IDYES) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
//eof


