//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Zona\VTZonas.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "TDiagArranjo.h"
#include "TDiagBIso.h"
#include "TDiagBarra.h"
#include "TDiagCabo.h"
#include "TDiagCapacitor.h"
#include "TDiagCarga.h"
#include "TDiagChave.h"
#include "TDiagRede.h"
#include "TDiagReg.h"
#include "TDiagTopo.h"
#include "TDiagTrafo.h"
#include "TDiagTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTDiagTopo* __fastcall NewObjDiagTopo(VTApl *apl_owner, VTProgresso *progresso)
   {
   try{//cria objeto TDiagEqpto
   return(new TDiagTopo(apl_owner, progresso));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
__fastcall  TDiagTopo::TDiagTopo(VTApl *apl_owner, VTProgresso *progresso)
{
    this->apl = apl_owner;
    //cria o objeto que trata o arquivo com a cfg  de Trecho
    diagArranjo   = new TDiagArranjo(apl);
    diagBarra     = new TDiagBarra(apl);
    diagBIso      = new TDiagBIso(apl);
    diagCabo      = new TDiagCabo(apl);
    diagCapacitor = new TDiagCapacitor(apl);
    diagCarga     = new TDiagCarga(apl);
    diagChave     = new TDiagChave(apl);
    diagRede      = new TDiagRede(apl);
    diagReg       = new TDiagReg(apl);
    diagTrecho    = new TDiagTrecho(apl);
    diagTrafo     = new TDiagTrafo(apl);
    //inicializ o flag preparada
    redePreparada = false;
    //tenta usar o progresso
//   progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
    //guarda ponteiro
    this->progresso = progresso;
//   if(!progresso)
//      {
//      progresso = DLL_NewObjProgresso(NULL, NULL);
//      }
    //destrói eventual objeto criado anteriormente
    if (arvore) {delete arvore;}
    //cria novo objeto Arvore
    arvore = DLL_NewObjArvore(apl);
    //this->Inicia();
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
__fastcall  TDiagTopo::~TDiagTopo(void)
{
    //corrige as zonas das barras
    if(redePreparada) PreparaRedeUso();
    //destroi arvore
    if(arvore){delete arvore; arvore = NULL;}
    //destroi os objetos
    if(diagArranjo)   {delete diagArranjo;     diagArranjo = NULL;}
    if(diagBarra)     {delete diagBarra;       diagBarra = NULL;}
    if(diagBIso)      {delete diagBIso;        diagBIso = NULL;}
    if(diagCabo)      {delete diagCabo;        diagCabo = NULL;}
    if(diagCapacitor) {delete diagCapacitor;   diagCapacitor = NULL;}
    if(diagCarga)     {delete diagCarga;       diagCarga = NULL;}
    if(diagChave)     {delete diagChave;       diagChave = NULL;}
    if(diagRede)      {delete diagRede;        diagRede = NULL;}
    if(diagReg)       {delete diagReg;         diagReg = NULL;}
    if(diagTrecho)    {delete diagTrecho;      diagTrecho = NULL;}
    if(diagTrafo)     {delete diagTrafo;       diagTrafo = NULL;}
//    if(progresso)     {delete progresso;       progresso = NULL;}
}

//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::AgregaLisBarras(TList *lisRede, TList *lisCarga)
   {//variaveis locais
   VTRede *redeSuprida;

   //adiciona as barras dessas redes à rede
   for(int nr = 0; nr < lisRede->Count; nr++)
      {
      redeSuprida = (VTRede*) lisRede->Items[nr];
      CopiaTList(redeSuprida->LisBarra(),lisCarga);
      }
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void  __fastcall TDiagTopo::Inicia(void)
{
    /* TODO : fazer ler todos os arquivos de config */
    //inicia a rede para diagnostico
    //PreparaRedeDiag();
    //inicializa os objetos
    diagArranjo->Inicia();
    diagBarra->Inicia();
    diagBIso->Inicia();
    diagCabo->Inicia();
    diagCapacitor->Inicia();
    diagCarga->Inicia();
    diagChave->Inicia();
    diagRede->Inicia();
    diagReg->Inicia();
    diagTrecho->Inicia();
    diagTrafo->Inicia();
}

//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::LeCfgDiagEqpto(int tipo_DGEQPTO)
{
   switch (tipo_DGEQPTO)
   {
   case dgeBARRA:
      /* TODO : implentar pro tipo barra */
      break;
   case dgeTRECHO:
      diagTrecho->LeArquivo();
      break;

   default:
      ;
   }
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagTopo::ListaRedesAlimentadas(VTRede *supridora,
                                                                  TList *lisEXT)
   {//variaveis locais
   VTRedes  *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTRede   *rede;
   TList    *lisRede = redes->LisRede();
   TList    *lisBarras = supridora->LisBarra();
   VTBarra  *barra;

   //varre todas as redes
   for(int nr = 0; nr < lisRede->Count; nr++)
      {
      rede = (VTRede*) lisRede->Items[nr];
      barra = rede->BarraInicial();
      //se existir a barra inicia
      if(barra != NULL)
         {//verifica se a barra pertence à lista de barras da supridora
         if(lisBarras->IndexOf(barra) >= 0)
            {//adiciona a rede analisada À lista de redes alimentadas
            lisEXT->Add(rede);
            }
         }
      }
   }

//---------------------------------------------------------------------------
TDiagArranjo* __fastcall TDiagTopo::PM_GetDGArranjo(void)
{
   return(diagArranjo);
}

//---------------------------------------------------------------------------
TDiagBarra* __fastcall TDiagTopo::PM_GetDGBarra(void)
{
   return(diagBarra);
}

//---------------------------------------------------------------------------
TDiagBIso* __fastcall TDiagTopo::PM_GetDGBIso(void)
{
   return(diagBIso);
}

//---------------------------------------------------------------------------
TDiagCabo* __fastcall TDiagTopo::PM_GetDGCabo(void)
{
   return(diagCabo);
}

//---------------------------------------------------------------------------
TDiagCapacitor* __fastcall TDiagTopo::PM_GetDGCapacitor(void)
{
   return(diagCapacitor);
}

//---------------------------------------------------------------------------
TDiagCarga* __fastcall TDiagTopo::PM_GetDGCarga(void)
{
   return(diagCarga);
}

//---------------------------------------------------------------------------
TDiagChave* __fastcall TDiagTopo::PM_GetDGChave(void)
{
   return(diagChave);
}

//---------------------------------------------------------------------------
TDiagRede* __fastcall TDiagTopo::PM_GetDGRede(void)
{
   return(diagRede);
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
TDiagReg* __fastcall TDiagTopo::PM_GetDGReg(void)
{
   return(diagReg);
}

//---------------------------------------------------------------------------
TDiagTrecho* __fastcall TDiagTopo::PM_GetDGTrecho(void)
{
   return(diagTrecho);
}

//---------------------------------------------------------------------------
TDiagTrafo* __fastcall TDiagTopo::PM_GetDGTrafo(void)
{
   return(diagTrafo);
}
//---------------------------------------------------------------------------
bool  __fastcall TDiagTopo::PM_GetRedePreparada(void)
   {
   return (redePreparada);
   }
//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::PreparaRedeDiag(void)
   {//variaveis locais
   VTZonas     *zonas =  (VTZonas*) apl->GetObject(__classid(VTZonas));
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTNode      *node;
   VTRede      *rede  =  redes->RedeIsolada();
   VTRede      *redeSuprida;
   VTBarra     *barra;
   TList       *lisBarra = new TList;
   TList       *lisNode;
   TList       *lisRedes = new TList;;
   TObject     *object;
   AnsiString  class_name;

   if(progresso) progresso->Start(progTEXTO);
   if(progresso) progresso->Add("Preparando rede para diagnóstico...") ;
   //inicializa o Zonas
   zonas->ChaveAbertaEnabled = false;
   zonas->Executa();
   //limpa as zonas de todas barras antes de executar o zonas
   //percorre todas as barras
   if(rede)
      {
      //caso tenha barras isoladas
      if(rede->LisBarra()->Count > 0)
         {
         CopiaTList(rede->LisBarra(),lisBarra);
         //inicia arvore de redes
         //arvore->TipoArvore = arvorePERDATEC;
         arvore->Inicia(redes);
         lisNode = arvore->LisNodeRaiz();
         //percorre os nodes raizes à procura do Redes Isoladas
         node = arvore->ExisteNode("Redes Isoladas");
         if(node)
            {//pega redes do node
            node->LisObjectNode(lisRedes);
            //pega as redes dos nodes a jusante
            node->LisObjectJusante(lisRedes);
            //sendo que possui algum item
            if (lisRedes->Count > 0)
               {
               AgregaLisBarras(lisRedes,lisBarra);
               }
            }
         }
      for(int nb = 0; nb < lisBarra->Count; nb++)
         {
         barra = (VTBarra*) lisBarra->Items[nb];
         //monta uma lista de barras sem zona
         barra->zona = NULL;
         }
      }
   //poe NULL nas barras com zonas indefinidas
   ZonasIndefParaNULL();
   //tira as zonas das redes estaticas desconexas
   PreparaSecundarios();
   //para o aguarde
   if(progresso) progresso->Stop();
   //deleta lista
   //if(lisRedesSupridas) {delete lisRedesSupridas;  lisRedesSupridas = NULL;}
   if(lisRedes)   {delete lisRedes;  lisRedes = NULL;}
   if(lisBarra)   {delete lisBarra;   lisBarra = NULL;}
   //marca rede como preparada
   redePreparada = true;
   }

//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::PreparaRedeUso(void)
   {//variaveis locais
   VTZonas     *zonas =  (VTZonas*) apl->GetObject(__classid(VTZonas));

   if(progresso) progresso->Start(progTEXTO);
   if(progresso) progresso->Add("Preparando rede para uso...") ;
   //inicializa o Zonas
   zonas->ChaveAbertaEnabled = false;
   zonas->Executa();
   if(progresso) progresso->Stop();

   }

//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::PreparaSecundarios(void)
   {//variáveis locais
   TList        *lisSEC, *lisTodasBarras;
   TList        *lisAux, *lisBarrasOrdenadas, *lisLigacao;
   VTSecundario *secundario;
   VTRedes      *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRadial     *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
   VTBarra      *barra;

   //cria as listas
   lisBarrasOrdenadas = new TList;
   lisLigacao = new TList;
   lisAux = new TList;
   //bloco try catch
   try
      {
      if (radial == NULL)
      {//cria objeto e insere em Apl
      apl->Add(radial = DLL_NewObjRadial(apl));
         }
      //inicia objeto Radial
      radial->Inicia(redes);
      //obtém lista de redes secundárias
      lisSEC = radial->LisSecundario();
      //percorre todos secundarios
      for (int ns = 0; ns < lisSEC->Count; ns++)
         {
         secundario = (VTSecundario*)lisSEC->Items[ns];
         secundario->Ordena(lisBarrasOrdenadas,lisLigacao);
         lisTodasBarras = secundario->Rede->LisBarra();
         //junta na lista auxiliar, todas as barras da rede que não foram ordenadas
         lisAux->Assign(lisBarrasOrdenadas,laDestUnique,lisTodasBarras);
         //percorre todas as
         for(int nb = 0; nb < lisAux->Count; nb++)
            {
            barra = (VTBarra*) lisAux->Items[nb];
            //monta uma lista de barras sem zona
            barra->zona = NULL;
            }
         }
      }
   catch (Exception &e)
      {
      //ainda nao sei ahaha
      }
   //destroi listas
   if(lisBarrasOrdenadas) {delete lisBarrasOrdenadas; lisBarrasOrdenadas = NULL;}
   if(lisLigacao) {delete lisLigacao; lisLigacao = NULL;}
   if(lisAux) {delete lisAux; lisAux = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::PreExecuta(void)
   {
   if(!redePreparada)
      {//caso a rede ainda não esteja preparada
      PreparaRedeDiag();
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::SalvaCfgDiagEqpto(int tipo_DGEQPTO)
{
   switch (tipo_DGEQPTO)
   {
   case dgeBARRA:
      /* TODO : implentar pro tipo barra */
      break;
   case dgeTRECHO:
      diagTrecho->SalvaArquivo();
      break;

   default:
      ;
   }
}
//---------------------------------------------------------------------------
void __fastcall  TDiagTopo::ZonasIndefParaNULL(void)
   {//variaveis locais
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTBarra     *barra;
   VTZona      *zona;
   TList       *lisBAR;

   lisBAR = redes->LisBarra();
   //percorre todas barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*) lisBAR->Items[nb];
      zona = barra->zona;
      if(zona != NULL)
         {
         if(zona->Tipo == zonaINDEFINIDA)
            {//torna nula todos ponteiros pra zona indefinida
            barra->zona = NULL;
            }
         }
      }
   }
//---------------------------------------------------------------------------

