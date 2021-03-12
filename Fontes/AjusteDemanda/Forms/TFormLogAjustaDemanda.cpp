//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormLogAjustaDemanda.h"
#include "VTAjusta.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\Radial\VTPrimario.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormLogAjustaDemanda::TFormLogAjustaDemanda(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl_owner;
   }

//---------------------------------------------------------------------------
__fastcall TFormLogAjustaDemanda::~TFormLogAjustaDemanda(void)
   {
	//nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogAjustaDemanda::ActionFecharExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogAjustaDemanda::ActionExportarExecute(TObject *Sender)
   {
	//variaveis locais
	AnsiString	nome_arq, titulo;
	VTReport 	*report;
	VTPath 		*path = (VTPath*)apl->GetObject(__classid(VTPath));

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
	//define pasta inicial
	SaveDialogExporta->InitialDir = path->DirExporta();
	if(SaveDialogExporta->Execute())
		{//recebe nome do arquivo a ser exportado e exporta listView
		nome_arq = SaveDialogExporta->FileName;
		if(CboxFase->Checked) titulo = "Ajuste com fator de correção por fase";
		else                  titulo = "Ajuste com fator de correção trifásico";
		if(report->ArquivoTexto(nome_arq, LViewLog, titulo, false))
			Aviso("A tabela foi exportada corretamente.");
		else Erro("Erro ao exportar tabela.");
		}
	//destroi report
	if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reapresenta form
	Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogAjustaDemanda::ActionLimpaExecute(TObject *Sender)
	{
	//variaveis locais
	bool append = false;

	//verifica se o log deve ser apagado
	switch(Confirma("Deseja limpar o log ?", ""))
		{
		case IDNO: //nada a fazer
			break;
		case IDYES: //limpa LView
			LViewLog->Clear();
			SalvaLog(append);
			break;
		case IDCANCEL: return;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormLogAjustaDemanda::Executa(TList *lisMedicao,
													bool ajuste_por_fase, VTAjusta *ajusta)
	{
   //var locais
   AnsiString diagnostico;
   AnsiString erro_perc;
	VTMedicao  *medicao;
   //VTPrimario *primario;
	TListItem  *item;

	//marca tipo de ajuste realizado
	CboxFase->Checked = ajuste_por_fase;
	CboxTrifasico->Checked = (! ajuste_por_fase);
   //limpa log
	LViewLog->Clear();
   for(int nm = 0; nm < lisMedicao->Count; nm++)
      {//loop em todas medicao
		medicao = (VTMedicao*)lisMedicao->Items[nm];
		item = LViewLog->Items->Add();
		item->Caption = medicao->chave_med->rede->Codigo;
		item->SubItems->Add(medicao->chave_med->Codigo);
      //diagnóstico
      switch(medicao->diagnostico)
         {
         case diagMED_OK:
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
            diagnostico = "OK";
				break;
         case diagMED_ERRO_INICIAL:
//				erro_perc   = ""; //DVK 2015.09.24
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
//				diagnostico = "Diferença inicial maior do que o limite definido";
				diagnostico.sprintf("Diferença inicial maior do que o limite definido (%2.1f%%)",
										  ajusta->AjusteMaximo * 100.);
            break;
			case diagMED_ERRO_FINAL:
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
				//verifica se o limite do ajuste foi definido
				if(! ajusta->LimitaAjuste)
					{//altera mensagem de diagnóstico
//					diagnostico.sprintf("A precisão máxima obtida com o ajuste foi de (%2.1f%%)",
//										  ajusta->ErroMaximo);
					diagnostico = "OK";
					break;
					}
				diagnostico.sprintf("O erro final é maior que o erro máximo definido (%2.1f%%)",
										  ajusta->ErroMaximo * 100.);
            break;
         case diagMED_ERRO_FLUXO:
            erro_perc   = "";
            diagnostico = "Fluxo não calculado para a rede";
				break;
			case diagTEMPO:
            erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
				diagnostico = "Tempo excedido para cálculo";
				break;
         default:
            erro_perc   = "";
				diagnostico = "";
            break;
         }
		//erro percentual
//		item->SubItems->Add(erro_perc);
		//diagnóstico
		item->SubItems->Add(diagnostico);
		}
	//salva log em arquivo
	SalvaLog();
	}

//---------------------------------------------------------------------------
void __fastcall TFormLogAjustaDemanda::Importa(AnsiString arquivo)
	{
	//variaveis locais
	TStringList *lines  = new TStringList();
	TStringList *campos = new TStringList();
	TListItem   *item;
	AnsiString  texto;

	//limpa LView
	LViewLog->Clear();
	//le arquivo
	lines->Clear();
	lines->LoadFromFile(arquivo);
	for(int nl = 0; nl < lines->Count; nl++)
		{//insere conteudo do arquivo no LView
		campos->Clear();
		ExtraiStrings(lines->Strings[nl], ";", campos);
		if(campos->Count < 3) continue;
		//descarta linha de cabeçalho
		texto = campos->Strings[2].Trim();
		if(texto.AnsiCompareIC("Diagnóstico") == 0) continue;
		item = LViewLog->Items->Add();
		item->Caption = campos->Strings[0];
		item->SubItems->Add(campos->Strings[1]);
		item->SubItems->Add(campos->Strings[2]);
//		item->SubItems->Add(campos->Strings[3]);
		}
	delete lines;
	delete campos;
	}

//---------------------------------------------------------------------------
void __fastcall TFormLogAjustaDemanda::SalvaLog(bool append)
	{
	//variaveis locais
	AnsiString	nome_arq;
	VTReport 	*report;
	VTPath 		*path = (VTPath*)apl->GetObject(__classid(VTPath));

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//exporta listView na pasta tmp
	nome_arq = path->DirTmp() + "\\LogAjuste.txt";
	report->ArquivoTexto(nome_arq, LViewLog, "", append);
	//destroi report
	if(report) {DLL_DeleteObjReport(report); report = NULL;}
	}

//---------------------------------------------------------------------------
//eof


