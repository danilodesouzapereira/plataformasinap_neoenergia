//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAnafas.h"
#include "VTPreMonta.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
//variáveis c/ escopo do arquivo
struct strCAMPO  {
                 int   col_ini, col_fim;
                 char* val_default;
                 };

//////////////////////////////////////////////////////////////////////////////
//<su> 2010.08.30
//alterações nos cartões devido à nova versão de 5 dígitos
//////////////////////////////////////////////////////////////////////////////
/*
strCAMPO cartao_barra[] = {{ 1,  4, "0"  },  //NÚMERO DA BARRA
									{ 6,  6, "0"  },  //CÓDIGO DE ATUALIZAÇÃO
                           { 8,  8, "0"  },  //TIPO DA BARRA (0=normal;1=fictícia;2=derivação)
									{10, 21, ""   },  //NOME DA BARRA
									{18, 21, ""   },  //TENSÃO NOMINAL DA BARRA
                           {23, 26, "1.0"},  //MÓDULO DA TENSÃO PRÉ-FALTA
                           {27, 30, "0.0"},  //ÂNGULO DA TENSÃO PRÉ-FALTA
                           {32, 35, ""   },  //TENSÃO DE BASE
									{71, 72, ""   }}; //NÚMERO DA ÁREA
*/

strCAMPO  cartao_barra[] ={{ 1,  5, "0"  },  //NÚMERO DA BARRA  5 dígitos
									{ 6,  6, "0"  },  //CÓDIGO DE ATUALIZAÇÃO
                           { 8,  8, "0"  },  //TIPO DA BARRA (0=normal;1=fictícia;2=derivação)
									{10, 21, ""   },  //NOME DA BARRA
									{18, 21, ""   },  //TENSÃO NOMINAL DA BARRA
                           {23, 26, "1.0"},  //MÓDULO DA TENSÃO PRÉ-FALTA
                           {27, 30, "0.0"},  //ÂNGULO DA TENSÃO PRÉ-FALTA
                           {32, 35, ""   },  //TENSÃO DE BASE
									{70, 72, ""   }}; //NÚMERO DA ÁREA
/*
strCAMPO cartao_liga[] =  {{ 1,  4, "0"  },  //NÚMERO DA BARRA 'DE'
									{ 6,  6, "0"  },  //CÓDIGO DE ATUALIZAÇÃO
									{ 9, 12, "0"  },  //NÚMERO DA BARRA 'PARA'
									{13, 16, ""   },  //NÚMERO DO CIRCUITO
                           {17, 17, "L"  },  //TIPO DA LIGAÇÃO
                           {18, 23, "0.0"},  //RESISTÊNCIA DE SEQ +
									{24, 29, "0.0"},  //REATÂNCIA DE SEQ +
									{30, 35, "0.0"},  //RESISTÊNCIA DE SEQ ZERO
                           {36, 41, "0.0"},  //REATÂNCIA DE SEQ ZERO
                           {42, 47, ""   },  //NOME DO CIRCUITO
                           {48, 52, "0.0"},  //SUSCEPTÂNCIA NA SEQ +
                           {53, 57, "0.0"},  //SUSCEPTÂNCIA NA SEQ ZERO
                           {58, 62, "1.0"},  //RELAÇÃO DE TRANSFORMAÇÃO (TAP)
									{63, 66, ""   },  //NÚMERO DA BARRA TERMINAL DO RAMO SÉRIE ASSOCIADO
									{67, 70, ""   },  //NÚMERO DO CIRCUITO DO RAMO SÉRIE ASSOCIADO
									{71, 72, ""   },  //NÚMERO DA ÁREA
									{76, 76, ""   }}; //INDICADOR DE DEFASAMENTO EXPLÍCITO
*/

strCAMPO cartao_liga[] =  {{ 1,  5, "0"  },  //NÚMERO DA BARRA 'DE'  5 dígitos
									{ 6,  6, "0"  },  //CÓDIGO DE ATUALIZAÇÃO
									{ 8, 12, "0"  },  //NÚMERO DA BARRA 'PARA'
									{13, 16, ""   },  //NÚMERO DO CIRCUITO
                           {17, 17, "L"  },  //TIPO DA LIGAÇÃO
                           {18, 23, "0.0"},  //RESISTÊNCIA DE SEQ +
									{24, 29, "0.0"},  //REATÂNCIA DE SEQ +
									{30, 35, "0.0"},  //RESISTÊNCIA DE SEQ ZERO
                           {36, 41, "0.0"},  //REATÂNCIA DE SEQ ZERO
                           {42, 47, ""   },  //NOME DO CIRCUITO
                           {48, 52, "0.0"},  //SUSCEPTÂNCIA NA SEQ +
                           {53, 57, "0.0"},  //SUSCEPTÂNCIA NA SEQ ZERO
                           {58, 62, "1.0"},  //RELAÇÃO DE TRANSFORMAÇÃO (TAP)
									{63, 67, ""   },  //NÚMERO DA BARRA TERMINAL DO RAMO SÉRIE ASSOCIADO
									{68, 69, ""   },  //NÚMERO DO CIRCUITO DO RAMO SÉRIE ASSOCIADO
									{70, 72, ""   },  //NÚMERO DA ÁREA
									{73, 75, "0"  },  //DEFASAGEM DE TRAFO DELTA-ESTRELA
                           {76, 76, ""   }}; //INDICADOR DE DEFASAMENTO EXPLÍCITO

/*
strCAMPO cartao_mutua[] = {{ 1,  4, "0"  },  //NÚMERO DA BARRA 'DE' DO CIRCUITO 1
									{ 6,  6, "0"  },  //CÓDIGO DE ATUALIZAÇÃO
									{ 9, 12, "0"  },  //NÚMERO DA BARRA 'PARA' DO CIRCUITO 1
									{13, 16, ""   },  //NÚMERO DO CIRCUITO  DO CIRCUITO 2
									{17, 20, "0"  },  //NÚMERO DA BARRA 'DE' DO CIRCUITO 2
									{25, 28, "0"  },  //NÚMERO DA BARRA 'PARA' DO CIRCUITO 2
                           {29, 32, ""   },  //NÚMERO DO CIRCUITO  DO CIRCUITO 2
                           {33, 38, "0.0"},  //RESISTÊNCIA DE SEQ ZERO
                           {39, 44, "0.0"},  //REATÂNCIA DE SEQ ZERO
									{71, 72, ""   }}; //NÚMERO DA ÁREA
*/

strCAMPO cartao_mutua[] = {{ 1,  5, "0"  },  //NÚMERO DA BARRA 'DE' DO CIRCUITO 1
									{ 6,  6, "0"  },  //CÓDIGO DE ATUALIZAÇÃO
									{ 8, 12, "0"  },  //NÚMERO DA BARRA 'PARA' DO CIRCUITO 1
									{13, 16, ""   },  //NÚMERO DO CIRCUITO  DO CIRCUITO 2
									{17, 21, "0"  },  //NÚMERO DA BARRA 'DE' DO CIRCUITO 2
									{24, 28, "0"  },  //NÚMERO DA BARRA 'PARA' DO CIRCUITO 2
                           {29, 32, ""   },  //NÚMERO DO CIRCUITO  DO CIRCUITO 2
                           {33, 38, "0.0"},  //RESISTÊNCIA DE SEQ ZERO
                           {39, 44, "0.0"},  //REATÂNCIA DE SEQ ZERO
									{70, 72, ""   }}; //NÚMERO DA ÁREA
/*
strCAMPO cartao_gbt[] =   {{ 1,  2, ""    },
                           { 4,  8, "138."}};
*/
//---------------------------------------------------------------------------
VTAnafas* NewObjAnafas(VTApl *apl)
   {
   return(new TAnafas(apl));
   }

//---------------------------------------------------------------------------
__fastcall TAnafas::TAnafas(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   try{//cria objetos
      strList = new TStringList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TAnafas::~TAnafas(void)
   {
   //destrói objetos criados
   if (strList) {delete strList; strList = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TAnafas::Comentario(AnsiString txt)
   {
   //proteçao
   if (txt=="") return (true);
   //verifica se é linha de cabeçalho ou comentário
   txt = txt.SubString(1, 4);
   if (txt[1] == '(')    return(true);  //cabeçalho
   if (txt    == "  1 ") return(true);  //título
   if (txt    == "  2 ") return(true);  //comentário
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TAnafas::Executa(TStrings *Lines)
   {
   //variáveis locais
   VTLog      *plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   VTPreMonta *pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));

   //reinicia objeto pre_monta
   pre_monta->Reinicia();
   //inicia estado
   estado = estNONE;
   //interpreta todas as linhas
   for (int n = 0; (n < Lines->Count)&&(estado != estFIM); n++)
      {//verifica se é linha de título ou comentario
      if (Comentario(Lines->Strings[n])) continue;
      //verifica se a linha contém palavra chave que indica fim de bloco de dados
      if (FimDeBloco(Lines->Strings[n]))
         {//fim de um bloco de dados
         estado = estNONE;
         continue;
         }
      switch(estado)
         {
         case estNONE: //procura cartão que identifica bloco de dados
            estado = InicioDeBloco(Lines->Strings[n]);
            break;
         case estDBAR: //procura cartão que identifica fim dos dados de Barras
            LeCartaoBarra(Lines->Strings[n]);
            if (! pre_monta->InsereDBAR(n+1, strList)) return(false);
            break;
         case estCIRC:
            LeCartaoCircuito(Lines->Strings[n]);
            if (! pre_monta->InsereDCIR(n+1, strList)) return(false);
            break;
         case estMUTUA:
            LeCartaoMutua(Lines->Strings[n]);
            if (! pre_monta->InsereDMUT(n+1, strList)) return(false);
            break;
         default:
            plog->Add("Erro na formatação do arquivo 'anafas'");
            return(false);
         }
      }
   //leitura correta dos dados
   pre_monta->TrataFimDados();
   return(pre_monta->Sucesso());
   }

//---------------------------------------------------------------------------
bool __fastcall TAnafas::FimDeBloco(AnsiString txt)
   {
   switch(estado)
      {
      case estDBAR:
      case estCIRC:
      case estMUTUA:
         txt = txt.SubString(1, 4);
         return(txt == "9999");
      default: break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TAnafas::InicioDeBloco(AnsiString txt)
   {
   //proteção
   if (txt.IsEmpty()) return(estNONE);
   //procura pelo cartão correspondente
   txt = (txt.SubString(1, 3)).Trim();
   if (txt.AnsiCompareIC("38") == 0) return(estDBAR);
   if (txt.AnsiCompareIC("37") == 0) return(estCIRC);
   if (txt.AnsiCompareIC("39") == 0) return(estMUTUA);
   //if (txt.AnsiCompareIC("39") == 0) return(estDGBT);
   if (txt.AnsiCompareIC("99") == 0) return(estFIM);
   return(estNONE);
   }

//---------------------------------------------------------------------------
void __fastcall TAnafas::LeCartaoBarra(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_barra)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_barra[n].col_ini;
      count = cartao_barra[n].col_fim - cartao_barra[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_barra[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnafas::LeCartaoCircuito(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_liga)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_liga[n].col_ini;
      count = cartao_liga[n].col_fim - cartao_liga[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_liga[n].val_default;
      strList->Add(campo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAnafas::LeCartaoMutua(AnsiString txt)
   {
   //variáveis locais
   int        ind, count;
   AnsiString campo;

   //limpa StringList
   strList->Clear();
   //lê campos do cartão
   for (int n = 0; n < sizeof(cartao_mutua)/sizeof(strCAMPO); n++)
      {
      ind   = cartao_mutua[n].col_ini;
      count = cartao_mutua[n].col_fim - cartao_mutua[n].col_ini + 1;
      campo = (txt.SubString(ind, count)).Trim();
      if (campo.IsEmpty()) campo = cartao_mutua[n].val_default;
      strList->Add(campo);
      }
   }

//-----------------------------------------------------------------------------
int __fastcall TAnafas::NumeroErros(void)
   {
   //variáveis locais
   VTPreMonta *pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));

   return(pre_monta->NumeroErros());
   }

//-----------------------------------------------------------------------------
// eof
