//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFases.h"
#include "VTEqpto.h"
#include "VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFases
//---------------------------------------------------------------------------
VTFases* __fastcall NewObjFases(void)
   {
   return(new TFases());
   }

//---------------------------------------------------------------------------
__fastcall TFases::TFases(void)
   {
   //cria listas
   lisFASE = new TList();
   lisTMP  = new TList();
   //inicia códigos p/ fases
   DefCodigoABC();
   }

//---------------------------------------------------------------------------
__fastcall TFases::~TFases(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   //destrói listas e seus objetos
   if (lisFASE) {LimpaTList(lisFASE); delete lisFASE; lisFASE = NULL;}
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFases::ConverteABCN_DEFN(AnsiString codigo)
   {
   //variáveis locais
   int Len;

   //determina código da fase em maiúscula
   codigo = codigo.UpperCase();
   //substitui  ABC por DEF
   Len = codigo.Length();
   for (int n = 1; n <= Len; n++)
      {
      if      (codigo[n] == 'A') codigo[n] = 'D';
      else if (codigo[n] == 'B') codigo[n] = 'E';
      else if (codigo[n] == 'C') codigo[n] = 'F';
      }
   return(codigo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFases::ConverteDEFN_ABCN(AnsiString codigo)
   {
   //variáveis locais
   int Len;

   //determina código da fase em maiúscula
   codigo = codigo.UpperCase();
   //substitui DEF por ABC
   Len = codigo.Length();
   for (int n = 1; n <= Len; n++)
      {
      if      (codigo[n] == 'D') codigo[n] = 'A';
      else if (codigo[n] == 'E') codigo[n] = 'B';
      else if (codigo[n] == 'F') codigo[n] = 'C';
      }
   return(codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFases::DefCodigo(AnsiString A, AnsiString B, AnsiString C)
   {
   //variáveis locais
   AnsiString N = "N", T = "T", at="t";

   //reinicia lisFASE
   LimpaTList(lisFASE);
   //preenche lisFASE c/ os todas instâncias possíveis de TFase, obedecendo a
   //sequência definida no enumFASE (arquivo Fontes\Constante\Fases.h)
   lisFASE->Add(new TFase(faseA,     A         ));
   lisFASE->Add(new TFase(faseB,     B         ));
   lisFASE->Add(new TFase(faseC,     C         ));
   lisFASE->Add(new TFase(faseN,     N         ));
   lisFASE->Add(new TFase(faseT,     T         ));
   lisFASE->Add(new TFase(faseAN,    A+N       ));
   lisFASE->Add(new TFase(faseBN,    B+N       ));
   lisFASE->Add(new TFase(faseCN,    C+N       ));
   lisFASE->Add(new TFase(faseAT,    A+T       ));
   lisFASE->Add(new TFase(faseBT,    B+T       ));
   lisFASE->Add(new TFase(faseCT,    C+T       ));
   lisFASE->Add(new TFase(faseNT,    N+T       ));
   lisFASE->Add(new TFase(faseANT,   A+N+at    ));
   lisFASE->Add(new TFase(faseBNT,   B+N+at    ));
   lisFASE->Add(new TFase(faseCNT,   C+N+at    ));
   lisFASE->Add(new TFase(faseAB,    A+B       ));
   lisFASE->Add(new TFase(faseBC,    B+C       ));
   lisFASE->Add(new TFase(faseCA,    C+A       ));
   lisFASE->Add(new TFase(faseABN,   A+B+N     ));
   lisFASE->Add(new TFase(faseBCN,   B+C+N     ));
   lisFASE->Add(new TFase(faseCAN,   C+A+N     ));
   lisFASE->Add(new TFase(faseABT,   A+B+T     ));
   lisFASE->Add(new TFase(faseBCT,   B+C+T     ));
   lisFASE->Add(new TFase(faseCAT,   C+A+T     ));
   lisFASE->Add(new TFase(faseABNT,  A+B+N+at  ));
   lisFASE->Add(new TFase(faseBCNT,  B+C+N+at  ));
   lisFASE->Add(new TFase(faseCANT,  C+A+N+at  ));
   lisFASE->Add(new TFase(faseABC,   A+B+C     ));
   lisFASE->Add(new TFase(faseABCN,  A+B+C+N   ));
   lisFASE->Add(new TFase(faseABCT,  A+B+C+T   ));
	lisFASE->Add(new TFase(faseABCNT, A+B+C+N+at));
   }

//---------------------------------------------------------------------------
void __fastcall TFases::DefCodigoABC(void)
   {
   DefCodigo("A", "B", "C");
   }

//---------------------------------------------------------------------------
void __fastcall TFases::DefCodigoDEF(void)
   {
   DefCodigo("D", "E", "F");
   }


//---------------------------------------------------------------------------
int __fastcall TFases::ExisteFase(AnsiString codigo)
   {
   //variáveis locais
   int   tag, len;
   TFase *fase;

   //converte codigo p/ ABCN
   codigo = ConverteDEFN_ABCN(codigo);
   //monta TAG da fase (pois o código pode estar c/ as letras em qualquer ordem)
   len = codigo.Length();
   tag = 0;
   for (int n = 1; n <= len; n++)
      {
      if      (toupper(codigo[n]) == 'A') tag |= faseA;
      else if (toupper(codigo[n]) == 'B') tag |= faseB;
      else if (toupper(codigo[n]) == 'C') tag |= faseC;
      else if (toupper(codigo[n]) == 'N') tag |= faseN;
      else if (toupper(codigo[n]) == 'T') tag |= faseT;
      }
   //determina Fase p/ o tag montado
   fase = TagToFase[tag];
   return((fase != NULL) ? fase->tag : 0);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFases::FaseIncluiFase(int fase_owner, int fase)
   {
   //variáveis locais
   AnsiString str_owner, str;

   str_owner = AsStrABCN[fase_owner];
   str       = AsStrABCN[fase];
   return(str_owner.Pos(str) != 0);
   }

//---------------------------------------------------------------------------
int __fastcall TFases::Intersecao(int fase1, int fase2)
   {
   //variáveis locais
   AnsiString str1, str2, str_comum;

   //verifica se são iguais
   if (fase1 == fase2) return(fase1);
   //determina strings das fases p/ poder comparar
   str_comum = "";
   str1      = AsStrABCN[fase1];
   str2      = AsStrABCN[fase2];
   for (int n = 1; (n <= str1.Length())&&(n <= str2.Length()); n++)
      {
      if (str2.AnsiPos(str1[n]) > 0) str_comum += str1[n];
      }
   return(ExisteFase(str_comum));
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFases::Fase1ContemFase2(int fase1, int fase2)
   {
   //return((fase1 & fase2) == fase2);
   return(((fase1 ^ fase2) | fase1) == fase1);
   }

//---------------------------------------------------------------------------
int __fastcall TFases::Fase1IntersecaoFase2(int fase1, int fase2)
   {
   return(fase1 & fase2);
   }

//---------------------------------------------------------------------------
int __fastcall TFases::Fase1UniaoFase2(int fase1, int fase2)
   {
   return(fase1 | fase2);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFases::LisFasesCarga(int carMBT)
   {
   //reinicia lisTMP
   lisTMP->Clear();
   switch(carMBT)
      {
      case cargaMONOFASICA: //preenche lisTMP c/ as fases p/ cargas monofásicas
         lisTMP->Add(TagToFase[faseAN]);
         lisTMP->Add(TagToFase[faseBN]);
         lisTMP->Add(TagToFase[faseCN]);
         lisTMP->Add(TagToFase[faseAT]);
         lisTMP->Add(TagToFase[faseBT]);
         lisTMP->Add(TagToFase[faseCT]);
         break;
      case cargaBIFASICA: //preenche lisTMP c/ as fases p/ cargas bifásicas
         lisTMP->Add(TagToFase[faseAB]);
         lisTMP->Add(TagToFase[faseBC]);
         lisTMP->Add(TagToFase[faseCA]);
         lisTMP->Add(TagToFase[faseABN]);
         lisTMP->Add(TagToFase[faseBCN]);
         lisTMP->Add(TagToFase[faseCAN]);
         break;
      case cargaTRIFASICA: //preenche lisTMP c/ as fases p/ cargas trifásicas
      default:
         lisTMP->Add(TagToFase[faseABC]);
         lisTMP->Add(TagToFase[faseABCN]);
         break;
      }
   return(lisTMP);
   }
   
//---------------------------------------------------------------------------
TList* __fastcall TFases::LisFasesTrafoMonoPrimario(void)
   {
   //reinicia SFcarga
   lisTMP->Clear();
   //insere os objetos TFase p/ os possíveis primários de Trafo mono
   lisTMP->Add(TagToFase[faseAB]);
   lisTMP->Add(TagToFase[faseBC]);
   lisTMP->Add(TagToFase[faseCA]);
   lisTMP->Add(TagToFase[faseAN]);
   lisTMP->Add(TagToFase[faseBN]);
   lisTMP->Add(TagToFase[faseCN]);
   lisTMP->Add(TagToFase[faseANT]);
   lisTMP->Add(TagToFase[faseBNT]);
   lisTMP->Add(TagToFase[faseCNT]);
   return(lisTMP);
   }
/*
//---------------------------------------------------------------------------
TList* __fastcall TFases::LisFasesTrafoMonoSecundario(bool com_neutro)
   {
   //reinicia SFcarga
   lisTMP->Clear();
   //insere os objetos TFase p/ os possíveis secundários de Trafo mono
   if (com_neutro)
      {
      lisTMP->Add(TagToFase[faseABN]);
      lisTMP->Add(TagToFase[faseBCN]);
      lisTMP->Add(TagToFase[faseCAN]);
      lisTMP->Add(TagToFase[faseABNT]);
      lisTMP->Add(TagToFase[faseBCNT]);
      lisTMP->Add(TagToFase[faseCANT]);
      lisTMP->Add(TagToFase[faseAN]);
      lisTMP->Add(TagToFase[faseBN]);
      lisTMP->Add(TagToFase[faseCN]);
      lisTMP->Add(TagToFase[faseANT]);
      lisTMP->Add(TagToFase[faseBNT]);
      lisTMP->Add(TagToFase[faseCNT]);
      }
   else
      {
      lisTMP->Add(TagToFase[faseAB]);
      lisTMP->Add(TagToFase[faseBC]);
      lisTMP->Add(TagToFase[faseCA]);
      }
   return(lisTMP);
   }
*/
//---------------------------------------------------------------------------
TList* __fastcall TFases::LisFasesTrafoMonoSecundario(bool mono_5terminais)
   {
   //reinicia lisTMP
   lisTMP->Clear();
   //insere os objetos TFase p/ os possíveis secundários de Trafo mono
   if (mono_5terminais)
      {
      lisTMP->Add(TagToFase[faseABN]);
      lisTMP->Add(TagToFase[faseBCN]);
      lisTMP->Add(TagToFase[faseCAN]);
      lisTMP->Add(TagToFase[faseABNT]);
      lisTMP->Add(TagToFase[faseBCNT]);
      lisTMP->Add(TagToFase[faseCANT]);
      }
   else
      {
      lisTMP->Add(TagToFase[faseAN]);
      lisTMP->Add(TagToFase[faseBN]);
      lisTMP->Add(TagToFase[faseCN]);
      lisTMP->Add(TagToFase[faseANT]);
      lisTMP->Add(TagToFase[faseBNT]);
      lisTMP->Add(TagToFase[faseCNT]);
      lisTMP->Add(TagToFase[faseAB]);
      lisTMP->Add(TagToFase[faseBC]);
      lisTMP->Add(TagToFase[faseCA]);
      }
   return(lisTMP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFases::LisFasesTrecho(void)
   {
   //variáveis locais
   int vet_tag [] = {faseABC,  faseAB, faseBC, faseCA,  faseA,  faseB,  faseC,
                     faseABCN, faseABN,faseBCN,faseCAN, faseAN, faseBN, faseCN};
   //reinicia lisTMP
   lisTMP->Clear();
   //preenche lisTMP com os objetos Fase
   for (int n = 0; n < sizeof(vet_tag)/sizeof(int); n++)
      {
      lisTMP->Add(TagToFase[vet_tag[n]]);
      }
   return(lisTMP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFases::LisFasesTrechoTrifasico(void)
   {
   //reinicia SFcarga
   lisTMP->Clear();
   //insere os objetos TFase p/ os possíveis tipos de Trecho
   lisTMP->Add(TagToFase[faseABC]);
   lisTMP->Add(TagToFase[faseABCN]);
   return(lisTMP);
   }

//---------------------------------------------------------------------------
int __fastcall TFases::NumeroDeFases(int fase)
   {
   //variáveis locais
   int num_fase = 0;

   if ((fase & faseA) == faseA) num_fase++;
   if ((fase & faseB) == faseB) num_fase++;
   if ((fase & faseC) == faseC) num_fase++;

   return(num_fase);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFases::PM_GetCodAsStrABCN(int tag)
   {
   return(ConverteDEFN_ABCN(AsStrUpper[tag]));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFases::PM_GetCodAsStrDEFN(int tag)
   {
   return(ConverteABCN_DEFN(AsStrUpper[tag]));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFases::PM_GetCodAsStrLower(int tag)
   {
   //variáveis locais
   TFase *fase;

   //proteção
   if ((fase = TagToFase[tag]) == NULL) return("*");
   return(fase->codigo.LowerCase());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFases::PM_GetCodAsStrUpper(int tag)
   {
   //variáveis locais
   TFase *fase;

   //proteção
   if ((fase = TagToFase[tag]) == NULL) return("*");
   return(fase->codigo.UpperCase());
   }

//---------------------------------------------------------------------------
TFase* __fastcall TFases::PM_GetIndexToFase(int index)
   {
   return(TagToFase[IndexToTag[index]]);
   }

//---------------------------------------------------------------------------
int __fastcall TFases::PM_GetIndexToTag(int index)
   {
   switch(index)
      {
      case indFASE_A: return(faseA);
      case indFASE_B: return(faseB);
      case indFASE_C: return(faseC);
      case indNEUTRO: return(faseN);
      }
   return(faseINV);
   }

//---------------------------------------------------------------------------
TFase* __fastcall TFases::PM_GetTagToFase(int tag)
   {
   //variáveis locais
   TFase *fase;

   for (int n = 0; n < lisFASE->Count; n++)
      {
      fase = (TFase*)lisFASE->Items[n];
      if (fase->tag == tag) return(fase);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TFases::PM_GetTagToIndex(int tag)
   {
   switch(tag)
      {
      case faseA: return(indFASE_A);  
      case faseB: return(indFASE_B);  
      case faseC: return(indFASE_C);  
      case faseN: return(indNEUTRO);
      }
   return(-1);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TFases::Uniao(int fase1, int fase2)
   {
   //variáveis locais
   AnsiString str1, str2, str_abcn, str_comum;

   //verifica se são iguais
   if (fase1 == fase2) return(fase1);
   //determina strings das fases p/ poder comparar
   str_comum = "";
   str_abcn  = AsStrABCN[faseABCN];
   str1      = AsStrABCN[fase1];
   str2      = AsStrABCN[fase2];
   for (int n = 1; n <= str_abcn.Length(); n++)
      {
      if      (str1.AnsiPos(str_abcn[n]) > 0) str_comum += str_abcn[n];
      else if (str2.AnsiPos(str_abcn[n]) > 0) str_comum += str_abcn[n];
      }
   return(ExisteFase(str_comum));
   }
*/
//---------------------------------------------------------------------------
//eof

