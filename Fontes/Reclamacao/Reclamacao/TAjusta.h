//---------------------------------------------------------------------------
#ifndef TAjustaH
#define TAjustaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAjusta.h"

//---------------------------------------------------------------------------
class VTCarga;
class VTFases;
class VTFlow;
class VTTrafo;
class VTSuprimento;
class VTProgresso;
//class VTPerfilSup;
class VTEt;

//---------------------------------------------------------------------------
class TAjusta : public VTAjusta
   {
   public:
           __fastcall  TAjusta(VTApl *apl);
           __fastcall ~TAjusta(void);
      bool __fastcall  CalculaFluxoComObra(void);
      bool __fastcall  Executa(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_cons);

   private: //métodos
      //VTBarra*       __fastcall BarraSecundarioET(void);
      bool           __fastcall CalculaFluxo(void);
      bool           __fastcall CalculaFluxoInicial(void);
      bool           __fastcall CalculaFluxoSemObra(void);
		VTEt*          __fastcall CriaEt(void);
      //void           __fastcall DefineAngulosDasFases(int fases, double t_rad[MAX_FASE]);
      //void           __fastcall DefineTensaoSuprimento(VTSuprimento *suprimento, int ind_pat, complex<double> vsup_pu[MAX_FASE]);
      VTTrafo*       __fastcall ExisteTrafoET(void);
      //void           __fastcall IniciaCurvaTensaoSuprimento(VTSuprimento *suprimento, int ind_dia);
      //void           __fastcall IniciaCurvaTensaoSuprimento_1pu(VTSuprimento *suprimento);
      void           __fastcall MontaPerfilRede(void);
      bool           __fastcall RedefineCargaComCurvaPropria(VTCarga* carga);;
      bool           __fastcall RedefineCargaComDemandaPorFase(VTCarga *carga);
      bool           __fastcall RedefineCargasComDemandaPorFase(void);
      void           __fastcall RedefineSuprimentoComFornecimentoPorFase(void);

   private: //objetos externos
      VTApl        *apl;
      VTSuprimento *suprimento;

   private: //dados locais
      VTFases     *fases;
      VTPerfilAlt *perfil_alt_padrao;
      VTProgresso *prog;
      VTFlow      *flow;
      TList       *lisCAR;
		TList       *lisTRF;
      VTEt        *et;
   };

//---------------------------------------------------------------------------
#endif
//eof

