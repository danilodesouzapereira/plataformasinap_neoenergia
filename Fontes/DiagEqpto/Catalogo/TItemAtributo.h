//---------------------------------------------------------------------------

#ifndef TItemAtributoH
#define TItemAtributoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
//Não sei se isso é necessário
enum dataTYPE {
               dtINT = 0,
               dtDOUBLE,
               dtANSISTRING,
               dtBOOL};
//---------------------------------------------------------------------------
class TItemAtributo : public TObject
   {
   public:
            __fastcall TItemAtributo(void);
            __fastcall ~TItemAtributo(void);
      bool  __fastcall InsereAtributoFilho(TItemAtributo *atributo);
      TItemAtributo*  __fastcall InsereAtributoFilho(AnsiString nome,AnsiString valor,
                                       int dataType, bool isGroup = false,
                     TList *listaAtributosFilhos = NULL, bool needCombo = false,
      TStringList *listaValoresCombo = NULL, TItemAtributo  *atributoPai = NULL);
      bool  __fastcall InsereValorLista(AnsiString valor);
      bool  __fastcall RetiraAtributoFilho(TItemAtributo *atributo);
      bool  __fastcall RetiraValorLista(AnsiString valor);

   public:  //dados externos

   public:  //dados locais

   public: //property
      __property AnsiString   Nome                 = {read = nome, write = nome};
      __property bool         Group                = {read = PM_GetGroup, write = PM_SetGroup};
      __property bool         NeedCombo            = {read = PM_GetNeedCombo, write = PM_SetNeedCombo};
      __property AnsiString   Valor                = {read = valor, write = valor};
      __property int          ValorDataType        = {read = dataType, write = dataType};
      __property TList*       ListaAtributosFilhos = {read = listAtributos, write =PM_SetListaAtributos};
      __property TStringList* ListaValoresCombo    = {read = listValoresCombo, write =listValoresCombo};
      __property TItemAtributo* AtributoPai        = {read = atributoPai, write = atributoPai};
      __property bool         Visible              = {read = visible, write = visible};
      __property bool         FilhosVisible        = {read = filhosVisible, write = filhosVisible};
   private: //User declarations

   private: //Dados externos

   private: //Dados proprios
      TStringList    *listValoresCombo;
      TList          *listAtributos;
      TItemAtributo  *atributoPai;

   protected: //dados acessados por property
      AnsiString     nome;
      AnsiString     valor;
      int      dataType;
      bool     needCombo;
      bool     group;
      bool     visible;
      bool     filhosVisible;

   protected://métodos acessados via property
      //Getters
      bool __fastcall  PM_GetGroup(void);
      bool __fastcall  PM_GetNeedCombo(void);
      //Setters
      void __fastcall  PM_SetGroup(bool isGroup);
      void __fastcall  PM_SetNeedCombo(bool needCombo);
      void __fastcall  PM_SetListaAtributos(TList *listaAtributosNovos);

   };
#endif
