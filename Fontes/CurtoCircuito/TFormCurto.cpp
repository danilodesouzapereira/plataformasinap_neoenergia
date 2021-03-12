//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormCurto.h"
#include "TFormCurtoArvore.h"
#include "TFormCurtoTabela.h"
#include "TFormPosDefTrecho.h"
#include "TFormRelChave.h"
#include "TFormZdef.h"
#include "VTCurtocircuito.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curto\VTCurto.h"
//#include "..\Diretorio\VTPath.h"
#include "..\Diretorio\VTPath.h"
#include "..\Distancia\VTCaminho.h"
#include "..\Editor\TEdtCurto.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTGrupo.h"
#include "..\Legenda\VTVetval.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\ResCC\VTResCC.h"
#include "..\Report\VTReport.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Sinap\VTSinapMdi.h"
#include "..\..\DLL_Inc\Distancia.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Protecao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"
using namespace std;
#include <complex>
//---------------------------------------------------------------------------
__fastcall TFormCurto::TFormCurto(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
					  : TFormEdt(Owner, parent)
   {
   try{//cria objetos
      lisBAR        = new TList();
      lisLIG        = new TList();
      curtocircuito = NewObjCurtocircuito(apl_owner);
      //obt�m objeto AplCurto
      apl    = curtocircuito->Apl;
      //obt�m objeto RedeCC
      redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
      //inicia controle de c�lculo de fluxo
      defeito.fluxo_calculado = false;
      //obt�m Zdef definida em VTGeral
      IniciaZdef();
      //exibe imped�ncia de defeito
      ExibeZdef();
      //inicia CBoxPatamar
      CBoxPatamarInicia();
      //cria Forms de apresenta��o dos resultados
      FormCurtoArvore = new TFormCurtoArvore(this, PanelCurto, apl);
      FormCurtoTabela = new TFormCurtoTabela(this, PanelCurto, apl);
      //configura op��es de usu�rio privilegiado
	  UsuarioPrivilegiado();
	  //cria progresso
	  progresso = DLL_NewObjProgresso(this,NULL); //n�o destruo o destrutor pois � um form owned por esse
      }catch (Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormCurto::~TFormCurto(void)
   {
   //vari�veis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //N�O destr�i FormCurtoArvore e FormCurtoTabela
   //if (FormCurtoArvore) {delete FormCurtoArvore; FormCurtoArvore = NULL;}
   //if (FormCurtoTabela) {delete FormCurtoTabela; FormCurtoTabela = NULL;}
   //destr�i lista sem destruir seus objetos
   //if (lisEQP_SEL) {delete lisEQP_SEL; lisEQP_SEL = NULL;}
   //destr�i lista e seus objetos
   //if (lisGRUPO) {LimpaTList(lisGRUPO); delete lisGRUPO; lisGRUPO = NULL;}
   //fecha eventuais TFormProtecao
   DLL_CloseFormProtecao(this);
   //apaga moldura e eqptos em destaque
   grafico->Moldura();
   grafico->DestacaEqpto(NULL, clBlack, 1);
   //destr�i objetos
   if (edt_curto)     {delete edt_curto;     edt_curto     = NULL;}
   if (curtocircuito) {delete curtocircuito; curtocircuito = NULL;}
   //destr�i lista sem destrui seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionEditaDefTrechoExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormPosDefTrecho *FormPosDefTrecho;

   //exibe janela de edi��o da imped�ncia de defeito
   FormPosDefTrecho = new TFormPosDefTrecho(this, apl);
   FormPosDefTrecho->DefinePosDefeito((VTTrecho*)defeito.eqpto, defeito.dist_bar1);
   if (FormPosDefTrecho->ShowModal() == mrOk)
      {//determina novo valor da dist�ncia do defeito em rela��o � Barra1 do Trecho
      defeito.dist_bar1 = FormPosDefTrecho->DistanciaBarra1KM();
      //recalcula o curto
      ExecutaCalculo();
      //Reinicia FormCurtoArvore e FormCurtoTabela
      //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
      //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
      }
   //destr�i FormPosDefTrecho
   delete FormPosDefTrecho;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionEditaZdefExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormZdef *FormZdef;
   VTGeral   *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //exibe janela de edi��o da imped�ncia de defeito
   FormZdef = new TFormZdef(this);
   FormZdef->DefineZdef(defeito.Zdef);
   if (FormZdef->ShowModal() == mrOk)
      {//determina novo valor da imped�ncia de defeito
      FormZdef->LeZdef(defeito.Zdef);
      //salva arquivo
      SalvaArqCfgCurto();
      //exibe imped�ncia de dedefeito
      ExibeZdef();
      //recalcula o curto
      ExecutaCalculo();
      //atualiza FormCurtoArvore e FormCurtoTabela
      //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
      //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
      }
   //destr�i FormZdef
   delete FormZdef;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionEdtExecute(TObject *Sender)
   {
   try{//se necess�rio, cria um objeto TEdtCurto
      if (edt_curto != NULL) delete edt_curto;
      //cria EdtSpy p/ controle do mouse
      edt_curto = DLL_NewObjEdtCurto(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      ((TEdtCurto*)edt_curto)->DefineActionCurto(ActionSelEqptoDefeito);
      //atualiza indica��o de mouse ativo e for�a bot�o Down
      AtualizaIndicacaoEditorOn(butEdt);
	  //verifica se o botao foi pressionado
	  if (butEdt->Down)
	  {
      	FormRelZent(false);
	  }
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionHelpExecute(TObject *Sender)
   {
   //variaveis locais
   TForm *formHelp;

   formHelp = DLL_NewFormHelp(this, apl, "Curto_Circuito");
   if(formHelp) formHelp->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionRelatorioChaveExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormRelChave *Form;

   try{//verifica se lista com as Ligacoes do caminho el�trico est� preenchida
      if (lisLIG->Count == 0)
         {//determina cainho el�trico
         MontaCaminhoCurto();
         if (lisLIG->Count == 0)
            {
            Aviso("N�o foi poss�vel identifica o caminho el�trico do defeito");
            return;
            }
         }
      //cria TFormRelChave como janela modal
      if ((Form = new TFormRelChave(this, apl)) != NULL)
         {
         Form->DefineCaminhoEletrico(lisLIG);
         Form->ShowModal();
         delete Form;
         }
      }catch(Exception &e)
         {
         //nada a fazer
         }
   }
//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionExportRelZentExecute(TObject *Sender)
{   //variaveis locais
	VTPath      *path  = (VTPath*) apl->GetObject(__classid(VTPath));
	AnsiString	nome_arq, titulo;
	VTReport 	*report;

	//prote��o
	if(! path) return;
	try
	{
        //cria objeto report
		report = DLL_NewObjReport(apl);
		//esconde form, para a janela Salvar Como n�o ficar por baixo
		Hide();
		titulo = "Imped�ncias de entrada por barra";
		//define pasta padrao para SaveDialog
		SaveDialog->InitialDir = path->DirExporta();
		//define nome padrao do arquivo
		SaveDialog->FileName = "ImpedanciaEntradaBarras.csv";
		//define extens�o
		SaveDialog->DefaultExt = ".csv";
		if(SaveDialog->Execute())
		{
			nome_arq = SaveDialog->FileName;
			if(report->ArquivoTexto(nome_arq, lviewZent, titulo, false))
				Aviso("A tabela foi exportada corretamente.");
			else Erro("Erro ao exportar tabela.");
		}
		//destroi report
		if(report) {DLL_DeleteObjReport(report); report = NULL;}
	   //reapresenta form
		Show();
	}
	catch (Exception &e)
	{
        //faz mada
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionRelatorioCurtoExecute(TObject *Sender)
   {
   //altera Form de apresenta��o de resultados
   if (FormCurtoArvore->Visible)
      {
      FormCurtoArvore->Hide();
      FormCurtoTabela->Show();
      //redimensiona este form
      ClientWidth = FormCurtoTabela->WidthMinimo();
      //altera �cone
      ActionRelatorioCurto->ImageIndex = 10;
      }
   else
      {
      FormCurtoTabela->Hide();
      FormCurtoArvore->Show();
      //redimensiona este form
      ClientWidth = FormCurtoArvore->WidthMinimo();
      //altera �cone
      ActionRelatorioCurto->ImageIndex = 11;
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionRelatorioZentradaExecute(TObject *Sender)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool showRelZent = false;

	//verifica se esta exibindo o relatorio
	if (PanelRelZent->Visible)
		{showRelZent = false;}
	else
	{	//verifica se � v�lido o c�lculo de curto p/ a rede selecionada
		if (! ValidaRedeParaCurto())
			{showRelZent = false;}
		else
		{	//verifica se a rede possui um numero excessivo de barras
			if (redes)
			{
				if (redes->LisBarra()->Count >= 700)
				{
					if (Confirma("Esse processo pode demorar.", "Deseja realiz�-lo assim mesmo?") == idYes)
						{showRelZent = true;}
					else
						{showRelZent = false;}
				}
				else
					{showRelZent = true;}
			}
		}
	}
	//atualiza form
	FormRelZent(showRelZent);
	if (showRelZent)
		{PreencheRelZent();}
}

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionProtecaoExecute(TObject *Sender)
   {
   //vari�veis locais
   bool         pre_falta = true;
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));
   VTRedes      *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o fluxo est� calculado
   if (! defeito.fluxo_calculado)
      {//executa c�lculo de fluxo com pr�_falta
      ExecutaCalculoFluxo(pre_falta);
      //verifica se existe o eqpto de defeito
      if (defeito.eqpto == NULL)
         {//reinicia RedeCC
         redeCC->Inicia(redes);
         }
      else
         {//torna a executar o c�lculo de curto porque os objetos Barra e Ligacao precisam
         //apontar para BarCC e LigCC
         ExecutaCalculo();
         }
      }
   //exibe TFormProtecao
   DLL_NewFormProtecao(this, apl, MultiPanel->PanelAtivo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionSelEqptoExecute(TObject *Sender)
{
   //vari�veis locais
   VTEqpto *eqpto = (VTEqpto*)Sender;

   if(PanelRelZent->Visible == true)
   {
	Aviso("Clicar em 'Equipamento selecionado' para selecionar o equipamento e aplicar o curto-circuito");
	return;
   }
   //prote��o: verifica se j� foi selecionado o eqpto de defeito
   if (defeito.eqpto == NULL) return;
   //prote��o: verifica se j� foi selecionado o eqpto
   if (eqpto == NULL) return;
   //indica Eqpto selecionado p/ FormCurtoArvore e FormCurtoTabela
   FormCurtoArvore->EqptoSelecionado(eqpto);
   FormCurtoTabela->EqptoSelecionado(eqpto);

   //salva Eqpto selecionado em lisEQP_SEL
   //if (lisEQP_SEL->IndexOf(eqpto) < 0) lisEQP_SEL->Add(eqpto);
   //insere no TView dados de curto do Eqpto
   //TViewInsereEqpto(eqpto);
}

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ActionSelEqptoDefeitoExecute(TObject *Sender)
{
	//vari�veis locais
	VTTrecho *trecho;
	VTEqpto  *eqpto = (VTEqpto*)Sender;
//	VTProgresso  *progresso   = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	if(PanelRelZent->Visible == true)
	{
		Aviso("Clicar em 'Equipamento selecionado' para selecionar o equipamento e aplicar o curto-circuito");
		return;
	}
	if(progresso)
	{
		progresso->Start(progDEFAULT);
	}
	//salva eqpto de defeito
	if (eqpto == NULL)
	{
		if(progresso)
		{
			progresso->Stop();
		}
		return;
	}
	//verifica se
	if ((eqpto->Tipo() != eqptoBARRA)&&(eqpto->Tipo() != eqptoTRECHO))
	{
		Aviso("An�lise de curto-circuito.\nSelecione barra ou trecho de defeito.");
		if(progresso)
		{
			progresso->Stop();
		}
		return;
	}
	//verifica se � v�lido o c�lculo de curto p/ a rede selecionada
	if (! ValidaRedeParaCurto())
	{
		if(progresso)
		{
			progresso->Stop();
		}
		return;
	}
	//reinicia lisEQP_SEL
	//lisEQP_SEL->Clear();
	//salva Eqpto de defeito
	defeito.eqpto = eqpto;
	if (eqpto->Tipo() == eqptoTRECHO)
	{	//determina Trecho selecionado
		trecho = (VTTrecho*)eqpto;
		//define dist�ncia em rela��o � barra 1, em km
		defeito.dist_bar1 = trecho->Comprimento_km / 2.;
	}
	//executa c�lculo de curto
	if (! ExecutaCalculo())
	{
		if(progresso)
		{
			progresso->Stop();
		}
		return;
	}
	//indica Eqpto de defeito p/ FormCurtoArvore e FormCurtoTabela
	//FormCurtoArvore->EqptoDefeito(eqpto);
	//FormCurtoTabela->EqptoDefeito(eqpto);
	//habilita/desabilita edi��o de posi��o do defeito em trecho
	ActionEditaDefTrecho->Visible = ((eqpto->Tipo() == eqptoTRECHO) && (PanelRelZent->Visible == false));
	//monta e exibe o caminho do curto
	MontaCaminhoCurto();
	ExibeCaminhoCurto(CheckBoxCaminho->Checked);
	if(progresso)
	{
		progresso->Stop();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::AtualizaCalculo(void)
   {
   //assume que houve mudan�a na rede e que o fluxo n�o foi calculado
   defeito.rede_alterada   = true;
   defeito.fluxo_calculado = false;
   //prote��o: verifica se h� um Eqpto de defeito selecionado
   if (defeito.eqpto != NULL)
      {//recalcula curto considerando o novo patamar
      ExecutaCalculo();
      //indica Eqpto de defeito p/ FormCurtoArvore e FormCurtoTabela
      //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
      //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
      }
   //cancela indica��o de mudan�a da rede
   defeito.rede_alterada = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::CBoxDefChange(TObject *Sender)
   {
   //vari�veis locais
   int       ind_def;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //determina tipo de defeito p/ mostrar anima��o no gr�fico
   switch(CBoxDef->ItemIndex)
      {
      case 0:  ind_def = defeito3F;  break;
      case 1:  ind_def = defeito2F;  break;
      case 2:  ind_def = defeitoFT;  break;
      case 3:  ind_def = defeitoFTZ; break;
      case 4:  ind_def = defeito2FT; break;
      default: return;
      }
   //seleciona defeito a ser exibido em RedeCC
   redeCC->ShowDefeito = ind_def;
   //atualiza gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::CBoxPatamarChange(TObject *Sender)
   {
   //recalcula curto-circuito p/ a condi��o selecionada
   ExecutaCalculo();
   //atualiza FormCurtoArvore e FormCurtoTabela
   //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
   //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::CBoxPatamarInicia(void)
   {
   //vari�veis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //reinicia CBoxPatamar
   CBoxPatamar->Clear();
   //insere op��o "N�o utilizado"
   CBoxPatamar->Items->AddObject("N�o Utilizado", NULL);
   //insere Patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = patamares->Patamar[ind_pat];
      CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      }
   //pr�-seleciona primeiro patamar
   CBoxPatamar->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::CheckBoxCaminhoClick(TObject *Sender)
   {
   //exibe o caminho do curto
   if (CheckBoxCaminho->Checked)
   {
        MontaCaminhoCurto();
   }
   ExibeCaminhoCurto(CheckBoxCaminho->Checked);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ControlSelEquipamento(void)
   {
    //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	   if(PanelRelZent->Visible == true)
	   {
			ActionEditaDefTrecho->Visible = false;
			grafico->Moldura();
			grafico->DestacaEqpto(NULL, clBlack, 1);
			//grafico->Disable();
	   }
   }

//---------------------------------------------------------------------------
int __fastcall TFormCurto::DeterminaPatamarSelecionado(void)
   {
   //vari�veis locais
   VTPatamar   *patamar;

   //prote��o:
   if (CBoxPatamar->ItemIndex <= 0) return(-1);
   //determina Patamar selecionado
   patamar  = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];
   return(patamar->Index);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurto::ExecutaCalculo(void)
   {
   //vari�veis locais
   bool         sucesso;
   bool         pre_falta;
   int          ind_pat;
   VTPatamar    *patamar;
   VTTrecho     *trecho;
   VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   VTRedes      *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTGrafico    *grafico    = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //desabilita anima��o do gr�fico
   grafico->AnimacaoEnabled = false;
   //desabilita medi��o de tempo
   cronometro = NULL;
   //verifica se o Eqpto de defeito est� definido
   if (defeito.eqpto == NULL)
      {
      Aviso("Selecione a barra ou trecho do defeito");
      return(false);
      }
   //determina algoritmo de c�lculo de curto
   switch(RadioGroupAlgoritmo->ItemIndex)
      {
      case  0: curtocircuito->Algoritmo = curtoCS; break;
      case  1: curtocircuito->Algoritmo = curtoMA; break;
      default: curtocircuito->Algoritmo = curtoMA; break;
      }
   //determina Patamar selecionado para a condi��o de pr�-falta
   if ((ind_pat = DeterminaPatamarSelecionado()) < 0)
      {//importante: define um valor v�lido para o patamar
      pre_falta = false;
      ind_pat   = 0;
      //desabilita visualiza��o do FormProtecao
      //ActionProtecao->Enabled = false;
      }
   else
      {//se necess�rio, executa c�lculo de fluxo
      pre_falta = true;
      if (! ExecutaCalculoFluxo(pre_falta)) return(false);
      //habilita visualiza��o do FormProtecao
      //ActionProtecao->Enabled = true;
      }
   //inicia cron�metro
   if (cronometro) cronometro->Reinicia();
   if (cronometro) cronometro->Start("Curto", "Curto circuito");
   //configura op��es do Curtocircuito
   curtocircuito->AvisoEnabled  = true;
   curtocircuito->TipoRelatorio = (CheckBoxRelatorio->Checked) ? relCOMPLETO : relNENHUM;
   //verifica o tipo de Eqpto selecionado
   if (defeito.eqpto->Tipo() == eqptoBARRA)
      {//executa Curto na Barra selecionada
      sucesso = curtocircuito->Executa(redes, defeito.Zdef, defeito.rede_alterada, (VTBarra*)defeito.eqpto, ind_pat, pre_falta);
		  if(!sucesso)
		  {
			barras_isoladas++;
		  }
	  }
   else if (defeito.eqpto->Tipo() == eqptoTRECHO)
      {//determina Trecho selecionado
      trecho = (VTTrecho*)defeito.eqpto;
      //executa Curto no Trecho selecionado
      sucesso = curtocircuito->Executa(redes, defeito.Zdef, defeito.rede_alterada, trecho, defeito.dist_bar1, ind_pat, pre_falta);
      }
   if (cronometro) cronometro->Stop("Curto");
   if (!sucesso)
	  {
		  if(barras_isoladas == 0)
		  {
			  Aviso("O c�lculo de curto-circuito n�o pode ser executado.");
			  return(false);
		  }
	  }
   //atualiza gr�fico
   grafico->Redraw();
   grafico->AnimacaoEnabled = true;
   //Reinicia FormCurtoArvore e FormCurtoTabela
   FormCurtoArvore->EqptoDefeito(defeito.eqpto);
   FormCurtoTabela->EqptoDefeito(defeito.eqpto);
   //atualiza FormProtecao
   DLL_AtualizaFormProtecao(this);

   //exibe tempo gasto no c�lculo
   if (cronometro) cronometro->Show();

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurto::ExecutaCalculoFluxo(bool pre_falta)
   {
   //vari�veis locais
   VTFlow    *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //2014.02.13 - CurtoCS sem pr�-falta n�o precisa do c�lculo de fluxo sem carga

   //verifica necessidade de calcular fluxo de pot�ncia
   if ((pre_falta)&&(! defeito.fluxo_calculado))
      {//executa c�lculo de fluxo
      flow->AvisoEnabled      = false;
      flow->CronometroEnabled = false;
      flow->TramoEnabled      = false;
      flow->TipoRelatorio     = relNENHUM;
      //calcula fluxo equilirado/desequilibrado conforme sele��o de c�lculo de curto
      if (RadioGroupAlgoritmo) {flow->TipoFluxo = fluxoEQUILIBRADO;}
      else                     {flow->TipoFluxo = fluxoDESEQUILIBRADO;}
      flow->UpToDate  = false;
      //calcula fluxo
      if (! flow->CalculaPorRede(redes))
      // if (! flow->CalculaRedeUnica(redes))
         {//gera aviso p/ usu�rio
         Aviso("Erro no c�lculo de fluxo de pot�ncia.\n"
               "N�o � poss�vel calcular a condi��o de pr�-carga");
         return(false);
         }
      //marca que o fluxo foi calculado
      defeito.fluxo_calculado = true;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ExibeCaminhoCurto(bool visible)
{
	//vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisVazia;
	
	grafico->Moldura();		
	//destaca Ligacoes no gr�fico
	if (visible)
	{
		
		grafico->DestacaEqpto(lisLIG, PanelColor->Color, 3);
	}
	else
	{
		lisVazia = new TList;
		grafico->DestacaEqpto(lisVazia, PanelColor->Color, 3);
		delete lisVazia;
	}
	grafico->Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TFormCurto::ExibeZdef(void)
   {
   //vari�veis locais
   TListItem *Item;
   //exibe valores
   //EditRdef->Text = DoubleToStr("%2.1f", defeito.Zdef.Zdef.r);
   //EditXdef->Text = DoubleToStr("%2.1f", defeito.Zdef.Zdef.x);
   //reinicia ListViewZdef
   ListViewZdef->Clear();
   //insere dados de Zdef3F_ff
   Item = ListViewZdef->Items->Add();
   Item->Caption = "3F";
   Item->SubItems->Add("fase/fase");
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef3F_ff.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef3F_ff.x));
   //insere dados de Zdef2F_ff
   Item = ListViewZdef->Items->Add();
   Item->Caption = "2F";
   Item->SubItems->Add("fase/fase");
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef2F_ff.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef2F_ff.x));
   //insere dados de Zdef2FT_ff
   Item = ListViewZdef->Items->Add();
   Item->Caption = "2FT";
   Item->SubItems->Add("fase/fase");
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef2FT_ff.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef2FT_ff.x));
   //insere dados de Zdef2FT_ft
   Item = ListViewZdef->Items->Add();
   Item->Caption = "2FT";
   Item->SubItems->Add("fase/terra");
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef2FT_ft.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef2FT_ft.x));
   //insere dados de Zdef1FT_ft
   Item = ListViewZdef->Items->Add();
   Item->Caption = "1FT";
   Item->SubItems->Add("fase/terra");
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef1FT_ft.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", defeito.Zdef.Zdef1FT_ft.x));
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //vari�veis locais
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   //esconde e desabilita o Form
   Hide();
   Enabled = false;
   //seleciona gr�fico de topologia
   sinap_child->ActionTopo->Execute();
   //destr�i o form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (! geral->Infoset->LoadFromFile(this))
         {//posiciona o Form sem alinhamento
         ActionAlignNoneExecute(NULL);
         }
      else
         {
         switch(Align)
			{
            case alLeft:   ActionAlignLeft->Execute();   break;
            case alRight:  ActionAlignRight->Execute();  break;
            case alTop:    ActionAlignUp->Execute();     break;
            case alBottom: ActionAlignDown->Execute();   break;
            case alNone:   //salva �ltima
               janela.top    = Top;
               janela.left   = Left;
               janela.width  = Width;
			   janela.height = Height;
			   break;
			default: break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::FormRelZent(bool showRelZent)
{
	if (showRelZent)
	{
		GBoxGrafico->Visible = false;
		AtualizaIndicacaoEditorOff();
		PanelRelZent->Align = alClient;
		PanelRelZent->Visible = true;
		PanelCurto->Visible = false;
		this->Width = 763;
		ActionRelatorioZentrada->ImageIndex = 15;
		ToolButton1->Hint = "Equipamento selecionado";
		ControlSelEquipamento();
	}
	else
	{
		GBoxGrafico->Visible = true;
		AtualizaIndicacaoEditorOn(butEdt);
		PanelRelZent->Align = alClient;
		PanelRelZent->Visible = false;
		PanelCurto->Visible = true;
		this->Width = 308;
		ActionRelatorioZentrada->ImageIndex = 16;
		ToolButton1->Hint = "Todas barras";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia objeto EdtExplorer
   ActionEdtExecute(NULL);
   //exibe um dos Forms de apresenta��o de resultados
   ActionRelatorioCurto->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::IniciaZdef(void)
   {
   //zera imped�ncia de defeito
   defeito.Zdef.Zdef3F_ff.r  = defeito.Zdef.Zdef3F_ff.x  = 0;
   defeito.Zdef.Zdef2F_ff.r  = defeito.Zdef.Zdef2F_ff.x  = 0;
   defeito.Zdef.Zdef2FT_ff.r = defeito.Zdef.Zdef2FT_ff.x = 0;
   defeito.Zdef.Zdef2FT_ft.r = defeito.Zdef.Zdef2FT_ft.x = 0;
   defeito.Zdef.Zdef1FT_ft.r = defeito.Zdef.Zdef1FT_ft.x = 0;
   //l� Zdef do arquivo de arquivo de configura��o de curto
   LeArqCfgCurto();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::LeArqCfgCurto(void)
   {
   //vari�veis locais
   VTGeral    *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString arq_cfg_curto = "CfgCurto";

   //l� valores de Zdef do arquivo
   if (geral->Infoset->LoadFromFile(arq_cfg_curto))
      {
      geral->Infoset->GetInfo("Zdef3F_ff.r",  defeito.Zdef.Zdef3F_ff.r );
      geral->Infoset->GetInfo("Zdef3F_ff.x",  defeito.Zdef.Zdef3F_ff.x );
      geral->Infoset->GetInfo("Zdef2F_ff.r",  defeito.Zdef.Zdef2F_ff.r );
      geral->Infoset->GetInfo("Zdef2F_ff.x",  defeito.Zdef.Zdef2F_ff.x );
      geral->Infoset->GetInfo("Zdef2FT_ff.r", defeito.Zdef.Zdef2FT_ff.r);
      geral->Infoset->GetInfo("Zdef2FT_ff.x", defeito.Zdef.Zdef2FT_ff.x);
      geral->Infoset->GetInfo("Zdef2FT_ft.r", defeito.Zdef.Zdef2FT_ft.r);
      geral->Infoset->GetInfo("Zdef2FT_ft.x", defeito.Zdef.Zdef2FT_ft.x);
      geral->Infoset->GetInfo("Zdef1FT_ft.r", defeito.Zdef.Zdef1FT_ft.r);
      geral->Infoset->GetInfo("Zdef1FT_ft.x", defeito.Zdef.Zdef1FT_ft.x);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::lviewZentClick(TObject *Sender)
{
	VTBarra *barra;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;

	item = lviewZent->Selected;

	if (item != NULL)
	{
		barra = (VTBarra*) item->Data;
		if (barra != NULL)
		{
			grafico->Moldura(barra);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::lviewZentDblClick(TObject *Sender)
{
	//vari�veis locais
	VTEqpto   *eqpto;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;

	try{//determina Eqpto
		item = lviewZent->Selected;
		eqpto = (VTEqpto*) item->Data;
		//seleciona �rea de zoom p/ destacar o Eqpto
		grafico->SelecionaZoom(eqpto);
		}
	catch(Exception &e)
	{//nada a fazer
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::MontaCaminhoCurto(void)
   {
   //vari�veis locais
   VTCaminho *caminho;

   try{//reinicia listas de Barras e Ligacoes do cominho el�trico
      lisBAR->Clear();
      lisLIG->Clear();
      //verifica se a exibi��o do caminho est� habilitada
      if (! CheckBoxCaminho->Checked) return;
      //cria objeto VTCaminho
      caminho = DLL_NewObjCaminho(apl);
      //identifica Ligacoes entre a Barra de defeito e os Suprimentos/Geradores
      lisBAR->Add(redeCC->BarraDefeito());
      caminho->Executa(lisBAR, redeCC->LisGeradorSuprimento(), redeCC->LisLigacao(), lisLIG);
      //destr�i objeto caminho
      delete caminho;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::PanelColorDblClick(TObject *Sender)
   {
   //exibe ColorDialog
   ColorDialog->Color = PanelColor->Color;
   if (ColorDialog->Execute())
      {//redefine ColorPanel com a cor selecionada
      PanelColor->Color = ColorDialog->Color;
      //atualiza o caminho do curto
      ExibeCaminhoCurto(CheckBoxCaminho->Checked);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::PreencheRelZent(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
//	VTProgresso  *progresso   = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	TList *lisBarras;
	VTBarra *barra;
	VTResCC *resCC;
	VTGrupo *grupo;
	VTDado	*dado;
	//VTVetval *vetval;
	AnsiString desc;
	TStringList *valores;
	TList *lisDado;
	//TList *lisVetVal;
	double vBase;
	double zBase;
	complex<double> z0_ohm, z1_ohm, z0_pu, z1_pu;
	TListItem *item;
	AnsiString txt = "";
    barras_isoladas = 0;

	if(progresso == NULL)
	{
		progresso = DLL_NewObjProgresso(NULL, NULL);
    }
	if(progresso)
	{
		progresso->Start(progDEFAULT);
	}
	valores = new TStringList;
	lisBarras = redes->LisBarra();
	//prepara o listview para preenchimento
	lviewZent->Items->BeginUpdate();
	//percorre todas barras, dando o curto e guardando o Zentrada
	for (int nb = 0; nb < lisBarras->Count; nb++)
	{
		//limpa valores
		valores->Clear();
		//olha a barrra
		barra = (VTBarra*) lisBarras->Items[nb];
		//protecao
		if(barra == NULL)
			{continue;}
		//salva id da barra
		valores->Add(IntToStr(barra->Id));
		//salva codigo da barra
		valores->Add(barra->Codigo);
		//salva Sbase
		valores->Add("100");
		//salva Vbase
		vBase = barra->vnom;
		valores->Add(DoubleToStr("%3.1f",vBase));
		//seleciona o defeito
		SelecionaEqptoDefeito((VTEqpto*)barra);
		//executa o calculo
		ExecutaCalculo();
		//pega o rescc
		resCC = (VTResCC*)apl->GetObject(__classid(VTResCC));
		//pega os valores de zentrada
		grupo = resCC->IniciaGrupoZentrada();
		//le os valores de zentrada
		desc = grupo->Descricao;
		//pega os dados
		lisDado = grupo->LisDado;
		//lisVetVal = grupo->LisVetval;
		//percorre todos dados
		//devem ser 2
		if (lisDado->Count != 2)
			{continue;}
		dado = (VTDado*) lisDado->Items[0];
		//valores->Add(dado->valor);
		z0_ohm = StrToComplex(dado->valor);
		dado = (VTDado*) lisDado->Items[1];
		//valores->Add(dado->valor);
		z1_ohm = StrToComplex(dado->valor);
		//calcula valor em pu
		if (IsDoubleZero(vBase))
		{
			z0_pu = 0.;
			z1_pu = 0;
		}
		else
		{	//calcula z base
			zBase = (vBase*vBase)*0.01; //k^2/M = 1
			z0_pu = z0_ohm/zBase;
			z1_pu = z1_ohm/zBase;
        }
		//adicionaValores
		valores->Add(DoubleToStr("%4.3f",z0_ohm.real()));//R0
		valores->Add(DoubleToStr("%4.3f",z0_ohm.imag()));//X0
		valores->Add(DoubleToStr("%4.3f",z1_ohm.real()));//R1
		valores->Add(DoubleToStr("%4.3f",z1_ohm.imag()));//X1
		valores->Add(DoubleToStr("%4.3f",z0_pu.real()));//r0
		valores->Add(DoubleToStr("%4.3f",z0_pu.imag()));//x0
		valores->Add(DoubleToStr("%4.3f",z1_pu.real()));//r1
		valores->Add(DoubleToStr("%4.3f",z1_pu.imag()));//x1
		//cria o item
		item = lviewZent->Items->Add();
		item->Data = barra;
		//preenche com os valores
		for (int nv = 0; nv  < valores->Count; nv++)
		{
			if (nv == 0)
				{item->Caption = valores->Strings[nv];}
			else
				{item->SubItems->Add(valores->Strings[nv]);}
		}
	}
	if(progresso)
	{
		progresso->Stop();
	}
	if(barras_isoladas > 0)
	{
		if(barras_isoladas == 1){txt = " barra isolada.";}
		else{txt = " barras isoladas.";}
		Aviso("O c�lculo de curto-circuito n�o foi executado para " + IntToStr(barras_isoladas) + txt);
	}
	lviewZent->Items->EndUpdate();

}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::RadioGroupAlgoritmoClick(TObject *Sender)
   {
   //verifica se h� um defeito selecionado
   if (defeito.eqpto != NULL)
      {//recalculo o curto para o Eqpto selecionadao
      AtualizaCalculo();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::SalvaArqCfgCurto(void)
   {
   //vari�veis locais
   VTGeral    *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString arq_cfg_curto = "CfgCurto";

   //reinicia InfoSet
   geral->Infoset->Clear();
   //salva valores de Zdef
   geral->Infoset->AddInfo("Zdef3F_ff.r",  defeito.Zdef.Zdef3F_ff.r );
   geral->Infoset->AddInfo("Zdef3F_ff.x",  defeito.Zdef.Zdef3F_ff.x );
   geral->Infoset->AddInfo("Zdef2F_ff.r",  defeito.Zdef.Zdef2F_ff.r );
   geral->Infoset->AddInfo("Zdef2F_ff.x",  defeito.Zdef.Zdef2F_ff.x );
   geral->Infoset->AddInfo("Zdef2FT_ff.r", defeito.Zdef.Zdef2FT_ff.r);
   geral->Infoset->AddInfo("Zdef2FT_ff.x", defeito.Zdef.Zdef2FT_ff.x);
   geral->Infoset->AddInfo("Zdef2FT_ft.r", defeito.Zdef.Zdef2FT_ft.r);
   geral->Infoset->AddInfo("Zdef2FT_ft.x", defeito.Zdef.Zdef2FT_ft.x);
   geral->Infoset->AddInfo("Zdef1FT_ft.r", defeito.Zdef.Zdef1FT_ft.r);
   geral->Infoset->AddInfo("Zdef1FT_ft.x", defeito.Zdef.Zdef1FT_ft.x);
   //grava o arquivo
   geral->Infoset->SaveToFile(arq_cfg_curto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurto::SelecionaEqptoDefeito(VTEqpto *eqpto)
{
	//vari�veis locais
   VTTrecho *trecho;

   //salva eqpto de defeito
   if (eqpto == NULL) return;
   //verifica se
   if ((eqpto->Tipo() != eqptoBARRA)&&(eqpto->Tipo() != eqptoTRECHO))
	  {
	  Aviso("An�lise de curto-circuito.\nSelecione barra ou trecho de defeito.");
	  return;
	  }
   //verifica se � v�lido o c�lculo de curto p/ a rede selecionada
   if (! ValidaRedeParaCurto()) return;
   //salva Eqpto de defeito
   defeito.eqpto = eqpto;
   if (eqpto->Tipo() == eqptoTRECHO)
	  {//determina Trecho selecionado
	  trecho = (VTTrecho*)eqpto;
	  //define dist�ncia em rela��o � barra 1, em km
	  defeito.dist_bar1 = trecho->Comprimento_km / 2.;
	  }
}
//---------------------------------------------------------------------------
complex<double> __fastcall TFormCurto::StrToComplex(AnsiString dadoValor)
{
	double dreal, dimag;
	AnsiString real, imag;
	bool imagPositivo = false;
	int pos;

	//verifica se possui valor positivo ou negativo
	if ((pos = dadoValor.Pos("+")) >= 0)
		{imagPositivo = true;}
	else if ((pos = dadoValor.Pos("-")) >= 0)
		{imagPositivo = false;}
	//pega a parte real
//	real = dadoValor.SubString(0,8);
//	imag = dadoValor.SubString(11,18);
	real = dadoValor.SubString(0,pos-1);
	imag = dadoValor.SubString(pos+2,dadoValor.Length());
	//atribui valor
	try
	{
		dreal = StrToDouble(real);
		dimag = StrToDouble(imag);
	}
	catch (Exception &e)
	{
		dreal = 0.;
		dimag = 0.;
    }
	if(!imagPositivo)
	{
    	dimag = dimag*(-1.);
    }
	complex<double> z(dreal,dimag);
	return z;
}
//---------------------------------------------------------------------------
void __fastcall TFormCurto::UsuarioPrivilegiado(void)
   {
   //vari�veis locais
   VTSinapMdi *sinap_mdi = (VTSinapMdi*)apl->GetObject(__classid(VTSinapMdi));

   //verifica se usu�rio privilegiado
   if (sinap_mdi->UsuarioPrivilegiado())
      {
      CheckBoxRelatorio->Visible = true;
      }
   else
      {
      CheckBoxRelatorio->Visible = false;
      }

   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurto::ValidaRedeParaCurto(void)
   {
   //vari�veis locais
   bool       rede_valida = true;
   AnsiString txt;
   VTReducao  *reducao;
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   TList      *lisREDU   = consulta->LisReducaoCarregada(redes);
   TList      *lisEQP;

   try{//cria lista p/ uso local
      lisEQP = new TList();
      }catch(Exception &e)
         {
         return(false);
         }
   //loop p/ todas Reducoes
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //verifica se a Reducao possui Surpimento ou Trafo
      lisEQP->Clear();
      reducao->LisEqpto(lisEQP, eqptoSUPRIMENTO);
      reducao->LisEqpto(lisEQP, eqptoTRAFO);
      if (lisEQP->Count > 0)
         {
         rede_valida = false;
         txt += "\n" + reducao->MRede->Codigo ;
         }
      }
   //gera aviso p/ usu�rio
   if (! rede_valida) Aviso("As seguintes redes n�o podem ser utilizadas na sua forma equivalente:" + txt);
   //destr�i lista lisEQP
   delete lisEQP;

   return(rede_valida);
   }

//---------------------------------------------------------------------------
//eof


