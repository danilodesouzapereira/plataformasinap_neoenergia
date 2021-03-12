//---------------------------------------------------------------------------
#pragma hdrstop
#include "TRes505.h"
#include "..\..\..\Fontes\Apl\VTApl.h"
#include "..\..\..\Fontes\Geral\VTGeral.h"
#include "..\..\..\Fontes\Geral\VTInfoset.h"
#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRes505* __fastcall NewObjRes505(VTApl *apl)
   {
   try{
      return(new TRes505(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TRes505::TRes505(VTApl *apl)
   {
   //salva ponteiro
   this->apl = apl;
   //assume faixa de tensão
   DefineFaixaTensao(0.230, 0.115);
   }

//---------------------------------------------------------------------------
__fastcall TRes505::~TRes505(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//void __fastcall  TRes505::DefineFaixaTensao(int ra505_vnom, double vff_kv, double vfn_kv)
void __fastcall  TRes505::DefineFaixaTensao(int ra505_vnom)
   {
   switch(ra505_vnom)
      {
      case ra505_GE230KV:
         break;
      case ra505_LT230KV_GE69KV:
         break;
      case ra505_LT69KV_GT1KV:
         descricao      = "Tensão Nominal entre 1 kV e 69 kV";
         tensoes.tensao = eFN;
         //tensoes.vfn    = vfn_kv;
         //tensoes.vff    = vff_kv;
         tensoes.vfn    = ((vContratadaMT_kv)/(sqrt((long double)3)));
         tensoes.vff    = vContratadaMT_kv;
         //adequada: (0.93 ≤ TL≤ 1.05)
         adequado.inf   = 0.93 * tensoes.vfn;
         adequado.sup   = 1.05 * tensoes.vfn;
         //critica: (TL< 0.90 ou TL>1.05)
         critico.inf   = 0.90 * tensoes.vfn;
         critico.sup   = 1.05 * tensoes.vfn;
         break;
      case ra505_LE1KV_TRI_220_127:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Trifásico 220-127V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.127;
         tensoes.vff    = 0.220;
         //adequada: (117 ≤ TL≤ 133) //fkm revisão 6 vigência 01/01/2015
         adequado.inf   = 0.117;
         adequado.sup   = 0.133;
         //critica: (TL< 110 ou TL>135) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.110;
         critico.sup   = 0.135;
         break;
      case ra505_LE1KV_TRI_380_220:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Trifásico 380-220V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.220;
         tensoes.vff    = 0.380;
         //adequada: (202 ≤ TL≤231) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.202;
         adequado.sup   = 0.231;
         //critica: (TL< 191 ou TL>233) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.191;
         critico.sup   = 0.233;
         break;
      case ra505_LE1KV_MON_254_127:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 254-127V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 127;
         tensoes.vff    = 254;
         //adequada: (117 ≤ TL≤ 133) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.117;
         adequado.sup   = 0.133;
         //critica: (TL< 110 ou TL>135) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.110;
         critico.sup   = 0.135;
         break;
      case ra505_LE1KV_MON_440_220:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 440-220V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.220;
         tensoes.vff    = 0.440;
         //adequada: (202 ≤ TL≤ 231) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.202;
         adequado.sup   = 0.231;
         //critica: (TL< 191 ou TL>233) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.191;
         critico.sup   = 0.233;
         break;
      case ra505_LE1KV_TRI_208_120:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Trifásico 208-120V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.120;
         tensoes.vff    = 0.208;
         //adequada: (110 ≤ TL≤ 126)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.110;
         adequado.sup   = 0.126;
         //critica: (TL< 104 ou TL>127)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.104;
         critico.sup   = 0.127;
         break;
      case ra505_LE1KV_MON_230_115:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 230-115V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.115;
         tensoes.vff    = 0.230;
         //adequada: (106 ≤ TL≤ 121) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.106;
         adequado.sup   = 0.121;
         //critica: (TL< 100 ou TL>122)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.100;
         critico.sup   = 0.122;
         break;
      case ra505_LE1KV_MON_240_120:
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 240-120V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.120;
         tensoes.vff    = 0.240;
         //adequada: (110 ≤ TL≤ 126) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.110;
         adequado.sup   = 0.126;
         //critica: (TL< 104 ou TL>127) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.104;
         critico.sup   = 0.127;
         break;
      case ra505_LE1KV_MON_220_110:     //padrão Eletropaulo
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 220-110V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.110;
         tensoes.vff    = 0.220;
         //adequada: (101 ≤ TL≤ 116) //fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.101;
         adequado.sup   = 0.116;
         //critica: (TL< 96 ou TL>117)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.096;
         critico.sup   = 0.117;
         break;
      case ra505_LE1KV_MON_110_110:     //padrão AES Sul
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 110-110V";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.110;
         tensoes.vff    = 0.110;
         //adequada: (101 ≤ TL≤ 116)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.101;
         adequado.sup   = 0.116;
         //critica: (TL< 96 ou TL>117)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.096;
         critico.sup   = 0.117;
         break;
      case ra505_LE1KV_MON_220_220:     //padrão AES Sul
         descricao      = "Tensão Nominal igual ou inferior a 1 kV. Monofásico 220-220";
         tensoes.tensao = eFN;
         tensoes.vfn    = 0.220;
         tensoes.vff    = 0.220;
         //adequada: (202 ≤ TL≤ 231)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         adequado.inf   = 0.202;
         adequado.sup   = 0.231;
         //critica: (TL< 191 ou TL>233)//fkm PRODIST mod8 revisão 6 vigência 01/01/2015
         critico.inf   = 0.191;
         critico.sup   = 0.233;
         break;
      default: //caso as tensões não sejam padronizadas: assume faixas em pu //DVK 2015.12.07
         descricao      = "Tensão Nominal inferior a 1 kV não padronizado";
         tensoes.tensao = eFN;
//         tensoes.vfn    = tensoes.vfn;
//         tensoes.vff    = tensoes.vff;
         //adequada: (0.92 ≤ TL≤ 1.05)
         adequado.inf   = 0.92 * tensoes.vfn;
         adequado.sup   = 1.05 * tensoes.vfn;
         //critica: (TL< 0.87 ou TL>1.06)
         critico.inf   = 0.87 * tensoes.vfn;
         critico.sup   = 1.06 * tensoes.vfn;
         break;
      }
   //precaria:
   precario1.inf  = critico.inf;
   precario1.sup  = adequado.inf;
   precario2.inf  = adequado.sup;
   precario2.sup  = critico.sup;
   }

//---------------------------------------------------------------------------
void __fastcall TRes505::DefineFaixaTensao(double vff_kv, double vfn_kv)
   {
   //inicia com valores default
   DefineFaixaTensaoDefault(vff_kv, vfn_kv);
   //troca limites pelos valores no infoset (se houver)
   if(! LeDados())
      {
      Aviso("Erro ao ler dados de limites para DRC e DRP");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRes505::DefineFaixaTensaoDefault(double vff_kv, double vfn_kv)
   {
   //variáveis locais
   double dv = 1e-3;
   //verifica se vff_kv acima de 1kV
   if (vff_kv > 1.0)
      {//verifica faixa de tensão
      //if      (vff_kv >= 230.) DefineFaixaTensao(ra505_GE230KV, vff_kv, vfn_kv);
      //else if (vff_kv >=  69.) DefineFaixaTensao(ra505_LT230KV_GE69KV, vff_kv, vfn_kv);
      //else                     DefineFaixaTensao(ra505_LT69KV_GT1KV, vff_kv, vfn_kv);
      if      (vff_kv >= 230.) DefineFaixaTensao(ra505_GE230KV);
      else if (vff_kv >=  69.) DefineFaixaTensao(ra505_LT230KV_GE69KV);
      else                     DefineFaixaTensao(ra505_LT69KV_GT1KV);
      }
   //vff_kv menor ou igual a 1kV
   else
      {//verifica tensões padronizadas
      if      (IsDoubleZero(vff_kv - 0.220, dv) && IsDoubleZero(vfn_kv - 0.127, dv))
         DefineFaixaTensao(ra505_LE1KV_TRI_220_127);
         //DefineFaixaTensao(ra505_LE1KV_TRI_220_127, vff_kv, vfn_kv);

      else if (IsDoubleZero(vff_kv - 0.380, dv) && IsDoubleZero(vfn_kv - 0.220, dv))
         DefineFaixaTensao(ra505_LE1KV_TRI_380_220);
         //DefineFaixaTensao(ra505_LE1KV_TRI_380_220, vff_kv, vfn_kv);
      else if (IsDoubleZero(vff_kv - 0.254, dv) && IsDoubleZero(vfn_kv - 0.127, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_254_127);
         //DefineFaixaTensao(ra505_LE1KV_MON_254_127, vff_kv, vfn_kv);
      else if (IsDoubleZero(vff_kv - 0.440, dv) && IsDoubleZero(vfn_kv - 0.220, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_440_220);
         //DefineFaixaTensao(ra505_LE1KV_MON_440_220, vff_kv, vfn_kv);
      //verifica tensões não padronizadas
      else if (IsDoubleZero(vff_kv - 0.208, dv) && IsDoubleZero(vfn_kv - 0.120, dv))
         DefineFaixaTensao(ra505_LE1KV_TRI_208_120);
         //DefineFaixaTensao(ra505_LE1KV_TRI_208_120, vff_kv, vfn_kv);
      else if (IsDoubleZero(vff_kv - 0.230, dv) && IsDoubleZero(vfn_kv - 0.115, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_230_115);
         //DefineFaixaTensao(ra505_LE1KV_MON_230_115, vff_kv, vfn_kv);
      else if (IsDoubleZero(vff_kv - 0.240, dv) && IsDoubleZero(vfn_kv - 0.120, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_240_120);
         //DefineFaixaTensao(ra505_LE1KV_MON_240_120, vff_kv, vfn_kv);
      //verifica tensões não padronizadas - Eletropaulo
      else if (IsDoubleZero(vff_kv - 0.220, dv) && IsDoubleZero(vfn_kv - 0.110, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_220_110);
         //DefineFaixaTensao(ra505_LE1KV_MON_220_110, vff_kv, vfn_kv);
      //verifica tensões não padronizadas - AES Sul
      else if (IsDoubleZero(vff_kv - 0.110, dv) && IsDoubleZero(vfn_kv - 0.110, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_110_110);
         //DefineFaixaTensao(ra505_LE1KV_MON_110_110, vff_kv, vfn_kv);
      else if (IsDoubleZero(vff_kv - 0.220, dv) && IsDoubleZero(vfn_kv - 0.220, dv))
         DefineFaixaTensao(ra505_LE1KV_MON_220_220);
         //DefineFaixaTensao(ra505_LE1KV_MON_220_220, vff_kv, vfn_kv);
      else
         {//DefineFaixaTensao(ra505_LE1KV_TRI_220_127);
         //DefineFaixaTensao(ra505_LE1KV_TRI_220_127, vff_kv, vfn_kv);
         tensoes.vfn = vfn_kv;
         tensoes.vff = vff_kv;
         DefineFaixaTensao(ra505_DEFAULT);  //DVK 2015.12.07
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TRes505::ClassificaAdequadaPrecariaCritica(double tensao)
   {
   //tensão em volts
   if (FaixaAdequada(tensao)) return eADEQUADA;
   if (FaixaCritica(tensao))  return eCRITICA;
   if (FaixaPrecaria(tensao)) return ePRECARIA;

   return(ePRECARIA);
   }

//---------------------------------------------------------------------------
bool __fastcall TRes505::FaixaAdequada(double tensao)
   {
   return((adequado.inf <= tensao) && (tensao <= adequado.sup));
   }

//---------------------------------------------------------------------------
bool __fastcall TRes505::FaixaCritica(double tensao)
   {
   return((tensao < critico.inf) || (tensao > critico.sup));
   }

//---------------------------------------------------------------------------
bool __fastcall TRes505::FaixaPrecaria(double tensao)
   {
   return(((precario1.inf <= tensao) && (tensao <  precario1.sup)) ||
          ((precario2.inf >  tensao) && (tensao <= precario2.sup)));
   }

//---------------------------------------------------------------------------
bool __fastcall TRes505::LeDados(void)
   {
   //variáveis locais
   VTGeral        *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //lê informações no infoset
   if(! geral) return(false);
   if(geral->Infoset->LoadFromFile(descricao))
      {
      geral->Infoset->GetInfo("Adeq1", adequado.inf);
      geral->Infoset->GetInfo("Adeq2", adequado.sup);
      geral->Infoset->GetInfo("Crit1", critico.inf);
      geral->Infoset->GetInfo("Crit2", critico.sup);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


