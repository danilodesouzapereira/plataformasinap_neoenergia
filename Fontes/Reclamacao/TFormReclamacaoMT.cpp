//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormReclamacaoMT.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Editor\VTEdt.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Ordena\VTOrdena.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTReducao.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTMRede.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Zona\VTZona.h>
#include <Fontes\Zona\VTZonas.h>
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\Editor.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Ordena.h>
#include <DLL_Inc\Estudo.h>
#include <DLL_Inc\Zona.h>
//---------------------------------------------------------------------------
#include "TFormAnaliseRT.h"
#include "TFormEditaObra.h"
#include "TFormTUSD.h"
#include "..\ImportaMedicao\VTExtrator.h"
#include "..\ImportaMedicao\TFormSelMed.h" //novo FKM 25.03.2015
#include "..\Reclamacao\VTAjusta.h"
#include "..\Reclamacao\VTMedicao.h"
#include "..\Reclamacao\VTMulta.h"
#include "..\Reclamacao\VTPerfilBarra.h"
#include "..\Reclamacao\VTPerfilRede.h"
#include "..\Reclamacao\VTPerfilRegulador.h"
#include "..\Reclamacao\VTRes505.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
TFormReclamacaoMT *FormReclamacaoMT;
//---------------------------------------------------------------------------
__fastcall TFormReclamacaoMT::TFormReclamacaoMT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(Owner, parent)
   {
   //salva ponteiro p/ objeto
//   this->apl = apl_owner;
   //DVK 2015.03.23 cria apl pr�prio
   this->apl = NewObjApl(this, apl_owner);
   //insere Form em seu Parent
   if (parent)
      {
      Parent = parent;
      Align  = alLeft;
      }
   //adiciona objetos do m�dulo
   apl->Add(DLL_NewObjZonas(apl));
   apl->Add(DLL_NewObjOrdena(apl));
   apl->Add(medicao = NewObjMedicao(apl));
   apl->Add(multa   = NewObjMulta(apl));
   apl->Add(res505  = NewObjRes505(apl));
   apl->Add(ajusta  = NewObjAjusta(apl));
   //cria listas
   lisCARGA    = new TList();
   lisEQP       = new TList();
   //inicia par�metros
   inicializado = Inicia();  //DVK 2015.03.23
   //cria TFormEstudo //DVK 2015.03.23 no apl_owner
   FormEstudo    = DLL_NewFormEstudo(this, apl_owner, SelecionaParentParaJanela());
//   //inicia par�metros
//   Inicia();
//   //cria TFormEstudo
//   FormEstudo    = DLL_NewFormEstudo(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
__fastcall TFormReclamacaoMT::~TFormReclamacaoMT(void)
   {
   //destroi lisEQP sem destruir seus objetos
   if(lisCARGA)    {delete lisCARGA; lisCARGA = NULL;}
   if(lisEQP)       {delete lisEQP;    lisEQP = NULL;}
   //fecha FormEstudo  //DVK 2015.03.23
   if(FormEstudo) FormEstudo->Close();
   //destroi apl       //DVK 2015.03.23
   if(apl)          {delete apl;       apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ActionAnaliseExecute(TObject *Sender)
   {
   //variavel local
   TFormAnaliseRT *formAnaliseRT;

   //guarda valores
   multa->Carga = carga_analisada;
   multa->RedeSec = rede_pri;
   multa->Suprimento = supr;
//   multa->ValorTUSD = StrToDouble(RichEditTUSD->Lines->Text);
   //cria TFormAnaliseRT
   formAnaliseRT  = new TFormAnaliseRT(this, apl);
   if(formAnaliseRT)
      {//mostra
      formAnaliseRT->ShowModal();
      //destroi form
      delete formAnaliseRT;
      formAnaliseRT = NULL;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ActionEditaObraExecute(TObject *Sender)
   {
   //vari�veis locais
   TForm *form;

   try{//cria TFormEditaObra e exibe como janela modal
      form = new TFormEditaObra(this, apl, (VTObra*)Sender);
      form->ShowModal();
      //destr�i form
      delete form;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ActionEdtExecute(TObject *Sender)
   {
   try{//se necess�rio, cria um objeto VTEdt
      if (edt != NULL) delete edt;
      //cria VTEdt p/ controle do mouse
      edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      //atualiza indica��o de mouse ativo e for�a bot�o Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ActionSelEqptoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto   *eqpto;
   TList     *lisTMP     = (TList*)Sender;

   try{//determina Eqpto selecionado
      if (Sender == NULL) return;
      //loop p/ todos Eqptos recebidos em lisTMP (Sender)
      for (int n = 0; n < lisTMP->Count; n++)
         {
         if ((eqpto = (VTEqpto*)lisTMP->Items[n]) == NULL) continue;
         if (eqpto->Tipo() == eqptoCARGA) break;
         eqpto = NULL;
         }
      //verifica se o eqpto � uma Carga
      if (eqpto == NULL)
         {
         Aviso("Selecione uma carga para an�lise.");
         return;
         }
      //seleciona carga no CBox
      CBoxConsumidores->ItemIndex = CBoxConsumidores->Items->IndexOf(eqpto->Codigo);
      CBoxConsumidoresChange(Sender);
      }catch(Exception &e)
         {
         //nada
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ActionTUSDExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormTUSD *form;
   int       modal_result;

   form = new TFormTUSD(this, apl);
   if(form)
      {
      modal_result = form->ShowModal();
//      if(modal_result == mrOk) SalvaTUSD();
      delete form;
      }
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TFormReclamacaoMT::BarraInicialRedePri(void)
   {
   //vari�veis locais
   VTSuprimento *suprimento;
   VTTrafo      *trafo;

   //prote��o: verifica se existe a rede prim�ria
   if (rede_pri == NULL) return(NULL);
   //determina trafo da rede prim�ria
   lisEQP->Clear();
   rede_pri->LisLigacao(lisEQP, eqptoTRAFO);
   if (lisEQP->Count > 0)
      {
      trafo = (VTTrafo*)lisEQP->First();
      return(trafo->BarraSecundario());
      }
   //rede prim�ria sem trafo: verifica se tem suprimento
   lisEQP->Clear();
   rede_pri->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   if (lisEQP->Count > 0)
      {
      suprimento = (VTSuprimento*)lisEQP->First();
      return(suprimento->pbarra);
      }
   //retorna Barra inicial
   return(rede_pri->BarraInicial());
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ButConsMedClick(TObject *Sender)
   {
   //valida consumidor medido
   if(EditConsMed->Text.IsEmpty())
      {
      Aviso("N�o h� um consumidor medido");
      return;
      }
   //analisa consumidor da medi��o
   CBoxConsumidores->ItemIndex = 0;
   CBoxConsumidoresChange(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::CBoxConsumidoresChange(TObject *Sender)
   {
   //variaveis locais
   VTCarga   *carga;

   //seleciona carga
   carga = (VTCarga*)CBoxConsumidores->Items->Objects[CBoxConsumidores->ItemIndex];
   Moldura(carga);
   //salva carga
   carga_analisada = carga;
   multa->Carga = carga;
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TFormReclamacaoMT::DeterminaCargaMedicao(AnsiString codigo)
   {
   //vari�veis locais
   VTCarga *carga;

   for(int nc = 0; nc < lisCARGA->Count; nc++)
      {//localiza carga pelo c�digo
      carga = (VTCarga*)lisCARGA->Items[nc];
      if(codigo.AnsiPos(carga->Codigo) != 0) return(carga);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TFormReclamacaoMT::DeterminaFasesRede(void)
   {
   //ser� sempre ABC
   return(faseABC);
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //apaga molduras
   Moldura(NULL);
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::FormResize(TObject *Sender)
   {
   //redimensiona group box
   GBdadosRT->Width = int(Width / 2);
   GBanalise->Width = int(Width / 2);
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::FormShow(TObject *Sender)
   {
   //exibe FormEstudo
   FormEstudo->Show();
   FormEstudo->Align         = alClient;
   FormEstudo->BorderStyle = bsNone;
   Moldura(medicao->Carga);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoBT::ImportaMedicao(void)
   {
   //variaveis locais
   AnsiString       codigo;
   VTBarra         *barra_ini;
   VTCarga         *carga;
   VTSuprimento   *sup;

   //abre arquivo de medi��o
   if(! OpenDialogMed->Execute())  return(false);
   //importa medi��es
   if (! LeArqMedicao(OpenDialogMed->FileName)) return(false);
   //determina c�digo da Carga a partir do nome do arquivo
   codigo = ChangeFileExt(ExtractFileName(OpenDialogMed->FileName), "");
   if ((carga = DeterminaCargaMedicao(codigo)) == NULL)
      {//determina carga
      Aviso("N�o existe a carga " + codigo + ".");
      ImportaMedicao();
      return(false);
      }
   if ((sup = (VTSuprimento*)lisEQP->Items[0]) == NULL)
      {//determina o suprimento da rede
      Aviso("Rede n�o apresenta nenhum suprimento.");
      return(false);
      }
   barra_ini = BarraInicialRedeSec();
   //atualiza EditConsMed
   EditConsMed->Text = carga->Codigo;
   //salva carga em que foi feita a medi��o
   medicao->Carga = carga;
   if (! ajusta->Executa(sup, barra_ini, carga->pbarra))
      {//ajusta as medi��es
      Aviso("Erro ao ajustar medi��es.");
      return(false);
      }
   //desenha moldura sobre consumidor medido
   CBoxConsumidores->Text = medicao->Carga->Codigo;
   Moldura(medicao->Carga);
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoMT::ImportaMedicao(void) //novo FKM 26.03.2015
   {
   //variaveis locais
   int             respForm;
   AnsiString      filepath;
   AnsiString       codigo;
   VTBarra         *barra_ini;
   VTCarga         *carga;
   VTSuprimento   *sup;
   TFormSelMed      *formSelMed;

   //abre arquivo de medi��o
   //if(! OpenDialogMed->Execute())  return(false); //fkm
   formSelMed = new TFormSelMed(this, apl);
   //sugest�o de relacao de tp e de tens�o contratada
   formSelMed->edRelTP->Text = "70";
   formSelMed->edVcontratada->Text = "13.8";
   respForm = formSelMed->ShowModal();
   if (respForm == mrOk)
      {
      filepath = formSelMed->GetFilepath();
      //importa medi��es
      if (! LeArqMedicao(filepath,formSelMed->GetMedidor())) return(false);
      //determina c�digo da Carga a partir do nome do arquivo
      codigo = ChangeFileExt(ExtractFileName(filepath), "");
      if ((carga = DeterminaCargaMedicao(codigo)) == NULL)
         {//determina carga
         Aviso("N�o existe a carga " + codigo + ".");
         ImportaMedicao();
         return(false);
         }
      if ((sup = (VTSuprimento*)lisEQP->Items[0]) == NULL)
         {//determina o suprimento da rede
         Aviso("Rede n�o apresenta nenhum suprimento.");
         return(false);
         }
      barra_ini = BarraInicialRedePri();
      //atualiza EditConsMed
      EditConsMed->Text = carga->Codigo;
      //salva carga em que foi feita a medi��o
      medicao->Carga = carga;
      if (! ajusta->Executa(sup, barra_ini, carga->pbarra))
         {//ajusta as medi��es
         Aviso("Erro ao ajustar medi��es.");
         return(false);
         }
      //desenha moldura sobre consumidor medido
      CBoxConsumidores->Text = medicao->Carga->Codigo;
      Moldura(medicao->Carga);
      if(formSelMed){delete formSelMed; formSelMed = NULL;}
      return(true);
      }
   else
      {
      if(formSelMed){delete formSelMed; formSelMed = NULL;}
      return (false);
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormReclamacao::Inicia(void)
   {
   //variaveis locais
   VTLicencas       *licencas     = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo       *estudo       = planejamento->ExisteEstudo();

   //configura Planejamento para n�o exibir bot�o de an�lise
   if (planejamento) planejamento->AnaliseEnabled = false;
   //configura Planejamento para abrir Form de edi��o de obra
   planejamento->ActionEdtObra = ActionEditaObra;
   //verifica se o Estudo � novo
   if (estudo->Novo)
      {//salva estudo na base (duplica a rede selcionada p/ o estudo)
      if (! SalvaEstudoBase(estudo)) return;
      }
   //inicia licen�a
   licenca  = licencas->ExisteLicenca();
   //prote��o
   if (licenca == NULL) return;
   //preenche valor de TUSD conforme empresa
   if      (licenca->Empresa == AES_ELETROPAULO) RichEditTUSD->Lines->Text = "163,34";
   else if (licenca->Empresa == AES_SUL)         RichEditTUSD->Lines->Text = "187,01";
   //limpa TLists, EditConsMed e CBoxConsumidores
   lisCARGA->Clear();
   lisEQP->Clear();
   EditConsMed->Clear();
   ListaCargas();
   if(! PreparaRede())
      {//retira redes eqv. e guarda temporariamente suprimento em lisEQP
      Aviso("N�o � poss�vel realizar um estudo Reclama��o BT com as redes carregadas.");
      Close();
      return;
      }
   if(! ImportaMedicao())
      {//importa e ajusta rede conforme medi��o
//      Aviso("� necess�rio ter um arquivo de medi��o para realizar o estudo.");
      Close();
      return;
      }
   //insere todas as cargas (c�digos) em CBoxConsumidores
   PreencheCBoxConsumidores();
   ButConsMedClick(NULL);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoMT::Inicia(void)
   {//DVK 2015.03.23 reimplementada para retornar true/false
   //variaveis locais
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo       *estudo       = planejamento->ExisteEstudo();

   //configura Planejamento para n�o exibir bot�o de an�lise
   if (planejamento) planejamento->AnaliseEnabled = false;
   //configura Planejamento para abrir Form de edi��o de obra
   planejamento->ActionEdtObra = ActionEditaObra;
   //verifica se o Estudo � novo
   if (estudo->Novo)
      {//salva estudo na base (duplica a rede selcionada p/ o estudo)
      if (! SalvaEstudoBase(estudo)) return false;
      }
   //limpa TLists, EditConsMed e CBoxConsumidores
   lisCARGA->Clear();
   lisEQP->Clear();
   EditConsMed->Clear();
   ListaCargas();
   if(! PreparaRede())
      {//retira redes eqv. e guarda temporariamente suprimento em lisEQP
      Aviso("N�o � poss�vel realizar um estudo Reclama��o MT com as redes carregadas.");
//      Close();
      return false;
      }
   if(! ImportaMedicao())
      {//importa e ajusta rede conforme medi��o
//      Aviso("� necess�rio ter um arquivo de medi��o para realizar o estudo.");
//      Close();
      return false;
      }
   //insere todas as cargas (c�digos) em CBoxConsumidores
   PreencheCBoxConsumidores();
   ButConsMedClick(NULL);
   return true;
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoBT::LeArqMedicao(AnsiString arquivo)
   {
   //vari�veis locais
   bool       sucesso;
   VTExtrator *extrator;

   extrator = NewObjExtratorElpa(apl);
   sucesso  = extrator->Executa(arquivo, medicao);
   delete extrator;
   return(sucesso);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoMT::LeArqMedicao(AnsiString arquivo, strMEDIDOR *medidor) //novo FKM 26.03.2015
   {
   //vari�veis locais
   bool       sucesso;
   VTExtrator *extrator;

   //cria obj extrator
   extrator = NewObjExtratorCustom(apl);
   sucesso  = extrator->Executa(arquivo, medicao, medidor);
   delete extrator;
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::ListaCargas(void)
   {
   //variaveis locais
   VTBarra        *barra;
   VTCarga         *carga;
   VTRedes         *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList            *lisBARRA;

   //preenche lisBARRA com todas as Barras
   lisBARRA = redes->LisBarra();
   for(int nb = 0; nb < lisBARRA->Count; nb++)
      {//preenche lisCARGA com todas as Cargas
      barra = (VTBarra*)lisBARRA->Items[nb];
      lisEQP->Clear();
      barra->LisEqbar(lisEQP, eqptoCARGA);
      for(int nc = 0; nc < lisEQP->Count; nc++)
         {//guarda todas as cargas
         carga = (VTCarga*)lisEQP->Items[nc];
         lisCARGA->Add(carga);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::Moldura(VTEqpto *eqpto)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //exibe moldura no Eqpto
   if(grafico) grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormReclamacaoMT::PreencheCBoxConsumidores(void)
   {
   //variaveis locais
   VTCarga         *carga;

   //limpa
   CBoxConsumidores->Items->Clear();
   //insere cons medido (ItemIndex = 0)
   CBoxConsumidores->Items->AddObject(medicao->Carga->Codigo, medicao->Carga);
   for(int nc = 0; nc < lisCARGA->Count; nc++)
      {//preenche CBoxConsumidores com os c�digos das Cargas
      carga = (VTCarga*)lisCARGA->Items[nc];
      CBoxConsumidores->Items->AddObject(carga->Codigo, carga);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoMT::PreparaRede(void)
   {
   //vari�veis locais
   VTRede       *rede;
   VTGrafico    *grafico  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   VTRedes      *redes    = (VTRedes*) apl->GetObject(__classid(VTRedes));
   TList        *lisREDE  = redes->LisRede();

   //verifica se existe uma Rede prim�ria c/ 1 Suprimento
   if (lisREDE->Count == 1)
      {//determina Rede
      rede = (VTRede*)lisREDE->First();
      if (rede->TipoRede->Segmento != redePRI) return(false);
      rede_pri = rede;
      //verifica se existe um �nico Suprimento
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      if (lisEQP->Count == 1)
         {
         supr = (VTSuprimento*)lisEQP->First();
         return(true);
         }
      //caso contrario
      else return(false);
      }
   else if (lisREDE->Count >= 2)
      {//redefine rede para conter somente rede pri.
      if(! TrataReducoes()) return(false);
      }
   //determina fases da rede prim�ria
   medicao->FasesRede = DeterminaFasesRede();
   //redefine �rea de zoom p/ rede secund�ria
   grafico->AreaRedeDefine();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoMT::SalvaEstudoBase(VTEstudo *estudo)
   {
   //vari�veis locais
   bool    sucesso;
   VTSalva *salva;

   try{//cria objeto VTSalva
      if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
      //salva Estudo
      sucesso = salva->SalvaEstudo(estudo);
      //destr�i objeto
      delete salva;
      }catch(Exception &e)
         {
         sucesso = false;
         }
    return(sucesso);
    }

//---------------------------------------------------------------------------
TWinControl* __fastcall TFormReclamacaoMT::SelecionaParentParaJanela(void)
   {
   //micro c/ apenas 1 monitor
   return(PanelEstudo);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormReclamacaoMT::TrataReducoes(void)
   {
   //vari�veis locais
   TList        *lisREDUCAO;
   VTBarra      *barra;
   VTReducao    *reducao;
   VTSuprimento *suprimento;
   VTRede       *rede, *rede_etd;
   VTEdita      *edita    = (VTEdita*) apl->GetObject(__classid(VTEdita));
   VTRedes      *redes    = (VTRedes*) apl->GetObject(__classid(VTRedes));
   TList        *lisREDE  = redes->LisRede();

   //verifica se h� uma Rede prim�ria e uma Rede subesta��o equivalente
   rede_etd = rede_pri = NULL;
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if      (rede->TipoRede->Segmento == redeETD) rede_etd = rede;
      else if (rede->TipoRede->Segmento == redePRI) rede_pri = rede;
      }
   //prote��o
   if ((rede_etd == NULL)||(rede_pri == NULL)) return(false);
   if (! rede_etd->Carregada)
      {//transfere Suprimento da rede subesta��o p/ prim�ria
      lisREDUCAO = rede_etd->MRede->LisReducao();
      reducao = (VTReducao*)lisREDUCAO->First();
      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      if (lisEQP->Count == 0) return(false);
      suprimento = (VTSuprimento*)lisEQP->First();
      //retira Suprimento da Reducao
      (reducao->LisEqbar())->Remove(suprimento);
      //insere Suprimento na barra inicial da Rede prim�ria
      if ((barra = suprimento->pbarra) == NULL) return(false);
      barra->InsereEqbar(suprimento);
      //retira rede subesta�ao
      edita->Retira(rede_etd);
      edita->Clear();
      }
   else
      {
      //localiza suprimento
      lisEQP->Clear();
      rede_etd->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      if (lisEQP->Count == 0) return(false);
      suprimento = (VTSuprimento*)lisEQP->First();
      }
   suprimento->Show[eqp_VISIBLE] = true;
   suprimento->Codigo = rede_etd->Codigo;
   supr = suprimento;

   return(true);
   }

//---------------------------------------------------------------------------
//eof


