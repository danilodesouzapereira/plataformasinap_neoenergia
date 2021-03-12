//---------------------------------------------------------------------------
#ifndef TMedicaoH
#define TMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class TMedicao : public TObject
   {
   public:
             __fastcall  TMedicao(VTRede *rede);
             __fastcall ~TMedicao(void);
      void   __fastcall  Clear(void);
      double __fastcall  PerdaTotal_mwhmes(void);
	  double __fastcall  PerdaComercial_perc(void);
	  double __fastcall  PerdaComercialA3a_perc(void);
	  double __fastcall  PerdaComercialA4_perc(void);
	  double __fastcall  PerdaComercialB_perc(void);
	  double __fastcall  PerdaComercialOutros_perc(void);
      double __fastcall  PerdaTecnica_perc(void);
	  double __fastcall  PerdaTotal_perc(void);

   private:
	  double   __fastcall  FatorCargaMT_perc(double energia_cargas);
      double   __fastcall  PerdaNT_perc(double energia_fim, double energia_ini);

   public:
      int        level;
      VTRede     *rede;
      AnsiString cod_rede;
      TList      *lisREDE_AJUS;
      TList      *lisREDE_FIXA;
      TList      *lisCAR_BT_AJUS;
      TList      *lisCAR_BT_FIXA;
	  TList      *lisCAR_MT_AJUS;
	  TList      *lisCAR_MT_FIXA;
	  bool valida;
	  struct{
			double medida_mwhmes;
			double carga_ini_mwhmes;
			double carga_fim_mwhmes;
			}energia;
      struct{
            double tecnica_mwhmes;
            }perda;
      struct{
            double sdbt_pu;
			double sdmt_pu;
            bool   valida;
			}alocacao_perda_comercial;
	  //guardar fatores
	  struct{
			double mt;
			double bt;
	  }fator;
	  //separacao do MT em A3A, A4 e Outros
	  struct{
		 TList* lisCAR;
		 struct
			{
			double carga_ini_mwhmes;
			double carga_fim_mwhmes;
			}energia;
		 }A3a,A4,outros, B;
	};

//---------------------------------------------------------------------------
#endif
//eof

