//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop
#include "TProtesw.h"
#include "..\Cript\VTBcd.h"
#include "..\Cript\VTCript.h"
#include "..\Diretorio\DirArq.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTProtesw* NewObjProtesw(void)
   {
   return(new TProtesw());
   }
   
//---------------------------------------------------------------------------
// TProtesw() - Construtor
//---------------------------------------------------------------------------
__fastcall TProtesw::TProtesw(void)
   {
   //cria objetos
   bcd    = NewObjBcd();
   cript = NewObjCript();
   }

//---------------------------------------------------------------------------
// ~TProtesw() - Destrutor
//---------------------------------------------------------------------------
__fastcall TProtesw::~TProtesw(void)
   {
   //destrói objetos
   if (bcd)   {delete bcd;   bcd   = NULL;}
   if (cript) {delete cript; cript = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::AbreArquivo(void)
   {
   //abre arquivo de entrada p/ leitura texto
   fpr = fopen(arq_ori.c_str(),"rt");
   //abre arquivo de saída p/ gravação
   fpw = fopen(arq_des.c_str(),"w");
   return(fpr && fpw);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::CalculaCRC(AnsiString arq, int &crc)
   {
   //variáveis locais
   int  car;
   FILE *fp;

   //proteção: verifica se o arquivo existe
   if (! FileExists(arq)) return(false);
   //abre arquivo p/ leitura binária
   if ((fp = fopen(arq.c_str(),"rb")) == NULL) return(false);
   //calcula crc do arquivo
   crc = 0;
   while((car = fgetc(fp)) != EOF) crc += car;
   //inverte os bits do CRC
   crc = ~crc;
   //fecha arquivo
   fclose(fp);
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TProtesw::FechaArquivo(void)
   {
   if (fpr) {fclose(fpr); fpr = NULL;}
   if (fpw) {fclose(fpw); fpw = NULL;}
   }

//-----------------------------------------------------------------------------
//Cria arquivo de proteção;
//-----------------------------------------------------------------------------
bool __fastcall TProtesw::Gera(AnsiString dirBin, AnsiString arq_texto)
   {
   //variáveis locais
   bool resp;

   //salva diretório de execuçao
   this->dirBin  = dirBin;
   //salva nomes dos arquivos
   arq_ori = arq_texto;
   arq_des = dirBin + "\\" + ARQ_PROTECAO;
   //proteção: verifica se existe o arquivo de entrada
   if (! FileExists(arq_ori)) return(false);
   //abre arquivos de entrada e saída
   if (! AbreArquivo()) return(false);
   //gera arquivo de proteção
   resp = GravaArqProtesw();
   //fecha arquivos
   FechaArquivo();
   return(resp);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::GravaArqProtesw(void)
   {
   //variáveis locais
   int        crc;
   AnsiString nome_arq, path;

   //lê uma linha do arquivo de origem c/ nome de um arquivo
   while((fgets(buf, sizeof(buf), fpr)) != NULL)
      {//monta path completo do arquivo
      nome_arq = AnsiString(buf).Trim();
      path     = dirBin + "\\" + nome_arq;
      //calcula CRC do arquivo
      if (! CalculaCRC(path, crc)) return(false);
      //grava linha do arquivo de destino c/ nome e crc do arquivo
      if (! GravaNomeArqCRC(nome_arq, crc)) return(false);
      }
   return(true);
  }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::GravaNomeArqCRC(AnsiString arq, int crc)
   {
   //variáveis locais
   char *pi;
   int  len;

   //monta string com: <arq>;<crc>;
   arq += AnsiString(";") + IntToStr(crc) + AnsiString(";");
   //criptografa string
   pi  = arq.c_str();
   len = arq.Length();
   if (! cript->Executa(pi, len)) return(false);
   //converte string criptografado de binário p/ hexa
   if (! bcd->BinToHexa(pi, len, buf, sizeof(buf))) return(false);
   //grava string  no arquivo
   return(fprintf(fpw, "%s\n", buf) != EOF);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::LeNomeArqCRC(AnsiString txt, AnsiString &arq, int &crc)
   {
   //variáveis locais
   #define PTO_VIRG ';'
   char       *pi, *pf;
   int        len;
   AnsiString crc_str;

   //converte string criptografado de hexa p/ binário
   pi  = txt.c_str();
   len = txt.Length();
   if (! bcd->HexaToBin(pi, len, buf, sizeof(buf))) return(false);
   //descriptografa string
   if (! cript->Executa(buf, len/2)) return(false);
   //determina início e fim do string
   txt = AnsiString(buf);
   pi  = txt.c_str();
   pf  = pi + txt.Length()-1;
   //extrai nome do arquivo
   arq = "";
   for (; (pi < pf)&&(*pi != PTO_VIRG); pi++) arq += *pi;
   //extrai string do CRC
   crc_str = "";
   for (pi++; (pi < pf)&&(*pi != PTO_VIRG); pi++) crc_str += *pi;
   //elimina brancos
   arq     = arq.Trim();
   crc_str = crc_str.Trim();
   //converte crc_str p/ inteiros
   try{
      crc = crc_str.ToInt();
      } catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
//Valida arquivo de proteção;
//-----------------------------------------------------------------------------
bool __fastcall TProtesw::Valida(AnsiString dirBin)
   {
//   //variáveis locais
//   AnsiString arq;
//   bool       resp = false;
//
//   //salva diretório Base
//   this->dirBin = dirBin;
//   //define path do arquivo de proteção
//   arq = dirBin + "\\" + ARQ_PROTECAO;
//   if (! FileExists(arq)) return(false);
//   //abre arquivo de proteção
//   if ((fpr = fopen(arq.c_str(),"rt")) != NULL)
//      {
//      //valida proteção
//      resp = ValidaProtesw();
//      //fecha arquivo
//      fclose(fpr);
//      }
//   fpr = NULL;
//   //informa usuário que a licença foi corrompida
//   //if (!resp) Erro("VIOLAÇÃO DE LICENÇA DE USO!\nContate a assistência técnica ou reinstale o programa.");
//   return(resp);

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::ValidaProtesw(void)
   {
//   //variáveis locais
//   bool       resp;
//   int        crc, crc_lido;
//   AnsiString arq_nome, path, txt;
//
//   //assume resposta segura
//   resp = false;
//   //lê linha do arquivo c/ nome do arquivo protegido e seu CRC
//   while(fgets(buf, sizeof(buf), fpr) != NULL)
//      {//extrai nome do arquivo e CRC
//      txt = AnsiString(buf).Trim();
//      if (! LeNomeArqCRC(txt, arq_nome, crc_lido)) return(false);
//      //calcula CRC do arquivo
//      path = dirBin + "\\" + arq_nome;
//      if (! CalculaCRC(path, crc)) return(false);
//      //compara CRC lido c/ o CRC calculado
//      if (crc != crc_lido) return(false);
//      //validou CRC de pelo menos um arquivo
//      resp = true;
//      }
//   return(resp);
	return(true);
   }

//-----------------------------------------------------------------------------


