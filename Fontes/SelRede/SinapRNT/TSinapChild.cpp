//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSinapChild.h"
//#include "TFormStk.h"
#include "..\Apl\VTApl.h"
//#include "..\Arranjo\VTArranjo.h"
//#include "..\Arranjo\VTCabo.h"
//#include "..\Arranjo\VTPtoFixacao.h"
//#include "..\Arranjo\VTSuporte.h"
//#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Topologia\VTTopologia.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSinapChild* __fastcall NewObjSinapChild(VTApl *apl_owner)
   {
   try{
		return (new TSinapChild(apl_owner));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TSinapChild::TSinapChild(VTApl *apl_owner)
	{
	//salva pontero par objetp
	this->apl = apl_owner;
   //cria listas locais
   lisEQP = new TList();
	}

//---------------------------------------------------------------------------
__fastcall TSinapChild::~TSinapChild(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::DefineArranjos(VTApl *apl_ext)
	{ /*
	//variáveis locais
	VTEdita  *edita;
	struct   {
				VTArranjos  *arranjos;
				VTArranjo   *arranjo;
				VTCabo      *cabo;
				VTSuporte   *suporte;
				TList       *lisARR, *lisCAB, *lisSUP;
				}ref, ext;

	//cancela Undo/Redo do objeto Edita
	edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	edita->Clear();
	//determina objeto Arranjos local e suas listas de Arranjo, Cabo e Suporte
	ref.arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   ref.lisARR   = ref.arranjos->LisArranjo();
   ref.lisCAB   = ref.arranjos->LisCabo();
   ref.lisSUP   = ref.arranjos->LisSuporte();
	//determina objeto Arranjos externo e suas listas de Arranjo, Cabo e Suporte
   ext.arranjos = (VTArranjos*)apl_ext->GetObject(__classid(VTArranjos));
   ext.lisARR   = ext.arranjos->LisArranjo();
   ext.lisCAB   = ext.arranjos->LisCabo();
   ext.lisSUP   = ext.arranjos->LisSuporte();
	//atualiza objetos locais a partir dos objetos externos
   for (int n = 0; n < ext.lisCAB->Count; n++)
      {//determina Cabo externo
      ext.cabo = (VTCabo*)ext.lisCAB->Items[n];
      //verifica se há Cabo local
      if ((ref.cabo = ref.arranjos->ExisteCabo(ext.cabo->Id)) == NULL)
			{//não existe Cabo local correspondente: cria um novo
			ref.cabo = ext.cabo->Clone();
			ref.cabo->Status[sttNOVO] = true;
			//insere novo Cabo em Arranjos local
         ref.arranjos->InsereCabo(ref.cabo);
         }
      else
			{//copia dados do Cabo externo
         ref.cabo->CopiaAtributosDe(*ext.cabo);
			ref.cabo->Status[sttALTERADO] = true;
			}
      }
   //atualiza objetos locais a partir dos objetos externos
   for (int n = 0; n < ext.lisSUP->Count; n++)
		{//determina Cabo externo
		ext.suporte = (VTSuporte*)ext.lisSUP->Items[n];
		//verifica se há Suporte local
		if ((ref.suporte = ref.arranjos->ExisteSuporte(ext.suporte->Id)) == NULL)
			{//não existe Suporte local correspondente: cria um novo
			ref.suporte = ext.suporte->Clone();
			ref.suporte->Status[sttNOVO] = true;
			//insere novo Suporte em Arranjos local
			ref.arranjos->InsereSuporte(ref.suporte);
			}
		else
			{//copia dados do Suporte externo
			ref.suporte->CopiaAtributosDe(*ext.suporte);
			ref.suporte->Status[sttALTERADO] = true;
			}
      }
   //atualiza objetos locais a partir dos objetos externos
   for (int n = 0; n < ext.lisARR->Count; n++)
		{//determina Arranjo externo
		ext.arranjo = (VTArranjo*)ext.lisARR->Items[n];
      //verifica se existe Arranjo local de mesmo Id
      if ((ref.arranjo = ref.arranjos->ExisteArranjo(ext.arranjo->Id)) == NULL)
         {//não existe Arranjo local correspondente: cria um novo
			ref.arranjo = ext.arranjo->Clone();
			ref.arranjo->Status[sttNOVO] = true;
			//insere novo Arranjo em Arranjos local
         ref.arranjos->InsereArranjo(ref.arranjo);
			}
      else
         {//copia dados do Arranjo externo
			ref.arranjo->CopiaAtributosDe(*ext.arranjo);
			ref.arranjo->Status[sttALTERADO] = true;
			}
		//redefine Suporte associado ao Arranjo
      if (ext.arranjo->suporte != NULL)
         {//redefine objeto Suporte
			ref.arranjo->suporte = ref.arranjos->ExisteSuporte(ext.arranjo->suporte->Id);
         }
      //redefine CaboZZ associado ao Arranjo
      if (ext.arranjo->caboZ0Z1 != NULL)
			{//redefine objeto CaboZZ
         ref.arranjo->caboZ0Z1 = ref.arranjos->ExisteCabo(ext.arranjo->caboZ0Z1->Id);
         }
      //redefine PtoFixacao e Cabo de cada fase
      if ((ext.arranjo->suporte != NULL)&&(ref.arranjo->suporte != NULL))
         {
         for (int nf = 0; nf < MAX_FASE; nf++)
				{
            if ((ext.arranjo->fase[nf].pto_fixacao != NULL)&&(ext.arranjo->fase[nf].cabo != NULL))
               {
               ref.arranjo->fase[nf].pto_fixacao = ref.arranjo->suporte->ExistePtoFixacao(ext.arranjo->fase[nf].pto_fixacao->id);
               ref.arranjo->fase[nf].cabo        = ref.arranjos->ExisteCabo(ext.arranjo->fase[nf].cabo->Id);
               }
            }
			}
		}  */
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::DefinePatamares(VTApl *apl_ext)
	{  /*
   //variáveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	struct   {
				VTPatamares *patamares;
            }ref, ext;

   //seleciona gráfico de topologia
	ActionTopo->Execute();
   //marca que o Flow não está atualizado
   flow->UpToDate = false;
   //determina objetos Patamares
   ref.patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	ext.patamares = (VTPatamares*)apl_ext->GetObject(__classid(VTPatamares));
   //proteção
   if (ref.patamares == NULL) return;
   if (ext.patamares == NULL) return;
	//redefine Escala de Patamares do objeto Apl local
	ref.patamares->CopiaAtributosDe(*ext.patamares);
	//redefine objetos afetados pela alteração de número de patamares
	TrataAlteracaoPatamares(ref.patamares->NumPatamar());  */
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::DefinePatamares(int escala)
   {
	//variáveis locais
   VTFlow      *flow      = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRedes 		*redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//DVK 2015.04.13 verifica se há redes abertas
	if(redes->LisRede()->Count > 0)
		{//seleciona gráfico de topologia
		ActionTopo->Execute();
		//marca que o Flow não está atualizado
		flow->UpToDate = false;
      }
   //altera escala de Patamares
   patamares->Escala = escala;
   //redefine objetos afetados pela alteração de número de patamares
   TrataAlteracaoPatamares(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkAdd(AnsiString caption, TActionList *ActionList, bool expand)
	{
//   insere ActionList em FormStk
//	if (FormStk != NULL) FormStk->TreeViewAdd(caption, ActionList, expand);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkAdd(AnsiString caption, TList *LisActionList, bool expand)
   {
   //insere ActionList em FormStk
//   if (FormStk != NULL) FormStk->TreeViewAdd(caption, LisActionList, expand);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkAdd(TWinControl *WinControl, TAlign align)
   {
//   WinControl->Parent = FormStk;
//   WinControl->Align  = align;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkClose(void)
	{
   //retira Form do seu Parent
//   FormStk->Parent = NULL;
	//fecha ForrmStk
//   FormStk->Close();
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkHide(void)
   {
//   if (FormStk) FormStk->Hide();
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkRemove(TActionList *ActionList)
   {
   //retira ActionList em FormStk
//   if (FormStk != NULL) FormStk->TreeViewRemove(ActionList);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::FormStkShow(AnsiString caption)
	{ /*
   //variáveis locais
	VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

   //se necessário, cria FormStk
	if (FormStk == NULL) FormStk = new TFormStk(apl->Form, apl, MultiPanel->Panel1);
   //atualiza Caption
   if (! caption.IsEmpty()) FormStk->Caption = caption;
   //exibe TFormStk
	if (! FormStk->Visible) FormStk->Show();  */
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::LeBaseCurvaTipica(void)
	{ /*
	//variáveis locais
	VTBD     *bd     = (VTBD*)apl->GetObject(__classid(VTBD));
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	try{//reinicia objeto Curvas
		curvas->Clear();
		//lê dados da base
		if (! bd->Conectado) bd->Conecta();
		bd->Geral->LeTipoConsumidor();
		bd->Geral->LeCurvaTipica();
		}catch(Exception &e)
			{//nada a fazer
			}    */
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::PanelToolBarGraficoAdd(TToolBar *ToolBar)
	{
//	ToolBar->Parent = PanelToolBarGrafico;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapChild::TrataAlteracaoPatamares(int num_pat)
	{
   //variáveis locais
	VTEdita 		*edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //cancela Undo/Redo do objeto Edita
   edita->Clear();
   //redefine Eqptos das Redes
   TrataAlteracaoPatamaresRede(num_pat);
   //redefine Eqptos das Reducoes
   TrataAlteracaoPatamaresReducao(num_pat);
   //redefine Eqptos do Estudo
	TrataAlteracaoPatamaresEstudo(num_pat);
	//redefine Eqptos das reduções das MNets   //DVK 2015.04.14
	topologia->Reconfigura(redes);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::TrataAlteracaoPatamaresEqpto(VTEqpto *eqpto, int num_pat)
   {
   //variáveis locais
   VTBarra        *barra;
   VTGerador      *gerador;
   VTLigacao      *ligacao;
   VTRede         *rede;
   VTSuprimento   *suprimento;

   //proteção
   if (eqpto == NULL) return;
   //verifica tipo Eqpto
   if (eqpto->TipoBarra())
      {
      barra = (VTBarra*)eqpto;
      barra->resflow->Max_pat = num_pat;
      }
   else if (eqpto->TipoLigacao())
      {
      ligacao = (VTLigacao*)eqpto;
      ligacao->resflow->Max_pat = num_pat;
      }
   else if (eqpto->Tipo() == eqptoREDE)
      {
      rede = (VTRede*)eqpto;
      rede->diag.flow.Length = num_pat;
      }
   else if (eqpto->Tipo() == eqptoGERADOR)
      {
      gerador = (VTGerador*)eqpto;
      gerador->resflow->Max_pat = num_pat;
      }
   else if (eqpto->Tipo() == eqptoSUPRIMENTO)
      {
      suprimento = (VTSuprimento*)eqpto;
      suprimento->resflow->Max_pat = num_pat;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::TrataAlteracaoPatamaresEstudo(int num_pat)
   {
   //variáveis locais
   TList          *lisALT;
   TList          *lisOBRA;
   TList          *lisACAO;
   VTAcao         *acao;
   VTAlternativa  *alternativa;
   VTObra         *obra;
   VTObras        *obras;
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo       *estudo       = planejamento->ExisteEstudo();

   //proteção
   if ((estudo == NULL)||(estudo->Id == estudoID_ZERO)) return;
   //loop p/ todas Alternativas
   lisALT = estudo->LisAlternativa();
   for (int i = 0; i < lisALT->Count; i++)
      {
      alternativa = (VTAlternativa*)lisALT->Items[i];
      //obtém lista de Obras da Alternativa
      lisOBRA = alternativa->Obras->LisObra();
      //loop p/ todas Obras
      for (int j = 0; j < lisOBRA->Count; j++)
         {
         obra = (VTObra*)lisOBRA->Items[j];
         //loop p/ todos objetos Acao
         lisACAO = obra->LisAcao;
         for (int k = 0; k < lisACAO->Count; k++)
            {
            acao = (VTAcao*)lisACAO->Items[k];
            //redefine Eqptos da Acao
            TrataAlteracaoPatamaresEqpto(acao->Eqpto(),                num_pat);
            TrataAlteracaoPatamaresEqpto(acao->CopiaDoEqptoOriginal(), num_pat);
            TrataAlteracaoPatamaresEqpto(acao->CopiaDoEqptoAlterado(), num_pat);
            }
         }
		}
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::TrataAlteracaoPatamaresRede(int num_pat)
   {
   //variáveis locais
   VTSuprimento *suprimento;
   VTRedes      *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisREDE = redes->LisRede();

   //atualiza todos objetos Rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      TrataAlteracaoPatamaresEqpto((VTEqpto*)lisREDE->Items[n], num_pat);
      }
   //atualiza todos objetos Barra
	lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoBARRA);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEQP->Items[n], num_pat);
      }
   //atualiza todos objetos Ligacao
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoLIGACAO);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEQP->Items[n], num_pat);
      }
   //atualiza todos objetos Gerador
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoGERADOR);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEQP->Items[n], num_pat);
      }
   //atualiza todos objetos Suprimento
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoSUPRIMENTO);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEQP->Items[n], num_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSinapChild::TrataAlteracaoPatamaresReducao(int num_pat)
   {
   //variáveis locais
   TList     *lisEXT;
   VTReducao *reducao;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //atualiza objetos das Reducoes
   lisEQP->Clear();
   redes->LisReducao(lisEQP);
   for (int nr = 0; nr < lisEQP->Count; nr++)
      {
      reducao = (VTReducao*)lisEQP->Items[nr];
      //atualiza Barras fictícias do esquemático da Reducao
      lisEXT = reducao->LisBarraEsquematico();
      for (int n = 0; n < lisEXT->Count; n++)
         {
         TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEXT->Items[n], num_pat);
         }
      //atualiza Ligacoes do esquemático da Reducao
      lisEXT = reducao->LisLigacaoEsquematico();
      for (int n = 0; n < lisEXT->Count; n++)
         {
         TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEXT->Items[n], num_pat);
         }
      //atualiza Ligacoes
      lisEXT = reducao->LisLigacao();
      for (int n = 0; n < lisEXT->Count; n++)
         {
         TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEXT->Items[n], num_pat);
         }
      //atualiza Suprimentos
      lisEXT = reducao->LisEqbar();
      for (int n = 0; n < lisEXT->Count; n++)
         {
         TrataAlteracaoPatamaresEqpto((VTEqpto*)lisEXT->Items[n], num_pat);
         }
      }
   }

//---------------------------------------------------------------------------
//eof
