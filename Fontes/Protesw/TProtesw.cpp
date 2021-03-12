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
//fun��o global p/ criar objeto da classe
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
   //destr�i objetos
   if (bcd)   {delete bcd;   bcd   = NULL;}
   if (cript) {delete cript; cript = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::AbreArquivo(void)
   {
   //abre arquivo de entrada p/ leitura texto
   fpr = fopen(arq_ori.c_str(),"rt");
   //abre arquivo de sa�da p/ grava��o
   fpw = fopen(arq_des.c_str(),"w");
   return(fpr && fpw);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::CalculaCRC(AnsiString arq, int &crc)
   {
   //vari�veis locais
   int  car;
   FILE *fp;

   //prote��o: verifica se o arquivo existe
   if (! FileExists(arq)) return(false);
   //abre arquivo p/ leitura bin�ria
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
//Cria arquivo de prote��o;
//-----------------------------------------------------------------------------
bool __fastcall TProtesw::Gera(AnsiString dirBin, AnsiString arq_texto)
   {
   //vari�veis locais
   bool resp;

   //salva diret�rio de execu�ao
   this->dirBin  = dirBin;
   //salva nomes dos arquivos
   arq_ori = arq_texto;
   arq_des = dirBin + "\\" + ARQ_PROTECAO;
   //prote��o: verifica se existe o arquivo de entrada
   if (! FileExists(arq_ori)) return(false);
   //abre arquivos de entrada e sa�da
   if (! AbreArquivo()) return(false);
   //gera arquivo de prote��o
   resp = GravaArqProtesw();
   //fecha arquivos
   FechaArquivo();
   return(resp);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::GravaArqProtesw(void)
   {
   //vari�veis locais
   int        crc;
   AnsiString nome_arq, path;

   //l� uma linha do arquivo de origem c/ nome de um arquivo
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
   //vari�veis locais
   char *pi;
   int  len;

   //monta string com: <arq>;<crc>;
   arq += AnsiString(";") + IntToStr(crc) + AnsiString(";");
   //criptografa string
   pi  = arq.c_str();
   len = arq.Length();
   if (! cript->Executa(pi, len)) return(false);
   //converte string criptografado de bin�rio p/ hexa
   if (! bcd->BinToHexa(pi, len, buf, sizeof(buf))) return(false);
   //grava string  no arquivo
   return(fprintf(fpw, "%s\n", buf) != EOF);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::LeNomeArqCRC(AnsiString txt, AnsiString &arq, int &crc)
   {
   //vari�veis locais
   #define PTO_VIRG ';'
   char       *pi, *pf;
   int        len;
   AnsiString crc_str;

   //converte string criptografado de hexa p/ bin�rio
   pi  = txt.c_str();
   len = txt.Length();
   if (! bcd->HexaToBin(pi, len, buf, sizeof(buf))) return(false);
   //descriptografa string
   if (! cript->Executa(buf, len/2)) return(false);
   //determina in�cio e fim do string
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
//Valida arquivo de prote��o;
//-----------------------------------------------------------------------------
bool __fastcall TProtesw::Valida(AnsiString dirBin)
   {
//   //vari�veis locais
//   AnsiString arq;
//   bool       resp = false;
//
//   //salva diret�rio Base
//   this->dirBin = dirBin;
//   //define path do arquivo de prote��o
//   arq = dirBin + "\\" + ARQ_PROTECAO;
//   if (! FileExists(arq)) return(false);
//   //abre arquivo de prote��o
//   if ((fpr = fopen(arq.c_str(),"rt")) != NULL)
//      {
//      //valida prote��o
//      resp = ValidaProtesw();
//      //fecha arquivo
//      fclose(fpr);
//      }
//   fpr = NULL;
//   //informa usu�rio que a licen�a foi corrompida
//   //if (!resp) Erro("VIOLA��O DE LICEN�A DE USO!\nContate a assist�ncia t�cnica ou reinstale o programa.");
//   return(resp);

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TProtesw::ValidaProtesw(void)
   {
//   //vari�veis locais
//   bool       resp;
//   int        crc, crc_lido;
//   AnsiString arq_nome, path, txt;
//
//   //assume resposta segura
//   resp = false;
//   //l� linha do arquivo c/ nome do arquivo protegido e seu CRC
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


