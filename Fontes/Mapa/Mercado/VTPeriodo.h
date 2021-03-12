//---------------------------------------------------------------------------
#ifndef VTPeriodoH
#define VTPeriodoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPeriodo : public TObject
   {
   public:  //property
      __property int    AnoFinal    = {read=PD.ano_fim, write=PD.ano_fim};
      __property int    AnoInicial  = {read=PD.ano_ini, write=PD.ano_ini};
      __property double Valor       = {read=PM_GetValor,   write=PM_SetValor};

   public:  //funções públicas
                   __fastcall VTPeriodo(void) {};
      virtual      __fastcall ~VTPeriodo(void) {};
      virtual bool __fastcall ContemAno(int ano) = 0;

   protected: //dados acessados via property
      virtual double __fastcall PM_GetValor(void) = 0;
      virtual void   __fastcall PM_SetValor(double valor) = 0;

   protected: //dados acessados via property
      struct   {
               int    ano_ini;
               int    ano_fim;
               float  valor;
               } PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
