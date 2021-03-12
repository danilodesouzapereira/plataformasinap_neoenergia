//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Filectrl.hpp>
#include "TValCabo.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Diretorio\VTPath.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Arranjo.h>

//---------------------------------------------------------------------------
VTValCabo* __fastcall NewObjValCabo(VTApl *apl)
   {
   return(new TValCabo(apl));
   }

//---------------------------------------------------------------------------
__fastcall TValCabo::TValCabo(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   //cria objetos
   campos       = new TStringList();
   lines        = new TStringList();
   lisCABO_MONO = new TList();
   lisCABO_MULT = new TList();
   //lê arquivos com dados dos cabos padrões
   LeArquivoCaboMonopolar();
   LeArquivoCaboMultiplexado();
   }

//---------------------------------------------------------------------------
__fastcall TValCabo::~TValCabo(void)
   {
   //destrói objetos
   if (campos)  {delete campos;  campos = NULL;}
   if (lines)   {delete lines;   lines  = NULL;}
   //destrói listas e seus objetos
   if (lisCABO_MONO)  {LimpaTList(lisCABO_MONO); delete lisCABO_MONO;  lisCABO_MONO = NULL;}
   if (lisCABO_MULT)  {LimpaTList(lisCABO_MULT); delete lisCABO_MULT;  lisCABO_MULT = NULL;}
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TValCabo::ExisteCabo(TList *lisCABO, AnsiString extern_id)
   {
   //variáveis locais
   VTCabo *cabo;

   //proteção
   if (extern_id.IsEmpty()) return(NULL);
   //verifica se existe Cabo padrao pelo atributo extern_id
   for (int n = 0; n < lisCABO->Count; n++)
      {
      cabo = (VTCabo*)lisCABO->Items[n];
      if (cabo->Extern_id.AnsiCompareIC(extern_id) == 0) return(cabo);
      }
   return(NULL);
   }
//---------------------------------------------------------------------------
int __fastcall TValCabo::ExtraiCampos(AnsiString txt)
   {
   //variáveis locais
   #define EOS '\0'
   char    *p_ini, *p_fim;
   int     len =  txt.Length();

   //reinicia StrinList campos
   campos->Clear();
   //proteção
   if (len < 2) return(0);
   //proteção: comentário
   if ((txt[1] == '/')&&(txt[2] == '/')) return(0);
   //inclui caractere ponto e vírgula no final de txt se ainda não existir
   if (txt[len] != ';') txt += ";";
   //extrai os campos delimitados por ';' em txt
   p_ini = txt.c_str();
   while ((p_fim = strchr(p_ini, ';')) != NULL)
      {//insere string na lista
      *p_fim = EOS; //insere delimitador de string
      //copia o substring p_ini a p_fim-1
      campos->Add(AnsiString(p_ini).Trim());
      //restaura ';' no string original
      *p_fim = ';';
      p_ini = p_fim + 1;
      }
   return(campos->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TValCabo::LeArquivoCaboMonopolar(void)
   {
   //variáveis locais
   VTCabo    *cabo;
   VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_cabo = path->DirGeral() + "\\CaboMonopolar.txt";

   //reincia lisCABO_MONO
   LimpaTList(lisCABO_MONO);
   //proteção
   if (! FileExists(arq_cabo)) return;
   //lê arquivo
   lines->LoadFromFile(arq_cabo);
   //cria cabos
   for (int n = 0; n < lines->Count; n++)
      {
      if (ExtraiCampos(lines->Strings[n]) < 5) continue;
      //cria um novo Cabo e insere em lisCABO_MONO
      lisCABO_MONO->Add(cabo = DLL_NewObjCabo());
      cabo->Codigo    = campos->Strings[0];
      cabo->Extern_id = campos->Strings[1];
      cabo->r         = StrToDouble(campos->Strings[2]);
      cabo->raio      = StrToDouble(campos->Strings[3]);
      cabo->gmr       = StrToDouble(campos->Strings[4]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TValCabo::LeArquivoCaboMultiplexado(void)
   {
   //variáveis locais
   VTCabo    *cabo;
   VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_cabo = path->DirGeral() + "\\CaboMultiplexado.txt";

   //reincia lisCABO_MONO
   LimpaTList(lisCABO_MULT);
   //proteção
   if (! FileExists(arq_cabo)) return;
   //lê arquivo
   lines->LoadFromFile(arq_cabo);
   //cria cabos
   for (int n = 0; n < lines->Count; n++)
      {
      if (ExtraiCampos(lines->Strings[n]) < 8) continue;
      //cria um novo Cabo e insere em lisCABO_MULT
      lisCABO_MULT->Add(cabo = DLL_NewObjCabo());
      cabo->Codigo    = campos->Strings[0];
      cabo->Extern_id = campos->Strings[1];
      cabo->z0.r      = StrToDouble(campos->Strings[2]);
      cabo->z0.x      = StrToDouble(campos->Strings[3]);
      cabo->c0        = StrToDouble(campos->Strings[4]);
      cabo->z1.r      = StrToDouble(campos->Strings[5]);
      cabo->z1.x      = StrToDouble(campos->Strings[6]);
      cabo->c1        = StrToDouble(campos->Strings[7]);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TValCabo::ValidaCaboMonopolar(VTCabo *cabo)
   {
   //variáveis locais
   VTCabo *cabo_padrao;

   //verificva se existe Cabo padrao
   if ((cabo_padrao = ExisteCabo(lisCABO_MONO, cabo->Codigo)) == NULL) return(false);
   //copia dados do Cabo padrão
   cabo->r    = cabo_padrao->r;
   cabo->raio = cabo_padrao->raio;
   cabo->gmr  = cabo_padrao->gmr;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValCabo::ValidaCaboMultiplexado(VTCabo *cabo)
   {
   //variáveis locais
   VTCabo *cabo_padrao;

   //verificva se existe Cabo padrao
   if ((cabo_padrao = ExisteCabo(lisCABO_MULT, cabo->Codigo)) == NULL) return(false);
   //copia dados do Cabo padrão
   cabo->z0   = cabo_padrao->z0;
   cabo->c0   = cabo_padrao->c0;
   cabo->z1   = cabo_padrao->z1;
   cabo->c1   = cabo_padrao->c1;
   return(true);
   }

//-----------------------------------------------------------------------------
// eof
