//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TInfoset.h"
#include "VTRegistro.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTInfoset* __fastcall NewObjInfoset(VTApl *apl)
	{
   try{//cria objeto
      return(new TInfoset(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TInfoset::TInfoset(VTApl *apl)
   {
   //salva ponteiros
   this->apl = apl;
   //cria objetos
   lisREG = new TList();
   lines  = new TStringList();
   campos = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TInfoset::~TInfoset(void)
   {
   //destrói lisINFO e seus objetos
   if (lisREG) {Clear(); delete lisREG; lisREG = NULL;}
   //destrói objetos
   if (lines)  {delete lines;  lines  = NULL;}
   if (campos) {delete campos; campos = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::AddInfo(AnsiString nome, AnsiString valor)
   {
   //se necessário, cria um novo Registro
   if (registro == NULL)
      {
      if (! InsertNewLine()) return(false);
      }
   //insere Info no Registro
   return(registro->AddInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::AddInfo(AnsiString nome, double valor)
   {
   //se necessário, cria um novo Registro
   if (registro == NULL)
      {
      if (! InsertNewLine()) return(false);
      }
   //insere Info no Registro
   return(registro->AddInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::AddInfo(AnsiString nome, float valor)
   {
   //se necessário, cria um novo Registro
   if (registro == NULL)
      {
      if (! InsertNewLine()) return(false);
      }
   //insere Info no Registro
   return(registro->AddInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::AddInfo(AnsiString nome, int valor)
   {
   //se necessário, cria um novo Registro
   if (registro == NULL)
      {
      if (! InsertNewLine()) return(false);
      }
   //insere Info no Registro
   return(registro->AddInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::AddInfo(AnsiString nome, unsigned valor)
   {
   //se necessário, cria um novo Registro
   if (registro == NULL)
      {
      if( ! InsertNewLine()) return(false);
      }
   //insere Info no Registro
   return(registro->AddInfo(nome, valor));
   }
//---------------------------------------------------------------------------
bool __fastcall TInfoset::AddInfo(AnsiString nome, bool valor)
	{
	int valor_int;
	//se necessário, cria um novo Registro
	if (registro == NULL)
	{
		if( ! InsertNewLine()) return(false);
	}

	if(valor == true)
		{valor_int = 1;}
	else
		{valor_int = 0;}
	//insere Info no Registro
	return(registro->AddInfo(nome, valor_int));
}
//---------------------------------------------------------------------------
void __fastcall TInfoset::Clear(void)
   {
   //reinicia lines e campos
   lines->Clear();
   campos->Clear();
   //destrói objetos VTRegistro em lisINFO
   registro = NULL;
   LimpaTList(lisREG);
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::GetInfo(AnsiString nome, AnsiString &valor)
   {
   //proteção
   if (registro == NULL) return(false);
   //insere Info no Registro
   return(registro->GetInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::GetInfo(AnsiString nome, double &valor)
   {
   //proteção
   if (registro == NULL) return(false);
   //insere Info no Registro
   return(registro->GetInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::GetInfo(AnsiString nome, float &valor)
   {
   //proteção
   if (registro == NULL) return(false);
   //insere Info no Registro
   return(registro->GetInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::GetInfo(AnsiString nome, int &valor)
   {
   //proteção
   if (registro == NULL) return(false);
   //insere Info no Registro
   return(registro->GetInfo(nome, valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::GetInfo(AnsiString nome, unsigned &valor)
   {
   //proteção
   if (registro == NULL) return(false);
   //insere Info no Registro
   return(registro->GetInfo(nome, valor));
   }
//---------------------------------------------------------------------------
bool __fastcall TInfoset::GetInfo(AnsiString nome, bool &valor)
	{
	int valor_int;
	bool sucesso = false;
	//proteção
	if (registro == NULL) return(false);
	//insere Info no Registro
	sucesso = registro->GetInfo(nome, valor_int);

	if(valor_int == 1)
		{valor = true;}
	else
		{valor = false;}

	return sucesso;
	}
//---------------------------------------------------------------------------
bool __fastcall TInfoset::InsertNewLine(void)
   {
   //cria um novo Registro e insere em lisREG
   if ((registro = NewObjRegistro()) != NULL)
      {//insere Registro em lisREG
      lisREG->Add(registro);
      }
   return(registro != NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TInfoset::LeCampos(AnsiString txt, TStringList *campos)
	{
	//variáveis locais
	AnsiString separadores = "=;";

	//reinicia StrinList campos
	campos->Clear();
	//extrai campos do txt
	ExtraiStrings(txt, separadores, campos);
	//elimina espaços em branco dos strings
	for (int n = 0; n < campos->Count; n++)
		{
		campos->Strings[n] = campos->Strings[n].Trim();
		}
   return(campos->Count);
	}

//---------------------------------------------------------------------------
bool __fastcall TInfoset::LoadFromFile(AnsiString arquivo)
   {
   //variáveis locais
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString diretorio = "";

   diretorio = ExtractFileDir(arquivo);
   if (diretorio.IsEmpty())
	  {
	  //define nome do arquivo
	  arquivo = path->DirUsuario() + "\\" + arquivo;
	  }
   //reinicia lista de Registros
   Clear();
   //verifica se existe o arquivo
   if (! FileExists(arquivo)) return(false);
   //lê arquivo e extrai parâmetros
   lines->LoadFromFile(arquivo);
   for (int n = 0; n < lines->Count; n++)
      {
      LeCampos(lines->Strings[n], campos);
      if (campos->Count < 2 ) continue;
      //cria um novo Registro e insere em lisREG
      if (! InsertNewLine()) return(false);
      for (int nc = 0; nc < campos->Count-1; nc+= 2)
         {//insere novo objeto TInfo no Registro
         registro->AddInfo(campos->Strings[nc], campos->Strings[nc+1]);
         }
      }
   //limpa lines e campos
   lines->Clear();
   campos->Clear();
   //seleciona primeiro Registro
   return(SelectLine(0));
   }
   
//---------------------------------------------------------------------------
bool __fastcall TInfoset::LoadFromFile(TForm *form)
   {
   //variáveis locais
   AnsiString arquivo;
   int        align, color, left, top, height, width;
   int        MaxWidth, MaxHeight;

   //define nome do arquivo
   if (form->Parent == NULL)
      {
      arquivo   = AnsiString(form->ClassName());
      MaxHeight = Screen->Height;
      MaxWidth  = Screen->Width;
      }
   else
      {
      arquivo = AnsiString(form->ClassName()) + "_" + AnsiString(form->Parent->Name);
      MaxHeight = form->Parent->Height;
      MaxWidth  = form->Parent->Width;
      }
   //lê informações do arquivo
   if (! LoadFromFile(arquivo)) return(false);
   //salva atributos originais do Form
   align  = int(form->Align);
   color  = int(form->Color);
   left   = form->Left;
   top    = form->Top;
   height = form->Height;
   width  = form->Width;
   //lê atributos do Form salvos no único registro do arquivo
   registro->GetInfo("Align",  align );
   registro->GetInfo("Color",  color );
   registro->GetInfo("Left",   left  );
   registro->GetInfo("Top",    top   );
   registro->GetInfo("Height", height);
   registro->GetInfo("Width",  width );
   //proteções
   if ((top  < 0)||(top  >= MaxHeight)) top  = 0;
   if ((left < 0)||(left >= MaxWidth))  left = 0;
   //redefine atributos do Form
   form->Align  = TAlign(align);
   form->Color  = TColor(color);
   form->Left   = left;
   form->Top    = top;
   form->Height = height;
   form->Width  = width;
   //reinicia lisINFO
   Clear();

   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TInfoset::PM_GetCount(void)
   {
   return(lisREG->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::SaveToFile(AnsiString arquivo)
   {
   //variáveis locais
   AnsiString diretorio, valor;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   diretorio = "";
   diretorio = ExtractFileDir(arquivo);

   //verifica se há registros em lisREG
   if (lisREG->Count == 0) return(true);
   //proteção
   if (! DirectoryExists(path->DirBase)) return(false);
   //verifica se existe o diretório do arquivo
   if (diretorio.IsEmpty())
	  {
	  //define nome do arquivo
	  diretorio = path->DirUsuario();
	  arquivo = diretorio + "\\" + arquivo;
	  }
   if (! DirectoryExists(diretorio))
      {//cria o diretorio
      CreateDir(diretorio);
      }
   //monta lines
   lines->Clear();
   //loop p/ todos Registros
   for (int n = 0; n < lisREG->Count; n++)
      {
      registro = (VTRegistro*)(lisREG->Items[n]);
      if (! registro->GetAllInfoAsString(valor)) return(false);
      lines->Add(valor);
      }
   //salva lines no arquivo
   lines->SaveToFile(arquivo);
   //reinicia lines
   lines->Clear();
   //reinicia lisREG
   Clear();

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::SaveToFile(TForm *form)
   {
   //variáveis locais
   bool       sucesso;
   AnsiString arquivo;

   //reinicia lisREG
   Clear();
   //define nome do arquivo
   if (form->Parent == NULL) arquivo = AnsiString(form->ClassName());
   else                      arquivo = AnsiString(form->ClassName()) + "_" + AnsiString(form->Parent->Name);
   //lê arquivo previamente gravado
   LoadFromFile(arquivo);
   if (registro == NULL) InsertNewLine();
   //monta atributos
   AddInfo("Align",  form->Align );
   AddInfo("Left",   form->Left  );
   AddInfo("Top",    form->Top   );
   AddInfo("Height", form->Height);
   AddInfo("Width",  form->Width );
   AddInfo("Color",  form->Color );
   //salva atributos no arquivo
   sucesso = SaveToFile(arquivo);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TInfoset::SelectLine(int index)
   {
   if (index < lisREG->Count) registro = (VTRegistro*)lisREG->Items[index];
   else                       registro = NULL;

   return(registro != NULL);
   }

//---------------------------------------------------------------------------
//eof
