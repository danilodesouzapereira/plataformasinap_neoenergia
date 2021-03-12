//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Arranjo\VTSuporte.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTBateria.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTCluster.h>
#include <Fontes\Rede\VTDominio.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTrafo.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Classes.h>
#include "TPreMonta.h"

//---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl)
   {
   try{//cria objeto TPreMonta
      return(new TPreMonta(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta::TPreMonta(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   monta     = (VTMonta*)apl->GetObject(__classid(VTMonta));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   //inicia opções default
   ConsA4_Energia = false;
   //inicia flag que indica sucesso/falha na montagem da rede
   sucesso = true;
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta::~TPreMonta(void)
   {
   //destrói objetos
   }

//---------------------------------------------------------------------------
void __fastcall TPreMonta::AtualizaCargaEquivalente(VTCarga *carga_eqv, VTCarga *carga)
   {
   //variáveis locais
   complex<double> s_mva;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //copia informações do esquemático
   carga_eqv->esq = carga->esq;
   //incorpora demanda da Carga na demanda da Carga equivalente
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina demanda da Carga equivalente da Et no patamar
      s_mva  = demanda->S_mva[carga_eqv][np];
      //soma demanda da Carga no patamar
      s_mva += demanda->S_mva[carga][np];
      //atualiza demanda da Carga equivalente da Et no patamar
      demanda->S_mva[carga_eqv][np] = s_mva;
      }
   //incrementa quantidade de consumidores da classe
   switch(carga->classe->Tag)
      {
      case consRESIDENCIAL:     carga_eqv->NumConsResidencial++;  break;
      case consCOMERCIAL:       carga_eqv->NumConsComercial++;    break;
      case consINDUSTRIAL:      carga_eqv->NumConsIndustrial++;   break;
      case consRURAL:           carga_eqv->NumConsRural++;        break;
      case consOUTROS:          carga_eqv->NumConsOutros++;       break;
      case consA4:              carga_eqv->NumConsA4++;           break;
      default:                                                    break;
      }
   //altera a definição de Classe da Carga equivalente
   if (carga_eqv->NumConsTotal == 1)
      {//assume mesma classe da Carga parcial
      carga_eqv->DefineClasse(carga->classe);
      }
   else
      {//assume
      carga_eqv->DefineClasse(classes->ExisteClasseTag(consEQUIVALENTE));
      }
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaEquivalente(int barra_id, AnsiString codigo, int fases_tag)
   {
   //variáveis locais
   strCARGA     str_carga;
   VTCarga     *carga;
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//cria Carga equivalente com fases iguais às da rede secundária e demanda nula
      str_carga.car_id       = FalsoId();
      str_carga.bar_id       = barra_id;
      str_carga.codigo       = codigo;
      //str_carga.extern_id    = "";
      str_carga.classe_cons  = consEQUIVALENTE;
      str_carga.num_cons.comercial   = 0;
      str_carga.num_cons.residencial = 0;
      str_carga.num_cons.industrial  = 0;
      str_carga.num_cons.rural       = 0;
      str_carga.num_cons.outros      = 0;
      str_carga.num_cons.a4          = 0;
      str_carga.modelo.icte_pu = 1;
      str_carga.modelo.scte_pu = 0;
      str_carga.modelo.zcte_pu = 0;
      str_carga.fases          = fases_tag & faseABCN;
      str_carga.energia_mwhmes = 0;
      //cancela dados de IP
      str_carga.ip.num_lamp =  0.;
      str_carga.ip.pot_kw   =  0.;
      str_carga.ip.fat_pot  =  1.;
      //define curva própria
      str_carga.curva.id        = -1;
      str_carga.curva.tipica    = false;
      str_carga.curva.num_pat   = patamares->NumPatamar();
      str_carga.curva.num_valor = nvCURVA_PQ;
      //define curva de demanda própria nula
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {//define hora inicial e final do patamar
         patamar = patamares->Patamar[np];
         str_carga.curva.patamar[np].hm_ini.hora   = patamar->HoraIni;
         str_carga.curva.patamar[np].hm_ini.minuto = patamar->MinutoIni;
         str_carga.curva.patamar[np].hm_fim.hora   = patamar->HoraFim;
         str_carga.curva.patamar[np].hm_fim.minuto = patamar->MinutoFim;
         str_carga.curva.patamar[np].valor[indP]   = 0;
         str_carga.curva.patamar[np].valor[indQ]   = 0;
         }
      //define posição da carga na Barra
      str_carga.esq.posbar  = 50.;
      str_carga.esq.dxbar   = 100;
	  str_carga.esq.dybar   = -100;
	  //novo campo de categoria
	  str_carga.categoria = -1;
	  //insere Carga em VTMonta
	  carga = ((VTCarga*)(monta->InsereCarga(str_carga)));
      }catch(Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

/*
//-----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaSEC(sdi_CARGA &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strCARGA    str_carga;
   VTCarga    *carga;

   try{//verifica se as fases estão definidas
      if (sdi.fases.IsEmpty())
         {//assume fases da ET sem neutro/terra
         if (plog) plog->Add("Carga BT sem fases definidas: UC= " + sdi.cod);
         fases_tag = fases->Fase1IntersecaoFase2(faseABC, sec.et->sec.fases_tag);
         }
      else
         {
         fases_tag = fases->ExisteFase(sdi.fases);
         //verifica se as fases da carga são compatíveis com as fases da ET
         if (! fases->Fase1ContemFase2(sec.et->sec.fases_tag, fases_tag))
            {//assume fases da ET, sem neutro e terra
            if (plog) plog->Add("Carga BT com fases incompatíveis com a ET: UC= " + sdi.cod);
            fases_tag = fases->Fase1IntersecaoFase2(faseABC, sec.et->sec.fases_tag);
            }
         }
      //preenche estrutura strCARGA
      str_carga.car_id       = sdi.id;
      str_carga.bar_id       = sdi.id_barra;
      str_carga.codigo       = sdi.cod;
      str_carga.extern_id    = "";
      //define classe de consumidor/energia
      str_carga.classe_cons  = TraduzClasseConsumidor(sdi.classe);
      str_carga.modelo.icte_pu = 1;
      str_carga.modelo.scte_pu = 0;
      str_carga.modelo.zcte_pu = 0;
      str_carga.fases          = fases_tag;
      //verifica tipo de medição
      if (sdi.tipo_medicao.AnsiCompareIC("consumo") == 0)
         {//energia: verifica consumo nulo
         if (IsDoubleZero(sdi.consumo_kwhm))
            {
            if (plog) plog->Add("Carga BT com consumo nulo: UC= " + sdi.cod);
            }
         //energia: verifica consumo muito grande
         else if (sdi.consumo_kwhm > 5000)
            {
            if (plog) plog->Add("Carga BT com consumo de " + DoubleToStr("%2.1f kWh/mês", sdi.consumo_kwhm) + ": UC= " + sdi.cod);
            }
         //define consumo
         str_carga.energia = sdi.consumo_kwhm * 0.001; //kWh/mes => MWh/mes
         //cancela vetor de demanda p/ curva própria
         str_carga.curva.id      = -1;
         str_carga.curva.tipica  = true;
         str_carga.curva.num_pat = 0;
         }
      else
         {//curva de demanda própria
         str_carga.curva.id        = -1;
         str_carga.curva.tipica    = false;
         str_carga.curva.num_pat   = 24;
         str_carga.curva.num_valor = nvCURVA_PQ;
         //define curva de demanda própria nula
         for (int np = 0; np < 24; np++)
            {//define hora inicial e final do patamar
            str_carga.curva.patamar[np].hm_ini.hora   = np;
            str_carga.curva.patamar[np].hm_ini.minuto = 0;
            str_carga.curva.patamar[np].hm_fim.hora   = np+1;
            str_carga.curva.patamar[np].hm_fim.minuto = 0;
            str_carga.curva.patamar[np].valor[indP]   = sdi.p_kw[np] * 0.001;
            str_carga.curva.patamar[np].valor[indQ]   = sdi.q_kvar[np] * 0.001;
            }
         }
      //cancela dados de IP
      str_carga.ip.num_lamp = 0.;
      str_carga.ip.pot_kw   = 0.;
      str_carga.ip.fat_pot  = 1.;
      str_carga.esq.posbar  =  50.;
      str_carga.esq.dxbar   =   0;
      str_carga.esq.dybar   = 2000;
      //insere Carga em VTMonta
	  carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
AnsiString  __fastcall TPreMonta::DefineCodigoArranjoCABOxFASE(VTCabo *caboA, VTCabo *caboB, VTCabo *caboC, VTCabo *caboN, AnsiString prefixo)
   {
   //variáveis locais
   AnsiString cod_cabo = "", cod_fase = "";
   struct {
          int        count;
          AnsiString codigo;
          }vet_cabo[3];

   //inicia vet_cabo
   for (int n = 0; n < 3; n++)
      {
      vet_cabo[n].count  = 0;
      vet_cabo[n].codigo = "";
      }
   //analisa cabo_faseA
   if (caboA != NULL)
      {
      cod_fase += "a";
      vet_cabo[0].count  = 1;
      vet_cabo[0].codigo = caboA->Codigo;
      }
   //analisa cabo_faseB
   if (caboB != NULL)
      {
      cod_fase += "b";
      if (caboB == caboA)
         {
         vet_cabo[0].count++;
         }
      else
         {
         vet_cabo[1].count  = 1;
         vet_cabo[1].codigo = caboB->Codigo;
         }
      }
   //analisa cabo_faseC
   if (caboC != NULL)
      {
      cod_fase += "c";
      if (caboC == caboA)
         {
         vet_cabo[0].count++;
         }
      else if (caboC == caboB)
         {
         vet_cabo[1].count++;
         }
      else
         {
         vet_cabo[2].count  = 1;
         vet_cabo[2].codigo = caboC->Codigo;
         }
      }
   //monta código do Arranjo
   for (int n = 0; n < 3; n++)
      {
      if (vet_cabo[n].count == 0) continue;
      //inclui o caractere '_' somente se o código já tiver algum caractere
      if (! cod_cabo.IsEmpty()) cod_cabo += "_";
      //inclui código do Cabo no código do Arranjo
      cod_cabo += IntToStr(vet_cabo[n].count) + "[" + vet_cabo[n].codigo + "]";
      }
   //verifica se tem neutro
   if (caboN != NULL)
      {
      cod_fase += "n";
      //inclui o caractere '_' somente se o código já tiver algum caractere
      if (! cod_cabo.IsEmpty()) cod_cabo += "_";
      //inclui código do Neutro no código do Arranjo
      cod_cabo += "N[" + caboN->Codigo + "]";
      }
   //monta código do Arranjo
   return(prefixo + "(" + cod_fase + ")" + cod_cabo);
   }

//-----------------------------------------------------------------------------
AnsiString  __fastcall TPreMonta::DefineCodigoArranjo_Z0Z1(VTCabo *cabo, int fases_tag)
   {
   //variáveis locais
   AnsiString codigo;
   AnsiString cod_cabo = cabo->Codigo;
   TFase      *fase    = fases->TagToFase[fases_tag];

   //monta código do cabo multiplexado
   codigo.sprintf("M[%s]_%s", cod_cabo, fase->codigo);
   return(codigo);
   }
*/
//-----------------------------------------------------------------------------
AnsiString __fastcall TPreMonta::DefineCodigoUnicoArranjo(AnsiString cod_orig)
   {
   //variáveis locais
   int count= 1;
   AnsiString cod_alt;

   //verifica se não existe arranjo com o código indicado
   if (arranjos->ExisteArranjo(cod_orig) == NULL) return(cod_orig);
   //código já existe: redefine o código
   do {
      cod_alt = cod_orig + "/1" + IntToStr(count++);
      }while (arranjos->ExisteArranjo(cod_alt) != NULL);
   return(cod_alt);
   }

//---------------------------------------------------------------------------
double __fastcall TPreMonta::DefineDefasagemTrafo(int ligacao_pri, int ligacao_sec)
   {
   if (ligacao_pri == ligacao_sec) return(0);
   if (ligacao_pri == lgTRIANG)    return(30);
   else                            return(-30);
   }

//---------------------------------------------------------------------------
VTDominio* __fastcall TPreMonta::ExisteDominio(void)
   {
   //variáveis locais
   TList *lisDOMINIO = redes->LisDominio();
   if (lisDOMINIO->Count > 0)
      {
      return((VTDominio*)(lisDOMINIO->First()));
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TPreMonta::ExtraiFases(AnsiString extern_id)
   {
   //variáveis locais
   int fases_int, pos_igual;
   AnsiString fases_str;

   //proteção
   if(extern_id.IsEmpty())
       return faseABC;
   //determina posição do igual
   pos_igual = extern_id.Pos("=");
   if(pos_igual <= 0)
       return faseABC;
   //determina string com fases e converte para inteiro
   fases_str = extern_id.SubString(pos_igual+1, extern_id.Length() - pos_igual);
   fases_int = fases->ExisteFase(fases_str);

   return fases_int;
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::IniciaClasseConsumidor(void)
   {
   //variáveis locais
   VTClasse  *classe;
   VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
   struct strCONS {
                  int    tag;
                  char   *codigo;
                  TColor color;
				  }vet_cons[] = {{consRESIDENCIAL, "Residencial",  (TColor)8388863},
								 {consCOMERCIAL,   "Comercial",    (TColor)16744448},
								 {consINDUSTRIAL,  "Industrial",   (TColor)4259584},
								 {consRURAL,       "Rural",        (TColor)33023},
								 {consOUTROS,      "Outros",       (TColor)12615935},
								 {consIP,          "IP",           (TColor)8454143},
								 {consA4,          "A4",           (TColor)8421440},
								 {consA2,          "A2",           (TColor)8421440},
                                 {consEQUIVALENTE, "Equivalente",  (TColor)16777088}
                                };
   //verifica se existem todas as classes de consumidor
   for (int n = 0; n < sizeof(vet_cons)/sizeof(strCONS); n++)
      {//verifica se existe a classe de consumidor
      if (classes->ExisteClasseTag(vet_cons[n].tag) == NULL)
         {//cria a Classe
         classe          = DLL_NewObjClasse();
         classe->Id      = -(vet_cons[n].tag + 1);
         classe->Tag     = vet_cons[n].tag;
         classe->Codigo  = vet_cons[n].codigo;;
         classe->Tipico  = true;;
         classe->Color   = vet_cons[n].color;
         //insere Classe em Classes
         classes->InsereClasse(classe);
         }
      }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereArranjoCaboPorfase(sdi_ARRANJO &sdi)
   {
   //variáveis locais
   strARRANJO2 str_arranjo;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Arranjo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura strARRANJO2
      str_arranjo.tipico     = true;
      str_arranjo.data       = DateOf(Now());
      str_arranjo.arranjo_id = sdi.id;
      str_arranjo.codigo     = DefineCodigoUnicoArranjo(sdi.cod);
      str_arranjo.extern_id  = sdi.extern_id;
      str_arranjo.tipo_cfg   = sdi.tipo;
      str_arranjo.fases      = fases->ExisteFase(sdi.fases);
      str_arranjo.res_terra  = 100.;
      str_arranjo.vmin_kv    = 0;
      str_arranjo.vmax_kv    = 0;
      str_arranjo.color      = clBlue;
      str_arranjo.status     = sttNOVO;
      //verifica o tipo de arranjo
      str_arranjo.cfg_suporte_cabo.suporte_id                = sdi.id_suporte;
      str_arranjo.cfg_suporte_cabo.fase[0].cabo_id           = sdi.id_caboA;
      str_arranjo.cfg_suporte_cabo.fase[0].pto_fixacao_id    = sdi.id_pto_caboA;
      str_arranjo.cfg_suporte_cabo.fase[0].multiplicidade =  1;
      str_arranjo.cfg_suporte_cabo.fase[1].cabo_id           = sdi.id_caboB;
      str_arranjo.cfg_suporte_cabo.fase[1].pto_fixacao_id    = sdi.id_pto_caboB;
      str_arranjo.cfg_suporte_cabo.fase[1].multiplicidade    =  1;
      str_arranjo.cfg_suporte_cabo.fase[2].cabo_id           = sdi.id_caboC;
      str_arranjo.cfg_suporte_cabo.fase[2].pto_fixacao_id    = sdi.id_pto_caboC;
      str_arranjo.cfg_suporte_cabo.fase[2].multiplicidade    =  1;
      str_arranjo.cfg_suporte_cabo.fase[3].cabo_id           = sdi.id_caboN;
      str_arranjo.cfg_suporte_cabo.fase[3].pto_fixacao_id    = sdi.id_pto_caboN;
      str_arranjo.cfg_suporte_cabo.fase[3].multiplicidade    =  1;
      //insere Arranjo em VTMonta
      if (monta->InsereArranjo(str_arranjo) == NULL) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereArranjoCaboZ0Z1(sdi_ARRANJO &sdi)
   {
   //variáveis locais
   strARRANJO2 str_arranjo;
   VTSuporte   *suporte;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Arranjo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura strARRANJO2
      str_arranjo.tipico     = true;
      str_arranjo.data       = DateOf(Now());
      str_arranjo.arranjo_id = sdi.id;
      str_arranjo.codigo     = DefineCodigoUnicoArranjo(sdi.cod);
      str_arranjo.extern_id  = sdi.extern_id;
      str_arranjo.tipo_cfg   = sdi.tipo;
      str_arranjo.fases      = fases->ExisteFase(sdi.fases);
      str_arranjo.res_terra  = 100.;
      str_arranjo.vmin_kv    = 0;
      str_arranjo.vmax_kv    = 0;
      str_arranjo.color      = clBlue;
      str_arranjo.status     = sttNOVO;
      str_arranjo.cfg_caboZOZ1.cabo_id    = sdi.id_cabo_Z0Z1;
      str_arranjo.cfg_caboZOZ1.suporte_id = ID_NULO;
      //insere Arranjo em VTMonta
      if (monta->InsereArranjo(str_arranjo) == NULL) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereArranjoPUSB100(sdi_ARRANJO &sdi)
   {
   //variáveis locais
   strARRANJO2 str_arranjo;
   VTSuporte   *suporte;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Arranjo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura strARRANJO2
      str_arranjo.tipico     = true;
      str_arranjo.data       = DateOf(Now());
      str_arranjo.arranjo_id = sdi.id;
      str_arranjo.codigo     = DefineCodigoUnicoArranjo(sdi.cod);
      str_arranjo.extern_id  = sdi.extern_id;
      str_arranjo.tipo_cfg   = sdi.tipo;
      str_arranjo.fases      = fases->ExisteFase(sdi.fases);
      str_arranjo.res_terra  = 100.;
      str_arranjo.vmin_kv    = 0;
      str_arranjo.vmax_kv    = 0;
      str_arranjo.color      = clBlue;
      str_arranjo.status     = sttNOVO;
      str_arranjo.cfg_arranjoZ0Z1.iadm_amp   = sdi.iadm_amp;
      str_arranjo.cfg_arranjoZ0Z1.z0         = sdi.z0_ohm_km;
      str_arranjo.cfg_arranjoZ0Z1.c0         = sdi.c0;
      str_arranjo.cfg_arranjoZ0Z1.z1         = sdi.z1_ohm_km;
      str_arranjo.cfg_arranjoZ0Z1.c1         = sdi.c1;
      str_arranjo.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
      //   if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z0) ||
      //       ! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z1))
      if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z0))
         {
         if (plog) plog->Add("Impedância Z0 nula");
         //return(true);
         }
      if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z1))
         {
         if (plog) plog->Add("Impedância Z1 nula");
         //return(true);
         }
      //insere Arranjo em VTMonta
      if (monta->InsereArranjo(str_arranjo) == NULL) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereArranjoZ0Z1(sdi_ARRANJO &sdi)
   {
   //variáveis locais
   strARRANJO2 str_arranjo;
   VTSuporte   *suporte;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Arranjo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura strARRANJO2
      str_arranjo.tipico     = true;
      str_arranjo.data       = DateOf(Now());
      str_arranjo.arranjo_id = sdi.id;
      str_arranjo.codigo     = DefineCodigoUnicoArranjo(sdi.cod);
      str_arranjo.extern_id  = sdi.extern_id;
      str_arranjo.tipo_cfg   = sdi.tipo;
      str_arranjo.fases      = fases->ExisteFase(sdi.fases);
      str_arranjo.res_terra  = 100.;
      str_arranjo.vmin_kv    = 0;
      str_arranjo.vmax_kv    = 0;
      str_arranjo.color      = clBlue;
      str_arranjo.status     = sttNOVO;
      //verifica o tipo de arranjo
      str_arranjo.cfg_arranjoZ0Z1.iadm_amp   = sdi.iadm_amp;
      str_arranjo.cfg_arranjoZ0Z1.z0         = sdi.z0_ohm_km;
      str_arranjo.cfg_arranjoZ0Z1.c0         = sdi.c0;
      str_arranjo.cfg_arranjoZ0Z1.z1         = sdi.z1_ohm_km;
      str_arranjo.cfg_arranjoZ0Z1.c1         = sdi.c1;
      str_arranjo.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
      //   if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z0) ||
      //       ! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z1))
      if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z0))
         {
         if (plog) plog->Add("Impedância Z0 nula");
         //return(true);
         }
      if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z1))
         {
         if (plog) plog->Add("Impedância Z1 nula");
         //return(true);
         }
      //insere Arranjo em VTMonta
      if (monta->InsereArranjo(str_arranjo) == NULL) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereBarra(sdi_BARRA &sdi)
   {
   //variáveis locais
   strBARRA str_barra;
   VTEqpto  *eqpto;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "BARRA");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede sendo importada
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //preenche estrutura strBARRA
	  str_barra.bar_id  = -sdi.id;
      str_barra.codigo  = sdi.cod;
      str_barra.vnom    = sdi.vnom_kv;
      str_barra.utm.x   = sdi.utm_m.x * 100; //m ==> cm
      str_barra.utm.y   = sdi.utm_m.y * 100; //m ==> cm
      str_barra.esq     = sdi.esq;
      //insere Barra
	  if (monta->InsereBarraEqpto(str_barra) == NULL) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TPreMonta::InsereBarraStart(void)
   {
   monta->InsereBarraStart();
   }

//---------------------------------------------------------------------------
void __fastcall TPreMonta::InsereBarraStop(void)
   {
   monta->InsereBarraStop();
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereBateria(sdi_BATERIA &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strBATERIA  str_bateria;
   VTBateria   *bateria;

   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//verifica se as fases estão definidas

	  if (sdi.fases.IsEmpty())
		 {//assume fases da ET sem terra
		 if (plog) plog->Add("Carga sem fases definidas");
		 fases_tag = faseABCN;
		 }
	  else
		 {//determina fases, filtrando terra
		 fases_tag = fases->ExisteFase(sdi.fases) & faseABCN;
		 }
	  //preenche estrutura strBATERIA
	  str_bateria.bat_id       	   = sdi.id;
	  str_bateria.bar_id       	   = sdi.id_barra;
	  str_bateria.codigo       	   = sdi.cod;
	  str_bateria.extern_id	   	   = sdi.extern_id;
	  str_bateria.fases		   	   = fases_tag;
	  str_bateria.tipo_operacao    = sdi.tipo_operacao;
	  str_bateria.modelo.icte_pu   = sdi.i_cte_pu;
	  str_bateria.modelo.scte_pu   = sdi.s_cte_pu;
	  str_bateria.modelo.zcte_pu   = sdi.z_cte_pu;
	  str_bateria.modelo_carga 	   = sdi.modelo_carga;
	  str_bateria.modelo_desc	   = sdi.modelo_desc;
	  str_bateria.p_nom_kw		   = sdi.p_nom_kw;
	  str_bateria.e_nom_kwh		   = sdi.e_nom_kwh;
	  str_bateria.cap_inv_kva	   = sdi.cap_inv_kva;
	  str_bateria.p_ociosa_pu	   = sdi.p_ociosa_pu;
	  str_bateria.q_ociosa_pu	   = sdi.q_ociosa_pu;
	  str_bateria.rend_carga_pu	   = sdi.rend_carga_pu;
	  str_bateria.rend_desc_pu	   = sdi.rend_desc_pu;
	  str_bateria.gatilho_carga_pu = sdi.gatilho_carga_pu;
	  str_bateria.gatilho_desc_pu  = sdi.gatilho_desc_pu;
	  //curva de carga e descarga
	  str_bateria.curva.id        = -1;
	  str_bateria.curva.tipica    = false;
	  //lê pontos da Curva
	  str_bateria.curva.num_pat   = sdi.curva.num_ponto;
	  str_bateria.curva.num_valor = sdi.curva.dim_valor;
	  //define curva de demanda própria nula
	  for (int np = 0; np < sdi.curva.num_ponto; np++)
	  {//define hora inicial e final do patamar
		str_bateria.curva.patamar[np].hm_ini        = sdi.curva.ponto[np].hm_ini;
		str_bateria.curva.patamar[np].hm_fim        = sdi.curva.ponto[np].hm_fim;
		for (int ind_val = 0; ind_val < sdi.curva.dim_valor; ind_val++)
		{
			str_bateria.curva.patamar[np].valor[ind_val] = sdi.curva.ponto[np].valor[ind_val];// * 0.001;  //kVA =>MVA
		}
	  }
	  //define parâmetros para esquemático
	  str_bateria.esq          = sdi.esq;
	  //insere bateria em VTMonta
	  if (! monta->InsereBateria(str_bateria)) return(true);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCaboUnipolar(sdi_CABO &sdi)
   {
   //variáveis locais
   strCABO str_cabo;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Cabo Unipolar");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura strCABO
      str_cabo.id       = sdi.id;
      str_cabo.codigo   = sdi.cod;
      str_cabo.iadm     = sdi.iadm_amp;
      str_cabo.vmin     = 0;
      str_cabo.vmax     = 0;
      str_cabo.status   = sttNOVO;
      str_cabo.tipo_cfg = sdi.tipo;
      switch (str_cabo.tipo_cfg)
         {
         case caboR_GMR:
            //preenche campos do cabo
            str_cabo.r       = sdi.r_ohm_km;
            str_cabo.raio    = sdi.raio_mm * 0.001;
            str_cabo.gmr     = sdi.rmg_mm * 0.001;
            //preenche campos não usados
            str_cabo.z0.r = str_cabo.z0.x = 0;
            str_cabo.z1.r = str_cabo.z1.x = 0;
            str_cabo.c0   = str_cabo.c1   = 0;
            if (IsDoubleZero(str_cabo.r)||IsDoubleZero(str_cabo.gmr))
               {
               if (plog) plog->Add("Cabo unipolar com atributos nulos");
               if (IsDoubleZero(str_cabo.r))     str_cabo.r    = 0.1696;
               if (IsDoubleZero(str_cabo.gmr))   str_cabo.gmr  = 2.69;
               if (IsDoubleZero(str_cabo.raio))  str_cabo.raio = 0.55;
               //return(true);
               }
            break;
         case caboZ0Z1:
            //preenche campos não usados
            str_cabo.r    = 0;
            str_cabo.raio = 0;
            str_cabo.gmr  = 0;
            //preenche campos do cabo
            str_cabo.z0   = sdi.z0_ohm_km;
            str_cabo.z1   = sdi.z1_ohm_km;
            str_cabo.c0   = sdi.c0;
            str_cabo.c1   = sdi.c1;
            //if ((! ValidaImpedancia(str_cabo.z0))||(! ValidaImpedancia(str_cabo.z1)
            if (! ValidaImpedancia(str_cabo.z0))
               {
               if (plog) plog->Add("Cabo multiplexado com impedância Z0 nula");
               //return(true);
               }
            if (! ValidaImpedancia(str_cabo.z1))
               {
               if (plog) plog->Add("Cabo multiplexado com impedância Z1 nula");
               }
            break;
         default:
            if (plog) plog->Add("Tipo de cabo inválido");
            return(true);
         }
      //insere Cabo em VTMonta
      if (! monta->InsereCabo(str_cabo)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCaboZ0Z1(sdi_CABO &sdi)
   {
   //variáveis locais
   strCABO str_cabo;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Cabo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura strCABO
      str_cabo.id       = sdi.id;
      str_cabo.codigo   = sdi.cod;
      str_cabo.iadm     = sdi.iadm_amp;
      str_cabo.vmin     = 0;
      str_cabo.vmax     = 0;
      str_cabo.status   = sttNOVO;
      str_cabo.tipo_cfg = sdi.tipo;
      switch (str_cabo.tipo_cfg)
         {
         case caboR_GMR:
            //preenche campos do cabo
            str_cabo.r       = sdi.r_ohm_km;
            str_cabo.raio    = sdi.raio_mm * 0.001;
            str_cabo.gmr     = sdi.rmg_mm * 0.001;
            //preenche campos não usados
            str_cabo.z0.r = str_cabo.z0.x = 0;
            str_cabo.z1.r = str_cabo.z1.x = 0;
            str_cabo.c0   = str_cabo.c1   = 0;
            if (IsDoubleZero(str_cabo.r)||IsDoubleZero(str_cabo.gmr))
               {
               if (plog) plog->Add("Cabo unipolar com atributos nulos");
               if (IsDoubleZero(str_cabo.r))     str_cabo.r    = 0.1696;
               if (IsDoubleZero(str_cabo.gmr))   str_cabo.gmr  = 2.69;
               if (IsDoubleZero(str_cabo.raio))  str_cabo.raio = 0.55;
               //return(true);
               }
            break;
         case caboZ0Z1:
            //preenche campos não usados
            str_cabo.r    = 0;
            str_cabo.raio = 0;
            str_cabo.gmr  = 0;
            //preenche campos do cabo
            str_cabo.z0   = sdi.z0_ohm_km;
            str_cabo.z1   = sdi.z1_ohm_km;
            str_cabo.c0   = sdi.c0;
            str_cabo.c1   = sdi.c1;
            //if ((! ValidaImpedancia(str_cabo.z0))||(! ValidaImpedancia(str_cabo.z1)
            if (! ValidaImpedancia(str_cabo.z0))
               {
               if (plog) plog->Add("Cabo multiplexado com impedância Z0 nula");
               //return(true);
               }
            if (! ValidaImpedancia(str_cabo.z1))
               {
               if (plog) plog->Add("Cabo multiplexado com impedância Z1 nula");
               }
            break;
         default:
            if (plog) plog->Add("Tipo de cabo inválido");
            return(true);
         }
      //insere Cabo em VTMonta
      if (! monta->InsereCabo(str_cabo)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCapacitor(sdi_CAPACITOR &sdi)
   {
   //variáveis locais
   strCAPACITOR str_capacitor;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Capacitor");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica pot.reativa
      if (IsDoubleZero(sdi.snom_kva))
         {
         if (plog) plog->Add("Potência reativa nula");
         return(true);
         }
      //preenche estrutura strCAPACITOR
      str_capacitor.cap_id       = sdi.id;
      str_capacitor.extern_id    = "";
      str_capacitor.codigo       = sdi.cod;
	  str_capacitor.bar_id       = -sdi.id_barra;
      str_capacitor.vnom         = sdi.vnom_kv;
      str_capacitor.q_mvar       = sdi.snom_kva * 0.001;   //kVAr => MVAr
      str_capacitor.ligacao      = TraduzTipoLigacao(sdi.ligacao);
      //define curva
      str_capacitor.curva.id        = sdi.curva.id;
      str_capacitor.curva.tipica    = (sdi.curva.tipica.AnsiCompareIC("tipica")==0);
      if (str_capacitor.curva.tipica)
         {//não preenche pontos da curva
         str_capacitor.curva.num_pat   = 0;
         str_capacitor.curva.num_valor = 0;
         }
      else
         {//preenche pontos da curva
         str_capacitor.curva.num_pat   = sdi.curva.num_ponto;
         str_capacitor.curva.num_valor = sdi.curva.dim_valor;
         for (int np = 0; np < sdi.curva.num_ponto; np++)
            {//preenche dados do patamar
            str_capacitor.curva.patamar[np].hm_ini      = sdi.curva.ponto[np].hm_ini;
            str_capacitor.curva.patamar[np].hm_fim      = sdi.curva.ponto[np].hm_fim;
            for (int nv = 0; nv < str_capacitor.curva.num_valor; nv++)
               {
               str_capacitor.curva.patamar[np].valor[nv] = sdi.curva.ponto[np].valor[nv];
               }
            }
         }
      //define parâmetros para esquemático
      str_capacitor.esq          = sdi.esq;
      //insere Capacitor em VTMonta
      if (! monta->InsereCapacitor(str_capacitor)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCapacitorSerie(sdi_CAPSERIE &sdi)
   {
   //variáveis locais
   strCAPSERIE  str_capserie;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Capserie");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura strCHAVE
      str_capserie.obra        = false;
      str_capserie.id          = sdi.id;
      str_capserie.rede_id     = rede->Id;
	  str_capserie.bar1_id     = -sdi.id_bar1;
	  str_capserie.bar2_id     = -sdi.id_bar2;
      str_capserie.codigo      = sdi.cod;
      str_capserie.extern_id   = sdi.extern_id;
      str_capserie.vnom        = sdi.vnom_kv;
      str_capserie.snom        = sdi.snom_kva;
      str_capserie.z           = sdi.z_ohm;
      str_capserie.esq         = sdi.esq;
      //insere Capserie em VTMonta
      if ((monta->InsereCapserie(str_capserie))== NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::InsereCargaComMedicao(sdi_CARGA &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strCARGA    str_carga;
   VTCarga    *carga;

   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//verifica se as fases estão definidas
      if (sdi.fases.IsEmpty())
         {//assume fases da ET sem neutro/terra
         if (plog) plog->Add("Carga sem fases definidas");
         fases_tag = faseABC;
         }
      else
         {//determina fases, filtrando neutro
         fases_tag = fases->ExisteFase(sdi.fases) & faseABC;
         }
      //preenche estrutura strCARGA
      str_carga.car_id         = sdi.id;
	  str_carga.bar_id         = -sdi.id_barra;
      str_carga.codigo         = sdi.cod;
      //str_carga.extern_id      = "";
      //define classe de consumidor/energia
	  str_carga.grupoA         = (sdi.grupo_tarifa.AnsiCompareIC("A") == 0);
	  str_carga.classe_cons    = TraduzClasseConsumidor(sdi.classe, sdi.grupo_tarifa);
      str_carga.modelo.icte_pu = sdi.i_cte_pu;
      str_carga.modelo.scte_pu = sdi.s_cte_pu;
      str_carga.modelo.zcte_pu = sdi.z_cte_pu;
      str_carga.fases          = fases_tag;
      str_carga.energia_mwhmes = sdi.consumo_kwhm * 0.001; //kWh/mes => MWh/mes
      //verifica se deve utilizar energia ou curva de demanda própria
      if ((ConsA4_Energia)&&(sdi.consumo_kwhm > 0))
         {//cancela vetor de demanda p/ curva própria
         str_carga.curva.id      = -1;
         str_carga.curva.tipica  = true;
         str_carga.curva.num_pat = 0;
         }
      else
         {//curva de demanda própria
         str_carga.curva.id        = -1;
         str_carga.curva.tipica    = false;
         //lê pontos da Curva
         str_carga.curva.num_pat   = sdi.curva.num_ponto;
         str_carga.curva.num_valor = sdi.curva.dim_valor;
         //define curva de demanda própria nula
         for (int np = 0; np < sdi.curva.num_ponto; np++)
            {//define hora inicial e final do patamar
            str_carga.curva.patamar[np].hm_ini        = sdi.curva.ponto[np].hm_ini;
            str_carga.curva.patamar[np].hm_fim        = sdi.curva.ponto[np].hm_fim;
            for (int ind_val = 0; ind_val < sdi.curva.dim_valor; ind_val++)
               {
               str_carga.curva.patamar[np].valor[ind_val] = sdi.curva.ponto[np].valor[ind_val] * 0.001;  //kVA =>MVA
			   }
			}
		 }
	  //insere quantidade de consumidores por classe
	  str_carga.num_cons.residencial = sdi.num_cons.residencial;
	  str_carga.num_cons.comercial = sdi.num_cons.comercial;
	  str_carga.num_cons.industrial = sdi.num_cons.industrial;
	  str_carga.num_cons.rural = sdi.num_cons.rural;
	  str_carga.num_cons.outros = sdi.num_cons.outros;
	  str_carga.num_cons.a4 = sdi.num_cons.a4;
	  //cancela dados de IP
      str_carga.ip.num_lamp = 0.;
      str_carga.ip.pot_kw   = 0.;
      str_carga.ip.fat_pot  = 1.;
	  str_carga.esq         = sdi.esq;
	  //novo campo de categoria
	  str_carga.categoria = -1;
      //insere Carga em VTMonta
	  carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::InsereCargaIP(sdi_CARGA_IP &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strCARGA    str_carga;
   VTCarga    *carga;

   if (plog) plog->DefineValor("Eqpto", "Carga IP");  //DVK 2015.10.22
   if (plog) plog->DefineValor("Código", sdi.cod);    //DVK 2015.10.22
   try{//verifica se as fases estão definidas
      if (sdi.fases.IsEmpty())
         {//assume fases da ET sem neutro/terra
         if (plog) plog->Add("Carga IP sem fases definidas");
         fases_tag = faseAB;
         }
      else
         {//determina fases, filtrando neutro
         fases_tag = fases->ExisteFase(sdi.fases) & faseABC;
         }
      //preenche estrutura strCARGA
      str_carga.car_id       = sdi.id;
	  str_carga.bar_id       = -sdi.id_barra;
      str_carga.codigo       = sdi.cod;
      //str_carga.extern_id    = "";
      //define classe de consumidor e modelo de carga
      str_carga.classe_cons    = consIP;
      str_carga.modelo.icte_pu = 0;
      str_carga.modelo.scte_pu = 1;
      str_carga.modelo.zcte_pu = 0;
      str_carga.fases          = fases_tag;
	  //define dados de IP
      str_carga.ip.num_lamp = sdi.num_lampada;
	  str_carga.ip.pot_kw   = sdi.p_w * 0.001; //W => kW
	  str_carga.ip.fat_pot  = sdi.fp_pu;
	  //dfine uso de curva típica
	  str_carga.energia_mwhmes = 0;
	  str_carga.curva.id       = -1;
	  str_carga.curva.tipica   = true;
	  str_carga.curva.num_pat  = 0;
	  //define dados esquemático
	  str_carga.esq           = sdi.esq;
	  //novo campo de categoria
	  str_carga.categoria = -1;
      //insere Carga em VTMonta
	  carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::InsereCargaTipica(sdi_CARGA &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strCARGA    str_carga;
   VTCarga    *carga;

   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//verifica se as fases estão definidas
      if (sdi.fases.IsEmpty())
		 {//assume fases da ET sem terra
		 if (plog) plog->Add("Carga sem fases definidas");
         fases_tag = faseABCN;
         }
      else
		 {//determina fases, filtrando terra
         fases_tag = fases->ExisteFase(sdi.fases) & faseABCN;
         }
	  //verifica se o consumo é nulo //DVK 2015.10.23
	  if(IsDoubleZero(sdi.consumo_kwhm ))
         {
         if (plog) plog->Add("Carga com consumo nulo");
         }
      //preenche estrutura strCARGA
      str_carga.car_id       = sdi.id;
	  str_carga.bar_id       = -sdi.id_barra;
	  str_carga.codigo       = sdi.cod;
	  //str_carga.extern_id    = "";
	  //define classe de consumidor/energia
	  str_carga.grupoA       = (sdi.grupo_tarifa.AnsiCompareIC("A") == 0);
	  str_carga.classe_cons    = TraduzClasseConsumidor(sdi.classe, sdi.grupo_tarifa);
	  str_carga.modelo.icte_pu = sdi.i_cte_pu;
      str_carga.modelo.scte_pu = sdi.s_cte_pu;
      str_carga.modelo.zcte_pu = sdi.z_cte_pu;
      str_carga.fases          = fases_tag;
	  str_carga.energia_mwhmes = sdi.consumo_kwhm * 0.001; //kWh/mes => MWh/mes
      //cancela vetor de demanda p/ curva própria
      str_carga.curva.id      = -1;
      str_carga.curva.tipica  = true;
      str_carga.curva.num_pat = 0;
      //insere quantidade de consumidores por classe
	  str_carga.num_cons.residencial = sdi.num_cons.residencial;
	  str_carga.num_cons.comercial = sdi.num_cons.comercial;
	  str_carga.num_cons.industrial = sdi.num_cons.industrial;
	  str_carga.num_cons.rural = sdi.num_cons.rural;
	  str_carga.num_cons.outros = sdi.num_cons.outros;
	  str_carga.num_cons.a4 = sdi.num_cons.a4;
	  //cancela dados de IP
      str_carga.ip.num_lamp = 0.;
      str_carga.ip.pot_kw   = 0.;
      str_carga.ip.fat_pot  = 1.;
	  str_carga.esq         = sdi.esq;
	  //novo campo de categoria
	  str_carga.categoria = -1;
	  //insere Carga em VTMonta
	  carga = (VTCarga*)monta->InsereCarga(str_carga);
	  }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereChave(sdi_CHAVE &sdi)
   {
   //variáveis locais
   strCHAVE  str_chave;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Chave");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   //verifica se está incluindo a topologia da subestação
   //if (! ETD_Enabled) return(true);
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //descarta chave "GRAMPO DE LINHA VIVA"
      //if (sdi.tipo.AnsiCompareIC("GRAMPO DE LINHA VIVA") == 0) return(true);
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura strCHAVE
	  str_chave.rede_id           = rede->Id;
	  str_chave.chv_id            = sdi.id;
	  str_chave.codigo            = sdi.cod;
	  str_chave.bar1_id           = -sdi.id_bar1;
	  str_chave.bar2_id           = -sdi.id_bar2;
	  str_chave.tipochave.id      = sdi.tipochave.id;
	  str_chave.tipochave.tipo    = sdi.tipochave.tipo;
	  str_chave.tipochave.codigo  = sdi.tipochave.cod;
      str_chave.estado            = (sdi.estado == 0) ? chvABERTA: chvFECHADA;
	  str_chave.corn              = sdi.iadm_amp;
	  str_chave.protecao_ativa    = sdi.protecao_ativa;
	  str_chave.telecomandada     = sdi.telecomandada;
	  str_chave.tecno_comunicacao = sdi.tecno_comunicacao;
	  //trunca tipo_gis se tiver mais de 15 caracteres (limite da base)
	  str_chave.tipo_gis    = sdi.tipo_gis;
	  if(sdi.tipo_gis.Length() > 15)
		 str_chave.tipo_gis = sdi.tipo_gis.SubString(0, 15);
	  str_chave.elo_fusivel = sdi.elo_fusivel;
	  //define parâmetros para esquemático
	  str_chave.esq         = sdi.esq;
      //insere Chave em VTMonta
      if ((monta->InsereChave(str_chave))== NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
VTCluster* __fastcall TPreMonta::InsereCluster(AnsiString codigo)
   {
   //variáveis locais
   strCLUSTER str;
   VTCluster *cluster;

   //verifica se foi definido o código
   if (codigo.IsEmpty()) return(NULL);
   //verifica se existe o Cluster
   if ((cluster = redes->ExisteCluster(codigo)) == NULL)
      {//inser um novo Cluster
      str.obra       = false;
      str.cluster_id = FalsoId();
      str.codigo     = codigo;
      cluster = (VTCluster*)monta->InsereCluster(str);
      }
   return(cluster);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCurvaCapacitor(sdi_CURVA &sdi)
   {
   //variáveis locais
   VTCurva         *curva;
   strCURVA_TIPICA str_curva;

   try{//inicia estrutura
      if (! MontaCurvaTipica(sdi, str_curva)) return(false);
      //insere Curva típica em VTMonta
      curva = monta->InsereCurvaTipicaCapacitor(str_curva);
      }catch (Exception &e)
         {
         curva = NULL;
         }
   return(curva);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCurvaCarga(sdi_CURVA &sdi)
   {
   //variáveis locais
   VTCurva         *curva;
   strCURVA_TIPICA str_curva;

   try{//inicia estrutura
      if (! MontaCurvaTipica(sdi, str_curva)) return(false);
      //complementa atributo específico de Curva de Carga
      str_curva.classe_tag = TraduzClasseConsumidor(sdi.classe_cons);
      //insere Curva típica em VTMonta
      curva = monta->InsereCurvaTipicaCarga(str_curva);
      }catch (Exception &e)
         {
         curva = NULL;
         }
   return(curva);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCurvaGerador(sdi_CURVA &sdi)
   {
   //variáveis locais
   VTCurva         *curva;
   strCURVA_TIPICA str_curva;

   try{//inicia estrutura
      if (! MontaCurvaTipica(sdi, str_curva)) return(false);
      //insere Curva típica em VTMonta
      curva = monta->InsereCurvaTipicaGerador(str_curva);
      }catch (Exception &e)
         {
         curva = NULL;
         }
   return(curva);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCurvaCargaIP(sdi_CURVA &sdi)
   {
   //variáveis locais
   VTCurva         *curva;
   strCURVA_TIPICA str_curva;

   try{//inicia estrutura
      if (! MontaCurvaTipica(sdi, str_curva)) return(false);
      //insere Curva típica em VTMonta
      curva = monta->InsereCurvaTipicaIP(str_curva);
      }catch (Exception &e)
         {
         curva = NULL;
         }
   return(curva);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCurvaReator(sdi_CURVA &sdi)
   {
   //variáveis locais
   VTCurva         *curva;
   strCURVA_TIPICA str_curva;

   try{//inicia estrutura
      if (! MontaCurvaTipica(sdi, str_curva)) return(false);
      //insere Curva típica em VTMonta
      curva = monta->InsereCurvaTipicaReator(str_curva);
      }catch (Exception &e)
         {
         curva = NULL;
         }
   return(curva);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDominio(AnsiString codigo)
   {
   //variáveis locais
   AnsiString  txt;
   strDOMINIO str_dominio;
   TDateTime   data = DateOf(Now());

   //verifica se já foi criado um Dominio
   if (dominio != NULL) return(true);
   //verifica se já existe um Dominio
   if ((dominio = ExisteDominio()) != NULL) return(true);
   //insere um Dominio em VTMonta
   txt.sprintf("%s_%04d.%02d.%02d", codigo.c_str(), YearOf(data), MonthOf(data), DayOf(data));
   str_dominio.obra        = false;
   str_dominio.dominio_id  = ID_NULO;
   str_dominio.codigo      = txt;
   str_dominio.data_criacao = data;
   //insere Dominio e salva ponteiro p/ objeto
   dominio = (VTDominio*)monta->InsereDominio(str_dominio);

   return(dominio != NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereFiltro(sdi_FILTRO &sdi)
   {
   //variáveis locais
   strFILTRO  str_filtro;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Filtro");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica pot.reativa
      if (IsDoubleZero(sdi.snom_kva))
         {
         if (plog) plog->Add("Potência nominal nula");
         return(true);
         }
      //preenche estrutura strFILTRO
      str_filtro.id           = sdi.id;
      str_filtro.extern_id    = "";
      str_filtro.codigo       = sdi.cod;
	  str_filtro.bar_id       = -sdi.id_bar;
      str_filtro.vnom         = sdi.vnom_kv;
      str_filtro.snom         = sdi.snom_kva * 0.001;   //kVAr => MVAr
      //define parâmetros para esquemático
      str_filtro.esq          = sdi.esq;
      //insere Filtro em VTMonta
      if (! monta->InsereFiltro(str_filtro)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereGerador(sdi_GERADOR &sdi)
   {
   //variáveis locais
   strGERADOR str_gerador;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Gerador");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se a tensão nominal do Gerador é nula
      if (IsDoubleZero(sdi.vnom_kv))
         {//assume tensão nominal da Barra
         if (plog) plog->Add("Tensão nominal nula");
         }
      //insere Rede em VTMonta
      str_gerador.ger_id       = sdi.id;
	  str_gerador.bar_id       = -sdi.id_bar;
	  str_gerador.codigo       = sdi.cod;
	  str_gerador.tiposup      = TraduzTipoSuprimento(sdi.tipo_sup);
      str_gerador.snom_mva     = sdi.snom_kva * 0.001;  //kVA => MVA
	  str_gerador.smax_mva     = str_gerador.snom_mva;
	  str_gerador.vnom         = sdi.vnom_kv;
	  str_gerador.z0           = sdi.z0_pu;
	  str_gerador.z1           = sdi.z1_pu;
	  str_gerador.ligacao      = sdi.ligacao; // DVK 2013.07.11
	  str_gerador.fplim        = sdi.fplim_pu; // DVK 2013.07.11
	  str_gerador.tipo_geracao = sdi.tipo_geracao; // DVK 2013.07.11
	  //fases
      if(sdi.fases == faseINV)
         {
         sdi.fases = ExtraiFases(sdi.extern_id);
         }
	  str_gerador.fases        = sdi.fases;
	  // define curva
	  str_gerador.curva.id     = sdi.curva.id;
      str_gerador.curva.tipica    = (sdi.curva.tipica.AnsiCompareIC("tipica")==0);
	  if (str_gerador.curva.tipica)
         {//não preenche pontos da curva
		 str_gerador.curva.num_pat   = 0;
		 str_gerador.curva.num_valor = 0;
		 }
	  else
         {//preenche pontos da curva
		 str_gerador.curva.num_pat   = sdi.curva.num_ponto;
		 str_gerador.curva.num_valor = sdi.curva.dim_valor;
		 for (int np = 0; np < sdi.curva.num_ponto; np++)
            {//preenche dados do patamar
			str_gerador.curva.patamar[np].hm_ini      = sdi.curva.ponto[np].hm_ini;
			str_gerador.curva.patamar[np].hm_fim      = sdi.curva.ponto[np].hm_fim;
			for (int nv = 0; nv < str_gerador.curva.num_valor; nv++)
			   {
			   str_gerador.curva.patamar[np].valor[nv] = sdi.curva.ponto[np].valor[nv];
			   }
            }
		 }
      //define parâmetros para esquemático
	  str_gerador.esq = sdi.esq;
      //insere Gerador
      if (monta->InsereGerador(str_gerador) == NULL) return(true);  //return(false);
      }catch(Exception &e)
		{
         return(false);
		}
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereMedidor(sdi_MEDIDOR &sdi)
   {
   //variáveis locais
   strMEDIDOR str_medidor;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Medidor");
   if (plog) plog->DefineValor("Código", sdi.cod);
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //preenche estrutura strMEDIDOR
      str_medidor.medidor_id  = sdi.id;
	  str_medidor.bar_id      = -sdi.id_bar;
      str_medidor.codigo      = sdi.cod;
      str_medidor.extern_id   = "";
      //define parâmetros para esquemático
      str_medidor.esq         = sdi.esq;
      //insere Medidor em VTMonta
      if (! monta->InsereMedidor(str_medidor)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }


//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereMedidorCanal(sdi_CANAL &sdi)
   {
   //variáveis locais
   strCANAL str_canal;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Canal");
   if (plog) plog->DefineValor("Código", sdi.cod);
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //preenche estrutura strCANAL
      str_canal.canal_id           = sdi.id;
      str_canal.medidor_id         = sdi.id_medidor;
      str_canal.codigo             = sdi.cod;
      str_canal.tipo_canal         = sdi.tipocanal;
      str_canal.eqpto_medido.tipo  = sdi.eqpto_tipo;
      str_canal.eqpto_medido.id    = sdi.eqpto_id;
      str_canal.eqpto_medido.fases = sdi.fases;
      str_canal.tolerancia_perc    = sdi.toler_perc;
      str_canal.curva.unidade      = sdi.curva.unidade;
      str_canal.curva.num_pat      = sdi.curva.num_ponto;
      str_canal.curva.num_valor    = sdi.curva.dim_valor;
      for (int np = 0; np < sdi.curva.num_ponto; np++)
         {//preenche dados do patamar
         str_canal.curva.patamar[np].hm_ini      = sdi.curva.ponto[np].hm_ini;
         str_canal.curva.patamar[np].hm_fim      = sdi.curva.ponto[np].hm_fim;
         for (int nv = 0; nv < str_canal.curva.num_valor; nv++)
            {
            str_canal.curva.patamar[np].valor[nv] = sdi.curva.ponto[np].valor[nv];
            }
         }
      //insere Canal em VTMonta
      if (! monta->InsereMedidorCanal(str_canal)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereMutua(sdi_MUTUA &sdi)
   {
   //variáveis locais
   strMUTUA str_mutua;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Mutua");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se os Trechos são distintos
      if (sdi.id_tre1 == sdi.id_tre2)
         {
         if (plog) plog->Add("Trechos iguais");
         return(true);
         }
      //preenche estrutura strMUTUA
      str_mutua.id           = sdi.id;
      str_mutua.extern_id    = "";
      str_mutua.codigo       = sdi.cod;
      str_mutua.tre1_id      = sdi.id_tre1;
      str_mutua.tre2_id      = sdi.id_tre2;
      //barras de referências dos trecos
	  str_mutua.tre1_bar_id  = -sdi.id_bar_tre2;
	  str_mutua.tre2_bar_id  = -sdi.id_bar_tre2;
      str_mutua.z0           = sdi.z0;
      str_mutua.z1           = sdi.z1;
      //define parâmetros para esquemático
      str_mutua.postre1      = sdi.postre1;
      str_mutua.postre2      = sdi.postre2;
      //insere Mutua em VTMonta
      if (! monta->InsereMutua(str_mutua)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReator(sdi_REATOR &sdi)
   {
   //variáveis locais
   strREATOR     str_reator;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Reator");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica pot.reativa
      if (IsDoubleZero(sdi.q_kvar))
         {
         if (plog) plog->Add("Potência reativa nula");
         return(true);
         }
      //preenche estrutura strREATOR
      str_reator.id           = sdi.id;
      str_reator.extern_id    = "";
      str_reator.codigo       = sdi.cod;
	  str_reator.bar_id       = -sdi.id_bar;
      str_reator.vnom         = sdi.vnom_kv;
      str_reator.q_mvar       = sdi.q_kvar * 0.001;   //kVAr => MVAr
      str_reator.ligacao      = sdi.ligacao;
      //define curva
      str_reator.curva.id        = sdi.curva.id;
      str_reator.curva.tipica    = (sdi.curva.tipica.AnsiCompareIC("tipica")==0);
      if (str_reator.curva.tipica)
         {//não preenche pontos da curva
         str_reator.curva.num_pat   = 0;
         str_reator.curva.num_valor = 0;
         }
      else
         {//preenche pontos da curva
         str_reator.curva.num_pat   = sdi.curva.num_ponto;
         str_reator.curva.num_valor = sdi.curva.dim_valor;
         for (int np = 0; np < sdi.curva.num_ponto; np++)
            {//preenche dados do patamar
            str_reator.curva.patamar[np].hm_ini      = sdi.curva.ponto[np].hm_ini;
            str_reator.curva.patamar[np].hm_fim      = sdi.curva.ponto[np].hm_fim;
            for (int nv = 0; nv < str_reator.curva.num_valor; nv++)
               {
               str_reator.curva.patamar[np].valor[nv] = sdi.curva.ponto[np].valor[nv];
               }
            }
         }
      //define parâmetros para esquemático
      str_reator.esq = sdi.esq;
      //insere Reator em VTMonta
      if (! monta->InsereReator(str_reator)) return(true);  //return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereRede(sdi_REDE &sdi)
   {
   //variáveis locais
   strREDE    str_rede;
   VTCluster *cluster;

   //se ainda não existir, cria o Dominio
   if (dominio == NULL) InsereDominio(codigo_dominio);
   //reinicia ponteiro p/ Rede sendo montada
   rede = NULL;
   //configura Log
   if (plog) plog->DefineValor("Eqpto", "Rede");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//insere Rede em VTMonta
      if (plog) plog->DefineValor("Rede", sdi.cod);
      str_rede.rede_id            = sdi.id;
      str_rede.mrede_id           = ID_NULO;
      str_rede.cluster_id         = ID_NULO;
      str_rede.barini_id          = sdi.barini_id;
      str_rede.dominio_id         = (dominio != NULL) ? dominio->Id : ID_NULO;
      str_rede.codigo             = sdi.cod;
      str_rede.tiporede.id        = sdi.tiporede.id;
      str_rede.tiporede.segmento  = sdi.tiporede.seg;
      str_rede.tiporede.codigo    = sdi.tiporede.cod;
      str_rede.color              = TColor(sdi.color_r + (sdi.color_g << 8) + (sdi.color_b << 16));
      //estática
      if      (sdi.estatica == 1) str_rede.estatica = true;
      else if (sdi.estatica == 2) str_rede.estatica = false;
      else                        str_rede.estatica = (sdi.tiporede.seg != redePRI);
      //radial
      if      (sdi.radial == 1) str_rede.radial = true;
      else if (sdi.radial == 2) str_rede.radial = false;
      else                      str_rede.radial = ((sdi.tiporede.seg == redePRI) || ((sdi.tiporede.seg == redeSEC)));
      //equivalente
      str_rede.equivalente        = false;
      //verifica se Rede está associada a um Cluster
      if ((cluster = InsereCluster(sdi.cluster)) != NULL)
         {//associa Rede ao Cluster
         str_rede.cluster_id = cluster->Id;
         //assume rede como estática
         //str_rede.estatica = true;
         }
      if ((rede = (VTRede*)monta->InsereRede(str_rede)) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {//cancela última Subestacao criada
         rede = NULL;
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReguladorAutomatico(sdi_REGULADOR &sdi)
   {
   //variáveis locais
   strREGULADOR str_reg;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Regulador Automatico");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura strREGULADOR
      str_reg.obra        = false;
      str_reg.id          = sdi.id;
      str_reg.rede_id     = rede->Id;
	  str_reg.bar1_id     = -sdi.id_bar1;
	  str_reg.bar2_id     = -sdi.id_bar2;
      str_reg.sentido.bar_ref_id = sdi.id_bar_ent;
      str_reg.modelo      = sdi.modelo;
      str_reg.codigo      = sdi.cod;
      str_reg.by_pass     = false;
      str_reg.ligacao     = TraduzTipoLigacao(sdi.ligacao);
      str_reg.fases       = fases->ExisteFase(sdi.fases);
      //proteção
      if (sdi.fases.IsEmpty())
         {//define fases em função do tipo da ligação
         switch(str_reg.ligacao)
            {
            case lgEST_AT:           str_reg.fases = faseABCNT; break;
            case lgTRIANG:           str_reg.fases = faseABC;   break;
            case lgEST_ISO:          str_reg.fases = faseABC;   break;
            case lgMONOFASICO:       str_reg.fases = faseAN;    break;
            case lgDELTA_ABERTO:     str_reg.fases = faseB;     break;
            case lgDELTA_FECHADO:    str_reg.fases = faseABC;   break;
            default:                 str_reg.fases = faseABC;   break;
            }
         }
      str_reg.snom_mva    = sdi.snom_kva * 0.001; //kVA =>MVA
      str_reg.var_tensao  = sdi.dv_perc;
//      str_reg.num_passo   = sdi.num_passo / 2;
      str_reg.num_passo   = sdi.num_passo;    //DVK 2015.11.09 no ExportaEqpto não multiplica por 2
      //proteção: se não houver valores de número de passos e faixa de tensão, assume desligado
      if ((str_reg.var_tensao == 0)||(str_reg.num_passo == 0))
         {
         str_reg.by_pass = true;
         if (str_reg.var_tensao == 0) str_reg.var_tensao = 10;
         if (str_reg.num_passo  == 0)  str_reg.num_passo = 16;
         if (plog) plog->Add("Regulador redefinido como inativo");
         }
      str_reg.z0          = sdi.z0_pu;
      str_reg.z1          = sdi.z1_pu;
      //configura parâmetros para ajuste automático
      str_reg.cfg_auto.bar_ref_id = sdi.ajuste_auto.id_bar_ref;
      str_reg.cfg_auto.v_pu       = sdi.ajuste_auto.v_pu;
      str_reg.cfg_auto.bar_ref_inv_id = sdi.ajuste_auto.id_bar_ref_inv;
	  str_reg.cfg_auto.vinv_pu        = sdi.ajuste_auto.vinv_pu;
	  str_reg.cfg_auto.larg_bandamorta_pu = sdi.ajuste_auto.larg_bandamorta_pu;
	  str_reg.cfg_auto.larg_bandamorta_inv_pu = sdi.ajuste_auto.larg_bandamorta_inv_pu;
      //parâmetros p/ ajuste fixo
	  str_reg.cfg_fixo.passo      = 0;
	  //relação de tap
	  str_reg.rel_tap = sdi.rtp;
      //define parâmetros para esquemático
      str_reg.esq = sdi.esq;
      //insere Regulador em VTMonta
      if (monta->InsereRegulador(str_reg)== NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReguladorTapFixo(sdi_REGULADOR &sdi)
   {
   //variáveis locais
   strREGULADOR str_reg;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Regulador Tap Fixo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
	  str_reg.bar1_id     = -sdi.id_bar1;
	  str_reg.bar2_id     = -sdi.id_bar2;
      str_reg.sentido.bar_ref_id = sdi.id_bar_ent;
      //preenche estrutura strREGULADOR
      str_reg.obra        = false;
      str_reg.id          = sdi.id;
      str_reg.rede_id     = rede->Id;
	  str_reg.bar1_id     = -sdi.id_bar1;
	  str_reg.bar2_id     = -sdi.id_bar2;
      str_reg.codigo      = sdi.cod;
      str_reg.by_pass     = false;
      str_reg.ligacao     = TraduzTipoLigacao(sdi.ligacao);
      str_reg.fases       = fases->ExisteFase(sdi.fases);
      str_reg.modelo      = sdi.modelo;
      //proteção
      if (sdi.fases.IsEmpty())
         {//define fases em função do tipo da ligação
         switch(str_reg.ligacao)
            {
            case lgEST_AT:           str_reg.fases = faseABCNT; break;
            case lgTRIANG:           str_reg.fases = faseABC;   break;
            case lgEST_ISO:          str_reg.fases = faseABC;   break;
            case lgMONOFASICO:       str_reg.fases = faseAN;    break;
            case lgDELTA_ABERTO:     str_reg.fases = faseB;     break;
            case lgDELTA_FECHADO:    str_reg.fases = faseABC;   break;
            default:                 str_reg.fases = faseABC;   break;
            }
         }
      str_reg.snom_mva    = sdi.snom_kva * 0.001; //kVA =>MVA
      str_reg.var_tensao  = sdi.dv_perc;
//      str_reg.num_passo   = sdi.num_passo / 2;
      str_reg.num_passo   = sdi.num_passo;    //DVK 2015.11.09 no ExportaEqpto não multiplica por 2
      //proteção: se não houver valores de número de passos e faixa de tensão, assume desligado
      if ((str_reg.var_tensao == 0)||(str_reg.num_passo == 0))
         {
         str_reg.by_pass = true;
         if (str_reg.var_tensao == 0) str_reg.var_tensao = 10;
         if (str_reg.num_passo  == 0)  str_reg.num_passo = 16;
         if (plog) plog->Add("Regulador redefinido como inativo");
         }
      str_reg.z0                 = sdi.z0_pu;
      str_reg.z1                 = sdi.z1_pu;
      //parâmetros p/ ajuste fixo
      str_reg.cfg_fixo.passo      = sdi.ajuste_fixo.passo;
      //configura parâmetros para ajuste automático
      str_reg.cfg_auto.bar_ref_id = ID_NULO;
      str_reg.cfg_auto.v_pu       = 1.;
      str_reg.cfg_auto.bar_ref_inv_id = ID_NULO;
      str_reg.cfg_auto.vinv_pu        = 1.;
      //define parâmetros para esquemático
      str_reg.esq = sdi.esq;
      //insere Regulador em VTMonta
      if (monta->InsereRegulador(str_reg)== NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSuporte(sdi_SUPORTE &sdi)
   {
   //variáveis locais
   strSUPORTE str;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Suporte");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//preenche estrutura do suporte
      str.id     = sdi.id;
      str.codigo = sdi.cod;
      str.status = sttNOVO;
      //define tipo
      if      (sdi.tipo.AnsiCompareIC("aéreo")       == 0) str.tipo = suporteAEREO;
      else if (sdi.tipo.AnsiCompareIC("subterrâneo") == 0) str.tipo = suporteSUBTERRANEO;
      else if (sdi.tipo.AnsiCompareIC("submerso")    == 0) str.tipo = suporteSUBMERSO;
      else                                                 str.tipo = suporteAEREO; //suporteINDEFINIDO;
      str.num_pto = sdi.num_pto_fixacao;
      //proteção
      if (str.num_pto > 20) str.num_pto = 20;
      for (int n = 0; n < str.num_pto; n++)
         {
         str.pto_fixacao[n].id      = sdi.pto_fixacao[n].id;   //n+1;
         str.pto_fixacao[n].codigo  = sdi.pto_fixacao[n].cod;
         str.pto_fixacao[n].coord_x = sdi.pto_fixacao[n].x_m;
         str.pto_fixacao[n].coord_y = sdi.pto_fixacao[n].y_m;
         }
      //insere Suporte em VTMonta
      if (monta->InsereSuporte(str) == NULL) return(true);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSuprimento(sdi_SUPRIMENTO &sdi)
   {
   //variáveis locais
   double        v_pu;
    strSUPRIMENTO str_sup;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Suprimento");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se a tensão nominal do Suprimento é nula
      if (IsDoubleZero(sdi.vnom_kv))
         {//assume tensão nominal da Barra
         if (plog) plog->Add("Tensão nominal nula");
         }
      //insere Rede em VTMonta
      str_sup.sup_id     = sdi.id;
	  str_sup.bar_id     = -sdi.id_bar;
      str_sup.codigo     = sdi.cod;
      str_sup.tiposup    = TraduzTipoSuprimento(sdi.tipo_sup);
      str_sup.smax       = sdi.smax_kva * 0.001;    //kVA  =>MVA
      str_sup.vnom_kv    = sdi.vnom_kv;
      str_sup.zeq0       = sdi.z0_pu;
      str_sup.zeq1       = sdi.z1_pu;
      str_sup.pcc_3f.p   = 0;
      str_sup.pcc_3f.q   = 0;
      str_sup.pcc_ft.p   = 0;
      str_sup.pcc_ft.q   = 0;
      //define curva com 24 pontos
      str_sup.curva.id        = -1;
      str_sup.curva.tipica    = false;
      str_sup.curva.num_pat   = sdi.curva.num_ponto;
      str_sup.curva.num_valor = sdi.curva.dim_valor;
      for (int np = 0; np < sdi.curva.num_ponto; np++)
         {//proteção: valor da tensão
         v_pu = sdi.curva.ponto[np].valor[indV];
         if ((v_pu < 0.8)||(v_pu > 1.2)) v_pu = 1.0;
         //preenche dados do patamar
         str_sup.curva.patamar[np].hm_ini      = sdi.curva.ponto[np].hm_ini;
         str_sup.curva.patamar[np].hm_fim      = sdi.curva.ponto[np].hm_fim;
         str_sup.curva.patamar[np].valor[indP] = sdi.curva.ponto[np].valor[indP] * 0.001;  //kW =>MW
         str_sup.curva.patamar[np].valor[indQ] = sdi.curva.ponto[np].valor[indQ] * 0.001;  //kvar =>MVAr
         str_sup.curva.patamar[np].valor[indV] = v_pu;  //sdi.curva.ponto[np].valor[indV];
         str_sup.curva.patamar[np].valor[indT] = DegToRad(sdi.curva.ponto[np].valor[indT]); //grau =>rad
         }
      //define parâmetros para esquemático
      str_sup.esq = sdi.esq;
      //insere Suprimento
      if (monta->InsereSuprimento(str_sup) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTipoChave(sdi_TIPO_CHAVE &sdi)
   {
   //variáveis locais
   strTIPO_CHAVE str_tipo;

   if (plog) plog->DefineValor("Código", sdi.codigo);  //DVK 2015.10.22
   try{
      str_tipo.id        = sdi.id;
      str_tipo.tipo      = sdi.tipo;
      str_tipo.codigo    = sdi.codigo;
      if      (sdi.operacao.AnsiCompareIC("Manual")         == 0) str_tipo.modo_operacao = chaveOP_MANUAL;
      else if (sdi.operacao.AnsiCompareIC("Telecomanadada") == 0) str_tipo.modo_operacao = chaveOP_TELECOMANDADA;
      else if (sdi.operacao.AnsiCompareIC("Automatica")     == 0) str_tipo.modo_operacao = chaveOP_AUTOMATICA;
      else                                                        str_tipo.modo_operacao = chaveOP_INDEFINIDA;
      //define status
      str_tipo.status = sttNOVO;
      //insere TipoChave
      if (monta->InsereTipoChave(str_tipo) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTipoConsumidor(sdi_TIPO_CONS &sdi)
   {
   //variáveis locais
   VTClasse  *classe;
   VTClasses *classes  = (VTClasses*)apl->GetObject(__classid(VTClasses));

   if (plog) plog->DefineValor("Código", sdi.codigo);  //DVK 2015.10.22
   try{//verifica se já existe a Classe
      if (classes->ExisteClasseTag(sdi.tag) != NULL) return(true);
      //cria objeto VTClasse
      if ((classe = DLL_NewObjClasse()) == NULL) return(false);
      //lê dados do TipoChave
      classe->Id        = sdi.id;
      classe->Tag       = sdi.tag;
      classe->Codigo    = sdi.codigo;
      classe->Color     = TColor(sdi.color_r + (sdi.color_g << 8) + (sdi.color_b << 16));
      //insere Classe
      classes->InsereClasse(classe);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTipoRede(sdi_TIPO_REDE &sdi)
   {
   //variáveis locais
   strTIPO_REDE str_tipo;

   if (plog) plog->DefineValor("Código", sdi.codigo);  //DVK 2015.10.22
   try{
      str_tipo.id        = sdi.id;
      str_tipo.segmento  = sdi.segmento;
      str_tipo.codigo    = sdi.codigo;
      str_tipo.descricao = sdi.descricao;
      str_tipo.vmin_kv   = sdi.vmin_kv;
      str_tipo.vmax_kv   = sdi.vmax_kv;
      str_tipo.qtde_rede = 0;
      str_tipo.color     = TColor(sdi.color_r + (sdi.color_g << 8) + (sdi.color_b << 16));
      //define status
      str_tipo.status = sttNOVO;
      
      if (monta->InsereTipoRede(str_tipo) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafo2E(sdi_TRAFO2E &sdi)
   {
   //variáveis locais
   strTRAFO   str_trafo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede da subestação
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.pri.id_bar == sdi.sec.id_bar)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //insere Trafo
      str_trafo.trf_id        = sdi.id;
      str_trafo.rede_id       = rede->Id;
	  str_trafo.bar1_id       = -sdi.pri.id_bar;
	  str_trafo.bar2_id       = -sdi.sec.id_bar;
      str_trafo.codigo        = sdi.cod;
      str_trafo.extern_id     = "";
      str_trafo.snom_mva      = sdi.snom_kva * 0.001; //kVA =>MVA
      str_trafo.z0            = sdi.z0_pu;
      str_trafo.z1            = sdi.z1_pu;
      if (IsDoubleZero(sdi.sbase_kva))
         {str_trafo.perda_ferro = 0.1;}
      else
         {str_trafo.perda_ferro = ((sdi.perda_fe_w /1000.) / sdi.sbase_kva) * 100.;}
      //primário
      str_trafo.pri.ligacao   = TraduzTipoLigacao(sdi.pri.ligacao);
      str_trafo.pri.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.pri.vnom      = sdi.pri.vnom_kv;
      str_trafo.pri.tap       = sdi.pri.vtap_kv;
      str_trafo.pri.defasagem = 0;
      str_trafo.pri.zterra    = sdi.pri.zat_ohm;
      //secundário
      str_trafo.sec.ligacao   = TraduzTipoLigacao(sdi.sec.ligacao);
      str_trafo.sec.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.sec.vnom      = sdi.sec.vnom_kv;
      str_trafo.sec.tap       = sdi.sec.vtap_kv;
      str_trafo.sec.defasagem = DefineDefasagemTrafo(str_trafo.pri.ligacao, str_trafo.sec.ligacao);
      str_trafo.sec.zterra    = sdi.sec.zat_ohm;
      //dados de LTC
      str_trafo.ltc.tipo   = TraduzTipoLTC(sdi.ajuste.tipo);
      if (str_trafo.ltc.tipo == ltcFIXO)
         {//sem ajuste
         str_trafo.ltc.bar_id = ID_NULO;
         str_trafo.ltc.v_pu   = 1.0;
         }
      else
         {//assume que a barra controlada é a do secundário
         str_trafo.ltc.bar_id = str_trafo.bar2_id;
         str_trafo.ltc.v_pu   = 1.0;
         }
      //define parâmetros para esquemático
      str_trafo.esq           = sdi.esq;
      if (monta->InsereTrafo(str_trafo) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafo3E(sdi_TRAFO3E &sdi)
   {
   //variáveis locais
   strTRAFO3E str_trafo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo3E");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   //verifica se está incluindo a topologia da subestação
   //if (! ETD_Enabled) return(true);
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se foi definido Sbase
      if (IsDoubleZero(sdi.sbase_kva))  sdi.sbase_kva = sdi.pri.snom_kva;
      if (IsDoubleZero(sdi.sbase_kva))  sdi.sbase_kva = 100000;
      //insere Trafo
      str_trafo.trf_id        = sdi.id;
      str_trafo.rede_id       = rede->Id;
	  str_trafo.bar1_id       = -sdi.pri.id_bar;
	  str_trafo.bar2_id       = -sdi.sec.id_bar;
	  str_trafo.bar3_id       = -sdi.ter.id_bar;
      str_trafo.codigo        = sdi.cod;
      str_trafo.extern_id     = "";
      str_trafo.perda_ferro   = ((sdi.perda_fe_w * 0.001) / sdi.sbase_kva) * 100.;
      //primário
      str_trafo.pri.ligacao   = TraduzTipoLigacao(sdi.pri.ligacao);
      str_trafo.pri.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.pri.snom      = sdi.pri.snom_kva * 0.001; //kva =>mva
      str_trafo.pri.vnom      = sdi.pri.vnom_kv;
      str_trafo.pri.tap       = sdi.pri.vtap_kv;
      str_trafo.pri.defasagem = 0;
      str_trafo.pri.zterra    = sdi.pri.zat_ohm;
      //secundário
      str_trafo.sec.ligacao   = TraduzTipoLigacao(sdi.sec.ligacao);
      str_trafo.sec.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.sec.snom      = sdi.sec.snom_kva * 0.001; //kva =>mva
      str_trafo.sec.vnom      = sdi.sec.vnom_kv;
      str_trafo.sec.tap       = sdi.sec.vtap_kv;
      str_trafo.sec.defasagem = DefineDefasagemTrafo(str_trafo.pri.ligacao, str_trafo.sec.ligacao);
      str_trafo.sec.zterra    = sdi.sec.zat_ohm;
      //terciário
      str_trafo.ter.ligacao   = TraduzTipoLigacao(sdi.ter.ligacao);
      str_trafo.ter.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.ter.snom      = sdi.ter.snom_kva * 0.001; //kva =>mva
      str_trafo.ter.vnom      = sdi.ter.vnom_kv;
      str_trafo.ter.tap       = sdi.ter.vtap_kv;
      str_trafo.ter.defasagem = DefineDefasagemTrafo(str_trafo.pri.ligacao, str_trafo.ter.ligacao);
      str_trafo.ter.zterra    = sdi.ter.zat_ohm;
      //impedâncias de sequência ps
      str_trafo.zps0 = sdi.zps0_pu;
      str_trafo.zps1 = sdi.zps1_pu;
      //impedâncias de sequência pt
      str_trafo.zpt0 = sdi.zpt0_pu;
      str_trafo.zpt1 = sdi.zpt1_pu;
      //impedâncias de sequência st
      str_trafo.zst0 = sdi.zst0_pu;
      str_trafo.zst1 = sdi.zst1_pu;
      //define parâmetros para esquemático
      str_trafo.esq           = sdi.esq;
      if (! monta->InsereTrafo3E(str_trafo)) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafoMono(sdi_TRAFO_MONO &sdi)
   {
   //variáveis locais
   strTRAFO   str_trafo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo Monofasico");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede da subestação
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.pri.id_bar == sdi.sec.id_bar)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //insere Trafo
      str_trafo.trf_id        = sdi.id;
      str_trafo.rede_id       = rede->Id;
	  str_trafo.bar1_id       = -sdi.pri.id_bar;
	  str_trafo.bar2_id       = -sdi.sec.id_bar;
      str_trafo.codigo        = sdi.cod;
      str_trafo.extern_id     = "";
      str_trafo.snom_mva      = sdi.snom_kva * 0.001; //kVA =>MVA
      str_trafo.z0            = sdi.z_pu;
      str_trafo.z1            = sdi.z_pu;
      if (IsDoubleZero(sdi.sbase_kva))
         {str_trafo.perda_ferro = 0.1;}
      else
         {str_trafo.perda_ferro = ((sdi.perda_fe_w * 0.001) / sdi.sbase_kva) * 100.;}
      //primário
      str_trafo.pri.ligacao   = lgMONOFASICO;
      str_trafo.pri.fases     = fases->ExisteFase(sdi.pri.fases);
      str_trafo.pri.vnom      = sdi.pri.vnom_kv;
      str_trafo.pri.tap       = sdi.pri.vtap_kv;
      str_trafo.pri.defasagem = 0;
      str_trafo.pri.zterra    = sdi.pri.zat_ohm;
      //secundário
      str_trafo.sec.ligacao   = lgMONOFASICO;
      str_trafo.sec.fases     = fases->ExisteFase(sdi.sec.fases);
      str_trafo.sec.vnom      = sdi.sec.vnom_kv;
      str_trafo.sec.tap       = sdi.sec.vtap_kv;
      str_trafo.sec.defasagem = 0;
      str_trafo.sec.zterra    = sdi.sec.zat_ohm;
      //dados de LTC: sem ajuste
      str_trafo.ltc.tipo    = ltcFIXO;
      str_trafo.ltc.bar_id  = ID_NULO;
      str_trafo.ltc.v_pu    = 1.0;
      //define parâmetros para esquemático
      str_trafo.esq           = sdi.esq;
      if (monta->InsereTrafo(str_trafo) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafoZZ(sdi_TRAFOZZ &sdi)
   {
   //variáveis locais
   strTRAFOZZ str_trafoZZ;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "TrafoZZ");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede da subestação
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //insere Trafo
      str_trafoZZ.id        = sdi.id;
	  str_trafoZZ.bar_id    = -sdi.id_bar;
	  str_trafoZZ.codigo    = sdi.cod;
      str_trafoZZ.extern_id = sdi.extern_id;
      str_trafoZZ.vnom      = sdi.vnom_kv;
      str_trafoZZ.snom      = sdi.snom_kva * 0.001; //kVA =>MVA
      str_trafoZZ.z0        = sdi.z0_pu;
      //define parâmetros para esquemático
      str_trafoZZ.esq       = sdi.esq;
      if (monta->InsereTrafoZZ(str_trafoZZ) == NULL) return(true);  //return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrechoComArranjo(sdi_TRECHO &sdi)
   {
   //variáveis locais
   AnsiString  cod_cabo;
   strTRECHO   str_tre;
   VTArranjo   *arranjo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trecho");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se existe o Arranjo indicado
      if ((arranjo = arranjos->ExisteArranjo(sdi.arranjo_tipico.id_arranjo)) == NULL)
         {
         if (plog) plog->Add("Não existe o arranjo indicado");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura do trecho
      str_tre.obra            = false;
      str_tre.tre_id         = sdi.id;
      str_tre.rede_id        = rede->Id;
	  str_tre.bar1_id        = -sdi.id_bar1;
	  str_tre.bar2_id        = -sdi.id_bar2;
      str_tre.codigo         = sdi.cod;
      str_tre.extern_id      = "";
      str_tre.fases          = arranjo->Fases;
      str_tre.comp_km        = sdi.comp_m * 0.001;  //m => km
      //define parâmetros para esquemático
      str_tre.esq            = sdi.esq;
      str_tre.comentario.comentario = "";
      str_tre.comentario.dx = 0;
      str_tre.comentario.dy = 0;
      //proteção
      if ((str_tre.comp_km < 0.001)||(str_tre.comp_km > 0.200))
         {//define comprimento de 10 metros
         if (plog) plog->Add("Trecho c/ comprimento = " + DoubleToStr("%2.1f m", sdi.comp_m));
         }
      //insere Trecho em VTMonta
      if (monta->InsereTrecho(str_tre, arranjo) == NULL) return(true);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrechoZ0Z1(sdi_TRECHO &sdi)
   {
   //variáveis locais
   strTRECHO   str_tre;
   strARRANJO1 str_arranjo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trecho");
   if (plog) plog->DefineValor("Código", sdi.cod);  //DVK 2015.10.22
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura do trecho
      str_tre.obra           = false;
      str_tre.tre_id         = sdi.id;
      str_tre.rede_id        = rede->Id;
	  str_tre.bar1_id        = -sdi.id_bar1;
      str_tre.bar2_id        = -sdi.id_bar2;
      str_tre.codigo         = sdi.cod;
      str_tre.extern_id      = "";
      str_tre.fases          = fases->ExisteFase(sdi.arranjo_proprio.fases);
      str_tre.comp_km        = sdi.comp_m * 0.001;  //m => km
      str_tre.esq            = sdi.esq;
      str_tre.comentario.comentario = "";
      str_tre.comentario.dx = 0;
      str_tre.comentario.dy = 0;
      //dados do arrranjo
      str_arranjo.tipico             = false;
      str_arranjo.tipo_cfg           = arranjoZ0Z1;
      str_arranjo.codigo             = "";
      str_arranjo.cfg_arranjoZ0Z1.iadm_amp = sdi.arranjo_proprio.iadm_amp;
      str_arranjo.cfg_arranjoZ0Z1.z0       = sdi.arranjo_proprio.z0;
      str_arranjo.cfg_arranjoZ0Z1.c0       = sdi.arranjo_proprio.c0;
      str_arranjo.cfg_arranjoZ0Z1.z1       = sdi.arranjo_proprio.z1;
      str_arranjo.cfg_arranjoZ0Z1.c1       = sdi.arranjo_proprio.c1;
      //proteção
      if ((str_tre.comp_km < 0.001)||(str_tre.comp_km > 0.200))
         {//define comprimento de 10 metros
         if (plog) plog->Add("Trecho c/ comprimento = " + DoubleToStr("%2.1f m", sdi.comp_m));
         }
      //insere Trecho em VTMonta
      if (monta->InsereTrecho(str_tre, str_arranjo) == NULL) return(true);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }
   
//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCurvaTipica(sdi_CURVA &sdi, strCURVA_TIPICA &str_curva)
   {
   try{//inicia estrutura
      str_curva.id                 = sdi.id;
      str_curva.tipo               = sdi.tipo;
      str_curva.unidade            = sdi.unidade; //DVK 2013.05.28
      str_curva.codigo             = sdi.cod;
      str_curva.classe_tag         = -1;
      str_curva.tipica             = true;
      str_curva.dia                = sdi.dia;
      str_curva.energia_min_kwhmes = sdi.consumo_min_kwhmes;
      str_curva.energia_max_kwhmes = sdi.consumo_max_kwhmes;
      str_curva.num_pat            = sdi.num_ponto;
      str_curva.num_valor          = sdi.dim_valor;
      str_curva.status             = sttNOVO;
      //prenche pontos
      for (int ind_pto = 0; ind_pto < sdi.num_ponto; ind_pto++)
         {
         str_curva.patamar[ind_pto].hm_ini = sdi.ponto[ind_pto].hm_ini;
         str_curva.patamar[ind_pto].hm_fim = sdi.ponto[ind_pto].hm_fim;
         for (int ind_valor = 0; ind_valor < str_curva.num_valor; ind_valor++)
            {
            str_curva.patamar[ind_pto].valor[ind_valor] = sdi.ponto[ind_pto].valor[ind_valor];
            }
         }
      }catch (Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
   {
   return(sucesso);
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzClasseConsumidor(AnsiString codigo, AnsiString grupo)
   {
   //variáveis locais
   int       classe_tag;
   VTClasse *classe;

   //verifica se existe a Classe indicada
   if ((classe = classes->ExisteClasse(codigo)) != NULL)
      {
      classe_tag = classe->Tag;
      }
   //tenta identificar outros códigos dentro de cada grupo tarifário
   else if (grupo.AnsiCompareIC("A2") == 0)
      {
      classe_tag = consA2;
      }
   else if (grupo.AnsiCompareIC("A4") == 0)
      {
      classe_tag = consA4;
      }
   else
      {
      if      (codigo.AnsiCompareIC("RESIDENCIAL") == 0) classe_tag = consRESIDENCIAL;
      else if (codigo.AnsiCompareIC("COMERCIAL")   == 0) classe_tag = consCOMERCIAL;
      else if (codigo.AnsiCompareIC("INDUSTRIAL")  == 0) classe_tag = consINDUSTRIAL;
      else if (codigo.AnsiCompareIC("RURAL")       == 0) classe_tag = consRURAL;
      else if (codigo.AnsiCompareIC("IP")          == 0) classe_tag = consIP; //DVK 2015.10.23
      else                                               classe_tag = consOUTROS;
      }
   return(classe_tag);
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoChave(AnsiString codigo)
   {
   //variáveis locais
   VTTipoChave *tipochave;

   //verifica se existe o tipo de chave
   if ((tipochave = tipos->ExisteTipoChave(codigo)) != NULL)
      {//verifica se tipochave indefinida
      if (tipochave->Tipo != chaveINDEFINIDA) return(tipochave->Tipo);
      }
   if (codigo.AnsiPos("FUSI") > 0) return(chaveBASE_FUSIVEL);
   if (codigo.AnsiPos("DISJ") > 0) return(chaveDISJUNTOR);
   if (codigo.AnsiPos("RELI") > 0) return(chaveRELIGADORA);
   if (codigo.AnsiPos("SECC") > 0) return(chaveSECCIONADORA);
   return(chaveINDEFINIDA);
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoLigacao(AnsiString ligacao)
   {
   //verifica tipo de ligação definida por string
   if      (ligacao.AnsiCompareIC("Yaterrado")    == 0) return(lgEST_AT);
   else if (ligacao.AnsiCompareIC("Delta")        == 0) return(lgTRIANG);
   else if (ligacao.AnsiCompareIC("Yisolado")     == 0) return(lgEST_ISO);
   else if (ligacao.AnsiCompareIC("Monofasico")   == 0) return(lgMONOFASICO);
   else if (ligacao.AnsiCompareIC("DeltaAberto")  == 0) return(lgDELTA_ABERTO);
   else if (ligacao.AnsiCompareIC("DeltaFechado") == 0) return(lgDELTA_FECHADO);
   else if (ligacao.AnsiCompareIC("Indefinido")   == 0) return(lgEST_ISO);
   else
      {//tipo de ligação definida por constante
      try{
         switch(StrToInteger(ligacao))
            {
            case lgEST_AT:           return(lgEST_AT);
            case lgTRIANG:           return(lgTRIANG);
            case lgEST_ISO:          return(lgEST_ISO);
            case lgMONOFASICO:       return(lgMONOFASICO);
            case lgDELTA_ABERTO:     return(lgDELTA_ABERTO);
            case lgDELTA_FECHADO:    return(lgDELTA_FECHADO);
            default:                 return(lgEST_ISO);
            }
         }catch(Exception &e)
            {//retorna default
            return(lgEST_ISO);
            }

      }
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoLTC(AnsiString ajuste)
   {
   //ajuste definido como string
   if (ajuste.AnsiCompareIC("primario")      == 0) return(ltcPRIMARIO);
   if (ajuste.AnsiCompareIC("secundario")    == 0) return(ltcSECUNDARIO);
   if (ajuste.AnsiCompareIC("NAO UTILIZADO") == 0) return(ltcFIXO);
   //ajuste definido com valor numérico
   try{
      switch(StrToInteger(ajuste))
         {
         case 0:  return(ltcFIXO);
         case 1:  return(ltcPRIMARIO);
         case 2:  return(ltcSECUNDARIO);
         default: return(ltcFIXO);
         }
      }catch(Exception &e)
         {//retorna default
         return(ltcFIXO);
         }
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoRede(AnsiString tipo)
   {
   //variáveis locais
   int        segmento = redeINDEF;
   VTTipoRede *tiporede;

   if ((tiporede = tipos->ExisteTipoRede(tipo)) != NULL)
      {
      segmento = tiporede->Segmento;
      }
   return(segmento);
   }


//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoSuprimento(int tipo_sup)
   {
   if (tipo_sup == 1) return(supPV);
   if (tipo_sup == 2) return(supPQ);
   if (tipo_sup == 3) return(supVF);
   //retorna default
   return(supVF);
   }
   
//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataIniciaDados(AnsiString cod_dominio)
   {
   //reinicia dados locais
   rede    = NULL;
   dominio = NULL;
   //insere um novo Domínio
   //InsereDominio(cod_dominio);
   //salva o código do Domínio que só será criado se houver inserção de rede
   codigo_dominio = cod_dominio;
   //se necessário, inicia classes de consumidores
   IniciaClasseConsumidor();

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataFimDados(void)
   {
   //variáveis locais
   VTRede      *rede;
   TList       *lisREDE = redes->LisRede();

   //DVK 2015.03.05 código comentado
//   //cancela Barra inicial das Redes
//   for (int n = 0; n < lisREDE->Count; n++)
//      {
//      rede = (VTRede*)lisREDE->Items[n];
//      rede->DefineBarraInicial(NULL);
//      }
   return(monta->TrataFimDeDados());
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValidaImpedancia(strIMP &z)
   {
   if (IsDoubleZero(z.r) && IsDoubleZero(z.x)) //return(false);
      {//define valores default para R e X
      z.r = 0.01;
      z.x = 0.05;
      return(false);
      }
   return(true);
   }
   
//-----------------------------------------------------------------------------
// eof



