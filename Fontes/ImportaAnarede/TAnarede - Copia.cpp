//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAnarede.h"
#include "VTPreMonta.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"
#include "..\Rede\VTCarga.h"
#include "..\Constante\Const.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
//variáveis c/ escopo do arquivo
struct strCAMPO  {
                 int   col_ini, col_fim;
                 char* val_default;
                 };
strCAMPO cartao_DBAR[] =  {{ 1,  5, ""   },		//número
									{ 6,  6, "A"  },     //operação
									{ 7,  7, "L"  },     //estado
									{ 8,  8, "0"  },     //tipo
									{ 9, 10, "0"  },     //grupo de base de tensão
									{11, 22, ""   },     //nome
									{23, 24, "0"  },     //grupo de limite de tensão
									{25, 28, "1.0"},     //tensão
									{29, 32, "0.0"},     //ângulo
									{33, 37, "0.0"},     //geração ativa
									{38, 42, "0.0"},     //geração reativa
									{43, 47, ""   },     //geração reativa mínima
									{48, 52, ""   },     //geração reativa máxima
									{53, 58, ""   },     //barra controlada
									{59, 63, "0.0"},     //carga ativa
									{64, 68, "0.0"},     //carga reativa
									{69, 73, "0.0"},     //capacitor ou reator
									{74, 76, "0"  },     //área
									{77, 80, "1.0"}};    //tensão para dedinição de carga

strCAMPO cartao_DLIN[] =  {{ 1,  5, ""   },		//da barra
									{ 6,  6, ""   },     //abertura da barra (não utilizado)
									{ 8,  8, "A"  },     //operação
									{10, 10, ""   },     //abertura para barra (não utilizado)
									{11, 15, ""   },     //para barra
									{16, 17, ""   },     //circuito
									{18, 18, "L"  },     //estado
									{19, 19, "F"  },     //proprietário
									{21, 26, "0.0"},     //resistência
									{27, 32, ""   },     //reatância
									{33, 38, "0.0"},     //susceptância
									{39, 43, ""   },     //tap
									{44, 48, ""   },     //tap mínimo
									{49, 53, ""   },     //tap máximo
									{54, 58, "0.0"},     //defasagem
									{59, 64, ""   },     //barra controlada
									{65, 68, ""   },     //capacidade normal
									{69, 72, ""   },     //capacidade emergência
                           {73, 74, ""   }};    //steps

strCAMPO cartao_DGBT[] =  {{ 1,  2, ""    },    //grupo
									{ 4,  8, "1."  }};   //tensão

strCAMPO cartao_DINJ[] =  {{ 1,  5, ""    },    //número
									{ 7,  7, "A"   },    //operação
									{ 9, 15, "0.0" },    //injeção equivalente ativa
									{16, 22, "0.0" },    //injeção equiavlente reativa
									{23, 29, "0.0" },    //shunt equivalente
                           {30, 36, "0.0" }};   //fator de participação  equivalente

strCAMPO cartao_DGBA[] =  {{ 1,  5, ""    },  //número da barra
									{ 7, 11, ""    },  //abscissa
									{13, 17, ""    },  //ordenada
									{19, 20, ""    },  //capacidade
                           {22, 22, "V"   }}; //disposição

//---------------------------------------------------------------------------
VTAnarede* NewObjAnarede(VTApl *apl)
   {
   return(new TAnarede(apl));
   }

//---------------------------------------------------------------------------
__fastcall TAnarede::TAnarede(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   strList   = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TAnarede::~TAnarede(void)
   {
   //destrói objetos criados
   if (strList)   {delete strList;   strList   = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TAnarede::Comentario(AnsiString txt)
   {
   if (txt[1] == '*') return(true);
   if (txt[1] == '(') return(true);
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TAnarede::Executa(TStrings *Lines, int num_area)
   {
   //obtém ponteiro p/ obetos
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
   //reinicia controle de comandos repetidos
   for (int n = 0; n < sizeof(ja_tratou)/sizeof(bool); n++) ja_tratou[n] = false;
   //reinicia objeto pre_monta
   pre_monta->Reinicia();
   pre_monta->AreaFiltrada = num_area;
   //inicia estado
   estado = estNONE;
   //interpreta todas as linhas
   for (int n = 0; (n < Lines->Count)&&(estado != estFIM)&&(estado != estERRO); n++)
      {//verifica se é linha de comentario
      if (Comentario(Lines->Strings[n])) continue;
      //verifica se a linha contém palavra chave que indica fim de bloco de dados
      if (FimDeBloco(Lines->Strings[n])) estado = estNONE;
      else switch(estado)
         {
         case estNONE: //procura cartão que identifica bloco de dados
            estado = InicioDeBloco(Lines->Strings[n]);
            break;
         case estDBAR: //procura cartão que identifica fim dos dados de Barras
            LeCartaoDBAR(Lines->Strings[n]);
            if (! pre_monta->InsereDBAR(strList)) return(false);
            break;
         case estDLIN:
            LeCartaoDLIN(Lines->Strings[n]);
            if (! pre_monta->InsereDLIN(strList)) return(false);
            break;
         case estDGBT:
            LeCartaoDGBT(Lines->Strings[n]);
            if (! pre_monta->InsereDGBT(strList)) return(false);
            break;
         case estDINJ:
            LeCartaoDINJ(Lines->Strings[n]);
            if (! pre_monta->InsereDINJ(strList)) return(false);
            break;
         case estDGBA:
            LeCartaoDGBA(Lines->Strings[n]);
            if (! pre_monta->InsereDGBA(strList)) return(false);
            break;
         default:
            plog->Add("Erro na formatação do arquivo 'anarede'");
            return(false);
         }
      }
   if (estado == estERRO) return(false);
   //leitura correta dos dados
   if (! pre_monta->TrataFimDados()) return(false);
   //marca que a rede foi modificada
   RedeAlterada();
   return(pre_monta->Sucesso());
   }

//---------------------------------------------------------------------------
bool __fastcall TAnarede::FimDeBloco(AnsiString txt)
   {
   switch(estado)
      {
      case estDBAR:
      case estDLIN:
      case estDGBA:
      case estDINJ:
         txt = txt.SubString(1, 4);
         return(txt == "9999");
      case estDGBT:
         txt = txt.SubString(1, 2);
         return(txt == "99");
      default: break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TAnarede::InicioDeBloco(AnsiString txt)
   {
   txt = (txt.SubString(1, 4)).Trim();
   //verifica DBAR
   if (txt.AnsiCompareIC("DBAR") == 0) return(JaTratou(estDBAR) ? estERRO : estDBAR);
   if (txt.AnsiCompareIC("DLIN") == 0) return(JaTratou(estDLIN) ? estERRO : estDLIN);
   if (txt.AnsiCompareIC("DGBT") == 0) return(JaTratou(estDGBT) ? estERRO : estDGBT);
   if (txt.AnsiCompareIC("DINJ") == 0) return(JaTratou(estDINJ) ? estERRO : estDINJ);
   if (txt.AnsiCompareIC("DGBA") == 0) return(JaTratou(estDGBA) ? estERRO : estDGBA);
   if (txt.AnsiCompareIC("FIM")  == 0) return(estFIM);
   return(estNONE);
   }

//---------------------------------------------------------------------------
bool __fastcall TAnarede::JaTratou(int estado)
   {
   //variáveis locais
   char *cmd[sizeof(ja_tratou)/sizeof(bool)] = {"DBAR", "DLIN", "DGBT", "DINJ", "DGBA", "NONE", "ERRO", "FIM"};

   //proteção
   if ((estado < 0)||(estado > sizeof(ja_tratou)/sizeof(bool)))
      {
      plog->Add("Existência de comando inválido");
      return(true);
      }
   if (ja_tratou[estado])
      {
      plog->Add(AnsiString("Arquivo 'anarede' c/ comando repetido: ") + cmd[estado]);
      return(true);
      }
   //atualiza controle de que já tratou o comando
   ja_tratou[estado] = true;
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TAnarede::LeCartaoDBAR(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_DBAR)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_DBAR[n].col_ini;
      count = cartao_DBAR[n].col_fim - cartao_DBAR[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_DBAR[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnarede::LeCartaoDGBA(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_DGBA)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_DGBA[n].col_ini;
      count = cartao_DGBA[n].col_fim - cartao_DGBA[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_DGBA[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnarede::LeCartaoDGBT(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_DGBT)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_DGBT[n].col_ini;
      count = cartao_DGBT[n].col_fim - cartao_DGBT[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_DGBT[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnarede::LeCartaoDINJ(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_DINJ)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_DINJ[n].col_ini;
      count = cartao_DINJ[n].col_fim - cartao_DINJ[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_DINJ[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnarede::LeCartaoDLIN(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_DLIN)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_DLIN[n].col_ini;
      count = cartao_DLIN[n].col_fim - cartao_DLIN[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_DLIN[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnarede::RedeAlterada(void)
   {
   }

//-----------------------------------------------------------------------------
// eof
