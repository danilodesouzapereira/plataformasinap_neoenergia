//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Importa\RetiraEtd\VTRetiraEtd.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Importa.h"
#include "TEditaSED.h"

//---------------------------------------------------------------------------
__fastcall TEditaSED::TEditaSED(VTApl *apl_owner)
   {
   //salva ponteiro do apl
   apl = apl_owner;
   //cria StringList
   file_lines = new TStringList();
   campos     = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TEditaSED::~TEditaSED(void)
   {
   //destroi StringList
   if(file_lines) {delete file_lines; file_lines = NULL;}
   if(campos)     {delete campos;     campos = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TEditaSED::AlteraZeq(void)
   {
   //formato esperado do arquivo
   //Número;Cod_rede;Tensão;MOD(Z1%);ANG(gr);MOD(Z0%);ANG(gr);MOD(Z%);ANG(gr);(pu);

   //variaveis locais
   TList        *lisSUP;
   VTSuprimento *suprimento;
   double       v_kv, r0, x0, r1, x1, z0, z1, ang0, ang1;

   try{//verifica nível de tensão
      v_kv = StrToDouble(campos->Strings[2]);
      if(v_kv > 13.8)  return;
      //cria lista
      lisSUP = new TList();
      //localiza suprimento na rede
      LisSuprimento(campos->Strings[1], lisSUP);
      if(lisSUP->Count > 0)
         {//converte valores
         z1 = StrToDouble(campos->Strings[3]) / 100.;
         ang1 = StrToDouble(campos->Strings[4]) * (M_PI / 180.);
         r1 = z1 * cos(ang1);
         x1 = sqrt((z1 * z1) - (r1 * r1));
         z0 = StrToDouble(campos->Strings[5]) / 100.;
         ang0 = StrToDouble(campos->Strings[6]) * (M_PI / 180.);
         r0 = z0 * cos(ang0);
         x0 = sqrt((z0 * z0) - (r0 * r0));
         }
      for(int ns = 0; ns < lisSUP->Count; ns++)
         {//altera cada suprimento
         suprimento = (VTSuprimento*)lisSUP->Items[ns];
         suprimento->zeq0.r = IsDoubleZero(r0) ? 0. : r0;
         suprimento->zeq0.x = IsDoubleZero(x0) ? 0. : x0;
         suprimento->zeq1.r = IsDoubleZero(r1) ? 0. : r1;
         suprimento->zeq1.x = IsDoubleZero(x1) ? 0. : x1;
         suprimento->pcc_3f.p = 0.;
         suprimento->pcc_3f.q = 0.;
         suprimento->pcc_ft.p = 0.;
         suprimento->pcc_ft.q = 0.;
         }
      //destroi lista
      delete lisSUP;
      lisSUP = NULL;
   }catch(Exception &e)
      {
      if(lisSUP) delete lisSUP;
      Aviso("Erro na leitura do arquivo");
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TEditaSED::ExisteSED(void)
   {
   //variaveis locais
   bool     sucesso  = false;
   TList    *lisREDE = new TList();
   VTRedes  *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //lista redes do tipo ETD
   redes->LisRede(lisREDE, redeETD);
   sucesso = (lisREDE->Count > 0);
   //destroi lista
   delete lisREDE;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TEditaSED::Executa(AnsiString arquivo)
   {
   //variaveis locais
   bool sucesso = true;
   VTRetiraEtd  *retira;

   //verifica se existem SED na rede
   if(ExisteSED())
      {//cria objeto p/ retirar a SED
      retira  = DLL_NewObjRetiraEtd(apl);
      sucesso = retira->Executa();
      delete retira;
      }
   //altera suprimentos
   if(sucesso) sucesso = ImportaZeqSuprimentos(arquivo);
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TEditaSED::LisSuprimento(AnsiString cod_rede, TList *lisSUP)
   {
   //variaveis locais
   TList        *lisREDE, *lisREDE_SUP;
   VTRede       *rede;
   VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //cria lista
   lisREDE_SUP = new TList();
   //localiza redes da mesma subestação
   lisREDE = redes->LisRede();
   for(int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if(rede->Codigo.AnsiPos(cod_rede) > 0) lisREDE_SUP->Add(rede);
      }
   for(int nr = 0; nr < lisREDE_SUP->Count; nr++)
      {
      rede = (VTRede*)lisREDE_SUP->Items[nr];
      //lista suprimentos de cada rede
      rede->LisEqbar(lisSUP, eqptoSUPRIMENTO);
      }
   //destroi lista
   delete lisREDE_SUP;
   }

//---------------------------------------------------------------------------
bool __fastcall TEditaSED::ImportaZeqSuprimentos(AnsiString arquivo)
   {
   //variaveis locais
   bool       sucesso = false;

   //abre arquivo com potências de curto dos suprimentos
   sucesso = LeArqZeq(arquivo);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TEditaSED::LeArqZeq(AnsiString filefullpath)
   {
   //variaveis locais
   bool       sucesso = false;
   AnsiString filename, line;
   AnsiString separadores = ";";

   //abre arquivo com impedâncias equivalentes dos suprimentos
   try{//salva apenas nome do arquivo
      filename = ExtractFileName(filefullpath);
      //proteção: arquivo inexistente
      if(! FileExists(filefullpath))
         {
         Aviso("Arquivo inexistente: " + filename);
         return false;
         }
      //lê arquivo no caminho especificado
      file_lines->Clear();
      file_lines->LoadFromFile(filefullpath);
      for(int nl = 0; nl < file_lines->Count; nl++)
         {//lê cada linha do arquivo com Zeq
         line = file_lines->Strings[nl];
         //verifica linha de comentário
         if(VerificaComentario(line)) continue;
         //extrai campos
         campos->Clear();
         ExtraiStrings(line, separadores, campos);
         if(campos->Count < 10) continue;
         //localiza suprimento e altera Zeq
         AlteraZeq();
         }
      sucesso = true;
   }catch(Exception &e)
      {
      sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TEditaSED::VerificaComentario(AnsiString linha)
   {
   if (linha.IsEmpty())   return(true);
   if (linha.Length() < 2) return(false);
   return((linha[1] == '/')&&(linha[2] == '/'));
   }

//---------------------------------------------------------------------------

