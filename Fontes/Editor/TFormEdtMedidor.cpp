//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#include "TFormEdtMedidor.h"
#include "TFormEdtCurvaMed.h"
#include "TImportaCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRedes.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Ordena.h"

//---------------------------------------------------------------------------
__fastcall TFormEdtMedidor::TFormEdtMedidor(TComponent* Owner, VTApl *apl, VTMedidor* medidor)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl     = apl;
   this->medidor = medidor;
   //cria listas
   lisEQP  = new TList();
   lisFORM = new TList();
   //exibe dados do Medidor
   EditCodigo->Text = medidor->Codigo;
   //determina lista de Eqptos que podem ter medição
   IniciaLisEqptoComMedicao(lisEQP);
   //inicia LBoxEqpto
   LBoxEqptoInicia(lisEQP);
   //determina fases de todos os nós das barras
   FasesDeTodasBarras();
   //cria todos os possíveis Canais dos Eqptos
   CriaCanais(lisEQP);
   //cria um objeto TFormEdtCurvaMed p/ cada Canal
   CriaFormEdtCurvaMedicao();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtMedidor::~TFormEdtMedidor(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destroi net
   if (net)     {delete net;    net    = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP; lisEQP = NULL;}
   //destrói lista e seus objetos
   if (lisFORM) {LimpaTList(lisFORM);  delete lisFORM;  lisFORM  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::ActionCancelaExecute(TObject *Sender)
   {
   //retira todos os canais temporários do Medidor
   LimpaCanalTemporario();
   //finaliza edição
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::ActionConfirmaExecute(TObject *Sender)
   {
   //lê dados do Medidor e dos Canais
   if (! LeDadosMedidor()) return;
   if (! LeDadosCanal())   return;
   //retira todos os canais temporários do Medidor
   LimpaCanalTemporario();
   //retorna Ok
   ModalResult = mrOk;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::ActionImportaCurvaExecute(TObject *Sender)
   {//DVK 2015.10.28
   //variaveis locais
   bool              importado = false;
   AnsiString       filename;
   VTCanal            *canal;
   VTPath           *path = (VTPath*)apl->GetObject(__classid(VTPath));
   TImportaCurva    *importa;

   //inicia opendialog
   OpenDialog->InitialDir = path->DirImporta();
   OpenDialog->Filter = "Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*";
   if (OpenDialog->Execute())
      {//importa apenas 1 arquivo
      if (OpenDialog->Files->Count != 1) return;
      //cria obj importa medição
      importa = new TImportaCurva(apl);
      filename = OpenDialog->Files->Strings[0];
      importado = importa->Executa(filename, medidor);
      if(importado)
         {//atualiza curvas dos canais
         AtualizaCurvas();
         //atualiza CLBox dos canais  //DVK 2015.12.17
         AtualizaCLBoxCanal();
         }
      //destroi obj importa medição
      delete importa;
      }
   }
*/  //DVK 2015.12.18 reescrito considerando eqpto selecionado
//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::ActionImportaCurvaExecute(TObject *Sender)
   {
   //variaveis locais
   bool             importado = false;
   AnsiString       filename;
   VTEqpto          *eqpto;
   VTPath           *path = (VTPath*)apl->GetObject(__classid(VTPath));
   TImportaCurva    *importa;

   //determina Canal
   if(LBoxEqpto->ItemIndex < 0) {Aviso("Selecione um equipamento com medição"); return;}
   eqpto = (VTEqpto*)LBoxEqpto->Items->Objects[LBoxEqpto->ItemIndex];
   if(! eqpto)                  {Aviso("Selecione um eqpto com medição"); return;}
   //inicia opendialog
   OpenDialog->InitialDir = path->DirImporta();
   OpenDialog->Filter = "Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*";
   if (OpenDialog->Execute())
      {//importa apenas 1 arquivo
      if (OpenDialog->Files->Count != 1) return;
      //cria obj importa medição
      importa = new TImportaCurva(apl, NULL);
      filename = OpenDialog->Files->Strings[0];
      importado = importa->Executa(filename, eqpto, medidor);
      if(importado)
         {//atualiza curvas dos canais
         AtualizaCurvas();
         //atualiza CLBox dos canais
         AtualizaCLBoxCanal();
         }
      //destroi obj importa medição
      delete importa;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::ActionRetiraExecute(TObject *Sender)
   {
   //retira todos os canais temporários do Medidor
   LimpaCanalTemporario();
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::AtualizaCLBoxCanal(void)
   { //DVK 2015.12.17
   //variáveis locais
   VTCanal *canal;

   //loop em todos canais
   for(int nc = 0; nc < CLBoxCanal->Count; nc++)
      {//determina Canal
      canal = (VTCanal*)CLBoxCanal->Items->Objects[nc];
      //atualiza check
      CLBoxCanal->Checked[nc] = canal->Enabled;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::AtualizaCurvas(void)
   {
   //variáveis locais
   VTCanal *canal;
   TList     *lisCANAL = medidor->LisCanal();

   //loop em todos canais enabled
   for(int nc = 0; nc < lisCANAL->Count; nc++)
      {//determina Canal
      canal = (VTCanal*)lisCANAL->Items[nc];
      if(! canal->Enabled) continue;
      //determina FormEdtCurvaMed do Canal
      for (int n = 0; n < lisFORM->Count; n++)
         {
         FormEdtCurvaMed = (TFormEdtCurvaMed*)lisFORM->Items[n];
         if (FormEdtCurvaMed->Canal == canal)
            {//atualiza FormEdtCurvaMed do Canal
            FormEdtCurvaMed->Canal = canal;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CLBoxCanalInicia(VTEqpto *eqpto)
   {
   //variáveis locais
   AnsiString txt;
   int        index;
   VTCanal    *canal;
   TList      *lisCANAL = medidor->LisCanal();
   VTFases    *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //atualiza Caption do GroupBox
//   GBoxCanalEqpto->Caption = eqpto->TipoAsString() + ": canais disponíveis";
   //reinicia CLBoxCanal
   CLBoxCanal->Clear();
   for (int n = 0; n < lisCANAL->Count; n++)
      {
      canal = (VTCanal*)lisCANAL->Items[n];
      if (canal->Eqpto != eqpto) continue;
      //define texto p/ o tipo do canal
      txt   = canal->TipoCanalAsString + ": " + fases->AsStrUpper[canal->Fases]; //DVK 2015.10.28
      index = CLBoxCanal->Items->AddObject(txt, canal);
      CLBoxCanal->Checked[index] = canal->Enabled;
      //pré-seleciona primeiro Canal habilitado
      if ((CLBoxCanal->ItemIndex < 0)&&(canal->Enabled)) CLBoxCanal->ItemIndex = index;
      }
   //simula seleção de um canal
   CLBoxCanalClick(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CLBoxCanalClick(TObject *Sender)
   {
   //variáveis locais
   VTCanal *canal;

   //esconde dados do Canal anteriormente selecionado
   if (FormEdtCurvaMed) FormEdtCurvaMed->Hide();
   //proteção
   if (CLBoxCanal->ItemIndex < 0) return;
   //verifica se o Canal associado está habilitado
   if (CLBoxCanal->Checked[CLBoxCanal->ItemIndex])
      {//determina Canal
      canal = (VTCanal*)CLBoxCanal->Items->Objects[CLBoxCanal->ItemIndex];
      //determina FormEdtCurvaMed do Canal
      for (int n = 0; n < lisFORM->Count; n++)
         {
         FormEdtCurvaMed = (TFormEdtCurvaMed*)lisFORM->Items[n];
         if (FormEdtCurvaMed->Canal == canal)
            {//exibe FormEdtCurvaMed do Canal
            //FormEdtCurvaMed->Canal = canal;
            FormEdtCurvaMed->Show();
            break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CLBoxCanalClickCheck(TObject *Sender)
   {
   //variáveis locais
   VTCanal *canal, *canal_outro;
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //DVK 2016.10.07 verifica se o canal_outro existe
   //proteção
   if (CLBoxCanal->ItemIndex < 0) return;
   //determina o Canal associado
   canal = (VTCanal*)CLBoxCanal->Items->Objects[CLBoxCanal->ItemIndex];
   canal->Enabled = CLBoxCanal->Checked[CLBoxCanal->ItemIndex];
   //DVK 2015.12.18 se o canal é de corrente, desmarca canais de potência
   if(canal->TipoCanal == canalI)
      {
      for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {
         canal_outro = medidor->ExisteCanal(canal->Eqpto, canalP, fases->IndexToTag[ind_fase]);
         if(canal_outro != NULL)
            {
            if(canal_outro->Enabled && canal->Enabled) canal_outro->Enabled = false;
            }
         canal_outro = medidor->ExisteCanal(canal->Eqpto, canalQ, fases->IndexToTag[ind_fase]);
         if(canal_outro != NULL)
            {
            if(canal_outro->Enabled && canal->Enabled) canal_outro->Enabled = false;
            }
         }
      //trifásico
      canal_outro = medidor->ExisteCanal(canal->Eqpto, canalP, faseABC);
      if(canal_outro != NULL)
         {
         if(canal_outro->Enabled && canal->Enabled) canal_outro->Enabled = false;
         }
      canal_outro = medidor->ExisteCanal(canal->Eqpto, canalQ, faseABC);
      if(canal_outro != NULL)
         {
         if(canal_outro->Enabled && canal->Enabled) canal_outro->Enabled = false;
         }
      }
   //se o canal é de potência, desmarca canais de corrente
   if((canal->TipoCanal == canalP) || (canal->TipoCanal == canalQ))
      {
      for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {
         canal_outro = medidor->ExisteCanal(canal->Eqpto, canalI, fases->IndexToTag[ind_fase]);
         if(canal_outro == NULL) continue;
         if(canal_outro->Enabled && canal->Enabled) canal_outro->Enabled = false;
         }
      }
   AtualizaCLBoxCanal();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanal(VTEqpto *eqpto, int tipo_canal, int fases)
   {
   //variáveis locais
   VTCanal *canal;
   int     fase_tag = fases & faseABC;

   try{//verifica se o Medidor já possui esse Canal //DVK 2015.10.30 fases
      if ((canal = medidor->ExisteCanal(eqpto, tipo_canal, fase_tag)) == NULL)
         {//cria um novo Canal
         canal = medidor->InsereCanal(eqpto, tipo_canal, fase_tag);
         //cria Curva p/ a Canal
         canal->Curva                = DLL_NewObjCurva();
         canal->Curva->Tipica        = false;
         canal->Curva->NumeroValores = nvCURVA_CANAL; //valor medido, nominal e valor estimado
         canal->Curva->Escala        = patEDITAVEL;
         switch(tipo_canal)
            {
            case canalV:
               canal->Curva->Tipo          = curvaV;
               canal->Curva->Unidade       = unidKV;
               break;
            case canalI:
               canal->Curva->Tipo          = curvaI;
               canal->Curva->Unidade       = unidA;
               break;
            case canalP:
               canal->Curva->Tipo          = curvaP;
               canal->Curva->Unidade       = unidMW;
               break;
            case canalQ:
               canal->Curva->Tipo          = curvaQ;
               canal->Curva->Unidade       = unidMVAR;
               break;
            case canalS:
               canal->Curva->Tipo          = curvaS;
               canal->Curva->Unidade       = unidMVA;
               break;
            default:
               break;
            }
         canal->Curva->Reconfigura(patEDITAVEL);
         //define Canal como desabilitado
         canal->Enabled = false;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanais(TList *lisEQP)
   {
   //variáveis locais
   int      index;
   VTEqpto  *eqpto;
   VTCanal  *canal;

   //loop p/ todos Eqptos que podem ter medição
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //descarta Chaves //DVK 2015.10.28 comentado
//      if (eqpto->Tipo() == eqptoCHAVE) continue;
      //cria todos os possíveis Canais p/ o Eqpto
      if      (eqpto->Tipo() == eqptoBARRA)      CriaCanaisBarra(eqpto);
      else if (eqpto->Tipo() == eqptoCARGA)      CriaCanaisCarga(eqpto);
      else if (eqpto->Tipo() == eqptoGERADOR)    CriaCanaisGerador(eqpto);
      else if (eqpto->Tipo() == eqptoSUPRIMENTO) CriaCanaisSuprimento(eqpto);
      else if (eqpto->TipoLigacao())             CriaCanaisLigacao(eqpto);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanaisBarra(VTEqpto *eqpto)
   {
   //variáveis locais
   int fases_tag;
//   int vet_tipo[] = {canalV, canalVF, canalP, canalPQ, canalPV, canalI, canalIF};
   int vet_tipo[] = {canalV}; //DVK 2015.10.30
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina fases
   fases_tag = FasesDaBarra(medidor->Barra);
   for (int n = 0; n < sizeof(vet_tipo)/sizeof(int); n++)
      {//cria um novo Canal para cada fase disponível
      if((fases_tag & faseA) == faseA) CriaCanal(eqpto, vet_tipo[n], faseA);
      if((fases_tag & faseB) == faseB) CriaCanal(eqpto, vet_tipo[n], faseB);
      if((fases_tag & faseC) == faseC) CriaCanal(eqpto, vet_tipo[n], faseC);
      if(fases->NumeroDeFases(fases_tag) > 1) CriaCanal(eqpto, vet_tipo[n], fases_tag);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanaisCarga(VTEqpto *eqpto)
   {
   //variáveis locais
   int fases_tag;
//   int vet_tipo[] = {canalPQ, canalI, canalIF};
   int vet_tipo[] = {canalI, canalP, canalQ, canalS};  //DVK 2015.10.30
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina fases
   fases_tag = FasesDaBarra(medidor->Barra);
   for (int n = 0; n < sizeof(vet_tipo)/sizeof(int); n++)
      {//cria um novo Canal para cada fase disponível
      if((fases_tag & faseA) == faseA) CriaCanal(eqpto, vet_tipo[n], faseA);
      if((fases_tag & faseB) == faseB) CriaCanal(eqpto, vet_tipo[n], faseB);
      if((fases_tag & faseC) == faseC) CriaCanal(eqpto, vet_tipo[n], faseC);
      if(vet_tipo[n] == canalI) continue; //não insere canal de corrente de outras fases
      if(fases->NumeroDeFases(fases_tag) > 1) CriaCanal(eqpto, vet_tipo[n], fases_tag);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanaisGerador(VTEqpto *eqpto)
   {
   //variáveis locais
   int fases_tag;
//   int vet_tipo[] = {canalV, canalVF, canalP, canalPQ};
   int vet_tipo[] = {canalV, canalP, canalQ, canalS};  //DVK 2015.10.30
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina fases
   fases_tag = FasesDaBarra(medidor->Barra);
   for (int n = 0; n < sizeof(vet_tipo)/sizeof(int); n++)
      {//cria um novo Canal para cada fase disponível
      if((fases_tag & faseA) == faseA) CriaCanal(eqpto, vet_tipo[n], faseA);
      if((fases_tag & faseB) == faseB) CriaCanal(eqpto, vet_tipo[n], faseB);
      if((fases_tag & faseC) == faseC) CriaCanal(eqpto, vet_tipo[n], faseC);
      if(fases->NumeroDeFases(fases_tag) > 1) CriaCanal(eqpto, vet_tipo[n], fases_tag);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanaisLigacao(VTEqpto *eqpto)
   {
   //variáveis locais
   int fases_tag;
//   int vet_tipo[] = {canalPQ, canalI, canalIF};
   int      vet_tipo[] = {canalI, canalP, canalQ, canalS};  //DVK 2015.10.30
   VTFases *fases      = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina fases
   fases_tag = FasesDaBarra(medidor->Barra);
   for (int n = 0; n < sizeof(vet_tipo)/sizeof(int); n++)
      {//cria um novo Canal para cada fase disponível
      if((fases_tag & faseA) == faseA) CriaCanal(eqpto, vet_tipo[n], faseA);
      if((fases_tag & faseB) == faseB) CriaCanal(eqpto, vet_tipo[n], faseB);
      if((fases_tag & faseC) == faseC) CriaCanal(eqpto, vet_tipo[n], faseC);
      if(vet_tipo[n] == canalI) continue; //não insere canal de corrente de outras fases
      if(fases->NumeroDeFases(fases_tag) > 1) CriaCanal(eqpto, vet_tipo[n], fases_tag);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaCanaisSuprimento(VTEqpto *eqpto)
   {
   //variáveis locais
   int fases_tag;
//   int vet_tipo[] = {canalV, canalVF, canalP, canalPQ};
   int vet_tipo[] = {canalV, canalP, canalQ, canalS};  //DVK 2015.10.30
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina fases
   fases_tag = FasesDaBarra(medidor->Barra);
   for (int n = 0; n < sizeof(vet_tipo)/sizeof(int); n++)
      {//cria um novo Canal para cada fase disponível
      if((fases_tag & faseA) == faseA) CriaCanal(eqpto, vet_tipo[n], faseA);
      if((fases_tag & faseB) == faseB) CriaCanal(eqpto, vet_tipo[n], faseB);
      if((fases_tag & faseC) == faseC) CriaCanal(eqpto, vet_tipo[n], faseC);
      if(fases->NumeroDeFases(fases_tag) > 1) CriaCanal(eqpto, vet_tipo[n], fases_tag);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::CriaFormEdtCurvaMedicao(void)
   {
   //variáveis locais
   VTCanal *canal;
   TList   *lisCANAL = medidor->LisCanal();

   //retira todos os canais temporários do Medidor
   for (int n = lisCANAL->Count - 1; n >= 0; n--)
      {
      canal = (VTCanal*)lisCANAL->Items[n];
      //cria FormEdtCurvaMed para edicao de Curva de medição do Canal
      FormEdtCurvaMed = new TFormEdtCurvaMed(this, apl, GBoxCanal);
      FormEdtCurvaMed->Canal = canal;
      //salva FormEdtCurvaMed em lisFORM
      lisFORM->Add(FormEdtCurvaMed);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TFormEdtMedidor::FasesDaBarra(VTBarra *barra)
   {//DVK 2015.10.30
   //proteção
   if(! net)            return(faseINV);
   if(! barra)          return(faseINV);
   if(! barra->No[net]) return(faseINV);
   //retorna fases do nó da barra
   return(barra->No[net]->Fases);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtMedidor::FasesDeTodasBarras(void)
   {//DVK 2015.10.30
   //variaveis locais
   bool     sucesso = true;
   VTOrdena *ordena;
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //cria obj ordena e net
   ordena = DLL_NewObjOrdena(apl);
   if(! ordena) return(false);
   net = ordena->Executa(redes);
   if(! net)    sucesso = false;
   //net é usado para determinar os nós das barras e suas fases
   delete ordena;
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::FormShow(TObject *Sender)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::IniciaLisEqptoComMedicao(TList *lisEQP)
   {
   //variáveis locais
   VTBarra          *barra;
   VTLigacao        *ligacao;
   TList            *lisBAR = new TList();
   VTRedes          *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia lisBAR c/ todas Barras do No do Medidor (Barras do Medidor e Barras conectadas por Chave)
   lisEQP->Clear();
   lisBAR->Add(medidor->pbarra);
   redes->LisLigacao(lisEQP, medidor->pbarra);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      ligacao = (VTLigacao*)lisEQP->Items[n];
      if (ligacao->Tipo() != eqptoCHAVE) continue;
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
         }
      }
   //limpa lisEQP
   lisEQP->Clear();
   //inicia lisEQP c/ a Barra do medidor
   lisEQP->Add(medidor->pbarra);
   //inclui em lisEQP todas as Ligacoes que conectam as Barras em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      redes->LisLigacao(lisEQP, barra);
      barra->LisEqbar(lisEQP, eqptoCARGA);
      barra->LisEqbar(lisEQP, eqptoGERADOR);
      barra->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      }
   delete lisBAR;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::LBoxEqptoClick(TObject *Sender)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //proteção
   if (LBoxEqpto->ItemIndex < 0) return;
   //determina Eqpto selecionado
   if ((eqpto = (VTEqpto*)LBoxEqpto->Items->Objects[LBoxEqpto->ItemIndex]) == NULL) return;
   //reinicia CLBoxCanal c/ todos os Canais do Eqpto
   CLBoxCanalInicia(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::LBoxEqptoInicia(TList *lisEQP)
   {
   //variáveis locais
   VTEqpto *eqpto;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      LBoxEqpto->Items->AddObject(eqpto->TipoAsString() + ": " + AnsiString(eqpto->Codigo), eqpto);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtMedidor::LeDadosCanal(void)
   {
   //variáveis locais
   int               num_canal = 0;
   TFormEdtCurvaMed  *FormEdtCurvaMed;

   //valida todos os canais habilitados
   for (int n = 0; n < lisFORM->Count; n++)
      {
      FormEdtCurvaMed = (TFormEdtCurvaMed*)lisFORM->Items[n];
      if (FormEdtCurvaMed->Canal->Enabled)
         {
         if (! FormEdtCurvaMed->LeDadosCanal()) return(false);
         //incrementa numero de canais selecionados
         num_canal++;
         }
      }
   //verifica se há pelo menos um canal selecionado
   if (num_canal == 0)
      {
      Aviso("Configure pelo menos 1 canal de medição");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtMedidor::LeDadosMedidor(void)
   {
   //verifica se foi definido código do medidor
   medidor->Codigo = EditCodigo->Text.Trim();
   if (medidor->Codigo.IsEmpty())
      {
      Aviso("Defina o código do medidor.");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::LimpaCanalTemporario(void)
   {
   //variáveis locais
   VTCanal *canal;
   TList   *lisCANAL = medidor->LisCanal();

   //retira todos os canais temporários do Medidor
   for (int n = lisCANAL->Count - 1; n >= 0; n--)
      {
      canal = (VTCanal*)lisCANAL->Items[n];
      if (! canal->Enabled)
         {//descarta o Canal
         medidor->RemoveCanal(canal);
         delete canal;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedidor::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
//eof
