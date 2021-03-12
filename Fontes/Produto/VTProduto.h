//-----------------------------------------------------------------------------
#ifndef VTProdutoH
#define VTProdutoH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

enum produtoID {
					produtoINDEFINIDO  = -1,
					produtoSINAP       =  2,
					produtoSINAP_GRID  =  2,
					produtoSINAP_RNT   =  3,
               produtoWISE        =  4,
               produtoSELFHEALING =  5
					};

//-----------------------------------------------------------------------------
class VTProduto  : public TObject
   {
  public:  //property
      __property int        Id            = {read=PM_GetId};
      __property AnsiString Codigo        = {read=PM_GetCodigo};
      __property AnsiString CodLicenca    = {read=PM_GetCodLicenca};
      __property AnsiString CodRev        = {read=PM_GetCodRev};
      __property AnsiString CodRevSeq     = {read=PM_GetCodRevSeq};
      __property AnsiString CodRevSeqData = {read=PM_GetCodRevSeqData};
      __property AnsiString Data          = {read=PM_GetData};
      __property AnsiString Descricao     = {read=PM_GetDescricao};
      __property AnsiString Revisao       = {read=PM_GetRevisao};
      __property AnsiString RevisaoSeq    = {read=PM_GetRevisaoSeq};
      __property AnsiString Sinapsis      = {read=PM_GetSinapsis};
      __property AnsiString WinRegister   = {read=PM_GetWinRegister};

   public:
      __fastcall  VTProduto(void) {};
      __fastcall ~VTProduto(void){};

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
      virtual AnsiString __fastcall PM_GetCodLicenca(void) = 0;
      virtual AnsiString __fastcall PM_GetCodRev(void) = 0;
      virtual AnsiString __fastcall PM_GetCodRevSeq(void) = 0;
      virtual AnsiString __fastcall PM_GetCodRevSeqData(void) = 0;
      virtual AnsiString __fastcall PM_GetData(void) = 0;
      virtual AnsiString __fastcall PM_GetDescricao(void) = 0;
      virtual int        __fastcall PM_GetId(void) = 0;
      virtual AnsiString __fastcall PM_GetRevisao(void) = 0;
      virtual AnsiString __fastcall PM_GetRevisaoSeq(void) = 0;
      virtual AnsiString __fastcall PM_GetSinapsis(void) = 0;
      virtual AnsiString __fastcall PM_GetWinRegister(void) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTProduto* __fastcall NewObjProduto(void);

#endif
//---------------------------------------------------------------------------
//eof
