//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDensidade.h"
#include "TPtocarga.h"
#include "VTResumoCarga.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTNode.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTPrimario.h>
#include <Fontes\Radial\VTSecundario.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Curvas.h>
#include <DLL_Inc\Historico.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Rede.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTDensidade* __fastcall NewObjDensidade(VTApl *apl_owner)
   {
   try{
      return(new TDensidade(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TDensidade::TDensidade(VTApl *apl_owner)
   {
   //salva ponteiros p/ objetos
   apl       = apl_owner;
   classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   //se necessário, cria objeto Arvore
   //if (arvore == NULL) arvore = DLL_NewObjArvore(apl);
   //cria listas
   lisEQP    = new TList();
   lisPTC    = new TList();
   lisNODE   = new TList();
   lisRESUMO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDensidade::~TDensidade(void)
   {
   //destrói arvore e os objetos VTResumoCarga armazenados na Arvore
   //if (arvore) {arvore->ClearObjects(); delete arvore; arvore = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisNODE) {delete lisNODE; lisNODE = NULL;}
   //destrói lista e seus objetos
   if (lisPTC) {LimpaTList(lisPTC); delete lisPTC; lisPTC = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TDensidade::CopiaCargasPtocarga(TList *lisCAR)
   {
   //variáveis locais
   TPtocarga *ptocarga;

   //reinicia lisEQP
   lisCAR->Clear();
   //loop p/ todos Ptocarga
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptocarga = (TPtocarga*)lisPTC->Items[n];
      if (ptocarga->num_cons_res > 0) {ptocarga->cons_res->IP_NumLamp = ptocarga->num_cons_res; lisCAR->Add(ptocarga->cons_res); ptocarga->cons_res = NULL; }
      if (ptocarga->num_cons_com > 0) {ptocarga->cons_com->IP_NumLamp = ptocarga->num_cons_com; lisCAR->Add(ptocarga->cons_com); ptocarga->cons_com = NULL; }
      if (ptocarga->num_cons_ind > 0) {ptocarga->cons_ind->IP_NumLamp = ptocarga->num_cons_ind; lisCAR->Add(ptocarga->cons_ind); ptocarga->cons_ind = NULL; }
      if (ptocarga->num_cons_rur > 0) {ptocarga->cons_rur->IP_NumLamp = ptocarga->num_cons_rur; lisCAR->Add(ptocarga->cons_rur); ptocarga->cons_rur = NULL; }
      if (ptocarga->num_cons_out > 0) {ptocarga->cons_out->IP_NumLamp = ptocarga->num_cons_out; lisCAR->Add(ptocarga->cons_out); ptocarga->cons_out = NULL; }
      if (ptocarga->num_cons_a4  > 0) {ptocarga->cons_a4->IP_NumLamp  = ptocarga->num_cons_a4 ; lisCAR->Add(ptocarga->cons_a4 ); ptocarga->cons_a4  = NULL; }
      if (ptocarga->num_cons_ip  > 0) {ptocarga->cons_ip->IP_NumLamp  = ptocarga->num_cons_ip ; lisCAR->Add(ptocarga->cons_ip ); ptocarga->cons_ip  = NULL; }
      }
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TDensidade::CriaCarga(VTBarra *barra, int classe_tag, AnsiString codigo)
   {
   //variáveis locais
   VTCarga *carga;

   //cria Carga
   if ((carga  = DLL_NewObjCarga(patamares->NumPatamar())) == NULL) return(NULL);
   carga->Codigo = codigo;
   //cria Curva de demanda p/ cada Carga
   carga->DefineCurva(CriaCurvaPQ());
   //define Classe de cada Carga
   carga->DefineClasse(classes->ExisteClasseTag(classe_tag));
   //associa Cargas com Barra SEM INCLUI-LAS na Barra
   carga->pbarra = barra;

   return(carga);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TDensidade::CriaCurvaPQ(void)
   {
   //variáveis locais
   VTCurva *curva;

   //cria Curva
   curva = DLL_NewObjCurva();
   //inicia dados
   curva->Id            = FalsoId();
   curva->Codigo        = "";
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Escala        = patamares->NumPatamar();

   return(curva);
   }

//---------------------------------------------------------------------------
bool __fastcall TDensidade::CriaLisPtocarga(VTRede *rede)
   {
   //variáveis locais
   AnsiString      codigo;
   complex<double> s_mva;
   TPtocarga       *ptocarga;
   VTBarra         *barra;
   VTCarga         *carga;

   //obtém lista de Cargas da Rede
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoCARGA);
   //loop p/ todas Cargas
   for (int nc = 0; nc < lisEQP->Count; nc++)
      {
      carga = (VTCarga*)lisEQP->Items[nc];
      //verifica se é uma rede secundária
      if (rede->TipoRede->Segmento == redeSEC)
         {//cria todos os Ptocarga na Barra inicial da rede
         barra  = rede->BarraInicial();
         codigo = rede->Codigo;
         }
      else
         {//cria Ptocarga na Barra da Carga
         barra  = carga->pbarra;
         codigo = carga->Codigo;
         }
      //protecao
      if (barra == NULL) continue;
      //verifica se existe um Ptocarga na coordenada da Carga
      if ((ptocarga = ExistePtocarga(rede, barra)) == NULL)
         {//cria um novo Ptocarga
         lisPTC->Add(ptocarga = InserePtocarga(rede, barra, codigo));
         }
      //verifica a classe do consumidor
      switch(carga->classe->Tag)
         {
         case consRESIDENCIAL:
            ptocarga->num_cons_res++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_res);
            break;
         case consCOMERCIAL:
            ptocarga->num_cons_com++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_com);
            break;
         case consINDUSTRIAL:
            ptocarga->num_cons_ind++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_ind);
            break;
         case consRURAL:
            ptocarga->num_cons_rur++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_rur);
            break;
         case consOUTROS:
            ptocarga->num_cons_out++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_out);
            break;
         case consIP:
            ptocarga->num_cons_ip++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_ip);
            break;
         case consA4:
            ptocarga->num_cons_a4++;
            SomaDemandaCarga1NaCarga2(carga, ptocarga->cons_a4);
            break;
         default: continue;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDensidade::Executa(TDateTime date)
   {
   //variáveis locais
   VTResumoCarga *resumo;

   try{//inicia objeto Arvore
      arvore->Inicia((VTRedes*)apl->GetObject(__classid(VTRedes)));
      //remonta objetos Nodes, trocando seus objetos Rede pelos correspondentes ResumoCarga
      if (! ReiniciaArvoreComResumoCarga()) return(false);
      //monta conjunto de Cargas para cada ResumoCarga
      lisRESUMO->Clear();
      arvore->LisAllObject(lisRESUMO);
      for (int n = 0; n < lisRESUMO->Count; n++)
         {
         resumo = (VTResumoCarga*)lisRESUMO->Items[n];
         //monta lista lisPTC com objetos PtoCarga para a Rede do ResumoCarga
         LimpaTList(lisPTC);
         CriaLisPtocarga(resumo->Rede);
         //copia Cargas dos objetos Ptocarga da Rede para o ResumoCarga
         CopiaCargasPtocarga(resumo->LisCarga());
         //destrói objetos Ptocarga criados
         LimpaTList(lisPTC);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
TPtocarga* __fastcall TDensidade::ExistePtocarga(VTRede *rede, VTBarra *barra)
   {
   //variáveis locais
   TPtocarga *ptocarga;

   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptocarga = (TPtocarga*)lisPTC->Items[n];
      if ((ptocarga->rede == rede)&&(ptocarga->barra == barra)) return(ptocarga);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TPtocarga* __fastcall TDensidade::InserePtocarga(VTRede *rede, VTBarra *barra, AnsiString codigo)
   {
   //variáveis locais
   TPtocarga  *ptocarga;

   //cria um novo Ptocarga
   ptocarga = new TPtocarga(rede, barra);
   //cria Cargas com respectivas Cruvas
   ptocarga->cons_res  = CriaCarga(barra, consRESIDENCIAL, codigo);
   ptocarga->cons_com  = CriaCarga(barra, consCOMERCIAL, codigo);
   ptocarga->cons_ind  = CriaCarga(barra, consINDUSTRIAL, codigo);
   ptocarga->cons_rur  = CriaCarga(barra, consRURAL, codigo);
   ptocarga->cons_out  = CriaCarga(barra, consOUTROS, codigo);
   ptocarga->cons_a4   = CriaCarga(barra, consA4, codigo);
   ptocarga->cons_ip   = CriaCarga(barra, consIP, codigo);

   return(ptocarga);
   }
/*
//---------------------------------------------------------------------------
VTArvore* __fastcall TDensidade::PM_GetArvore(void)
   {
   return(arvore);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TDensidade::ReiniciaArvoreComResumoCarga(void)
   {
   //variáveis locais
   TList         *lisREDE;
   VTNode        *node;
   VTRede        *rede;
   VTResumoCarga *resumo;

   try{////inicia lista com todos Nodes da Arvore
      lisNODE->Clear();
      arvore->LisAllNode(lisNODE);
      //remonta objetos Nodes, trocando seus objetos Rede pelos correspondentes ResumoCarga
      for (int n = 0; n < lisNODE->Count; n++)
         {
         node = (VTNode*)lisNODE->Items[n];
         lisRESUMO->Clear();
         //loop p/ todas as Redes do Node
         lisREDE = node->LisObjectNode();
         for (int nr = 0; nr < lisREDE->Count; nr++)
            {
            rede = (VTRede*)lisREDE->Items[nr];
            //cria objeto ResumoCarga para a Rede e insere em lisEQP
            lisRESUMO->Add(resumo = NewObjResumoCarga());
            resumo->Rede = rede;
            resumo->Node = node;
            }
         //reinicia lista de objetos do Node
         node->ReiniciaLisObject(lisRESUMO);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TDensidade::SomaDemandaCarga1NaCarga2(VTCarga *carga1, VTCarga *carga2)
   {
   //variáveis locais
   complex<double> s_mva;

   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//determina demanda da Carga2
      s_mva = demanda->S_mva[carga2][ind_pat];
      //acumula demanda da Carga1 no Patamar
      s_mva = s_mva + demanda->S_mva[carga1][ind_pat];
      //atualiza demanda da Carga2
      demanda->S_mva[carga2][ind_pat] = s_mva;
      }
   }
//---------------------------------------------------------------------------
//eof

