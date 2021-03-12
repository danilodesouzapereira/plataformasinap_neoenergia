//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TIcones.h"
#include "VTIcone.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTIcones* __fastcall NewObjIcones(TForm *Owner, AnsiString dir_icone)
   {
   try{
      return(new TIcones(Owner, dir_icone));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TIcones::TIcones(TForm *Owner, AnsiString dir_icone)
   {
   //variáveis locais
   TStrings     *lines;
   TStrings     *campos;
   TFileListBox *FileListBox;

   //cria lista
   lisICONE = new TList();
   //cria objetos p/ leitura dos arquivos txt
   lines       = new TStringList();
   campos      = new TStringList();
   FileListBox = new TFileListBox(Owner);
   FileListBox->Parent = Owner;
   //cria objetos Icone a partir dos arquivos txt do diretório
   FileListBox->Directory = dir_icone;
   FileListBox->Mask = "*.txt";
   MontaIcones(FileListBox, lines, campos);
   //destrói objetos
   delete FileListBox;
   delete lines;
   delete campos;
   }

//---------------------------------------------------------------------------
__fastcall TIcones::~TIcones(void)
   {
   //destrói lista e seus objetos
   if (lisICONE) {LimpaTList(lisICONE); delete lisICONE; lisICONE = NULL;}
   }

//---------------------------------------------------------------------------
VTIcone* __fastcall TIcones::ExisteIcone(AnsiString eqpto_cod, int index)
   {
   //variáveis locais
   VTIcone *icone;

   for (int n = 0; n < lisICONE->Count; n++)
      {
      icone = (VTIcone*)lisICONE->Items[n];
      if ((icone->eqpto_cod.AnsiCompareIC(eqpto_cod) == 0)&&(icone->index == index)) return(icone);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTIcone* __fastcall TIcones::ExisteIcone(int eqpto_id, int index)
   {
   //variáveis locais
   VTIcone *icone;

   for (int n = 0; n < lisICONE->Count; n++)
      {
      icone = (VTIcone*)lisICONE->Items[n];
      if ((icone->eqpto_id == eqpto_id)&&(icone->index == index)) return(icone);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TIcones::ExtraiCampos(AnsiString txt, TStrings *campos)
	{
	//variáveis locais
	TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	WideString  ws_txt = txt;

	//reinicia StrinList campos
	campos->Clear();
	//proteção
	if (txt.Length() < 2) return(0);
	//verifica se linha de comentário
	if ((txt[1] == '/')&&(txt[2] == '/')) return(0);
	//inicia conjunto de caracteres separadores
	Separators << ';';
	//inicia conjunto de caracteres de espaçamento
	WhiteSpace << ' ';
	//separa campos do txt
	ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
	return(campos->Count);
	}
*/
//---------------------------------------------------------------------------
int __fastcall TIcones::ExtraiCampos(AnsiString txt, TStrings *campos)
	{
	//variáveis locais
	AnsiString separadores = ";";

	//reinicia StrinList campos
	campos->Clear();
	//proteção
	if (txt.Length() < 2) return(0);
	//verifica se linha de comentário
	if ((txt[1] == '/')&&(txt[2] == '/')) return(0);
	//separa strings do txt
	return(ExtraiStrings(txt, separadores, campos));
	}


//---------------------------------------------------------------------------
void __fastcall TIcones::MontaIcones(TFileListBox *FileListBox, TStrings *lines, TStrings *campos)
   {
   //loop p/ todos arquivos
   for (int n = 0; n < FileListBox->Count; n++)
      {//lê conteúdo do arquivo
      lines->LoadFromFile(FileListBox->Items->Strings[n]);
      MontaUmIcone(lines, campos);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TIcones::MontaUmIcone(TStrings *lines, TStrings *campos)
   {
   //variáveis locais
   VTIcone *icone;
   TColor   cor;
   int      estado = 0;
   int      ind_linha, x1, y1, x2, y2, x3, y3, x4, y4, width;

   //lê dados do ícone: descarta linhas de comentário
   for (ind_linha = 0; ind_linha < lines->Count; ind_linha++)
      {//descrta linhas em branco ou de comentário
      if (ExtraiCampos(lines->Strings[ind_linha], campos) == 0) continue;
      //verifica estado: leitura do icone  ou das formas      
      if (estado == 0)
         {//dados do ícone
         if (campos->Count != 6) return;
         //cria um novo Icone
         lisICONE->Add(icone = NewObjIcone());
         icone->eqpto_cod    = campos->Strings[0];
         icone->eqpto_id     = StrToInteger(campos->Strings[1]);
         icone->index        = StrToInteger(campos->Strings[2]);
         icone->height       = StrToInteger(campos->Strings[3]);
         icone->width        = StrToInteger(campos->Strings[4]);
         icone->cor_fundo    = TColor(StrToInteger(campos->Strings[5]));
         //altera estado
         estado = 1;
         }
      else
         {//dados de uma forma: determina o tipo da forma
         switch(StrToInt(campos->Strings[0]))
            {
            case formaARCO:
               //proteção
               if (campos->Count != 11)
                  {
                  Aviso("Arco com número incorreto de atributos");
                  continue;
                  }
               x1    = StrToInt(campos->Strings[1]);
               y1    = StrToInt(campos->Strings[2]);
               x2    = StrToInt(campos->Strings[3]);
               y2    = StrToInt(campos->Strings[4]);
               x3    = StrToInt(campos->Strings[5]);
               y3    = StrToInt(campos->Strings[6]);
               x4    = StrToInt(campos->Strings[7]);
               y4    = StrToInt(campos->Strings[8]);
               width = StrToInt(campos->Strings[9]);
               cor   = TColor(StrToInt(campos->Strings[10]));
               icone->InsereArco(x1, y1, x2, y2, x3, y3, x4, y4, width, cor);
               break;
            case formaELIPSE:
               //proteção
               if (campos->Count != 7)
                  {
                  Aviso("Elipse com número incorreto de atributos");
                  continue;
                  }
               x1    = StrToInt(campos->Strings[1]);
               y1    = StrToInt(campos->Strings[2]);
               x2    = StrToInt(campos->Strings[3]);
               y2    = StrToInt(campos->Strings[4]);
               width = StrToInt(campos->Strings[5]);
               cor   = TColor(StrToInt(campos->Strings[6]));
               icone->InsereElipse(x1, y1, x2, y2, width, cor);
               break;
            case formaLINHA:
               //proteção
               if (campos->Count != 7)
                  {
                  Aviso("Linha com número incorreto de atributos");
                  continue;
                  }
               x1    = StrToInt(campos->Strings[1]);
               y1    = StrToInt(campos->Strings[2]);
               x2    = StrToInt(campos->Strings[3]);
               y2    = StrToInt(campos->Strings[4]);
               width = StrToInt(campos->Strings[5]);
               cor   = TColor(StrToInt(campos->Strings[6]));
               icone->InsereLinha(x1, y1, x2, y2, width, cor);
               break;
            default: //nada a fazer
               break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof



