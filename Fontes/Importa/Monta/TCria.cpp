//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TCria.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\..\Arranjo\VTCabo.h"
#include "..\..\Arranjo\VTPtoFixacao.h"
#include "..\..\Arranjo\VTSuporte.h"
//#include "..\..\BDado\VTBDrede.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Curvas\VTCurvas.h"
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\MontaZ\VTMontaZ.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTArea.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCanal.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCapserie.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTCargaNL.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTCluster.h"
#include "..\..\Rede\VTDadosExt.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTFiltro.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTMatC.h"
#include "..\..\Rede\VTMatZ.h"
#include "..\..\Rede\VTMedidor.h"
#include "..\..\Rede\VTMutua.h"
#include "..\..\Rede\VTReator.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrafoZZ.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Rede\VTTipoChave.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTipos.h"
#include "..\..\Topologia\VTTopologia.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\Zona\VTZonas.h"
#include "..\..\..\DLL_Inc\Arranjo.h"
#include "..\..\..\DLL_Inc\Curvas.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTCria* NewObjCria(VTApl *apl)
   {
   return(new TCria(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCria::TCria(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   curvas    = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   //lista de Eqptos criados nas Obras
   lisEQPTO_OBRA = NULL;
   //cria objetos locais
   lisTMP      = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TCria::~TCria(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::CriaArranjoProprio_Z0Z1(strARRANJO1 &str)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTMontaZ  *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //cria um novo objeto Arranjo do tipo arranjoZ0Z1
   if ((arranjo = DLL_NewObjArranjo()) == NULL) return(NULL);
   //define Arranjo como novo
   arranjo->Status[sttNOVO] = true;
   arranjo->Owner           = NULL;
   //verifica o tipo de configuração
   if (str.tipo_cfg == arranjoZ0Z1)
      {//define dados do Arranjo
      arranjo->Codigo    = "";
      arranjo->Extern_id = "";
      arranjo->TipoCfg   = arranjoZ0Z1;
      arranjo->Fases     = faseABCN;
      arranjo->z0        = str.cfg_arranjoZ0Z1.z0;
      arranjo->z1        = str.cfg_arranjoZ0Z1.z1;
      arranjo->c0        = str.cfg_arranjoZ0Z1.c0;
      arranjo->c1        = str.cfg_arranjoZ0Z1.c1;
      arranjo->Iadm_amp  = str.cfg_arranjoZ0Z1.iadm_amp;
      //proteção: r0 >= r1
      if (arranjo->z0.r < arranjo->z1.r) arranjo->z0.r = arranjo->z1.r;
      //proteção: x0 >= x1
      if (arranjo->z0.x < arranjo->z1.x) arranjo->z0.x = arranjo->z1.x;
      //proteção: z0 > 0
      if (IsDoubleZero(arranjo->z0.r)&&IsDoubleZero(arranjo->z0.x))
         {
         arranjo->z0 = arranjo->z1;
         }
      //proteção: z1 > 0
      if (IsDoubleZero(arranjo->z1.r)&&IsDoubleZero(arranjo->z1.x))
         {
         arranjo->z1 = arranjo->z0;
         }
      //proteção: (z0 > 0)ou (z1 > 0)
      if (IsDoubleZero(arranjo->z0.r)&&IsDoubleZero(arranjo->z0.x)&&
          IsDoubleZero(arranjo->z1.r)&&IsDoubleZero(arranjo->z1.x))
         {
         arranjo->z0.r     = 0.02;
         arranjo->z0.x     = 0.05;
         arranjo->c0       = 0;
         arranjo->z1.r     = 0.02;
         arranjo->z1.x     = 0.05;
         arranjo->c1       = 0;
         }
      }
   else
      {//define dados default p/ o Arranjo
      arranjo->Codigo   = "";
      arranjo->TipoCfg  = arranjoZ0Z1;
      arranjo->Fases    = faseABCN;
      arranjo->z0.r     = 0.02;
      arranjo->z0.x     = 0.05;
      arranjo->c0       = 0;
      arranjo->z1.r     = 0.02;
      arranjo->z1.x     = 0.05;
      arranjo->c1       = 0;
      arranjo->Iadm_amp = 350;
      }
   //monta matriz de impedâncias
   if (! montaZ->Executa(arranjo)) return(NULL);
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::CriaArranjoTipicoCABOxFASE(VTSuporte *suporte, VTCabo *cabo[])
   {
   //variáveis locais
   int           tag = 0;
   int           num_cabo = 0;
   VTPtoFixacao *pto_fixacao;
   VTArranjo    *arranjo;
   VTMontaZ     *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //proteção: verifica se há Cabos definidos
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      if (cabo[nf] != NULL) num_cabo++;
      }
   if (num_cabo < 2) return(NULL);
   //cria um novo Arranjo
   if ((arranjo = DLL_NewObjArranjo()) == NULL) return(NULL);
   //define Arranjo como novo
   arranjo->Status[sttNOVO] = true;
   arranjo->Owner           = NULL;
   //inicia dados
   arranjo->Id        = FalsoId();
   arranjo->TipoCfg   = arranjoCABOxFASE;
   arranjo->Codigo    = DefineCodigoArranjo();
   arranjo->Extern_id = "";
   //define fases do Arranjo em função de existência de cabos nas fases
   if (cabo[0] != NULL) tag = fases->Fase1UniaoFase2(tag, faseA);
   if (cabo[1] != NULL) tag = fases->Fase1UniaoFase2(tag, faseB);
   if (cabo[2] != NULL) tag = fases->Fase1UniaoFase2(tag, faseC);
   if (cabo[3] != NULL) tag = fases->Fase1UniaoFase2(tag, faseN);
//proteção temporária
//int PENDENCIA_MONTAGEM_DE_ARRANJO=1;
//if (tag != faseABCN) {tag = faseABCN;}
   //define fases do Arranjo
   arranjo->Fases = tag;
   //define Suporte
   arranjo->suporte  = suporte;
   //define PtoFixacao e Cabo do Arranjo
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//verifica se tem a fase
      if (cabo[nf] == NULL) continue;
      //verifica se existe PtoFixacao p/ fase
      if ((pto_fixacao = suporte->ExistePtoFixacaoIndex(nf)) == NULL) return(NULL);
      //define PtoFixacao e Cabo p/ fase do Arranjo
      arranjo->DefPtoFixacaoFase(nf, pto_fixacao);
      arranjo->DefCaboFase(nf, cabo[nf]);
      }
   //calcula Matriz de impedâncias do Arranjo
   if (! montaZ->Executa(arranjo))
      {//destrói arranjo
      delete arranjo;
      return(NULL);
      }
   //insere Arranjo em Arranjos
   arranjos->InsereArranjo(arranjo);
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::CriaArranjoTipicoCABO_Z0Z1(VTSuporte *suporte, VTCabo *cabo)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTMontaZ  *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //cria um novo Arranjo
   if ((arranjo = DLL_NewObjArranjo()) == NULL) return(NULL);
   //define Arranjo como novo
   arranjo->Status[sttNOVO] = true;
   arranjo->Owner           = NULL;
   //inicia dados
   arranjo->Id        = FalsoId();
   arranjo->TipoCfg   = arranjoCABO_Z0Z1;
   arranjo->Fases     = faseABCN;
   arranjo->Codigo    = DefineCodigoArranjo();
   arranjo->Extern_id = "";
   //define Suporte
   arranjo->suporte  = suporte;
   //define CaboZ0Z1
   arranjo->caboZ0Z1 = cabo;
   //calcula Matriz de impedâncias do Arranjo
   if (! montaZ->Executa(arranjo))
      {//destrói arranjo
      delete arranjo;
      return(NULL);
      }
   //insere Arranjo em Arranjos
   arranjos->InsereArranjo(arranjo);
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::CriaArranjoTipico_Z0Z1(strARRANJO1 &str)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTMontaZ  *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //cria um novo objeto Arranjo do tipo arranjoZ0Z1
   if ((arranjo = DLL_NewObjArranjo()) == NULL) return(NULL);
   //define Arranjo como novo
   arranjo->Status[sttNOVO] = true;
   arranjo->Owner           = NULL;
   //verifica o tipo de configuração
   if (str.tipo_cfg == arranjoZ0Z1)
      {//define dados do Arranjo
      arranjo->Codigo    = str.codigo;
      arranjo->Extern_id = str.extern_id;
      arranjo->TipoCfg   = arranjoZ0Z1;
      arranjo->Vmax_KV   = 0.;
      arranjo->Fases     = faseABCN;
      arranjo->z0        = str.cfg_arranjoZ0Z1.z0;
      arranjo->z1        = str.cfg_arranjoZ0Z1.z1;
      arranjo->c0        = str.cfg_arranjoZ0Z1.c0;
      arranjo->c1        = str.cfg_arranjoZ0Z1.c1;
      arranjo->Iadm_amp  = str.cfg_arranjoZ0Z1.iadm_amp;
      //proteção: r0 >= r1
      if (arranjo->z0.r < arranjo->z1.r) arranjo->z0.r = arranjo->z1.r;
      //proteção: x0 >= x1
      if (arranjo->z0.x < arranjo->z1.x) arranjo->z0.x = arranjo->z1.x;
      //proteção: z0 > 0
      if (IsDoubleZero(arranjo->z0.r)&&IsDoubleZero(arranjo->z0.x))
         {
         arranjo->z0 = arranjo->z1;
         }
      //proteção: z1 > 0
      if (IsDoubleZero(arranjo->z1.r)&&IsDoubleZero(arranjo->z1.x))
         {
         arranjo->z1 = arranjo->z0;
         }
      //proteção: (z0 > 0)ou (z1 > 0)
      if (IsDoubleZero(arranjo->z0.r)&&IsDoubleZero(arranjo->z0.x)&&
          IsDoubleZero(arranjo->z1.r)&&IsDoubleZero(arranjo->z1.x))
         {
         arranjo->z0.r     = 0.02;
         arranjo->z0.x     = 0.05;
         arranjo->c0       = 0;
         arranjo->z1.r     = 0.02;
         arranjo->z1.x     = 0.05;
         arranjo->c1       = 0;
         }
      }
   else
      {//define dados default p/ o Arranjo
      arranjo->Codigo   = "";
      arranjo->TipoCfg  = arranjoZ0Z1;
      arranjo->Fases    = faseABCN;
      arranjo->z0.r     = 0.02;
      arranjo->z0.x     = 0.05;
      arranjo->c0       = 0;
      arranjo->z1.r     = 0.02;
      arranjo->z1.x     = 0.05;
      arranjo->c1       = 0;
      arranjo->Iadm_amp = 350;
      }
   //monta matriz de impedâncias
   if (! montaZ->Executa(arranjo)) return(NULL);
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TCria::CriaCurvaCanal(strCANAL &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   union    {
            double vet[nvCURVA_PQVT];
            struct {double p, q, v, t;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = str.curva.id;
   curva->Codigo        = str.codigo;
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = nvCURVA_PQVT;
   curva->Tipica        = false;
   curva->Classe        = NULL;
   //define pontos da Curva
   for (int np = 0; np < str.curva.num_pat; np++)
       {
       hm_ini = str.curva.patamar[np].hm_ini;
       hm_fim = str.curva.patamar[np].hm_fim;
       valor.var.p = str.curva.patamar[np].valor[indP];
       valor.var.q = str.curva.patamar[np].valor[indQ];
       valor.var.v = str.curva.patamar[np].valor[indV];
       valor.var.t = str.curva.patamar[np].valor[indT];
       curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
       }
   return(curva);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TCria::CriaCurvaCapacitor(strCAPACITOR &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   union    {
            double vet[nvCURVA_PQ];
            struct {double p, q;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = str.curva.id;
   curva->Codigo        = str.codigo;
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Tipica        = false;
   //curva->Eqpto       = pger;
   curva->Classe        = NULL;
   //define pontos da Curva
   for (int np = 0; np < str.curva.num_pat; np++)
       {
       hm_ini      = str.curva.patamar[np].hm_ini;
       hm_fim      = str.curva.patamar[np].hm_fim;
       valor.var.p = 0;
       //valor.var.q = str.demanda[np].q_mvar;
       valor.var.q = str.curva.patamar[np].valor[indQ];
       curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
       }
   return(curva);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TCria::CriaCurvaCarga(strCARGA &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   union    {
            double vet[nvCURVA_PQ];
            struct {double p, q;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = str.curva.id;
   curva->Codigo        = str.codigo;
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Tipica        = false;
   //curva->Eqpto       = pger;
   curva->Classe        = NULL;
   //define pontos da Curva
   for (int np = 0; np < str.curva.num_pat; np++)
       {
       hm_ini      = str.curva.patamar[np].hm_ini;
       hm_fim      = str.curva.patamar[np].hm_fim;
       //valor.var.p = str.demanda[np].p_mw;
       //valor.var.q = str.demanda[np].q_mvar;
       valor.var.p = str.curva.patamar[np].valor[indP];
       valor.var.q = str.curva.patamar[np].valor[indQ];
       curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
       }
   return(curva);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TCria::CriaCurvaGerador(strGERADOR &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   union    {
            double vet[nvCURVA_PQVT];
            struct {double p, q, v, t;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = str.curva.id;
   curva->Codigo        = str.codigo;
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = nvCURVA_PQVT;
   curva->Tipica        = false;
   //curva->Eqpto       = pger;
   curva->Classe        = NULL;
   //define pontos da Curva (unck@20080507)
   for (int np = 0; np < str.curva.num_pat; np++)
       {
       hm_ini = str.curva.patamar[np].hm_ini;
       hm_fim = str.curva.patamar[np].hm_fim;
       valor.var.p = str.curva.patamar[np].valor[indP];
       valor.var.q = str.curva.patamar[np].valor[indQ];
       valor.var.v = str.curva.patamar[np].valor[indV];
       valor.var.t = str.curva.patamar[np].valor[indT];
       //proteção
       if (valor.var.v <= 0.) valor.var.v = 1.0;
       curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
       }
   return(curva);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16916628
VTCurva* __fastcall TCria::CriaCurvaRede(strREDE &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   union    {
            double vet[nvCURVA_PQ];
            struct {double p, q;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = str.dados_ext.curva_demanda.id;
   curva->Codigo        = str.dados_ext.curva_demanda.codigo;
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Tipica        = false;
   //curva->Eqpto       = pger;
   curva->Classe        = NULL;
   //define pontos da Curva
   for (int np = 0; np < str.dados_ext.curva_demanda.num_pat; np++)
       {
       hm_ini      = str.dados_ext.curva_demanda.patamar[np].hm_ini;
       hm_fim      = str.dados_ext.curva_demanda.patamar[np].hm_fim;
       //valor.var.p = str.demanda[np].p_mw;
       //valor.var.q = str.demanda[np].q_mvar;
       valor.var.p = str.dados_ext.curva_demanda.patamar[np].valor[indP];
       valor.var.q = str.dados_ext.curva_demanda.patamar[np].valor[indQ];
       curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
       }
   return(curva);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TCria::CriaCurvaSuprimento(strSUPRIMENTO &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   union    {
            double vet[nvCURVA_PQVT];
            struct {double p, q, v, t;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = str.curva.id;
   curva->Codigo        = str.codigo;
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = nvCURVA_PQVT;
   curva->Tipica        = false;
   //curva->Eqpto       = psup;
   curva->Classe        = NULL;
   //define pontos da Curva (unck@20080507)
   for (int np = 0; np < str.curva.num_pat; np++)
       {
       hm_ini      = str.curva.patamar[np].hm_ini;
       hm_fim      = str.curva.patamar[np].hm_fim;
       valor.var.p = str.curva.patamar[np].valor[indP];
       valor.var.q = str.curva.patamar[np].valor[indQ];
       valor.var.v = str.curva.patamar[np].valor[indV];
       valor.var.t = str.curva.patamar[np].valor[indT];
       //proteção
       if (valor.var.v <= 0.) valor.var.v = 1.0;
       curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
       }
   return(curva);
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TCria::DefineCodigoArranjo(void)
   {
   //variáveis locais
   AnsiString codigo;
   int        count = 1;

   do {//cria um novo código
      codigo.sprintf("Arranjo_%04d", count++);
      } while(arranjos->ExisteArranjo(codigo) != NULL);
   return(codigo);
   }

//-----------------------------------------------------------------------------
void __fastcall TCria::DefLisEqptoObra(TList *lisEXT)
   {
   lisEQPTO_OBRA = lisEXT;
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::ExisteArranjoTipico(int arranjo_id)
   {
   return(arranjos->ExisteArranjo(arranjo_id));
   }
   
//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::ExisteArranjoTipico(AnsiString cod_arranjo, int cfg_arranjo)
   {
   //variáveis locais
   VTArranjo *arranjo = NULL;

   //verifica se já existe arranjo na lista
   arranjo = arranjos->ExisteArranjo(cod_arranjo, cfg_arranjo);

   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::ExisteArranjoTipico_CABO_FASE(strARRANJO1 &str, VTTipoRede *tiporede)
   {
   //variáveis locais
   int       num_fase = 0;
   VTArranjo *arranjo;
   VTCabo    *cabo[MAX_FASE];
   VTSuporte *suporte;

   //determina Suporte do Arranjo
   if (str.cfg_suporte_cabo.estgeo_id.IsEmpty())
      {
      if ((suporte = arranjos->ExisteSuporte(tiporede->Codigo)) == NULL)
         {
         plog->Add("Erro: não existe poste/torre p/ Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      }
   else
      {
      if ((suporte = arranjos->ExisteSuporte(str.cfg_suporte_cabo.estgeo_id)) == NULL)
         {
         plog->Add("Erro: não existe poste/torre " + AnsiQuotedStr(str.cfg_suporte_cabo.estgeo_id, '\'') +
                   " p/ Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      }
   //determina Cabos do Arranjo
   for (int nf = 0; nf < MAX_FASE; nf++) cabo[nf] = NULL;
   //determina Cabo da fase A
   if (! str.cfg_suporte_cabo.cod_caboA.IsEmpty())
      {
      if ((cabo[0] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboA)) == NULL)
         {
         plog->Add("Erro: não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboA, '\'') +
                   " p/ fase A do Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      num_fase++;
      }
   //determina Cabo da fase B
   if (! str.cfg_suporte_cabo.cod_caboB.IsEmpty())
      {
      if ((cabo[1] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboB)) == NULL)
         {
         plog->Add("Erro: não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboB, '\'') +
                   " p/ fase B do Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      num_fase++;
      }
   //determina Cabo da fase C
   if (! str.cfg_suporte_cabo.cod_caboC.IsEmpty())
      {
      if ((cabo[2] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboC)) == NULL)
         {
         plog->Add("Erro: não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboC, '\'') +
                   " p/ fase C do Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      num_fase++;
      }
   //determina Cabo do neutro
   if (! str.cfg_suporte_cabo.cod_caboN.IsEmpty())
      {
      if ((cabo[3] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboN)) == NULL)
         {
         plog->Add("Erro: não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboN, '\'') +
                   " p/ neutro do Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      }
   //proteção: verifica se existe Cabo de uma única fase e não existe cabo de neutro
   if ((num_fase == 1)&&(cabo[3] == NULL))
      {
      if      (cabo[0] != NULL) cabo[3] = cabo[0];
      else if (cabo[1] != NULL) cabo[3] = cabo[1];
      else if (cabo[2] != NULL) cabo[3] = cabo[2];
      }
   //verifica se existe um Arranjo típico
   arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, cabo[0], cabo[1], cabo[2], cabo[3]);
   if (arranjo == NULL)
      {//cria Arranjo típico
      arranjo = CriaArranjoTipicoCABOxFASE(suporte, cabo);
      }
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::ExisteArranjoTipico_CABO_Z0Z1(strARRANJO1 &str)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTCabo    *cabo;

   //determina Cabo
   if ((cabo = arranjos->ExisteCabo(str.cfg_caboZ0Z1.cod_cabo, caboZ0Z1)) == NULL) return(NULL);
   //verifica se existe Arranjo típico, do tipo arranjoCABO_Z0Z1
   if ((arranjo = arranjos->ExisteArranjoCABO_Z0Z1(cabo)) == NULL)
      {//cria um novo Arranjo típico
      arranjo = CriaArranjoTipicoCABO_Z0Z1(NULL, cabo);
      }
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::ExisteArranjoTipico_Z0Z1(strARRANJO1 &str)
   {
   //variáveis locais
   double    r0, x0, c0, r1, x1, c1;
   VTArranjo *arranjo;

   //verifica se existe Arranjo típico, do tipo arranjoZ0Z1
   r0 = str.cfg_arranjoZ0Z1.z0.r;
   x0 = str.cfg_arranjoZ0Z1.z0.x;
   c0 = str.cfg_arranjoZ0Z1.c0;
   r1 = str.cfg_arranjoZ0Z1.z1.r;
   x1 = str.cfg_arranjoZ0Z1.z1.x;
   c1 = str.cfg_arranjoZ0Z1.c1;
  arranjo = arranjos->ExisteArranjoZ0Z1(r0, x0, c0, r1, x1, c1);

  return(arranjo);
  }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TCria::ExisteBarra(int barra_id)
   {
   //variáveis locais
   VTBarra *barra;
   VTEqpto *eqpto;

   try
	   {//procura em Redes
	   if ((barra = redes->ExisteBarra(barra_id)) != NULL) return(barra);
	   //procura Barra em lisEQPTO_OBRA
	   if (lisEQPTO_OBRA != NULL)
		  {
		  for (int n = 0; n < lisEQPTO_OBRA->Count; n++)
			 {
			 eqpto = (VTEqpto*)lisEQPTO_OBRA->Items[n];
			 if (eqpto->TipoBarra())
				{
				barra = (VTBarra*)eqpto;
				if (barra->Id == barra_id) return(barra);
				}
			 }
		  }
	   }
   catch(Exception &e)
	   {
	   return(NULL);
	   }

   return NULL;
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TCria::ExisteBarra(AnsiString codigo)
   {
   //variáveis locais
   VTBarra *barra;

   //procura em Redes
   barra = redes->ExisteBarra(codigo);

   return(barra);
   }

//-----------------------------------------------------------------------------
VTCarga* __fastcall TCria::ExisteCarga(VTBarra *pbarra)
   {
   //variáveis locais
   VTCarga *pcarga = NULL;

   //obtém lista de Cargas da Barra
   lisTMP->Clear();
   pbarra->LisEqbar(lisTMP, eqptoCARGA);
   if (lisTMP->Count > 0)
      {
      pcarga = (VTCarga*)lisTMP->First();
      }
   //limpa lisTMP
   lisTMP->Clear();
   return(pcarga);
   }

//-----------------------------------------------------------------------------
VTClasse* __fastcall TCria::ExisteClasse(strCARGA &str)
   {
   //variáveis locais
   VTClasse  *classe;

   //verifica se existe a Classe indicada
   if ((classe = classes->ExisteClasseTag(str.classe_cons)) == NULL)
      {//assume Classe igual a consOUTROS
      classe = classes->ExisteClasseTag(consOUTROS);
      }
   return(classe);
   }

//-----------------------------------------------------------------------------
VTChave* __fastcall TCria::ExisteChave(VTRede *rede, int chave_id)
   {
   //variáveis locais
   VTChave *pchave;

   //determina lista de Chaves da Rede
   lisTMP->Clear(); rede->LisLigacao(lisTMP, eqptoCHAVE);
   //procura Chave na lista
   for (int n = 0; n < lisTMP->Count; n++)
      {
      pchave = (VTChave*)lisTMP->Items[n];
      if (pchave->Id == chave_id) return(pchave);
      }
   //não encontrou Chave na lista
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::ExisteEqpto(int eqpto_tipo, AnsiString codigo)
   {
   //variáveis locais
   VTEqpto* eqpto;

   //obtém lista de Eqptos do tipo indicado
   lisTMP->Clear();
   redes->LisEqpto(lisTMP, eqpto_tipo);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqpto = (VTEqpto*)lisTMP->Items[n];
      if (eqpto->Codigo.AnsiCompareIC(codigo) == 0) return(eqpto);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::ExisteEqpto(int eqpto_tipo, int eqpto_id)
   {
   //variáveis locais
   VTEqpto* eqpto;

   //obtém lista de Eqptos do tipo indicado
   lisTMP->Clear();
   redes->LisEqpto(lisTMP, eqpto_tipo);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqpto = (VTEqpto*)lisTMP->Items[n];
      if (eqpto->Id == eqpto_id) return(eqpto);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
int __fastcall TCria::ExisteFase(AnsiString codigo)
   {
   return(fases->ExisteFase(codigo));
   }
   
//-----------------------------------------------------------------------------
int __fastcall TCria::ExisteFaseCarga(AnsiString codigo)
   {
   //verifica se o código possui uma única letra
   if (codigo.Length() == 1)
      {//inclui neutro
      codigo += "N";
      }
   return(ExisteFase(codigo));
   }

//-----------------------------------------------------------------------------
VTRede* __fastcall TCria::ExisteRede(int rede_id)
   {
   //variáveis locais
   VTEqpto *eqpto;
   VTRede  *rede;

   //verifica se a Rede existe em Redes
   if ((rede = redes->ExisteRede(rede_id)) != NULL) return(rede);
   //verifica se a Rede existe em lisEQPTO_OBRA
   for (int n = 0; n < lisEQPTO_OBRA->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQPTO_OBRA->Items[n];
      if (eqpto->Tipo() == eqptoREDE)
         {
         rede = (VTRede*)eqpto;
         if (rede->Id == rede_id) return(rede);
         }
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTTipoChave* __fastcall TCria::ExisteTipoChave(int id, int tipo, AnsiString codigo)
   {
   //variáveis locais
   VTTipoChave *tipochave;

   //dtermina TipoChave por Id
   if ((tipochave = tipos->ExisteTipoChave(id)) == NULL)
      {//não encontrou: procura por tipo ou código
      tipochave = tipos->ExisteTipoChave(codigo, tipo);
      //determina TipoChave indefinida
      tipochave = tipos->ExisteTipoChave("", chaveINDEFINIDA);
      }
   return(tipochave);
   }

/*
//-----------------------------------------------------------------------------
VTTipoRede* __fastcall TCria::ExisteTipoRede(strREDE &str)
   {
   //variáveis locais
   AnsiString codigo;
   VTTipoRede *tiporede = NULL;

   //verifica se foi definido ID do TipoRede
   if (str.tiporede.id > 0)
      {//determina TipoRede por ID
      tiporede = tipos->ExisteTipoRede(str.tiporede.id);
      if (tiporede != NULL) return(tiporede);
      }
   //verifica se não foi definido segmento do TipoRede
   if (str.tiporede.segmento < 0)
      {//tenta identificar segmento do TipoRede
      if (! str.tiporede.codigo.IsEmpty())
         {
         codigo = str.tipo_rede.codigo.LowerCase();
         if      (codigo.AnsiPos("sub")   > 0) str.tipo_rede.segmento = redeSUB;
         else if (codigo.AnsiPos("trans") > 0) str.tipo_rede.segmento = redeTRANS;
         else if (codigo.AnsiPos("ett")   > 0) str.tipo_rede.segmento = redeETT;
         else if (codigo.AnsiPos("etd")   > 0) str.tipo_rede.segmento = redeETD;
         else if (codigo.AnsiPos("pri")   > 0) str.tipo_rede.segmento = redePRI;
         else if (codigo.AnsiPos("sec")   > 0) str.tipo_rede.segmento = redeSEC;
         }
      }
   //determina TipoRede por Codigo ou Segmento
   tiporede = tipos->ExisteTipoRede(str.tipo_rede.codigo, str.tipo_rede.segmento);
   if (tiporede != NULL) return(tiporede);
   //assume primeiro TipoRede disponível
   return((VTTipoRede*)(tipos->LisTipoRede())->First());
   }
*/

//-----------------------------------------------------------------------------
VTTipoRede* __fastcall TCria::ExisteTipoRede(int id, int segmento, AnsiString codigo)
   {
   //variáveis locais
   VTTipoRede *tiporede = NULL;

   //determina TipoRede por ID
   if ((tiporede = tipos->ExisteTipoRede(id)) == NULL)
      {//não encontrou: determina TipoRede por Codigo ou Segmento
      if ((tiporede = tipos->ExisteTipoRede(codigo, segmento)) == NULL)
         {//assume primeiro TipoRede disponível
         tiporede = ((VTTipoRede*)(tipos->LisTipoRede())->First());
        }
      }
   return(tiporede);
   }


//-----------------------------------------------------------------------------
VTArranjo* __fastcall TCria::NewObjArranjo(strARRANJO2 &str)
   {
   //variáveis locais
   int          dim;
   VTArranjo    *arranjo;
   VTCabo       *cabo;
   VTPtoFixacao *pto_fixacao;
   VTSuporte    *suporte;
   VTMontaZ     *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //proteção
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   if (arranjos == NULL) return(NULL);
   //verifica se existe o Arranjo
   if (str.arranjo_id > 0)
      {//procura Arranjo por ID
      if ((arranjo = arranjos->ExisteArranjo(str.arranjo_id)) != NULL) return(arranjo);
      }
   //procura Arranjo por código
   if ((arranjo = arranjos->ExisteArranjo(str.codigo)) != NULL) return(arranjo);
   //cria um novo objeto Arranjo do tipo arranjoZ0Z1
   if ((arranjo = DLL_NewObjArranjo()) == NULL) return(NULL);
   arranjo->Id        = str.arranjo_id;
   arranjo->Codigo    = str.codigo;
   arranjo->Extern_id = str.extern_id;
   arranjo->Owner     = NULL;
   arranjo->Fases     = str.fases;
   arranjo->TipoCfg   = str.tipo_cfg;
   arranjo->ResTerra  = str.res_terra;
   arranjo->Vmin_KV   = str.vmin_kv;
   arranjo->Vmax_KV   = str.vmax_kv;
   arranjo->Color     = str.color;
   arranjo->StatusAsUnsigned = str.status;
   //determina Suporte (se definido)
   //verifica o tipo de configuração
   switch(str.tipo_cfg)
      {
      case arranjoCABOxFASE:
         //determina Suporte
         if ((suporte = arranjos->ExisteSuporte(str.cfg_suporte_cabo.suporte_id)) == NULL) return(NULL);
         //define Suporte
         arranjo->suporte  = suporte;
         //determina Cabos e PtoFixao no Suporte
         for (int nf = 0; nf < MAX_FASE; nf++)
            {//verifica se o Cabo está efinido p/ a fase
            if (str.cfg_suporte_cabo.fase[nf].cabo_id < 1) continue;
            //determina o Cabo
            if ((cabo = arranjos->ExisteCabo(str.cfg_suporte_cabo.fase[nf].cabo_id)) == NULL) return(NULL);
            //determina PtoFixacao no Suporte
            if ((pto_fixacao = suporte->ExistePtoFixacaoIndex(nf)) == NULL) return(NULL);
            //define PtoFixacao e Cabo p/ fase do Arranjo
            arranjo->DefPtoFixacaoFase(nf, pto_fixacao);
            arranjo->DefCaboFase(nf, cabo);
            }
         break;
      case arranjoCABO_Z0Z1:
         //determina o Cabo
         if ((cabo = arranjos->ExisteCabo(str.cfg_caboZOZ1.cabo_id)) == NULL) return(NULL);
         //define CaboZ0Z1
         arranjo->caboZ0Z1 = cabo;
         //determina Suporte (opcional => pode ser NULL)
         arranjo->suporte = arranjos->ExisteSuporte(str.cfg_caboZOZ1.suporte_id);
         break;
      case arranjoZ0Z1:
      case arranjoPU_SB100:
         arranjo->z0 = str.cfg_arranjoZ0Z1.z0;
         arranjo->z1 = str.cfg_arranjoZ0Z1.z1;
         arranjo->c0 = str.cfg_arranjoZ0Z1.c0;
         arranjo->c1 = str.cfg_arranjoZ0Z1.c1;
         //define corrente admissível
         arranjo->Iadm_amp = str.cfg_arranjoZ0Z1.iadm_amp;
         //determina Suporte (opcional => pode ser NULL)
         arranjo->suporte = arranjos->ExisteSuporte(str.cfg_arranjoZ0Z1.suporte_id);
         break;
      case arranjoMAT_Z:
         arranjo->Iadm_amp = str.cfg_matZ.iadm_amp;
         //inicia MatZ
         dim = arranjo->MatZ->Dimensao;
         for (int i = 0; i < dim; i++)
            {
            for (int j = 0; j < dim; j++)
               {
               arranjo->MatZ->R[i][j]   = str.cfg_matZ.mat_z[i][j].r;
               arranjo->MatZ->X[i][j]   = str.cfg_matZ.mat_z[i][j].x;
               }
            }
         //inicia MatC
         dim = arranjo->MatC->Dimensao;
         for (int i = 0; i < dim; i++)
            {
            for (int j = 0; j < dim; j++)
               {
               arranjo->MatC->Cap[i][j] =  str.cfg_matZ.mat_c[i][j];
               }
            }
         //define Matz como montada
         arranjo->MatZ->Montada = true;
         break;
      /*
      case arranjoPU_SB100:
         arranjo->Iadm_amp = str.cfg_puSB100.iadm_amp;
         arranjo->z0       = str.cfg_puSB100.z0;
         arranjo->z1       = str.cfg_puSB100.z1;
         arranjo->c0       = str.cfg_puSB100.c0;
         arranjo->c1       = str.cfg_puSB100.c1;
         //determina Suporte (opcional => pode ser NULL)
         arranjo->suporte = arranjos->ExisteSuporte(str.cfg_puSB100.suporte_id);
         break;
      */
      default:
         return(NULL);
      }
   //monta matriz de impedâncias
   if (! montaZ->Executa(arranjo)) return(NULL);
   //se é um Arranjo típico, insere em Arranjos
   //if (str.tipico) arranjos->InsereArranjo(arranjo);
   return(arranjo);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjBarra(strBARRA &str)
   {
   //variáveis locais
   VTBarra *barra;

   try{//cria um novo Eqpto
      if ((barra = DLL_NewObjBarra(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      //barra->Id        = (str.bar_id > 0) ? str.bar_id : FalsoId();
      barra->Id        = (str.bar_id == 0) ? FalsoId() : str.bar_id;
      barra->Codigo    = str.codigo;
      barra->Extern_id = str.codigo;
      barra->ShowAsUnsigned = str.show;
      barra->vnom      = str.vnom;
      barra->utm       = str.utm;
      barra->esq       = str.esq;
      //associa Barra c/ Redes (sem inserir Barra em Redes)
      //barra->redes = redes;
      //insere Eqpto em Redes
      //redes->InsereBarra(barra);
      //salva Barra na lista local lisBAR
      //lisBAR->Add(barra);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(barra);
   }


//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjCabo(strCABO &str)
   {
   //variáveis locais
   VTCabo *cabo;

   //proteção
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   if (arranjos == NULL) return(NULL);
   //verifica se existe o Cabo
   if (str.id > 0) 
      {//procura Cabo por ID
      if ((cabo = arranjos->ExisteCabo(str.id)) != NULL) return(cabo);
      }
   //procura Cabo por código
   if ((cabo = arranjos->ExisteCabo(str.codigo)) != NULL) return(cabo);
   //valida parâmetros
   switch(str.tipo_cfg)
      {
      case caboR_GMR:
         if (! ValidaCaboR_GMR(str)) return(NULL);
         break;
      case caboZ0Z1:
         if (! ValidaCaboZOZ1(str))  return(NULL);
         break;
      default:
         return(NULL);
      }
   try{//cria um novo Eqpto
      if ((cabo = DLL_NewObjCabo()) == NULL) return(NULL);
      //define Cabo como novo
      cabo->Status[sttNOVO] = true;
      //copia dados do Cabo
      cabo->Id          = (str.id > 0) ? str.id : FalsoId();
      cabo->Codigo      = str.codigo;
      cabo->Extern_id   = str.extern_id;
      cabo->TipoCfg     = str.tipo_cfg;
      cabo->Iadm_A      = str.iadm;
      cabo->Vmax_KV     = str.vmin;
      cabo->Vmin_KV     = str.vmax;
      cabo->r           = str.r;
      cabo->gmr         = str.gmr;
      cabo->z0          = str.z0;
      cabo->c0          = str.c0;
      cabo->z1          = str.z1;
      cabo->c1          = str.c1;
      //insere Cabo em Arranjos
      //arranjos->InsereCabo(cabo);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(cabo);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjCanal(strCANAL &str)
   {
   Aviso("TCria::NewObjCanal() - precisa ser refeita");
   return(NULL);
   /*
   //variáveis locais
   VTCanal   *canal;
   VTCurva   *curva;
   VTEqpto   *eqpto;
   VTMedidor *medidor;

   try{//determina Medidor do Canal
      if ((eqpto = ExisteEqpto(eqptoMEDIDOR, str.med_id)) == NULL)
         {
         plog->Add("Erro: não existe Medidor da medição " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      medidor = (VTMedidor*)eqpto;
      //determina Eqpto associado à medição
      if ((eqpto = ExisteEqpto(str.eqpto_medido.tipo, str.eqpto_medido.codigo)) == NULL)
         {
         plog->Add("Erro: não existe Eqpto associado à medição " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //proteção: necessária p/ Barras de Corte
      //verifica se o Medidor já tem um Canal p/ o Eqpto
      if ((canal = medidor->ExisteCanal(eqpto, str.tipo_canal)) != NULL) return(canal);
      //insere um novo Canal no medidor
      if ((canal = medidor->InsereCanal(eqpto, str.tipo_canal)) == NULL) return(NULL);
      //inicia dados Canal
      canal->Id        = str.canal_id;
      canal->Codigo    = str.codigo;
      //canal->TipoCanal = str.tipo_canal;
      canal->Erro      = str.sigma;
      //cria Curva do Canal
      if ((curva = CriaCurvaCanal(str)) == NULL)
         {
         DLL_DeleteEqpto(canal);
         return(NULL);
         }
      //define Curva do Canal
      canal->Curva = curva;
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(canal);
   */
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjCapacitor(strCAPACITOR &str)
   {
   //variáveis locais
   VTBarra    *pbarra;
   VTCapacitor *pcap;
   VTCurva     *curva;
   VTEqbar     *eqbar;

   try{//determina Barra do Capacitor
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra da Capacitor " +
                      AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se Capacitor definido por Curva típica
      if (str.curva.tipica)
         {//verifica se Curva típica já definida por ID
         if (str.curva.id > 0) {curva = curvas->ExisteCurvaTipicaCapacitor(str.curva.id);}
         else                  {curva = curvas->ExisteCurvaTipicaCapacitor();}
         if (curva == NULL)
            {
            plog->Add("Erro: não existe curva típica do Capacitor " +
                      AnsiQuotedStr(str.codigo, '\''));
            return(NULL);
            }
         }
      else
         {
         if ((curva = CriaCurvaCapacitor(str)) == NULL)
            {
            plog->Add("Erro: não foi possível criar curva do Capacitor " +
                      AnsiQuotedStr(str.codigo, '\''));
            return(NULL);
            }
         }
      //verifica se o Capacitor já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoCAPACITOR, str.cap_id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((pcap = DLL_NewObjCapacitor(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      pcap->Id        = str.cap_id;
      pcap->Codigo    = str.codigo;
      pcap->Extern_id = str.codigo;
      pcap->ShowAsUnsigned = str.show;
      pcap->vnom      = str.vnom;
      //pcap->ligacao   = lgTRIANG;
      pcap->ligacao   = str.ligacao;
      pcap->q         = str.q_mvar;
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      pcap->esq       = str.esq;
      //verifica tensão nominal
      if (IsDoubleZero(pcap->vnom)) pcap->vnom = pbarra->vnom;
      //define Curva
      pcap->DefineCurva(curva);
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      pcap->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(pcap);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(pcap);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
VTEqpto* __fastcall TCria::NewObjCapserie(strCAPSERIE &str)
   {
   //variáveis locais
   VTCapserie *capserie;
   VTBarra    *pbar1, *pbar2;
   VTRede     *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjCapserie(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      if ((pbar1 == NULL)||(pbar2 == NULL))
         {
         plog->Add("Erro: não existe Barra do Capacitor Série " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((capserie = DLL_NewObjCapserie(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      capserie->Id        = str.id;
      capserie->Codigo    = str.codigo;
      capserie->Extern_id = str.extern_id;
      capserie->ShowAsUnsigned = str.show;
      capserie->snom      = str.snom;
      capserie->vnom      = str.vnom;
      capserie->z         = str.z;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      capserie->esq       = str.esq;
      //define Barras da Ligacao
      capserie->DefineObjBarra(pbar1, pbar2);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      capserie->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(capserie);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(capserie);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjCarga(strCARGA &str)
   {
   //variáveis locais
   VTBarra  *pbarra;
   VTCarga  *pcarga;
   VTClasse *classe;
   VTCurva  *curva;
   VTEqbar  *eqbar;

   try{//determina Barra do Carga
      if ((pbarra = str.obj.barra) == NULL)
         {//procura Barra pelo seu ID
         if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
            {
            plog->Add("Erro: não existe Barra da Carga " + AnsiQuotedStr(str.codigo, '\''));
            return(NULL);
            }
         }
      //determina Classe de consumidor da Carga
      if ((classe = ExisteClasse(str)) == NULL)
         {
         plog->Add("Erro: não existe classe de consumidor da Carga " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se a Carga já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoCARGA, str.car_id)) != NULL) return(eqbar);
      //cria uma nova Carga
      if ((pcarga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados da Carga
      pcarga->Id             = str.car_id;
      pcarga->Codigo         = str.codigo;
      pcarga->Extern_id      = str.codigo;
      pcarga->ShowAsUnsigned = str.show;
      pcarga->Energia_mwhmes = str.energia_mwhmes;
      pcarga->Fases          = str.fases;
      pcarga->IP_NumLamp     = str.ip.num_lamp;
      pcarga->IP_Pot_kw      = str.ip.pot_kw;
      pcarga->IP_Fatpot      = str.ip.fat_pot;
      //modelo de carga
      if (! IsDoubleZero(1 - (str.modelo.icte_pu + str.modelo.scte_pu + str.modelo.zcte_pu)))
         {
         str.modelo.icte_pu = 1.;
         str.modelo.scte_pu = 0.;
         str.modelo.zcte_pu = 0.;
         }
      pcarga->Icte_pu    = str.modelo.icte_pu;
      pcarga->Scte_pu    = str.modelo.scte_pu;
      pcarga->Zcte_pu    = str.modelo.zcte_pu;
      //número de consumidores para Carga equivalente
      if (str.classe_cons == consEQUIVALENTE)
         {
         pcarga->NumConsResidencial = str.num_cons.residencial;
         pcarga->NumConsComercial   = str.num_cons.comercial;
         pcarga->NumConsIndustrial  = str.num_cons.industrial;
         pcarga->NumConsRural       = str.num_cons.rural;
         pcarga->NumConsOutros      = str.num_cons.outros;
         pcarga->NumConsA4          = str.num_cons.a4;
         }
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      pcarga->esq        = str.esq;
      //verifica se Carga de IP
      if (str.classe_cons == consIP)
         {//determina Curva típica de IP
         curva = curvas->ExisteCurvaTipicaIP();
         }
      //verifica se Carga definida por curva típica ou curva própria
      else if (str.curva.tipica)
         {//verifica se a Curva típica já está definida pelo seu ID
         if (str.curva.id > 0)
            {
            curva = curvas->ExisteCurvaTipicaCarga(str.curva.id);
            }
         else
            {//determina Curva típica por classe/consumo
            curva = curvas->ExisteCurvaTipicaCarga(classe, pcarga->Energia_kwhmes);
            }
         }
      else
         {//carga definida por Curva própria: cria Curva própria
         curva = CriaCurvaCarga(str);
         }
      //proteção: verifica se existe Curva da Carga
      if (curva == NULL)
         {
         plog->Add("Erro: não existe Curva de demanda da Carga " +
                   AnsiQuotedStr(str.codigo, '\''));
         //destrói objeto
         DLL_DeleteEqpto(pcarga);
         return(NULL);
         }

      //verifica potência nula de carga IP
      if (str.classe_cons == consIP)
         {
         if (IsDoubleZero(pcarga->IP_Pot_kw))
            {
            plog->Add("Erro: potência nula para Carga de IP " +
                      AnsiQuotedStr(str.codigo, '\''));

            }
         }
      //associa Carga c/ Barra, Classe e Curva
      pcarga->DefineObjBarra(pbarra);
      pcarga->DefineClasse(classe);
      pcarga->DefineCurva(curva);
      //2017.01.19 IMPORTANTE: não define Crescimento de Mercado
      //mercado->DefineCrescimento(pcarga, redes);
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      pcarga->pbarra = pbarra;
      //insere Carga na Barra
      //pbarra->InsereEqbar(pcarga);
      }catch(Exception &e)
        {//nada a fazer
        return(NULL);
        }
   return(pcarga);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjCargaNL(strCARGANL &str)
   {
   //variáveis locais
   VTBarra       *pbarra;
   VTCargaNL     *pcargaNL;
   VTEqbar       *eqbar;
   VTComentario  *comentario;
   struct strFUN fun;
   struct strHRM hrm;

   try{//determina Barra do Carga
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Não existe Barra da CargaNL " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      // Verifica se trata-se de uma nova carga (fundamental) ou se é uma harmônica
      // Verifica se a potência é em kVA (poderia ser verificada a ordem)
      if (str.flag_kVA)
         {
         // É a fundamental, então cria a carga e salva os parâmetros que interessam
         if ((pcargaNL = DLL_NewObjCargaNL()) == NULL) return(NULL);

         //inicia dados da Carga
         pcargaNL->Id         = str.car_id;
         pcargaNL->Codigo     = str.codigo;
         pcargaNL->Extern_id  = str.extern_id;
         pcargaNL->ShowAsUnsigned = str.show;
         pcargaNL->FatPot_pu  = str.fat_pot;
         pcargaNL->Indutiva   = str.flag_indutivo;
         // Dados da fundamental
         fun.sa_mod_kva       = str.snomA;
         fun.sb_mod_kva       = str.snomB;
         fun.sc_mod_kva       = str.snomC;
         fun.sa_ang_grau      = str.angA;
         fun.sb_ang_grau      = str.angB;
         fun.sc_ang_grau      = str.angC;
         pcargaNL->DefineFundamental(&fun);

         //valida strESQ_EQBAR
         ValidaEsquematicoEqbar(str.esq);
         pcargaNL->esq        = str.esq;

         //associa Carga c/ Barra, Classe e Curva
         pcargaNL->DefineObjBarra(pbarra);
         //insere Carga na Barra
         pbarra->InsereEqbar(pcargaNL);
         }
      else
         {
         // Trata-se de uma harmônica, então procura a carga com o mesmo id
         if ((pcargaNL = (VTCargaNL*)ExisteEqpto(eqptoCARGANL, str.car_id)) == NULL) return(NULL);
         // Carrega os dados da harmônica
         hrm.ordem       = str.ordem;
         hrm.sa_mod_pu   = str.snomA;
         hrm.sb_mod_pu   = str.snomB;
         hrm.sc_mod_pu   = str.snomC;
         hrm.sa_ang_grau = str.angA;
         hrm.sb_ang_grau = str.angB;
         hrm.sc_ang_grau = str.angC;
         if (!pcargaNL->InsereHarmonica(&hrm)) return(NULL);
         }
      }catch(Exception &e)
        {//nada a fazer
        return(NULL);
        }
   return(pcargaNL);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjChave(strCHAVE &str)
   {
   //variáveis locais
   VTChave  *pchave;
   VTBarra *pbar1, *pbar2;
   VTRede  *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjChave(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      if ((pbar1 == NULL)||(pbar2 == NULL))
         {
         plog->Add("Erro: não existe Barra da Chave " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((pchave = DLL_NewObjChave(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      pchave->Id               = str.chv_id;
      pchave->Codigo           = str.codigo;
      pchave->Extern_id        = str.codigo;
      pchave->ShowAsUnsigned   = str.show;
      pchave->Inom_amp         = str.corn;
      pchave->Estado           = (str.estado == chvABERTA) ? chvABERTA : chvFECHADA;
      //JCG 2015.07.17 - isso precisará ser alterado
      pchave->EstadoNormal     = pchave->Estado;
      //
      pchave->ProtecaoAtiva    = str.protecao_ativa;
      pchave->Telecomandada    = str.telecomandada;
      pchave->TecnoComunicacao = str.tecno_comunicacao;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      pchave->esq       = str.esq;
      //determina TipoChave
      pchave->TipoChave = ExisteTipoChave(str.tipochave.id, str.tipochave.tipo, str.tipochave.codigo);
      if (pchave->TipoChave == NULL)
         {
         if (plog) plog->Add("Não existe tipo da chave " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //define Barras da Ligacao
      pchave->DefineObjBarra(pbar1, pbar2);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      pchave->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(pchave);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(pchave);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjCluster(strCLUSTER &str)
   {
   //variáveis locais
   VTCluster *cluster;

   //verifica se já existe o Cluster
   if ((cluster = redes->ExisteCluster(str.cluster_id)) == NULL)
      {//cria um novo Cluster
      cluster = DLL_NewObjCluster();
      cluster->Id     = str.cluster_id;
      cluster->Codigo = str.codigo;
      //OBS: Cluster não tem associação c/ Redes
      //cluster->redes = redes;
      //insere Cluster em Redes
      //redes->InsereCluster(cluster);
      }
   return(cluster);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjFiltro(strFILTRO &str)
   {
   //variáveis locais
   VTBarra  *pbarra;
   VTEqbar  *eqbar;
   VTFiltro *filtro;

   try{//determina Barra do Eqbar
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra do Filtro " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se o Filtro já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoFILTRO, str.id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((filtro = DLL_NewObjFiltro()) == NULL) return(NULL);
      //inicia dados do Eqpto
      filtro->Id        = str.id;
      filtro->Codigo    = str.codigo;
      filtro->Extern_id = str.extern_id;
      filtro->ShowAsUnsigned = str.show;
      filtro->vnom      = str.vnom;
      filtro->snom      = str.snom;
      filtro->z0        = str.z0;
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      filtro->esq       = str.esq;
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      filtro->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(filtro);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(filtro);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjGerador(strGERADOR &str)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTCurva   *curva;
   VTEqbar   *eqbar;
   VTGerador *pger;

   try{//determina Barra do Gerador
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra do Gerador " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se o Gerador já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoGERADOR, str.ger_id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((pger = DLL_NewObjGerador(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      pger->Id           = str.ger_id;
      pger->Codigo       = str.codigo;
      pger->Extern_id    = str.codigo;
      pger->ShowAsUnsigned = str.show;
      pger->ligacao      = str.ligacao;
      pger->Fases        = str.fases;
      pger->vnom         = str.vnom;
      pger->snom         = str.snom_mva;
      pger->smax         = str.smax_mva;
      pger->tiposup      = str.tiposup;
      pger->FonteEnergia = str.tipo_geracao;
      pger->fplim        = str.fplim;
      pger->z0.r         = str.z0.r;
      pger->z0.x         = str.z0.x;
      pger->z1.r         = str.z1.r;
      pger->z1.x         = str.z1.x;
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      pger->esq         = str.esq;
      //verifica se o gerador está associado a uma curva típica
      if (str.curva.tipica)
         {//obtém curva típica
         curva = curvas->ExisteCurvaTipicaGeradorFotovoltaicoId(str.curva.id);
         }
      else
         {//cria Curva própria p/ o Gerador
         curva = CriaCurvaGerador(str);
         }
      //proteção
      if (curva  == NULL)
         {
         DLL_DeleteEqpto(pger);
         return(NULL);
         }
      pger->DefineCurva(curva);
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      pger->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(pger);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(pger);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjMedidor(strMEDIDOR &str)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTEqbar   *eqbar;
   VTMedidor *medidor;

   try{//determina Barra do Eqbar
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra do Medidor " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se o Medidor já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoMEDIDOR, str.medidor_id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((medidor = DLL_NewObjMedidor()) == NULL) return(NULL);
      //inicia dados do Eqpto
      medidor->Id        = str.medidor_id;
      medidor->Codigo    = str.codigo;
      medidor->Extern_id = str.extern_id;
      medidor->ShowAsUnsigned = str.show;
      medidor->esq       = str.esq;
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      medidor->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(medidor);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(medidor);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjMutua(strMUTUA &str)
   {
   //variáveis locais
   VTBarra  *bar_tre1, *bar_tre2;
   VTTrecho *trecho1, *trecho2;
   VTMutua  *mutua;
   VTRede   *rede;

   try{//determina Rede da Mutua
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Mutua com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjMutua(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Trechos da Mutua
      trecho1 = (VTTrecho*)rede->ExisteLigacao(eqptoTRECHO, str.tre1_id);
      trecho2 = (VTTrecho*)rede->ExisteLigacao(eqptoTRECHO, str.tre2_id);
      if ((trecho1 == NULL)||(trecho2 == NULL))
         {
         plog->Add("Erro: não existem Trechos do Mútua " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //determina Barras de referências da Mutua
      //bar_tre1 = (str.sentido_tre1 == sentidoB1B2) ? trecho1->pbarra1 : trecho1->pbarra2;
      //bar_tre2 = (str.sentido_tre2 == sentidoB1B2) ? trecho2->pbarra1 : trecho2->pbarra2;
      bar_tre1 = (str.tre1_bar_id == trecho1->pbarra1->Id) ? trecho1->pbarra1 : trecho1->pbarra2;
      bar_tre2 = (str.tre2_bar_id == trecho2->pbarra1->Id) ? trecho2->pbarra1 : trecho2->pbarra2;
      //cria um novo Eqpto
      if ((mutua = DLL_NewObjMutua()) == NULL) return(NULL);
      //inicia dados do Eqpto
      mutua->Id        = str.id;
      mutua->Codigo    = str.codigo;
      mutua->Extern_id = str.extern_id;
      mutua->ShowAsUnsigned = str.show;
      mutua->ptre1     = trecho1;
      mutua->pbar_tre1 = bar_tre1;
      mutua->ptre2     = trecho2;
      mutua->pbar_tre2 = bar_tre2;
      mutua->z0        = str.z0;
      mutua->z1        = str.z1;
      mutua->postre1   = str.postre1;
      mutua->postre2   = str.postre2;
      //OBS: Mutua não possui ponteiro p/ Rede
      //mutua->rede = rede;
      //insere Mutua na Rede
      //rede->InsereMutua(mutua);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(mutua);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjReator(strREATOR &str)
   {
   //variáveis locais
   VTBarra  *pbarra;
   VTCurva  *curva;
   VTEqbar  *eqbar;
   VTReator *reator;

   try{//determina Barra do Eqbar
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra do Reator " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //determina Curva típica
      if ((curva = curvas->ExisteCurvaTipicaReator()) == NULL)
         {
         plog->Add("Erro: não existe curva típica do Reator " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se o Reator já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoREATOR, str.id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((reator = DLL_NewObjReator(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      reator->Id        = str.id;
      reator->Codigo    = str.codigo;
      reator->Extern_id = str.extern_id;
      reator->ShowAsUnsigned = str.show;
      reator->ligacao   = str.ligacao;
      reator->vnom      = str.vnom;
      reator->q         = str.q_mvar;
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      reator->esq       = str.esq;
      //define Curva típica
      reator->DefineCurva(curva);
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      reator->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(reator);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(reator);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15630027
//https://sinapsisenergia.teamwork.com/#/tasks/16916628
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
VTEqpto* __fastcall TCria::NewObjRede(strREDE &str)
   {
   //variáveis locais
   VTRede    *rede;
   VTCluster *cluster;
   VTCurva   *curva;

   //verifica se pertence a um Cluster;
   cluster = NULL;
   if (str.cluster_id > 0) cluster = redes->ExisteCluster(str.cluster_id);
   try{//cria um novo Eqpto
      if ((rede = DLL_NewObjRede(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      rede->Id             = str.rede_id;
      rede->Codigo         = str.codigo;
	  rede->Extern_id      = IntToStr(FalsoId());//str.codigo;  //para Copel
      rede->ShowAsUnsigned = str.show;
      rede->TipoRede       = ExisteTipoRede(str.tiporede.id, str.tiporede.segmento, str.tiporede.codigo);
      rede->BarraInicialId = str.barini_id;
      rede->DefineBarraInicial(ExisteBarra(str.barini_id));
      rede->Estatica       = str.estatica;
      rede->Radial         = str.radial;
      rede->Color          = str.color;
      rede->Carregada      = true;
      rede->Valida         = true;
      //insere dados externos
	  rede->dados_ext->IdAlimPai = str.dados_ext.id_alim_pai;
	  rede->dados_ext->CodChave = str.dados_ext.cod_chave;
	  rede->dados_ext->Sigla = str.dados_ext.sigla;
	  rede->dados_ext->Regional = str.dados_ext.regional;
	  rede->dados_ext->MaeFilhaConectada = str.dados_ext.mae_filha_conectada;
      if ((curva = CriaCurvaRede(str)) != NULL)
         {
         rede->dados_ext->DefineCurva(curva);
         }
      //associa Rede com Redes (sem inserir Rede em Redes)
      rede->Redes          = redes;
      //insere VTRede em VTRedes
      //redes->InsereRede(rede);
      //associa Rede com seu Cluster (sem inserir a Rede no Cluster)
      rede->cluster = cluster;
      //insere VTRede no seu Cluster
      //if (cluster) cluster->InsereRede(rede);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(rede);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjRegulador(strREGULADOR &str)
   {
   //variáveis locais
   VTRegulador *regulador;
   VTBarra     *pbar1, *pbar2, *barra_ref, *barra_ref_inv;
   VTRede      *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjRegulador(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
	  pbar1 = ExisteBarra(str.bar1_id);
	  pbar2 = ExisteBarra(str.bar2_id);
      if ((pbar1 == NULL)||(pbar2 == NULL))
         {
         if (plog) plog->Add("Não existe Barra do Regulador de Tensão " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //determina barra de referência p / ajuste automático
//      barra_ref = barra_ref_inv = NULL;
	  barra_ref = pbar2;
	  barra_ref_inv = pbar1;
	  if (str.modelo != reguFIXO)
         {
         if ((barra_ref = ExisteBarra(str.cfg_auto.bar_ref_id)) == NULL)
            {
            if (plog) plog->Add("Não existe Barra de referência do Regulador de Tensão " + AnsiQuotedStr(str.codigo, '\''));
            //redefine regulador com tap fixo
            if (str.num_passo == 0) str.num_passo = 16;
            str.cfg_fixo.passo = 0;
            str.modelo = reguFIXO;
            }
         if ((str.modelo == reguAUTO_BI) && ((barra_ref_inv = ExisteBarra(str.cfg_auto.bar_ref_inv_id)) == NULL))
            {
            if (plog) plog->Add("Não existe Barra de referência sentido inverso do Regulador de Tensão " + AnsiQuotedStr(str.codigo, '\''));
            //redefine regulador automático unidirecional
            str.modelo = reguAUTO_UNI;
            }
         }
      //cria um novo Eqpto
	  if ((regulador = DLL_NewObjRegulador(patamares->NumPatamar())) == NULL) return(NULL);
	  //inicializa configuracoes default
	  regulador->var_tensao           = 10;
	  regulador->num_passo            = 16;
	  regulador->cfg_fixo.passo       = 0;
	  regulador->cfg_auto.bar_ref     = barra_ref;
	  regulador->cfg_auto.bar_ref_inv = barra_ref_inv;
	  regulador->cfg_auto.v_pu        = 1;
	  regulador->cfg_auto.vinv_pu     = 1;
	  regulador->cfg_auto.larg_bandamorta_pu 	 = 0.01;
	  regulador->cfg_auto.larg_bandamorta_inv_pu = 0.01;
      //inicia dados do Eqpto
      regulador->Id                 = str.id;
      regulador->modelo             = str.modelo;
      regulador->Codigo             = str.codigo;
      regulador->Extern_id          = str.extern_id;
      regulador->ShowAsUnsigned     = str.show;
      regulador->ligacao            = str.ligacao;
      //proteção na definição das fases
      switch(str.ligacao)
         {
         case lgEST_ISO:   //define fases
            regulador->fases = faseABC;
            break;
         case lgDELTA_FECHADO:   //define fases
            regulador->fases = faseABC;
            break;
         case lgDELTA_ABERTO:   //define fase comum
            if      (str.fases == faseA) regulador->fases = faseA;
            else if (str.fases == faseB) regulador->fases = faseB;
            else if (str.fases == faseC) regulador->fases = faseC;
            else                         regulador->fases = faseB;
            break;
         case lgMONOFASICO:   //define fases
            if      (str.fases == faseAN)  regulador->fases = faseAN;
            else if (str.fases == faseBN)  regulador->fases = faseBN;
			else if (str.fases == faseCN)  regulador->fases = faseCN;
			else if (str.fases == faseAB)  regulador->fases = faseAB;
			else if (str.fases == faseBC)  regulador->fases = faseBC;
			else if (str.fases == faseCA)  regulador->fases = faseCA;
            else if (str.fases == faseABN) regulador->fases = faseABN;
            else if (str.fases == faseBCN) regulador->fases = faseBCN;
            else if (str.fases == faseCAN) regulador->fases = faseCAN;
            else                           regulador->fases = faseABN;
            break;
         default:
            regulador->ligacao = lgEST_ISO;
            regulador->fases   = faseABC;
            break;
		 }
		regulador->cfg_auto.bar_ref   = barra_ref;
		regulador->cfg_auto.bar_ref_inv = barra_ref_inv;
		switch(str.modelo)
		{
            case reguPADRAO:
			case reguAUTO_BI:
			case reguAUTO_UNI:
			case reguAUTO_FIXA:
				regulador->cfg_auto.v_pu      = str.cfg_auto.v_pu;
				regulador->cfg_auto.vinv_pu   = str.cfg_auto.vinv_pu;
				regulador->cfg_auto.larg_bandamorta_pu 	   = str.cfg_auto.larg_bandamorta_pu;
				regulador->cfg_auto.larg_bandamorta_inv_pu = str.cfg_auto.larg_bandamorta_inv_pu;
				break;
			default:
				regulador->cfg_auto.v_pu      = 1;
				regulador->cfg_auto.vinv_pu   = 1;
				regulador->cfg_auto.larg_bandamorta_pu 	   = 0.01;
				regulador->cfg_auto.larg_bandamorta_inv_pu = 0.01;
				break;
		}
	  regulador->snom               = str.snom_mva;
      regulador->by_pass            = str.by_pass;
      regulador->sentido.bar_ref    = ExisteBarra(str.sentido.bar_ref_id);
      regulador->var_tensao         = str.var_tensao;
      regulador->num_passo          = str.num_passo;
	  regulador->cfg_fixo.passo     = str.cfg_fixo.passo;
	  //relação de tap
	  regulador->rel_tap            = str.rel_tap;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      regulador->esq                = str.esq;
      //define Barras da Ligacao
      regulador->DefineObjBarra(pbar1, pbar2);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      regulador->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(regulador);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(regulador);
   }


//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjSuporte(strSUPORTE &str)
   {
   //variáveis locais
   VTPtoFixacao *pto_fixacao;
   VTSuporte    *suporte;

   //proteção
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   if (arranjos == NULL) return(NULL);
   //verifica se já existe o Suporte
   if (str.id > 0) 
      {//procura Suporte por ID
      if ((suporte = arranjos->ExisteSuporte(str.id)) != NULL) return(suporte);
      }
   //procura Suporte por código
   if ((suporte = arranjos->ExisteSuporte(str.codigo)) != NULL) return(suporte);
   try{//cria um novo Suporte
      suporte     = DLL_NewObjSuporte();
      suporte->Id = str.id;
      //verifica se é um Suporte novo
      if (str.id < 1)
         {//define Suporte como novo
         suporte->Id              = FalsoId();
         suporte->Status[sttNOVO] = true;
         }
      suporte->Codigo        = str.codigo;
      suporte->Extern_id     = str.extern_id;
      suporte->TipoDeSuporte = str.tipo;
      suporte->Vmin_KV       = str.vmin;
      suporte->Vmax_KV       = str.vmax;
      //cria pontos de fixação
      for (int n = 0; n < str.num_pto; n++)
         {//cria um novo PtoFixacao
         if ((pto_fixacao = DLL_NewObjPtoFixacao()) == NULL) continue;
         suporte->InserePtoFixacao(pto_fixacao);
         //lê dados do PtoFixacao
         pto_fixacao->id      = str.pto_fixacao[n].id;
         pto_fixacao->codigo  = str.pto_fixacao[n].codigo;
         pto_fixacao->coord.x = str.pto_fixacao[n].coord_x;
         pto_fixacao->coord.y = str.pto_fixacao[n].coord_y;
         }
      //insere Suporte em Arranjos
      //arranjos->InsereSuporte(suporte);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(suporte);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjSuprimento(strSUPRIMENTO &str)
   {
   //variáveis locais
   VTBarra      *pbarra;
   VTCurva      *curva;
   VTEqbar      *eqbar;
   VTSuprimento *psup;

   try{//determina Barra do Suprimento
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra do Suprimento " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se o Suprimento já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoSUPRIMENTO, str.sup_id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((psup = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      psup->Id        = str.sup_id;
      psup->Codigo    = str.codigo;
      psup->Extern_id = str.extern_id;
      psup->ShowAsUnsigned = str.show;
      psup->vnom      = str.vnom_kv;
      psup->smax      = str.smax;
      psup->tiposup   = str.tiposup;
      psup->zeq0      = str.zeq0;
      psup->zeq1      = str.zeq1;
      psup->pcc_3f    = str.pcc_3f;
      psup->pcc_ft    = str.pcc_ft;
      //valida estrutura strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      psup->esq       = str.esq;
      //cria Curva própria p/ o Suprimento
      if ((curva = CriaCurvaSuprimento(str)) == NULL)
         {
         DLL_DeleteEqpto(psup);
         return(NULL);
         }
      psup->DefineCurva(curva);
      //valida dados de equivalente
      if (! ValidaZeqv(psup) && (! ValidaPotCC(psup)))
         {//assume Pcc default
         psup->pcc_3f.p =   0.;
         psup->pcc_3f.q = 600.;
         psup->pcc_ft.p =   0.;
         psup->pcc_ft.q = 400.;
         }
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      psup->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(psup);
      //define Barra do Suprimento como Barra inicial da sua Rede
      //DefineBarraInicial(pbarra);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(psup);
   }
/*
//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrafo(strTRAFO &str)
   {
   //variáveis locais
   VTTrafo *ptrafo;
   VTBarra *pbar1, *pbar2;
   VTRede  *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjTrafo(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      if ((pbar1 == NULL)||(pbar2 == NULL))
         {
         plog->Add("Erro: não existe Barra do Trafo " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      ptrafo->Id            = str.trf_id;
      ptrafo->Codigo        = str.codigo;
      ptrafo->Extern_id     = str.codigo;
      ptrafo->ShowAsUnsigned = str.show;
      ptrafo->snom          = str.snom_mva;
      ptrafo->z0            = str.z0;
      ptrafo->z1            = str.z1;
      ptrafo->perda_ferro   = str.perda_ferro;
      ptrafo->auto_trafo    = str.auto_trafo;
      ptrafo->pri.fase      = str.pri.fases;
      ptrafo->pri.ligacao   = str.pri.ligacao;
      ptrafo->pri.vnom      = str.pri.vnom;
      ptrafo->pri.tap       = str.pri.tap;
      ptrafo->pri.defasagem = str.pri.defasagem;
      ptrafo->pri.zterra    = str.pri.zterra;
      ptrafo->sec.fase      = str.sec.fases;
      ptrafo->sec.ligacao   = str.sec.ligacao;
      ptrafo->sec.vnom      = str.sec.vnom;
      ptrafo->sec.tap       = str.sec.tap;
      ptrafo->sec.defasagem = str.sec.defasagem;
      ptrafo->sec.zterra    = str.sec.zterra;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      ptrafo->esq           = str.esq;
      //dados de LTC
      ptrafo->ltc.tipo             = str.ltc.tipo;
      ptrafo->ltc.ajuste_auto.v_pu = str.ltc.v_pu;
      if (ptrafo->ltc.tipo != ltcFIXO)
         {
         ptrafo->ltc.ajuste_auto.bar_ref  = ExisteBarra(str.ltc.bar_id);
         if (ptrafo->ltc.ajuste_auto.bar_ref == NULL) ptrafo->ltc.tipo = ltcFIXO;
         }
      //proteções
      if (ptrafo->pri.tap == 0.) ptrafo->pri.tap = ptrafo->pri.vnom;
      if (ptrafo->sec.tap == 0.) ptrafo->sec.tap = ptrafo->sec.vnom;
      if (IsDoubleZero(ptrafo->snom))
         {
         ptrafo->snom = 0.1;
         plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com potência nominal nula");
         }
      if ((ptrafo->z1.r == 0.)&&(ptrafo->z1.x == 0.))
         {
         ptrafo->z1.x = 0.05;  //pu
         plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com impedância de seq.1 nula");
         }
      if ((ptrafo->z0.r == 0.)&&(ptrafo->z0.x == 0.))
         {
         ptrafo->z0.x = 0.05;  //pu
         plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com impedância de seq.0 nula");
         }
      //define Barras da Ligacao
      ptrafo->DefineObjBarra(pbar1, pbar2);
      //garante ordenação das barras
      ptrafo->OrdenaBar12PriSec();
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      ptrafo->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(ptrafo);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(ptrafo);
   }
*/
//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrafo(strTRAFO &str)
   {
   //variáveis locais
   VTBarra *pbar1, *pbar2;

   try{//determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      return(NewObjTrafo(str, pbar1, pbar2));
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2)
   {
   //variáveis locais
   VTTrafo *ptrafo;
   VTRede  *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
Aviso("TCria::NewObjTrafo(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      //barra1 = ExisteBarra(str.bar1_id);
      //barra2 = ExisteBarra(str.bar2_id);
      if ((barra1 == NULL)||(barra2 == NULL))
         {
         plog->Add("Erro: não existe Barra do Trafo " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      ptrafo->Id            = str.trf_id;
      ptrafo->Codigo        = str.codigo;
      ptrafo->Extern_id     = str.codigo;
      ptrafo->ShowAsUnsigned = str.show;
      ptrafo->snom          = str.snom_mva;
      ptrafo->z0            = str.z0;
      ptrafo->z1            = str.z1;
      ptrafo->perda_ferro   = str.perda_ferro;
      ptrafo->auto_trafo    = str.auto_trafo;
      ptrafo->pri.fase      = str.pri.fases;
      ptrafo->pri.ligacao   = str.pri.ligacao;
      ptrafo->pri.vnom      = str.pri.vnom;
      ptrafo->pri.tap       = str.pri.tap;
      ptrafo->pri.defasagem = str.pri.defasagem;
      ptrafo->pri.zterra    = str.pri.zterra;
      ptrafo->sec.fase      = str.sec.fases;
      ptrafo->sec.ligacao   = str.sec.ligacao;
      ptrafo->sec.vnom      = str.sec.vnom;
      ptrafo->sec.tap       = str.sec.tap;
      ptrafo->sec.defasagem = str.sec.defasagem;
      ptrafo->sec.zterra    = str.sec.zterra;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      ptrafo->esq           = str.esq;
      //dados de LTC
		ptrafo->ltc.tipo             = str.ltc.tipo;
		ptrafo->ltc.ajuste_auto.v_pu = str.ltc.v_pu;
		if (ptrafo->ltc.tipo != ltcFIXO)
         {
			ptrafo->ltc.ajuste_auto.bar_ref  = ExisteBarra(str.ltc.bar_id);
			if (ptrafo->ltc.ajuste_auto.bar_ref == NULL) ptrafo->ltc.tipo = ltcFIXO;
         }
      //proteções
      if (ptrafo->pri.tap == 0.) ptrafo->pri.tap = ptrafo->pri.vnom;
      if (ptrafo->sec.tap == 0.) ptrafo->sec.tap = ptrafo->sec.vnom;
      if (IsDoubleZero(ptrafo->snom))
         {
         ptrafo->snom = 0.1;
         plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com potência nominal nula");
         }
      if ((ptrafo->z1.r == 0.)&&(ptrafo->z1.x == 0.))
         {
         ptrafo->z1.x = 0.05;  //pu
         plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com impedância de seq.1 nula");
         }
      if ((ptrafo->z0.r == 0.)&&(ptrafo->z0.x == 0.))
         {
         ptrafo->z0.x = 0.05;  //pu
         plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com impedância de seq.0 nula");
         }
      //define Barras da Ligacao
      ptrafo->DefineObjBarra(barra1, barra2);
      //garante ordenação das barras
      ptrafo->OrdenaBar12PriSec();
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      ptrafo->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(ptrafo);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(ptrafo);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrafo3E(strTRAFO3E &str)
   {
   //variáveis locais
   VTTrafo3E  *ptrafo;
   VTBarra   *pbar1, *pbar2, *pbar3;
   VTRede    *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjTrafo3E(): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      pbar3 = ExisteBarra(str.bar3_id);
      if ((pbar1 == NULL)||(pbar2 == NULL)||(pbar3 == NULL))
         {
         plog->Add("Erro: não existe Barra do Trafo3E " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((ptrafo = DLL_NewObjTrafo3E(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      ptrafo->Id            = str.trf_id;
      ptrafo->Codigo        = str.codigo;
      ptrafo->Extern_id     = str.codigo;
      ptrafo->ShowAsUnsigned = str.show;
      //ptrafo->pri.fases   = ExisteFases(str.pri.ligacao);
      ptrafo->pri.ligacao   = str.pri.ligacao;
      ptrafo->pri.snom      = str.pri.snom;
      ptrafo->pri.vnom      = str.pri.vnom;
      ptrafo->pri.tap       = str.pri.tap;
      ptrafo->pri.defasagem = str.pri.defasagem;
      //ptrafo->sec.fases   = ExisteFases(str.sec.ligacao);
      ptrafo->sec.ligacao   = str.sec.ligacao;
      ptrafo->sec.snom      = str.sec.snom;
      ptrafo->sec.vnom      = str.sec.vnom;
      ptrafo->sec.tap       = str.sec.tap;
      ptrafo->sec.defasagem = str.sec.defasagem;
      //ptrafo->ter.fases   = ExisteFases(str.ter.ligacao);
      ptrafo->ter.ligacao   = str.ter.ligacao;
      ptrafo->ter.snom      = str.ter.snom;
      ptrafo->ter.vnom      = str.ter.vnom;
      ptrafo->ter.tap       = str.ter.tap;
      ptrafo->ter.defasagem = str.ter.defasagem;
      ptrafo->zps0          = str.zps0;
      ptrafo->zps1          = str.zps1;
      ptrafo->zpt0          = str.zpt0;
      ptrafo->zpt1          = str.zpt1;
      ptrafo->zst0          = str.zst0;
      ptrafo->zst1          = str.zst1;
      ptrafo->perda_ferro   = str.perda_ferro;
      //ltc
      ptrafo->ltc.tipo      = str.ltc.tipo;
      ptrafo->ltc.dv_perc   = str.ltc.dv_perc;
      ptrafo->ltc.num_passo = str.ltc.num_passo;
      ptrafo->ltc.passo     = str.ltc.passo;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      ptrafo->esq           = str.esq;
      //define Barras da Ligacao
      ptrafo->DefineObjBarra(pbar1, pbar2, pbar3);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      ptrafo->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(ptrafo);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(ptrafo);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrafoZZ(strTRAFOZZ &str)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTEqbar   *eqbar;
   VTTrafoZZ *trafoZZ;

   try{//determina Barra do Eqbar
      if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
         {
         plog->Add("Erro: não existe Barra do TrafoZZ " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //verifica se o TrafoZZ já existe
      if ((eqbar = pbarra->ExisteEqbar(eqptoTRAFOZZ, str.id)) != NULL) return(eqbar);
      //cria um novo Eqpto
      if ((trafoZZ = DLL_NewObjTrafoZZ()) == NULL) return(NULL);
      //inicia dados do Eqpto
      trafoZZ->Id        = str.id;
      trafoZZ->Codigo    = str.codigo;
      trafoZZ->Extern_id = str.extern_id;
      trafoZZ->ShowAsUnsigned = str.show;
      trafoZZ->vnom      = str.vnom;
      trafoZZ->snom      = str.snom;
      trafoZZ->z0        = str.z0;
      //valida strESQ_EQBAR
      ValidaEsquematicoEqbar(str.esq);
      trafoZZ->esq       = str.esq;
      //associa Eqbar c/ Barra (sem inserir Eqbar na Barra)
      trafoZZ->pbarra = pbarra;
      //insere Eqpbar na Barra
      //pbarra->InsereEqbar(trafoZZ);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(trafoZZ);
   }


//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrecho(strTRECHO &str, strARRANJO1 &str_arr)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTBarra   *pbar1, *pbar2;
   VTCabo    *cabo[MAX_FASE];
   VTSuporte *suporte;
   VTTrecho  *trecho;
   VTRede    *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjTrecho(1): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      if ((pbar1 == NULL)||(pbar2 == NULL))
         {
         plog->Add("Erro: não existe Barra do Trecho " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }

//INCLUIR verificação por código, tipocfg

      //verifica tipo do arranjo do Trecho
      switch(str_arr.tipo_cfg)
         {
         case arranjoCABO_Z0Z1: //arranjo típico definido por um único Cabo do tipo caboZ0Z1
            if ((arranjo = ExisteArranjoTipico_CABO_Z0Z1(str_arr)) == NULL)
               {//não existe Arranjo típico: cria um Arranjo próprio
               plog->Add("Erro: não existe Arranjo p/ Trecho " + AnsiQuotedStr(str.codigo, '\'') +
                         " da Rede " + AnsiQuotedStr(rede->Codigo, '\''));
               arranjo = CriaArranjoProprio_Z0Z1(str_arr);
               }
            break;
         case arranjoCABOxFASE:  //arranjo típico definido por Supote + Cabos por fase
            if ((arranjo = ExisteArranjoTipico_CABO_FASE(str_arr, rede->TipoRede)) == NULL)
               {//não existe Arranjo típico: cria um Arranjo próprio
               plog->Add("Erro: não existe Arranjo p/ Trecho " + AnsiQuotedStr(str.codigo, '\'') +
                         " da Rede " + AnsiQuotedStr(rede->Codigo, '\''));
               arranjo = CriaArranjoProprio_Z0Z1(str_arr);
               }
            break;
         case arranjoZ0Z1:  //arranjo próprio definido por Z0 e Z1
            //tenta localizar um Arranjo típico
            if ((arranjo = ExisteArranjoTipico_Z0Z1(str_arr)) == NULL)
               {//não existe Arranjo típico: cria um Arranjo próprio
               arranjo = CriaArranjoProprio_Z0Z1(str_arr);
               }
            break;
         default:
            arranjo = NULL;
         }
      //se não existe Arranjo, determina Arranjo por Vnom
      if (arranjo == NULL)
         {
         arranjo = ExisteArranjoTipico(pbar1->vnom);
         }
      if (arranjo == NULL)
         {
         plog->Add("Erro: não existe Arranjo do Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL)
        return(NULL);
      //inicia dados do Eqpto
      trecho->Id             = str.tre_id;
      trecho->Codigo         = str.codigo;
      trecho->Extern_id      = str.codigo;
      trecho->ShowAsUnsigned = str.show;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      trecho->esq            = str.esq;
      trecho->Comprimento_km = str.comp_km;
      //define Barras da Ligacao
      trecho->DefineObjBarra(pbar1, pbar2);
      //define Arranjo do Trecho
      trecho->DefineObjArranjo(arranjo);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      trecho->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(trecho);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(trecho);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrecho(strTRECHO &str, strARRANJO2 &str_arr)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTEqpto  *trecho;

   try{//verifica se Arranjo típico
      if (str_arr.tipico)
         {//determina Arranjo típico
         arranjo = ExisteArranjoTipico(str_arr.arranjo_id);
         }
      else
         {//cria Arranjo próprio
         arranjo = NewObjArranjo(str_arr);
         }
      if (arranjo == NULL)
         {
         plog->Add("Erro: não existe Arranjo do Trecho " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria o Trecho
      trecho = (VTEqpto*)NewObjTrecho(str, arranjo);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(trecho);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrecho(strTRECHO &str, VTArranjo *arranjo)
   {
   //variáveis locais
   VTBarra   *pbar1, *pbar2;
   VTCabo    *cabo[MAX_FASE];
   VTSuporte *suporte;
   VTTrecho  *trecho;
   VTRede    *rede;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjTrecho(2): rede indefinida: " + AnsiQuotedStr(str.codigo, '\''));
//fim teste em modo debug =================
#endif

         }
      //determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      if ((pbar1 == NULL)||(pbar2 == NULL))
         {
         plog->Add("Erro: não existe Barra do Trecho " +
                   AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //cria um novo Eqpto
      if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL)
        return(NULL);
      //inicia dados do Eqpto
      trecho->Id             = str.tre_id;
      trecho->Codigo         = str.codigo;
      trecho->Extern_id      = str.codigo;
      trecho->ShowAsUnsigned = str.show;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      trecho->esq            = str.esq;
      trecho->Comprimento_km = str.comp_km;
      //define Barras da Ligacao
      trecho->DefineObjBarra(pbar1, pbar2);
      //define Arranjo do Trecho
      trecho->DefineObjArranjo(arranjo);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      trecho->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(trecho);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(trecho);
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TCria::NewObjTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2, VTArranjo *arranjo)
   {
   //variáveis locais
   VTSuporte    *suporte;
   VTTrecho     *trecho;
   VTRede       *rede;
   VTComentario *comentario;

   try{//determina Rede da Ligacao
      if ((rede = ExisteRede(str.rede_id)) == NULL)
         {//associa Ligacao com a Rede isolada
         rede = redes->RedeIsolada();

#ifdef _DEBUG
//teste em modo debug =====================
//Aviso("TCria::NewObjTrecho(3): rede indefinida");
//fim teste em modo debug =================
#endif

         }
      //cria um novo Eqpto
      if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL)
        return(NULL);
      //inicia dados do Eqpto
      trecho->Id             = str.tre_id;
      trecho->Codigo         = str.codigo;
      trecho->Extern_id      = str.codigo;
      trecho->ShowAsUnsigned = str.show;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      trecho->esq            = str.esq;
      trecho->Comprimento_km = str.comp_km;
      //define Barras da Ligacao
      trecho->DefineObjBarra(barra1, barra2);
      //define Arranjo do Trecho
      trecho->DefineObjArranjo(arranjo);
      //associa Ligacao c/ Rede (sem inserir Ligacao na Rede)
      trecho->rede = rede;
      //insere Ligacao na Rede
      //rede->InsereLigacao(trecho);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(trecho);
   }

//---------------------------------------------------------------------------
bool __fastcall TCria::RedefineTipoChave(AnsiString &codigo, int &tipo)
   {
   //variáveis locais
   AnsiString tipoStr;
   int        tipoChave = -1;

   //determina tipo da chave
   if        (codigo.AnsiCompare("CHAVE A OLEO") == 0)                           tipoChave = chaveSEC;
   else if (codigo.AnsiCompare("CHAVE FACA ABERTURA COM CARGA") == 0)         tipoChave = chaveSEC;
   else if (codigo.AnsiCompare("CHAVE FACA ABERTURA SEM CARGA") == 0)         tipoChave = chaveSEC;
   else if (codigo.AnsiCompare("CHAVE FACA TRIPOLAR COM LOADBUSTER") == 0)   tipoChave = chaveSEC;
   else if (codigo.AnsiCompare("CHAVE FUSÍVEL ABERTURA COM CARGA") == 0)       tipoChave = chaveBF;
   else if (codigo.AnsiCompare("CHAVE FUSÍVEL RELIGADORA") == 0)             tipoChave = chaveRA;
   else if (codigo.AnsiCompare("CHAVE FUSÍVEL RELIGADORA 3 ESTÁGIOS") == 0)  tipoChave = chaveRA;
   else if (codigo.AnsiCompare("CHAVE REMOTA ABERTURA COM CARGA") == 0)      tipoChave = chaveSEC;
   else if (codigo.AnsiCompare("RELIGADOR DE TENSAO") == 0)                  tipoChave = chaveRA;
   else if (codigo.AnsiCompare("UNIDADE SECCIONADORA AUXILIAR") == 0)         tipoChave = chaveSEC;
   //define código em função do tipo definido
   switch (tipoChave)
      {
      case chaveDJ:  tipoStr = "Disjuntor";    break;
      case chaveRA:  tipoStr = "Religadora";   break;
      case chaveBF:  tipoStr = "Base Fusível"; break;
      case chaveSEC: tipoStr = "Seccionadora"; break;
      default:       return(false);
      }
   //atualiza parâmetros passados por referência
   tipo   = tipoChave;
   codigo = tipoStr;
   return(true);
   }
   
//-----------------------------------------------------------------------------
bool __fastcall TCria::ValidaCaboR_GMR(strCABO &str)
   {
   if (IsDoubleZero(str.r))
      {
      plog->Add("Cabo " + str.codigo + " com resistência nula");
      return(false);
      }
   if (IsDoubleZero(str.gmr))
      {
      plog->Add("Cabo " + str.codigo + " com raio médio geométrico nulo");
      return(false);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TCria::ValidaCaboZOZ1(strCABO &str)
   {
   if (IsDoubleZero(str.z0.r)&&IsDoubleZero(str.z0.x))
      {
      plog->Add("Cabo " + str.codigo + " com impedância Zseq0 nula");
      return(false);
      }
   if (IsDoubleZero(str.z1.r)&&IsDoubleZero(str.z1.x))
      {
      plog->Add("Cabo " + str.codigo + " com impedância Zseq1 nula");
      return(false);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TCria::ValidaLtcTrafo(void)
   {
   //variáveis locais
   VTBarra *barra;
   VTTrafo *ptrafo;

   //obtém lista de Trafos
   lisTMP->Clear();
   redes->LisLigacao(lisTMP, eqptoTRAFO);
   //loop p/ todos Trafos
   for (int n = 0; n < lisTMP->Count; n++)
      {
      ptrafo = (VTTrafo*)lisTMP->Items[n];
      //verifica se o Trafo possui LTC
      if ((ptrafo->ltc.tipo != ltcFIXO)&&
          (ptrafo->ltc.ajuste_auto.bar_ref != NULL))
         {//verifica se a Barra controlada possui Suprimento
         if (ptrafo->ltc.ajuste_auto.bar_ref->ExisteGeracao())
            {//desabilita o LTC
            ptrafo->ltc.tipo = ltcFIXO;
            }
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCria::ValidaPotCC(VTSuprimento *suprimento)
   {
   //variáveis locais
   complex<double> pcc_ft(suprimento->pcc_ft.p, suprimento->pcc_ft.q);
   complex<double> pcc_3f(suprimento->pcc_3f.p, suprimento->pcc_3f.q);

   //verifica valores nulos
   if (IsDoubleZero(suprimento->pcc_3f.p) && IsDoubleZero(suprimento->pcc_3f.q)) return(false);
   if (IsDoubleZero(suprimento->pcc_ft.p) && IsDoubleZero(suprimento->pcc_ft.q)) return(false);
   //verifica se pcc_3f > pcc_ft
   //if (Abs(pcc_3f) < Abs(pcc_ft)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCria::ValidaEsquematicoEqbar(strESQ_EQBAR &str_esq)
   {
   //valida posbar
   if      (str_esq.posbar < 0)   str_esq.posbar =   0;
   else if (str_esq.posbar > 100) str_esq.posbar = 100;
   //valida dxbar e dybar nulos
   if ((str_esq.dxbar == 0)&&(str_esq.dybar == 0))
      {
      str_esq.dxbar =   2000;
      str_esq.dybar =  -2000;
      }
   /*
   //valida dxbar
   if      (str_esq.dxbar < -2000) str_esq.dxbar = -2000;
   else if (str_esq.dxbar >  2000) str_esq.dxbar =  2000;
   //valida dybar
   if      (str_esq.dybar < -2000) str_esq.dybar = -2000;
   else if (str_esq.dybar >  2000) str_esq.dybar =  2000;
   */
   }

//---------------------------------------------------------------------------
void __fastcall TCria::ValidaEsquematicoLigacao(strESQ_LIGA &str_esq)
   {
   //valida posbar1
   if      (str_esq.posbar1 <   0) str_esq.posbar1 =   0;
   else if (str_esq.posbar1 > 100) str_esq.posbar1 = 100;
   //valida posbar2
   if      (str_esq.posbar2 <   0) str_esq.posbar2 =   0;
   else if (str_esq.posbar2 > 100) str_esq.posbar2 = 100;
   //valida posbar3
   if      (str_esq.posbar3 <   0) str_esq.posbar3 =   0;
   else if (str_esq.posbar3 > 100) str_esq.posbar3 = 100;
   }

//-----------------------------------------------------------------------------
bool __fastcall TCria::ValidaZeqv(VTSuprimento *suprimento)
   {
   //variáveis locais
   complex<double> zeq0(suprimento->zeq0.r, suprimento->zeq0.x);
   complex<double> zqv1(suprimento->zeq1.r, suprimento->zeq1.x);

   //verifica valores negativos
   if ((suprimento->zeq0.r < 0) || (suprimento->zeq0.x < 0)) return(false);
   if ((suprimento->zeq1.r < 0) || (suprimento->zeq1.x < 0)) return(false);
   //verifica Zeqv nulo
   if (IsDoubleZero(suprimento->zeq0.r) && IsDoubleZero(suprimento->zeq0.x)) return(false);
   if (IsDoubleZero(suprimento->zeq1.r) && IsDoubleZero(suprimento->zeq1.x)) return(false);
   //verifica Zeqv0 > Zeqv1
   //if (Abs(zeq0) < Abs(zqv1)) return(false);

   return(true);
   }

//-----------------------------------------------------------------------------
// eof

