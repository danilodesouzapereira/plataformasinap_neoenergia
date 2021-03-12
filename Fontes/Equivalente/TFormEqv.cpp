//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEqv.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Editor\VTEdt.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Redutor\VTRedutor.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Redutor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEqv::TFormEqv(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
                             :TFormEdt(Owner, parent)
   {
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   try{//cria objetos
      data         = DLL_NewObjData(apl);
      ordena       = DLL_NewObjOrdena(apl);
      lisBAR       = new TList();
      lisBAR_PRE   = new TList();
      lisBAR_SEL   = new TList();
      lisCHV       = new TList();
      lisEQP       = new TList();
      lisTMP       = new TList();
      //pré-seleciona Eqptos que irão permanecer da rede
      CLBoxOpcaoDefault();
      //inicia lista de Barras dos Eqptos selecionados
      IniciaLisBarraPre();
      //inicia ListBox de Barras pré-selecionadas
      IniciaLBox(LBoxBarraPre, lisBAR_PRE);
      //destaca Barras pré-selecionadas
      DestacaBarraNoGrafico();
      // Inicializa outras opções
      CheckBoxEqvEquilibrado->Checked = false;
      CheckBoxRedeUnica->Checked      = true;
      CheckBoxImpressao->Checked      = true;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormEqv::~TFormEqv(void)
   {
   //variáveia locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza gráfico
   grafico->Moldura();
   //destrói objetos
   if (edt)          {delete edt;          edt          = NULL;}
   if (data)         {delete data;         data         = NULL;}
   if (ordena)       {delete ordena;       ordena       = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisBAR)     {delete lisBAR;     lisBAR     = NULL;}
   if (lisBAR_PRE) {delete lisBAR_PRE; lisBAR_PRE = NULL;}
   if (lisBAR_SEL) {delete lisBAR_SEL; lisBAR_SEL = NULL;}
   if (lisEQP)     {delete lisEQP;     lisEQP     = NULL;}
   if (lisCHV)     {delete lisCHV;     lisCHV     = NULL;}
   if (lisTMP)     {delete lisTMP;     lisTMP     = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::ActionEdtExecute(TObject *Sender)
   {
   try{//se necessário, cria um objeto TEdtSpy
      if (edt != NULL) delete edt;
      //cria EdtMedidor p/ controle do mouse
      edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      //atualiza indicação de mouse ativo e força botão Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::ActionFechaExecute(TObject *Sender)
   {
   // ModalResult = mrCancel;
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::ActionOpcaoDefaultExecute(TObject *Sender)
   {
   //redefine seleção de Eqptos
   CLBoxOpcaoDefault();
   //atualiza seleção de Barras
   TrataAlteracaoEqptosSelecionados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::ActionOpcaoWiseExecute(TObject *Sender)
   {
   //redefine seleção de Eqptos
   CLBoxOpcaoWise();
   //atualiza seleção de Barras
   TrataAlteracaoEqptosSelecionados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::ActionRedeEqvExecute(TObject *Sender)
   {
   //variáveia locais
   bool      sucesso;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //proteção: verifica se há pelo menos uma Barra selecionada
   if ((lisBAR_PRE->Count == 0) && (lisBAR_SEL->Count == 0))
      {
      Aviso("Não há barras selecionadas para permanecer na rede equivalente");
      return;
      }
   //seleciona gráfico de topologia
   grafico->Show(grafTOPO);
   //monta Rede equivalente
   if (CheckBoxRedeUnica->Checked) sucesso = MontaRedeEquivalenteUnica();
   else                            sucesso = MontaRedeEquivalentePorRede();
   if (! sucesso)
      {
      Erro("Não foi possível criar a rede equivalente");
      return;
      }
   //atualiza gráfico
   grafico->AreaRedeAltera();
   grafico->Moldura();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::ActionSelEqptoExecute(TObject *Sender)
   {
   //variáveis locais
   int     index;
   VTEqpto *eqpto;
   TList   *lisEQP = (TList*)Sender;

   //proteção
   if (lisEQP == NULL) return;
   //loop p/ todos Eqptos selecionados em lisEQP
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (! eqpto->TipoBarra()) continue;
      //verifica se é uma Barra de um Eqpto selecionado
      if (lisBAR_PRE->IndexOf(eqpto) >= 0) continue;
      //verifica se é uma Barra selecionada
      if ((index = lisBAR_SEL->IndexOf(eqpto)) < 0)
         {//inclui Barra em lisBAR_SEL
         lisBAR_SEL->Add(eqpto);
         }
      else
         {//retira Barra de lisBAR_SEL
         lisBAR_SEL->Delete(index);
         }
      }
   //atualiza CLBoxBarraSel
   IniciaLBox(LBoxBarraSel, lisBAR_SEL);
   //atualiza gráfico
   DestacaBarraNoGrafico();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::CLBoxOpcaoClickCheck(TObject *Sender)
   {
   //atualiza seleção de Barras
   TrataAlteracaoEqptosSelecionados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::CLBoxOpcaoDefault()
   {
   //habilita opções
   CLBoxOpcao->Checked[ 0] = true;   //Barras c/ suprimento
   CLBoxOpcao->Checked[ 1] = true;   //Barras c/ gerador
   CLBoxOpcao->Checked[ 2] = true;   //Barras c/ carga
   CLBoxOpcao->Checked[ 3] = true;   //Barras c/ capacitor
   CLBoxOpcao->Checked[ 4] = false;  //Barras c/ ligações c/fases diferentes
   CLBoxOpcao->Checked[ 5] = false;  //Barras c/ bifurcações
   CLBoxOpcao->Checked[ 6] = false;  //Chaves tipo disjuntor
   CLBoxOpcao->Checked[ 7] = false;  //Chaves tipo religador
   CLBoxOpcao->Checked[ 8] = true;   //Chaves abertas
   CLBoxOpcao->Checked[ 9] = true;   //Chaves fechadas
   CLBoxOpcao->Checked[10] = false;  //Reguladores de tensão
   CLBoxOpcao->Checked[11] = false;  //Transformadores
   CLBoxOpcao->Checked[12] = false;  //Trechos que conectam chaves
   CLBoxOpcao->Checked[13] = true;   //Equipamentos com medição
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::CLBoxOpcaoWise(void)
   {
   //habilita opções
   CLBoxOpcao->Checked[ 0] = true;   //Barras c/ suprimento
   CLBoxOpcao->Checked[ 1] = true;   //Barras c/ gerador
   CLBoxOpcao->Checked[ 2] = true;   //Barras c/ carga
   CLBoxOpcao->Checked[ 3] = true;   //Barras c/ capacitor
   CLBoxOpcao->Checked[ 4] = false;  //Barras c/ ligações c/fases diferentes
   CLBoxOpcao->Checked[ 5] = false;  //Barras c/ bifurcações
   CLBoxOpcao->Checked[ 6] = false;  //Chaves tipo disjuntor
   CLBoxOpcao->Checked[ 7] = false;  //Chaves tipo religador
   CLBoxOpcao->Checked[ 8] = false;  //Chaves abertas
   CLBoxOpcao->Checked[ 9] = false;  //Chaves fechadas
   CLBoxOpcao->Checked[10] = false;  //Reguladores de tensão
   CLBoxOpcao->Checked[11] = false;  //Transformadores
   CLBoxOpcao->Checked[12] = false;  //Trechos que conectam chaves
   CLBoxOpcao->Checked[13] = true;   //Equipamentos com medição
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::DestacaBarraNoGrafico(void)
   {
   //variáveia locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //monta lista c/ todas Barras que devem permanecer na rede
   lisBAR->Clear();
   CopiaTList(lisBAR_PRE, lisBAR);
   CopiaTList(lisBAR_SEL, lisBAR);
   //atualiza gráfico
   grafico->Moldura(lisBAR);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEqv::ExistemLigacoesComFasesDistintas(TList *lisLIG, VTBarra *barra)
   {
   //variáveis locais
   int       fases_ref = faseINV;
   int       fases_lig;
   VTLigacao *ligacao;

   //proteção
   if (lisLIG->Count <= 1) return(false);
   //loop para todas Ligacoes em lisLIG
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao   = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoCHAVE) continue;
      if (fases_ref == faseINV)
         {
         fases_ref = ligacao->Fases(barra);
         }
      else
         {
         fases_lig = ligacao->Fases(barra);
         if ((fases_ref | fases_lig) != fases_ref) return(true);
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::FormShow(TObject *Sender)
   {
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   //inicia objeto EdtMedidor
   ActionEdtExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::IniciaLBox(TListBox *LBox, TList *lisEXT)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //limpa CheckListBox
   LBox->Clear();
   //insere Eqptos em CheckListBox
   for (int n = 0; n < lisEXT->Count; n++)
      {
      eqpto = (VTEqpto*)lisEXT->Items[n];
      LBox->Items->AddObject(eqpto->Codigo, eqpto);
      }
   //pré-seleciona primeiro Eqpto
   if (LBox->Items->Count > 0) LBox->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::IniciaLisBarraPre(void)
   {
   // 0: Barras c/ suprimento
   // 1: Barras c/ gerador
   // 2: Barras c/ carga
   // 3: Barras c/ capacitor
   // 4: Barras c/ ligações c/fases diferentes
   // 5: Barras c/ bifurcações
   // 6: Chaves tipo disjuntor
   // 7: Chaves tipo religador
   // 8: Chaves abertas
   // 9: Chaves fechadas
   //10: Reguladores de tensão
   //11: Transformadores
   //12: Trechos que conectam chaves
   //13: Equipamentos com medição

   //reinicia lisBAR_PRE com Barras dos Eqptos selecionados
   lisBAR_PRE->Clear();
   //inicia lisCHV com todas as Chaves selecionadas
   lisCHV->Clear();
   if (CLBoxOpcao->Checked[ 6]) SelecionaChavePorTipo(chaveDJ);
   if (CLBoxOpcao->Checked[ 7]) SelecionaChavePorTipo(chaveRA);
   if (CLBoxOpcao->Checked[ 8]) SelecionaChavePorEstado(chvABERTA);
   if (CLBoxOpcao->Checked[ 9]) SelecionaChavePorEstado(chvFECHADA);
   //inclui Barras das Chaves
   SelecionaBarraChave();
   //verifica demais Eqptos selecionados
   if (CLBoxOpcao->Checked[ 0]) SelecionaBarraEqbar(eqptoSUPRIMENTO);
   if (CLBoxOpcao->Checked[ 1]) SelecionaBarraEqbar(eqptoGERADOR);
   if (CLBoxOpcao->Checked[ 2]) SelecionaBarraEqbar(eqptoCARGA);
   if (CLBoxOpcao->Checked[ 3]) SelecionaBarraEqbar(eqptoCAPACITOR);
   if (CLBoxOpcao->Checked[ 4]) SelecionaBarraLigacoesComFasesDistintas();
   if (CLBoxOpcao->Checked[ 5]) SelecionaBarraComBifurcacao();
   //if (CLBoxOpcao->Checked[ 6]) SelecionaChavePorTipo(chaveDJ);
   //if (CLBoxOpcao->Checked[ 7]) SelecionaChavePorTipo(chaveRA);
   //if (CLBoxOpcao->Checked[ 8]) SelecionaChavePorEstado(chvABERTA);
   //if (CLBoxOpcao->Checked[ 9]) SelecionaChavePorEstado(chvFECHADA);
   if (CLBoxOpcao->Checked[10]) SelecionaBarraLigacao(eqptoREGULADOR);
   if (CLBoxOpcao->Checked[11]) SelecionaBarraLigacao(eqptoTRAFO);
   if (CLBoxOpcao->Checked[12]) SelecionaBarraTrechoConectaChave();
   if (CLBoxOpcao->Checked[13]) SelecionaBarraEqptoComMedicao();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEqv::MontaRedeEquivalentePorRede(void)
   {
   //variáveia locais
   bool       calcular_fluxo;
   TList     *lisBAR_REDE;
   VTBarra   *barra;
   VTRede    *rede;
   VTRedutor *redutor;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //ATENÇÃO: não utilizar lisREDE de VTRedes porque as Redes podem ser reposicionadas na lista
   lisTMP->Clear();
   redes->LisRede(lisTMP);
   //loop p/ todas Redes
   for (int n = 0; n < lisTMP->Count; n++)
      {
      rede        = (VTRede*)lisTMP->Items[n];
      lisBAR_REDE = rede->LisBarra();
      //inicia lisBAR com as Barras da Rede que devem ser mantidas na rede
      lisBAR->Clear();
      for (int nb = 0; nb < lisBAR_REDE->Count; nb++)
         {
         barra = (VTBarra*)lisBAR_REDE->Items[nb];
         //verifica se a Barra está em lisBAR_PRE ou lisBAR_SEL
         if ((lisBAR_PRE->IndexOf(barra) >= 0) ||(lisBAR_SEL->IndexOf(barra) >= 0))
            {//insere Barra em lisBAR
            lisBAR->Add(barra);
            }
         }
      //verifica se há Barras da rede a serem mantidas
      if (lisBAR->Count == 0) continue;
      try{//cria Redutor e monta equivalente
         redutor = DLL_NewObjRedutor(apl);
         // Analisa opção de impressão
         if(CheckBoxImpressao->Checked) redutor->AvisoEnabled = true;
         else                           redutor->AvisoEnabled = false;
         redutor->ModeloEqv = (CheckBoxEqvEquilibrado->Checked) ? eqvEQUILIBRADO : eqvDESEQUILIBRADO;
         //executa o Redutor, calculando o fluxo somente para a primeira rede
         calcular_fluxo = (n == 0);
         redutor->ExecutaReducaoUmaRede(rede, lisBAR, calcular_fluxo);
         //destrói Redutor
         delete redutor;
         }catch(Exception &e)
            {
            return(false);
            }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEqv::MontaRedeEquivalenteUnica(void)
   {
   //variáveia locais
   bool      calcular_fluxo = true;
   VTRedutor *redutor;

   //monta lista c/ todas Barras que devem permanecer na rede
   lisBAR->Clear();
   CopiaTList(lisBAR_PRE, lisBAR);
   CopiaTList(lisBAR_SEL, lisBAR);
   try{//cria Redutor e monta equivalente
      redutor = DLL_NewObjRedutor(apl);
      // Analisa opção de impressão
      if(CheckBoxImpressao->Checked) redutor->AvisoEnabled = true;
      else                           redutor->AvisoEnabled = false;
      redutor->ModeloEqv = (CheckBoxEqvEquilibrado->Checked) ? eqvEQUILIBRADO : eqvDESEQUILIBRADO;
      redutor->ExecutaReducaoAllRede(lisBAR, calcular_fluxo);
      //destrói Redutor
      delete redutor;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraChave(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;

   //loop p/ todas Chaves em lisCHV
   for (int n = 0; n < lisCHV->Count; n++)
      {
      chave = (VTChave*)lisCHV->Items[n];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = chave->Barra(ind_bar)) != NULL)
            {//insere Barra do Eqbar em lisBAR_PRE
            if (lisBAR_PRE->IndexOf(barra) < 0) lisBAR_PRE->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraComBifurcacao(void)
   {
   //variáveis locais
   TList     *lisBAR;
   TList     *lisLIG;
   TList     *lisREDE;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia campo Tag de todas as Barras com valor 0
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->Tag = 0;
      }
   //loop para todas Redes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //loop p/ todas Ligacoes da Rede
      lisLIG = rede->LisLigacao();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //loop p/ todas Barras da ligacao
         for (int ind_bar=0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            //incrementa valor de Tag da Barra
            barra->Tag++;
            }
         }
      }
   //seleciona todas as Barras com Tag maior que 2
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->Tag > 2)
         {
         if (lisBAR_PRE->IndexOf(barra) < 0)
            {//insere Barra do Eqbar em lisBAR_PRE
            lisBAR_PRE->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraEqbar(int tipo_eqbar)
   {
   //variáveis locais
   VTEqbar *eqbar;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Eqbar da rede
   lisTMP->Clear();
   redes->LisEqbar(lisTMP, tipo_eqbar);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqbar = (VTEqbar*)lisTMP->Items[n];
      if (lisBAR_PRE->IndexOf(eqbar->pbarra) < 0)
         {//insere Barra do Eqbar em lisBAR_PRE
         lisBAR_PRE->Add(eqbar->pbarra);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraEqptoComMedicao(void)
   {
   //variáveis locais
   TList     *lisCANAL;
   VTBarra   *barra;
   VTCanal   *canal;
   VTEqpto   *eqpto;
   VTLigacao *ligacao;
   VTMedidor *medidor;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Geradores e Suprimentos da rede
   lisTMP->Clear();
   redes->LisEqbar(lisTMP, eqptoMEDIDOR);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      medidor = (VTMedidor*)lisTMP->Items[n];
      //inclui Barra do Medidor
      if (lisBAR_PRE->IndexOf(medidor->pbarra) < 0)  lisBAR_PRE->Add(medidor->pbarra);
      //obtém lista de Eqptos associados aos Canais
      lisEQP->Clear();
      medidor->LisEqptoDosCanais(lisEQP);
      //loop p/ todos os Eqptos associados aos Canais
      for (int i = 0; i < lisEQP->Count; i++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[i];
         if (eqpto->TipoBarra())
            {//inclui Barra
            barra = (VTBarra*)eqpto;
            if (lisBAR_PRE->IndexOf(barra) < 0) lisBAR_PRE->Add(barra);
            }
         else if (eqpto->TipoEqbar())
            {//inclui Barra do Eqbar
            barra = ((VTEqbar*)eqpto)->pbarra;
            if (lisBAR_PRE->IndexOf(barra) < 0) lisBAR_PRE->Add(barra);
            }
         else if (eqpto->TipoLigacao())
            {//inclui Barras da Ligacao
            ligacao = (VTLigacao*)eqpto;
            for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
               {
               if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
               if (lisBAR_PRE->IndexOf(barra) < 0) lisBAR_PRE->Add(barra);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraLigacao(int tipo_ligacao)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Ligacoes
   lisTMP->Clear();
   redes->LisLigacao(lisTMP, tipo_ligacao);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      ligacao = (VTLigacao*)lisTMP->Items[n];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         if (lisBAR_PRE->IndexOf(barra) < 0)
            {//insere Barra do Eqbar em lisBAR_PRE
            lisBAR_PRE->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraLigacoesComFasesDistintas(void)
   {
   //variáveis locais
   int       fases_ref, fases_lig;
   TList     *lisLIG_NET;
   TList     *lisNO_NET;
   VTBarra   *barra;
   VTNet     *net;
   VTNo      *no;
   VTLigacao *ligacao;
   VTOrdena  *ordena;
   VTRedes   *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //cria objeto VTOrdena
   ordena  = DLL_NewObjOrdena(apl);
   //cria objeto Net, sem Tramos
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = false;
   ordena->TramoEnabled    = false;
   if ((net = ordena->Executa(redes)) == NULL)
      {
      delete ordena;
      return;
      }
   //obtém lista de Nos
   lisNO_NET  = net->LisNo();
   lisLIG_NET = net->LisLigacao();
   //loop p/ todos Nos
   for (int n = 0; n < lisNO_NET->Count; n++)
      {
      no = (VTNo*)lisNO_NET->Items[n];
      //inicia fases de referência e fases de uma ligacao
      fases_ref = fases_lig = faseINV;
      //obtém lista de Ligacoes que conectam as Barras do No
      for (int nb = 0; (fases_ref == fases_lig) && (nb < no->lisBAR->Count); nb++)
         {
         barra = (VTBarra*)no->lisBAR->Items[nb];
         //analisa fases das Ligacoes que conectam o No
         for (int nl = 0; (fases_ref == fases_lig) && (nl < lisLIG_NET->Count); nl++)
            {
            ligacao = (VTLigacao*)lisLIG_NET->Items[nl];
            if (ligacao->ConectaBarra(barra))
               {
               if (fases_ref == faseINV)
                  {
                  fases_ref = fases_lig = ligacao->Fases(barra);
                  }
               else
                  {
                  fases_lig = ligacao->Fases(barra);
                  }
               }
            }

         }
      //verifica se há diferenças entre as fases das ligacações
      if ((fases_ref != fases_lig))
         {//insere Barras do No em lisBAR_PRE
         for (int nb = 0; nb < no->lisBAR->Count; nb++)
            {
            barra = (VTBarra*)no->lisBAR->Items[nb];
            if (lisBAR_PRE->IndexOf(barra) < 0) lisBAR_PRE->Add(barra);
            }
         }
      }
   //destrói objetos criados
   delete net;
   delete ordena;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaBarraTrechoConectaChave(void)
   {
   //variáveis locais
   bool      incluir;
   TList     *lisBAR;
   TList     *lisLIG;
   TList     *lisREDE;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia campo Tag de todas as Barras com valor 0
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->Tag = 0;
      }
   //loop para todas Chaves em lisCHV
   for (int nl = 0; nl < lisCHV->Count; nl++)
      {
      ligacao = (VTLigacao*)lisCHV->Items[nl];
      //incrementa valor de Tag das Barras da Chave
      ligacao->pbarra1->Tag++;
      ligacao->pbarra2->Tag++;
      }
   //loop para todas Redes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //loop p/ todas Ligacoes da Rede
      lisLIG = rede->LisLigacao();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //verifica se a Ligacao conecta alguma Barra com Tag não nulo
         incluir = false;
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            //verifica Tag da Barra
            if (barra->Tag > 0)
               {
               incluir = true;
               break;
               }
            }
         if (! incluir) continue;
         //inclui Barras da Ligacao em lisBAR_PRE
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            if (lisBAR_PRE->IndexOf(barra) < 0)
               {//insere Barra do Eqbar em lisBAR_PRE
               lisBAR_PRE->Add(barra);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaChavePorEstado(int estado)
   {
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Geradores e Suprimentos da rede
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCHAVE);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      chave = (VTChave*)lisEQP->Items[n];
      if (chave->Estado == unsigned(estado))
         {//insere Chave em lisCHV
         if (lisCHV->IndexOf(chave) < 0) lisCHV->Add(chave);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::SelecionaChavePorTipo(int tipo)
   {
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Geradores e Suprimentos da rede
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCHAVE);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      chave = (VTChave*)lisEQP->Items[n];
      if (chave->TipoChave->Tipo == tipo)
         {//insere Chave em lisCHV
         if (lisCHV->IndexOf(chave) < 0) lisCHV->Add(chave);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqv::TrataAlteracaoEqptosSelecionados(void)
   {
   //reinicia lista de barras selecionadas
   IniciaLisBarraPre();
   //atualiza LBoxBarraPre com as barras pré-selecionadas
   IniciaLBox(LBoxBarraPre, lisBAR_PRE);
   //destaca Barras pré-selecionadas
   DestacaBarraNoGrafico();
   }

//---------------------------------------------------------------------------
//eof



