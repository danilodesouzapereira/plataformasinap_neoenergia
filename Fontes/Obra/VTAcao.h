//---------------------------------------------------------------------------
#ifndef VTAcaoH
#define VTAcaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//OBS: este enum está associado c/ base de dados
enum tipoACAO {acaoINDEFINIDA=-1, acaoALTERA=1, acaoINSERE, acaoRETIRA, acaoCOMPOSTA};

//---------------------------------------------------------------------------
class VTEqpto;
class VTObra;
class VTItemObra;

//---------------------------------------------------------------------------
class VTAcao : public TObject
   {
   public:  //property
      __property int     Id      = {read=PD.id,     write=PD.id};
      __property int     Index   = {read=PD.index,  write=PD.index};
      __property VTAcao *Pai     = {read=PD.pai,    write=PD.pai};
	  __property bool    Moving  = {read=PD.moving, write=PD.moving};
	  __property bool    Fixa    = {read=PD.fixa,   write=PD.fixa};
	  __property VTObra* Obra    = {read=PD.obra,   write=PD.obra};
	  __property VTItemObra* ItemObra    = {read=PD.itemObra,   write=PD.itemObra};
	  __property VTAcao* AcaoClone    = {read=PD.clone,    write=PD.clone};

   public:
                         __fastcall  VTAcao(void) {};
                         __fastcall ~VTAcao(void) {};
      virtual void       __fastcall  Add(VTAcao *acao) {};
      virtual VTEqpto*   __fastcall  Eqpto(void) = 0;
      virtual VTEqpto*   __fastcall  CopiaDoEqptoOriginal(void) = 0;
      virtual VTEqpto*   __fastcall  CopiaDoEqptoAlterado(void) = 0;
      virtual TList*     __fastcall  LisAcao(void) {return NULL;};
      virtual int        __fastcall  Tipo(void) = 0;
      virtual AnsiString __fastcall  TipoAsString(void) = 0;

   protected:  //dados acessados via property
      struct   {
               int     id;
               int     index;     //índice sequencial da Acao dentro da Obra
               bool    moving;
               bool    fixa;
               VTAcao  *pai;
			   VTObra  *obra;
			   VTItemObra *itemObra;
			   VTAcao *clone;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTAcao
//---------------------------------------------------------------------------
VTAcao* __fastcall NewObjAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original=NULL);
VTAcao* __fastcall NewObjAcaoInsere(VTEqpto *eqpto);
VTAcao* __fastcall NewObjAcaoRetira(VTEqpto *eqpto);
VTAcao* __fastcall NewObjAcaoComposta(void);

//---------------------------------------------------------------------------
#endif
//eof



 