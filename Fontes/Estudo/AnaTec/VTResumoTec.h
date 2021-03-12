//---------------------------------------------------------------------------
#ifndef VTResumoTecH
#define VTResumoTecH

//----------------------------------------------------------------------------
#include <Classes.hpp>

//----------------------------------------------------------------------------
class VTEqpto;
class VTRede;

//----------------------------------------------------------------------------
class VTResumoTec : public TObject
   {
   public:
      __property int    AnoInicial                       = {read=PM_GetAnoInicial                                 };
      __property int    AnoFinal                         = {read=PM_GetAnoFinal                                   };
      __property double Nota                             = {read=PM_GetNota                                       };
      __property bool   Calculo_OK[int ano]              = {read=PM_GetCalculo_OK,     write=PM_SetCalculo_OK     };
      __property double Compensacao[int ano]             = {read=PM_GetCompensacao_RS, write=PM_SetCompensacao_RS };
      __property double Perda_mwhm[int ano]              = {read=PM_GetPerda_mwhm,     write=PM_SetPerda_mwhm     };
      __property double Perda_perc[int ano]              = {read=PM_GetPerda_perc,     write=PM_SetPerda_perc     };
	  __property double DEC[int ano]                     = {read=PM_GetDEC,            write=PM_SetDEC            };
	  __property double END[int ano]                     = {read=PM_GetEND,            write=PM_SetEND            };
	  __property double FEC[int ano]                     = {read=PM_GetFEC,            write=PM_SetFEC            };
      __property double Entrada_mwhm[int ano]            = {read=PM_GetEntrada,        write=PM_SetEntrada        };
      __property double Entregue_mwhm[int ano]           = {read=PM_GetEntregue,       write=PM_SetEntregue       };
	  __property double Gerada_mwhm [int ano]            = {read=PM_GetGerada,         write=PM_SetGerada         };
	  __property int    DiagBarra[int ano][int idiag]    = {read=PM_GetDiagBarra,      write=PM_SetDiagBarra      };
	  __property int    DiagCarga[int ano][int idiag]    = {read=PM_GetDiagCarga,      write=PM_SetDiagCarga      };
      __property double DiagTrechoKm[int ano][int idiag] = {read=PM_GetDiagTrechoKm,   write=PM_SetDiagTrechoKm   };
	  __property int    DiagTrafo[int ano][int idiag]    = {read=PM_GetDiagTrafo,      write=PM_SetDiagTrafo      };
	  __property double PercDiagBarra[int ano][int idiag] = {read=PM_GetPercDiagBarra,  write=PM_SetPercDiagBarra  };
	  __property double PercDiagCarga[int ano][int idiag] = {read=PM_GetPercDiagCarga,  write=PM_SetPercDiagCarga  };
	  __property double PercDiagTrafo[int ano][int idiag] = {read=PM_GetPercDiagTrafo, write=PM_SetPercDiagTrafo   };
	  //novos
	  __property AnsiString Alimentador = {read=PD.alimentador, write=PD.alimentador  };
	  __property AnsiString Subestacao  = {read=PD.subestacao,  write=PD.subestacao   };
	  __property AnsiString CodAlimentador = {read=PD.cod_alimentador, write=PD.cod_alimentador  };
	  __property AnsiString CodSubestacao  = {read=PD.cod_subestacao,  write=PD.cod_subestacao   };

   public:
						   __fastcall  VTResumoTec(void) {};
	  virtual              __fastcall ~VTResumoTec(void) {};
	  virtual VTResumoTec* __fastcall ExisteResumoRede(AnsiString alimentador, AnsiString subestacao) = 0;
	  virtual       TList* __fastcall LisResumoTecRede(void) = 0;

   protected: //dados acessados via property
     struct str_ENERGIA {
                        bool   calculo_ok;
                        int    diag_bar_ruim;
						int    diag_bar_reg;
						int    diag_bar_bom;
						int    diag_car_ruim;
						int    diag_car_reg;
						int    diag_car_bom;
						int    diag_tra_ruim;
						int    diag_tra_reg;
                        int    diag_tra_bom;
						double diag_tre_bom_km;
						double diag_tre_ruim_km;
						double diag_tre_reg_km;
						double dec, end, fec, compensacao;
                        double entrada_mwhm;
                        double entregue_mwhm;
                        double gerada_mwhm;
                        double perda_mwhm;
						double perda_perc;
						double perc_bar_bom;
						double perc_bar_reg;
						double perc_bar_ruim;
						double perc_car_bom;
						double perc_car_reg;
						double perc_car_ruim;
						double perc_tra_bom;
						double perc_tra_reg;
						double perc_tra_ruim;
                        };
      struct   {
               int  ano_ini, ano_fim;
			   DynamicArray<str_ENERGIA> energia;
			   AnsiString alimentador, subestacao;
			   AnsiString cod_alimentador, cod_subestacao;
			   } PD;
      virtual int    __fastcall PM_GetAnoInicial(void) = 0;
	  virtual int    __fastcall PM_GetAnoFinal(void) = 0;
	  virtual bool   __fastcall PM_GetCalculo_OK(int ano) = 0;
	  virtual double __fastcall PM_GetCompensacao_RS(int ano) = 0;
	  virtual int    __fastcall PM_GetDiagBarra(int ano, int idiag) = 0;
	  virtual int    __fastcall PM_GetDiagCarga(int ano, int idiag) = 0;
	  virtual int    __fastcall PM_GetDiagTrafo(int ano, int idiag) = 0;
	  virtual double __fastcall PM_GetDiagTrechoKm(int ano, int idiag) = 0;
	  virtual double __fastcall PM_GetDEC(int ano) = 0;
	  virtual double __fastcall PM_GetEND(int ano) = 0;
	  virtual double __fastcall PM_GetFEC(int ano) = 0;
	  virtual double __fastcall PM_GetEntrada(int ano) = 0;
	  virtual double __fastcall PM_GetEntregue(int ano) = 0;
	  virtual double __fastcall PM_GetGerada(int ano) = 0;
	  virtual double __fastcall PM_GetNota(void) = 0;
	  virtual double __fastcall PM_GetPercDiagBarra(int ano, int idiag) = 0;  //DVK 2013.12.20
	  virtual double __fastcall PM_GetPercDiagCarga(int ano, int idiag) = 0;
      virtual double __fastcall PM_GetPercDiagTrafo(int ano, int idiag) = 0;
	  virtual double __fastcall PM_GetPerda_mwhm(int ano) = 0;
	  virtual double __fastcall PM_GetPerda_perc(int ano) = 0;
	  //
	  virtual void   __fastcall PM_SetCalculo_OK(int ano, bool calculo_ok) = 0;
	  virtual void   __fastcall PM_SetCompensacao_RS(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetDiagBarra(int ano, int idiag, int neq) = 0;
	  virtual void   __fastcall PM_SetDiagCarga(int ano, int idiag, int neq) = 0;
	  virtual void   __fastcall PM_SetDiagTrafo(int ano, int idiag, int neq) = 0;
	  virtual void   __fastcall PM_SetDiagTrechoKm(int ano, int idiag, double comp_km) = 0;
	  virtual void   __fastcall PM_SetDEC(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetEND(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetFEC(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetEntrada(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetEntregue(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetGerada(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetPercDiagBarra(int ano, int idiag, double neq) = 0;  //DVK 2013.12.20
	  virtual void   __fastcall PM_SetPercDiagCarga(int ano, int idiag, double neq) = 0;
	  virtual void   __fastcall PM_SetPercDiagTrafo(int ano, int idiag, double neq) = 0;
	  virtual void   __fastcall PM_SetPerda_mwhm(int ano, double valor) = 0;
	  virtual void   __fastcall PM_SetPerda_perc(int ano, double valor) = 0;
   };

//---------------------------------------------------------------------------
//função p/ criar método da classe
//---------------------------------------------------------------------------
VTResumoTec* __fastcall NewObjResumoTec(int ano_ini, int ano_fim);

//---------------------------------------------------------------------------
#endif
//eof
