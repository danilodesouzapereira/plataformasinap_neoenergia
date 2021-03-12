//---------------------------------------------------------------------------
#ifndef VTMunicipioH
#define VTMunicipioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTMunicipio : public TObject
   {
   public: //métodos
						   __fastcall  VTMunicipio(void) {};
						   __fastcall ~VTMunicipio(void) {};
	  virtual int          __fastcall  Codigo(AnsiString nome) = 0;
	  virtual bool         __fastcall  Insere(int codigo, AnsiString nome, AnsiString regiao) = 0;
	  virtual void         __fastcall  Limpa(void) = 0;
	  virtual AnsiString   __fastcall  Nome(int codigo) = 0;
	  virtual AnsiString   __fastcall  Regiao(int codigo) = 0; //Tabela HARDCODED
	  virtual TStringList* __fastcall  SLisNome(void) = 0;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMunicipio
//---------------------------------------------------------------------------
VTMunicipio* __fastcall NewObjMunicipio(void);

//---------------------------------------------------------------------------
#endif
//eof

