//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelMed.h"
#include "TMedidores.h"
#include "TFormCustomImporta.h"
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Reclamacao\ImportaMedicao\VTExtrator.h>
#include   <Fontes\Reclamacao\Reclamacao\VTRes505.h>
#include "..\..\DLL_Inc\Funcao.h"
#include <Fontes\Apl\VTApl.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelMed *FormSelMed;
//---------------------------------------------------------------------------
__fastcall TFormSelMed::TFormSelMed(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
{
   //salva ponteiro p/ objeto
   this->apl = apl_owner;
   this->owner = Owner;
   perfilMedidores = new TMedidores(apl);
   IniciaForm();
}
//---------------------------------------------------------------------------
__fastcall TFormSelMed::~TFormSelMed(void)
{
   if(perfilMedidores){delete perfilMedidores; perfilMedidores = NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TFormSelMed::ActionCancelExecute(TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelMed::ActionNewMeterExecute(TObject *Sender)
{
//implementar
   int respForm;
   TFormCustomImporta *formCImporta;

   //instruções de "uso"
   Aviso("Para criar um novo padrão de medidor é preciso selecionar um arquivo de medição desse novo medidor, assim você poderá visualizar parte do arquivo e fazer a correspondência das colunas dos dados corretamente");
   //abre o form de criação de padrão customizado
   formCImporta = new TFormCustomImporta(owner, apl, perfilMedidores);
   respForm = formCImporta->ShowModal();

   if(respForm == mrOk)
   {  //preenche combobox com todos padroes de medidores
      IniciaForm();
      //seleciona o medidor recem criado
      cbMedidor->ItemIndex = cbMedidor->Items->Count -1;
   }
   //destrói form
   if(formCImporta){delete formCImporta; formCImporta=NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TFormSelMed::ActionSelArqExecute(TObject *Sender)
{
   VTRes505    *res505;
   AnsiString   msg;
   AnsiString   itemSelected;
   int          resposta;
   bool         IsRelTPValid, isVContratadaValid;

   res505 = (VTRes505*) apl->GetObject(__classid(VTRes505));
   //inicializa bools de teste de validade
   IsRelTPValid = isVContratadaValid = true;
   //proteção //DVK 2015.04.14
   if(cbMedidor->ItemIndex < 0)
      {
      Aviso("Selecione um padrão para leitura da medição.");
      return;
      }
   //pega o nome do padrão do medidor
   itemSelected = cbMedidor->Items->Strings[cbMedidor->ItemIndex];
   //inicializa o opendialog
   OpenDialogMed->Title="Abrir Arquivo de Medição do tipo " +itemSelected;
   OpenDialogMed->Files->Clear();
   OpenDialogMed->Execute();
   if(! OpenDialogMed->FileName.IsEmpty())
   {
      if( (Confirma("O arquivo de medição selecionado corresponde ao medidor: ", itemSelected +" ?",MB_YESNO))==IDYES)
      {  //busca o padrao de medidor na lista do TMedidores através do nome
         medidorSelected = perfilMedidores->BuscaPorNome(itemSelected);

         if(! StrToDouble(edRelTP->Text,relTP))
         {
            Aviso("Valor de Relação de TP inválido");
            IsRelTPValid = false;
         }
         else{medidorSelected->relTP = relTP;}
         if(! StrToDouble(edVcontratada->Text,vContratada))
         {
            Aviso("Valor de Tensão Contratada inválido");
            isVContratadaValid = false;
         }
         else
         {
            medidorSelected->vContratada = vContratada;
            //passa o valor pro Res505
            res505->VcontratadaMT = vContratada;
         }
         //salva angulo de defasagem
         InsereAngDefasagem();
         /*
         try
         {  //verifica se os valores inseridos no TEdit são válidos
            relTP = StrToDouble(edRelTP->Text);
            medidorSelected->relTP = relTP;
         }
         catch( Exception &e)
         {  //caso não seja, avisa o usuário e invalida
            Aviso("Valor de Relação de TP inválido");
            IsRelTPValid = false;
         }
         try
         {  //verifica se os valores inseridos no TEdit são válidos
            vContratada = StrToDouble(edVcontratada->Text);
            medidorSelected->vContratada = vContratada;
         }
         catch( Exception &e)
         {  //caso não seja, avisa o usuário e invalida
            Aviso("Valor de Tensão Contratada inválido");
            isVContratadaValid = false;
         }  */
         if( (IsRelTPValid)&&(isVContratadaValid) )
         {  //caso os valores sejam válidos passa uma resposta ao FormReclamação
            ModalResult = mrOk;
         }
      }
   }
}

//---------------------------------------------------------------------------
AnsiString __fastcall TFormSelMed::GetFilepath(void)
{
   return OpenDialogMed->FileName;
}
//---------------------------------------------------------------------------
strMEDIDOR* __fastcall TFormSelMed::GetMedidor(void)
{
   return medidorSelected;
}
//---------------------------------------------------------------------------
double __fastcall TFormSelMed::GetRelTP(void)
{
   return relTP;
}
//---------------------------------------------------------------------------
double __fastcall TFormSelMed::GetVContratada(void)
{
   return vContratada;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelMed::IniciaForm(void)
{
   //variáveis locais
   TList          *listaMedidores;
   strMEDIDOR     *medidor;
   VTEstudo       *estudo;
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTPath         *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //guarda pasta Importa no OpenDialog
   OpenDialogMed->InitialDir = path->DirImporta();
   //verifica tipo de estudo aberto
   estudo = planejamento->ExisteEstudo();
   if(! estudo) return;
   //mostra/esconde opções de defasagem do trafo
   RGroupAng->Visible = (estudo->Tipo == estudoRECLAMACAO_MT);
   //limpa cbMedidor
   cbMedidor->Items->Clear();
   //pega a lista de padroes de medidor no TMedidores e adiciona ao combobox
   listaMedidores = perfilMedidores->GetListMedidores();
   for (int i = 0; i < listaMedidores->Count ; i++)
   {
      medidor = (strMEDIDOR*)listaMedidores->Items[i];
      cbMedidor->Items->Add(medidor->nome);
   }
   cbMedidor->ItemIndex =0;
}

//---------------------------------------------------------------------------
void __fastcall TFormSelMed::InsereAngDefasagem(void)
   {
   //proteção
   if(! medidorSelected) return;
   if(! RGroupAng->Visible)
      {//se o estudo não for MT
      medidorSelected->ang_defasagem = -1;
      return;
      }
   //determina ângulo de defasagem do trafo
   switch(RGroupAng->ItemIndex)
      {
      case 0:  medidorSelected->ang_defasagem = 0;   break;
      case 1:  medidorSelected->ang_defasagem = 30;  break;
      case 2:  medidorSelected->ang_defasagem = -30; break;
      default: medidorSelected->ang_defasagem = 0;   break;
      }
   }

//---------------------------------------------------------------------------

