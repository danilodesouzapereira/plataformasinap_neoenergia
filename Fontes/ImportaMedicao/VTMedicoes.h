//---------------------------------------------------------------------------
#ifndef VTMedicoesH
#define VTMedicoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCanal;
class VTChave;
class VTCurva;
class VTMedicao;
class VTMedidor;
class VTRede;

//---------------------------------------------------------------------------
//tipo de medicao
enum enumMEDICAO {medicaoIMAXxFASE=1, medicaoCURVA_Ix1FASE,  medicaoCURVA_Ix3FASE,
                  medicaoCURVA_P,     medicaoCURVA_PQxFASE,  medicaoCURVA_PQ,
                  medicaoENERGIA_MWH};

//---------------------------------------------------------------------------
class VTMedicoes : public TObject
   {
   public:  //property
      __property int     Tipo          = {read=PD.tipo,       write=PD.tipo   };

   public:
                         __fastcall  VTMedicoes(void) {};
      virtual            __fastcall ~VTMedicoes(void) {};
      virtual void       __fastcall  AtualizaMedidores(void) = 0;
      virtual void       __fastcall  AcertaBarraMedidores(void) = 0;
      virtual void       __fastcall  Clear(void)=0;
      virtual void       __fastcall  ConverteMedidorPtrifasicoToPQfase(void)=0;
      virtual void       __fastcall  CorrenteToPQfase(void)=0;
      virtual void       __fastcall  CorrenteToPQtrifasico(void)=0;
	  virtual bool       __fastcall  CriaMedicoesMedidores(void)=0;
	  virtual VTMedicao* __fastcall  CriaMedicaoMedidor(VTMedidor *medidor)=0;
      virtual VTMedicao* __fastcall  ExisteMedicao(AnsiString cod_chave)=0;
	  virtual VTMedidor* __fastcall  ExisteMedidor(VTChave *chave) = 0;
      virtual bool       __fastcall  FluxoZerado(VTRede *rede) = 0;
      virtual void       __fastcall  InsereDisjuntoresFicticios(bool clear_lisRedeVal = false) = 0;
      virtual VTMedidor* __fastcall  InsereMedidor(VTChave *chave) = 0;
      virtual TList*     __fastcall  LisMedicao(void)=0;
      virtual void       __fastcall  LisMedicaoComErroNoAjuste(TList *lisMED_ERRO) = 0;
      virtual TList*     __fastcall  LisMedidor(void)=0;
      virtual TList*     __fastcall  LisMedidorAjustado(void) = 0;
	  virtual TList*     __fastcall  LisMedicao(VTRede *rede)=0;
      virtual TList*     __fastcall  LisMedicaoDJ(void)=0;
      virtual TList*     __fastcall  LisMedicaoValida(bool med_neg_enabled)=0;
      virtual double     __fastcall  MaiorErro(VTCanal *canal) = 0;
      //virtual void       __fastcall  PreencheLisMedidor(void) = 0;
      virtual void       __fastcall  PreencheLisMedidor(bool clear_lisRedeVal = false) = 0;
      virtual void       __fastcall  PreencheLisRedeVal(TList *lisREDE) = 0;
      virtual void       __fastcall  RemoveChavesFic(void) = 0;
      virtual bool       __fastcall  Valida(bool limite_ajuste, double ajuste_max) = 0;

   protected:  //dados acessados via property
      struct   {
               int   tipo;
               }PD;
   };

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTMedicoes* __fastcall NewObjMedicoes(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof

