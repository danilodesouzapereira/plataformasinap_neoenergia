//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAtivo.h"
#include "VTAtivo.h"
#include "VTBaseAtivo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Diretorio\VTPath.h"
#include "..\Obra\VTObra.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAtivo::TFormAtivo(TComponent* Owner, VTEstudo *estudo, VTApl* apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro´p/ Estudo
   this->estudo = estudo;
   // salva ponteiro para apl
   apl = apl_owner;
   //determina ano inicial e final do Estudo
   ano_ini = estudo->AnoInicial;
   ano_fim = estudo->AnoFinal;
   //garante que todas as Alternativas tenham o objeto VTBaseAtivo
   IniciaAlternativas();
   //inicia CLBoxAlternativa
   CLBoxAlternativaInicia();
   //inicia LView
   LViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormAtivo::~TFormAtivo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::ActionConfirmaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::ActionImprimeExecute(TObject *Sender)
   {  //DVK 2014.01.06
   //variaveis locais
   AnsiString  nomeArquivo;
   VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));
   VTReport    *report;

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
   //define pasta inicial
   SaveDialogExporta->InitialDir = path->DirExporta();
   if(SaveDialogExporta->Execute())
      {//recebe nome do arquivo a ser exportado e exporta listView
      nomeArquivo = SaveDialogExporta->FileName;
      if(report->ArquivoTexto(nomeArquivo,LView, "", false))
         Aviso("A tabela foi exportada corretamente.");
		else Erro("Erro ao exportar tabela.");
      }
	//destroi report
	if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reapresenta form
	Show();
   }

//---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormAtivo::AlternativaSelecionada(void)
   {
   //variáveis locais
   VTAlternativa *alternativa;

   //verifica se há uma Alternativa selecionada
   if (CLBoxAlternativa->ItemIndex < 0) return(NULL);
   //determina Alternativa
   alternativa = (VTAlternativa*)CLBoxAlternativa->Items->Objects[CLBoxAlternativa->ItemIndex];

   return(alternativa);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::CLBoxAlternativaClick(TObject *Sender)
   {
   //inicia todos os LView
   LViewInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::CLBoxAlternativaClickCheck(TObject *Sender)
   {
   //reinicia LView
   LViewInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::CLBoxAlternativaInicia(void)
   {
   //variáveis locais
   int            index;
   VTAlternativa  *alternativa;
   VTBaseAtivo    *base_ativo;
   TList          *lisALTER = estudo->LisAlternativa();
   TCheckListBox  *CLBox    = CLBoxAlternativa;

   //reinicia CLBox
   CLBox->Clear();
   //insere Alternativas no CLBox
   for (int n = 0; n < lisALTER->Count; n++)
      {
      alternativa = (VTAlternativa*)lisALTER->Items[n];
      //descarta Alterantiva sem Ativos
      base_ativo = (VTBaseAtivo*)alternativa->Apl->GetObject(__classid(VTBaseAtivo));
      if (base_ativo->NumeroDeAtivos() == 0) continue;
      //if (alternativa == estudo->AlternativaPadrao) continue;
      //insere um novo item
      index = CLBox->Items->AddObject(alternativa->Codigo, alternativa);
      //marca Alternativa como selecionada (checked)
      CLBox->Checked[index] = alternativa->Valida;
      }
   //pré-seleciona primeira Alternativa
   if (CLBox->Items->Count > 0) CLBox->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::IniciaAlternativas(void)
	{
	//variáveis locais
	VTBaseAtivo    *base_ativo;
	VTAlternativa  *alternativa;
	TList          *lisALTER     = estudo->LisAlternativa();

	//garante que todas as Alternativas tenham o objeto VTBaseAtivo
	for (int n = 0; n < lisALTER->Count; n++)
		{
		alternativa = (VTAlternativa*)lisALTER->Items[n];
		base_ativo = (VTBaseAtivo*)alternativa->Apl->GetObject(__classid(VTBaseAtivo));
		if (base_ativo == NULL)
			{//cria objeto BaseAtivo e insere na Alternativa
			alternativa->Apl->Add(base_ativo = NewObjBaseAtivo());
			//inicia Base Ativo
//			base_ativo->Inicia(alternativa);
			}
		//inicia Base Ativo sempre (DVK 2015.09.03)
		base_ativo->Inicia(alternativa);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormAtivo::LViewInicia(void)
   {
   //variáveis locais
   double         valor;
   AnsiString     txt;
   TListItem      *Item;
   TList          *lisATIVO;
   VTAlternativa *alternativa;
   VTBaseAtivo    *base_ativo;
   VTAtivo        *ativo;

   //reinicia LView
   LView->Items->Clear();
   //loop p/ todas Alternativas selecionadas
   for (int n = 0; n < CLBoxAlternativa->Items->Count; n++)
      {//verifica se o Item está selecionado
      if (! CLBoxAlternativa->Checked[n]) continue;
      //exibe dados da Alterantiva selecionada
      alternativa = (VTAlternativa*)(CLBoxAlternativa->Items->Objects[n]);
      //determina lista de Ativos da Alternativa
      base_ativo = (VTBaseAtivo*)alternativa->Apl->GetObject(__classid(VTBaseAtivo));
      lisATIVO   = base_ativo->LisAtivo();
      //insere Ativos no LView
      for (int i = 0; i < lisATIVO->Count; i++)
          {
          ativo = (VTAtivo*)lisATIVO->Items[i];
          //insere novo ListItem
          Item          = LView->Items->Add();
          Item->Data    = ativo;
          Item->Caption = alternativa->Codigo;
          Item->SubItems->Add(ativo->Obra->Nome);
          Item->SubItems->Add(IntToStr(ativo->Ano));
          Item->SubItems->Add(ativo->RedeDoEqpto->Codigo);
          Item->SubItems->Add(ativo->TipoAcaoAsString());
          Item->SubItems->Add(ativo->Eqpto->TipoAsString());
          Item->SubItems->Add(ativo->Eqpto->Codigo);
          //insere informação complementar
          switch(ativo->Eqpto->Tipo())
             {
             case eqptoARRANJO:
                txt.sprintf("comp(km): %4.3f", ativo->Comprimento_km);
                break;
             case eqptoTRAFO:
                valor = (((VTTrafo*)ativo->Eqpto)->snom);
                txt.sprintf("Snom(kVA): %2.1f", valor * 1000.);
                break;
             case eqptoTRAFO3E:
                valor = (((VTTrafo3E*)ativo->Eqpto)->Sbase);
                txt.sprintf("Snom(kVA): %2.1f", valor * 1000.);
                break;
             case eqptoCAPACITOR:
                valor = (((VTCapacitor*)ativo->Eqpto)->q);
                txt.sprintf("Q(kVAr): %2.1f", valor * 1000.);
                break;
             case eqptoREATOR:
                valor = (((VTReator*)ativo->Eqpto)->q);
                txt.sprintf("Q(kVAr): %2.1f", valor * 1000.);
                break;
             default:
                txt = "";
                break;
             }
          Item->SubItems->Add(txt);
          }
      }
   //pré-seleciona primeiro Ativo
   if (LView->Items->Count > 0) LView->Selected = LView->Items->Item[0];
   }

//---------------------------------------------------------------------------
//eof

