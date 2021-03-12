//---------------------------------------------------------------------------
#ifndef TCurvaPQH
#define TCurvaPQH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCurva.h"

//---------------------------------------------------------------------------
class TCurvaPQ : public VTCurva
	{
   public:  //funções públicas
                        __fastcall  TCurvaPQ(void);
								__fastcall ~TCurvaPQ(void);
      virtual VTCurva*  __fastcall  Clone(void);
		virtual void      __fastcall  CopiaAtributosDe(VTCurva &ref);
		virtual void      __fastcall  DefineStatus(int bit_status, bool enabled);
      virtual double    __fastcall  DesvioPadrao(int ind_val);
      virtual bool      __fastcall  GetPeriodo(int index, strHM &hm_ini, strHM &hm_fim);
      virtual bool      __fastcall  GetPonto(int index, strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor);
      virtual bool      __fastcall  GetValor(strHM &hm, double valor[], int dim_valor);
      virtual double    __fastcall  Integral(int ind_val);
		virtual double    __fastcall  Maximo(int ind_val);
		virtual double    __fastcall  Media(int ind_val);
      virtual bool      __fastcall  MediaVarianciaDesvioPadrao(int index, double &media, double &variancia, double &desvio);
		virtual double    __fastcall  Minimo(int ind_val);
		virtual int       __fastcall  NumeroPontos(void);
		virtual void      __fastcall  OrdenaPontos(void);
      virtual void      __fastcall  Reconfigura(int escala);
		virtual void      __fastcall  Reinicia(void);
		virtual bool      __fastcall  RetiraPonto(int index);
      virtual bool      __fastcall  SetPonto(strHM &hm, double valor[], int dim_valor);
      virtual bool      __fastcall  SetPonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor);
      virtual bool      __fastcall  ValidaPeriodo24Horas(void);
      virtual double    __fastcall  Variancia(int ind_val);

   protected: //métodos acessados via property
      AnsiString __fastcall PM_GetDiaMedicaoAsStr(void);
      double     __fastcall PM_GetFatorCorrecaoEnergia(void);
      bool       __fastcall PM_GetStatus(unsigned bit);
      unsigned   __fastcall PM_GetStatusAsUnsigned(void);
      AnsiString __fastcall PM_GetTipoAsStr(void);
      //
      void       __fastcall PM_SetFatorCorrecaoEnergia(double fatcor);
      void       __fastcall PM_SetStatus(unsigned bit, bool enabled);
      void       __fastcall PM_SetStatusAsUnsigned(unsigned status);

   //===================================
   protected: //métodos temporários acessados via property
      AnsiString __fastcall PM_GetCodigoBD(void);
      void       __fastcall PM_SetCodigoBD(AnsiString codigo_bd);
   //===================================

   protected:  //dados locais
      struct strPONTO   {
                        strHM hm_ini;
                        strHM hm_fim;
                        float valor[2];
                        } ponto[24];
                        
   protected: //funções internas
      void       __fastcall ConvertePadraoAntigoHoraMinutoFinal(strHM &hm_fim);
      bool       __fastcall DefineHoraMinutoFinal(strHM &hm_ini, strHM &hm_fim);
      strPONTO*  __fastcall ExistePonto(strHM &hm);
      strPONTO*  __fastcall ExistePonto(strHM &hm_ini, strHM &hm_fim);
      bool       __fastcall InserePonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor);
      bool       __fastcall ValidaHoraMinutoInicial(strHM &hm_ini);
      bool       __fastcall ValidaHoraMinutoFinal(strHM &hm_fim);

   };

//---------------------------------------------------------------------------
#endif
//eof
