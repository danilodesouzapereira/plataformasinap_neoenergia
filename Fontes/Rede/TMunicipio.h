//---------------------------------------------------------------------------
#ifndef TMunicipioH
#define TMunicipioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMunicipio.h"
#include<map>

//---------------------------------------------------------------------------
class TMunicipio : public VTMunicipio
   {
   public: //métodos
					__fastcall  TMunicipio(void);
					__fastcall ~TMunicipio(void);
	  int        	__fastcall  Codigo(AnsiString nome);
	  bool        	__fastcall  Insere(int codigo, AnsiString nome, AnsiString regiao);
	  void         	__fastcall  Limpa(void);
	  AnsiString   	__fastcall  Nome(int codigo);
	  AnsiString    __fastcall  Regiao(int codigo);
	  TStringList* 	__fastcall  SLisNome(void);

   private:

   private:
	  TStringList *slisNome;
	  std::map<AnsiString,int> municipio;
      std::map<int,AnsiString> mun_regiao;

   };

//---------------------------------------------------------------------------
#endif
//eof

