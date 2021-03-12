//---------------------------------------------------------------------------
#ifndef VTVetvalH
#define VTVetvalH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTVetval : public TObject
   {
   protected: //dados acessados via property
      struct   {
               AnsiString *val_str;
               double     *valor;
               int        len;
               } PD;
   protected: //métodos acessados via property
      virtual double     __fastcall PM_GetValor(int ind) = 0;
      virtual AnsiString __fastcall PM_GetValorAsStr(int ind) = 0;
      virtual void       __fastcall PM_SetValor(int ind, double valor) = 0;
      virtual void       __fastcall PM_SetValorAsStr(int ind, AnsiString val_str) = 0;

   public:  //property
      __property int        Len                 = {read=PD.len};
      __property double     Valor[int ind]      = {read=PM_GetValor,      write=PM_SetValor};
      __property AnsiString ValorAsStr[int ind] = {read=PM_GetValorAsStr, write=PM_SetValorAsStr};

   public:
                         __fastcall VTVetval(void) {};
                         __fastcall ~VTVetval(void) {};
      //virtual void       __fastcall AjustaUE_A(void) = 0;
      //virtual void       __fastcall AjustaUE_KV(void) = 0;
      //virtual void       __fastcall AjustaUE_MVA(void) = 0;
      //virtual void       __fastcall AjustaUE_MVAR(void) = 0;
      //virtual void       __fastcall AjustaUE_MW(void) = 0;
      virtual AnsiString __fastcall DescUnid(void) = 0;
      
   public: //dados
      int        ind_atrib;
      AnsiString descricao, unidade;
   };
   
//---------------------------------------------------------------------------
//função global p/ criar objetos VTVetval
//---------------------------------------------------------------------------
VTVetval* __fastcall NewObjVetval(int ind_atrib, AnsiString descricao, int num_pat);

//---------------------------------------------------------------------------
#endif
//eof
