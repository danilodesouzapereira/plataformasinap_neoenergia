//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPlanejamento.h"
#include "VTEstudo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPlanejamento* __fastcall NewObjPlanejamento(VTApl *apl)
   {
   try{
      return(new TPlanejamento(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
__fastcall TPlanejamento::TPlanejamento(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   lisACTION_ANALISE   = new TList();
   lisACTION_RELATORIO = new TList();
   lisESTUDO           = new TList();
   //não determina o tipo de relatorio de obras
   PD.ActionRelObra    = NULL;
   //indica que não integração com sistema externo
   PD.ipe              = NULL;
   PD.ActionEdtObra    = NULL;
   //habilita análise
   AnaliseEnabled = true;
   //inicia objeto Padroes
   //padroes->IniciaPadraoDefault();
   }

//---------------------------------------------------------------------------
__fastcall TPlanejamento::~TPlanejamento(void)
   {
   //destrói lista e sem destruir seus objetos
   if (lisACTION_ANALISE)   {delete lisACTION_ANALISE;   lisACTION_ANALISE   = NULL;}
   if (lisACTION_RELATORIO) {delete lisACTION_RELATORIO; lisACTION_RELATORIO = NULL;}
   //destrói lista e seus objetos
   if (lisESTUDO) {LimpaTList(lisESTUDO); delete lisESTUDO; lisESTUDO = NULL;}
   }

//---------------------------------------------------------------------------
VTEstudo*__fastcall TPlanejamento::ExisteEstudo(AnsiString nome)
   {
   //variáveis locais
   VTEstudo *estudo;

   //loop p/ todos Estudos
   for (int n = 0; n < lisESTUDO->Count; n++)
      {
      estudo = (VTEstudo*)lisESTUDO->Items[n];
      if (estudo->Codigo.AnsiCompareIC(nome) == 0) return(estudo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTEstudo*__fastcall TPlanejamento::ExisteEstudo(int id)
   {
   //variáveis locais
   VTEstudo *estudo;

   //loop p/ todos Estudos
   for (int n = 0; n < lisESTUDO->Count; n++)
      {
      estudo = (VTEstudo*)lisESTUDO->Items[n];
      if ((id < 0)||(estudo->Id == id)) return(estudo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TPlanejamento::ExisteEstudoAlterado(void)
   {
   //variáveis locais
   VTEstudo *estudo;

   //loop p/ todos Estudos
   for (int n = 0; n < lisESTUDO->Count; n++)
      {
      estudo = (VTEstudo*)lisESTUDO->Items[n];
      if (estudo->Alterado) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TPlanejamento::ExisteEstudoNovo(void)
   {
   //variáveis locais
   VTEstudo *estudo;

   //loop p/ todos Estudos
   for (int n = 0; n < lisESTUDO->Count; n++)
      {
      estudo = (VTEstudo*)lisESTUDO->Items[n];
      if (estudo->Novo) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TPlanejamento::InsereActionAnalise(TAction *ActionExterna)
   {
   //insere ActionExterna em lisACTION
   if (lisACTION_ANALISE->IndexOf(ActionExterna) < 0) lisACTION_ANALISE->Add(ActionExterna);
   }

//---------------------------------------------------------------------------
void __fastcall TPlanejamento::InsereActionRelatorio(TAction *ActionExterna)
   {
   //insere ActionExterna em lisACTION
   if (lisACTION_RELATORIO->IndexOf(ActionExterna) < 0) lisACTION_RELATORIO->Add(ActionExterna);
   }

//---------------------------------------------------------------------------
VTEstudo*__fastcall TPlanejamento::InsereEstudo(AnsiString codigo, int tipo, int estudo_id)
   {
   //variáveis locais
   VTEstudo *estudo;

   //cria um estudo
   if ((estudo = NewObjEstudo(apl)) != NULL)
      {//define código
      estudo->Codigo = codigo;
      estudo->Id     = estudo_id;
      estudo->Tipo   = tipo;
      //insere estudo em lisESTUDO
      lisESTUDO->Add(estudo);
      }
   return(estudo);
   }

//---------------------------------------------------------------------------
VTEstudo*__fastcall TPlanejamento::InsereEstudoRedeOriginal(void)
   {
   //variáveis locais
   VTEstudo *estudo;

   //verifica se existe o Estudo c/ ID = 0
   if ((estudo = ExisteEstudo(estudoID_ZERO)) == NULL)
      {//cria o Estudo c/ estudoID_ZERO
      estudo = InsereEstudo("Rede Original", estudoREDE_ORIGINAL, estudoID_ZERO);
      //marca que o estudo NÃO é novo
      estudo->Novo = false;
      }
   return(estudo);
   }

//---------------------------------------------------------------------------
TList* __fastcall TPlanejamento::LisActionAnalise(void)
   {
   return(lisACTION_ANALISE);
   }

//---------------------------------------------------------------------------
TList* __fastcall TPlanejamento::LisActionRelatorio(void)
   {
   return(lisACTION_RELATORIO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TPlanejamento::LisEstudo(void)
   {
   return(lisESTUDO);
   }

//---------------------------------------------------------------------------
void __fastcall TPlanejamento::Reinicia(VTEstudo *estudo)
   {
   //verifica se foi definido um Estudo a ser mantido
   if (estudo == NULL)
      {//destrói todos estudos
      LimpaTList(lisESTUDO);
      }
   else
      {//retira estudo a ser mantido
      lisESTUDO->Remove(estudo);
      //destrói todos os demais estudos
      LimpaTList(lisESTUDO);
      //insere estudo
      lisESTUDO->Add(estudo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPlanejamento::RetiraActionAnalise(TAction *ActionExterna)
   {
   //retira ActionExterna de lisACTION
   lisACTION_ANALISE->Remove(ActionExterna);
   }

//---------------------------------------------------------------------------
void __fastcall TPlanejamento::RetiraActionRelatorio(TAction *ActionExterna)
   {
   //retira ActionExterna de lisACTION
   lisACTION_RELATORIO->Remove(ActionExterna);
   }

//---------------------------------------------------------------------------
bool __fastcall TPlanejamento::RetiraEstudo(VTEstudo *estudo)
   {
   //proteção
   if (estudo == NULL) return(false);
   //retira objeto de lisESTUDO   
   lisESTUDO->Remove(estudo);
   //destrói objeto
   delete estudo;

   return(true);
   }

//---------------------------------------------------------------------------
//eof



