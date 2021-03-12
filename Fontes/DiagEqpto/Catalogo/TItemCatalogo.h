//---------------------------------------------------------------------------
#ifndef TItemCatalogoH
#define TItemCatalogoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TItemAtributo;
//---------------------------------------------------------------------------
class TItemCatalogo : public TObject
{
   public:
            __fastcall TItemCatalogo(void);
            __fastcall ~TItemCatalogo(void);
      TItemAtributo*  __fastcall CriaAtributo(AnsiString nome,AnsiString valor,
                                       int dataType, bool isGroup = false,
                     TList *listaAtributosFilhos = NULL, bool needCombo = false,
                                          TStringList *listaValoresCombo = NULL,
                                          TItemAtributo  *atributoPai = NULL);
      TItemAtributo* __fastcall Get_ItemAtributo(AnsiString nomeAtributo);
      int            __fastcall Get_ItemAtributoIndex(AnsiString nomeAtributo);

   public:  //dados externos

   public:  //dados locais
      TList       *listAtributos;
      TItemAtributo *codigo;

   public: //property
      __property AnsiString   Codigo   = {read = PM_GetCodigo, write = PM_SetCodigo};

   private: //User declarations

   private: //Dados externos

   private: //Dados proprios

   protected: //dados acessados por property

   protected://métodos acessados via property
      //Getters
      AnsiString __fastcall  PM_GetCodigo(void);
      //Setters
      void __fastcall  PM_SetCodigo(AnsiString codigo);
};

#endif

//---------------------------------------------------------------------------

