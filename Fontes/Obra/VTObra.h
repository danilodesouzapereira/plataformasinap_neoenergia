//---------------------------------------------------------------------------
#ifndef VTObraH
#define VTObraH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDuplicacao;
class VTAcao;
class VTApl;
class VTEqpto;
class VTObras;
class VTLixeira;
class VTItemObra;
class VTRede;
//class VTPadrao;

//---------------------------------------------------------------------------
class VTObra : public TObject
   {
   public:  //property
      __property int        Id                    = {read=PM_GetId,                write=PM_SetId                };
      __property int        Tipo                  = {read=PM_GetTipo,              write=PM_SetTipo              };
      __property AnsiString Nome                  = {read=PM_GetNome,              write=PM_SetNome              };
      __property double     Custo                 = {read=PM_GetCusto,             write=PM_SetCusto             };
      __property double     ObEspecial            = {read=PM_GetObEspecial,        write=PM_SetObEspecial        };
      __property double     PartOtr               = {read=PM_GetPartOtr,           write=PM_SetPartOtr           };
      __property int        AnoAtivacao           = {read=PM_GetAnoAtivacao                                       };
      __property int        AnoInicio             = {read=PM_GetAnoInicio                                        };
      __property TDateTime  DataAtivacao          = {read=PM_GetDataAtivacao,      write=PM_SetDataAtivacao      };
      __property TDateTime  DataInicio            = {read=PM_GetDataInicio,        write=PM_SetDataInicio        };
      __property AnsiString Responsavel           = {read=PM_GetResponsavel,       write=PM_SetResponsavel       };
      __property VTLixeira* Lixeira               = {read=PM_GetLixeira                                          };
      __property VTObras*   Obras                 = {read=PM_GetObras,             write=PM_SetObras             };
	  __property bool       Visible               = {read=PM_GetVisible,           write=PM_SetVisible           };
	  __property bool       Incorporada           = {read=PM_GetIncorporada,       write=PM_SetIncorporada       };
      __property bool       Fixa                  = {read=PM_GetFixa,              write=PM_SetFixa              };
	  __property TList*     LisAcao               = {read=PM_GetLisAcao};
	  __property TList*     LisObject             = {read=PM_GetLisObject};
	  __property TList*     LisItemObra             = {read=PM_GetLisItemObra};
		__property int        ModalResult           = {read=PM_GetModalResult,       write=PM_SetModalResult       };
		__property TColor     Color                 = {read=PM_GetColor,             write=PM_SetColor             };
		__property bool       ColorEnabled          = {read=PM_GetColorEnabled,      write=PM_SetColorEnabled      };
      //alteração 2016.07.28
		__property bool       AlteraTaxaFalha       = {read=PM_GetAlteraTaxaFalha,   write=PM_SetAlteraTaxaFalha   };
      __property double     TaxaFalhaInsercao     = {read=PM_GetTaxaFalhaInsercao, write=PM_SetTaxaFalhaInsercao };
      __property double     TaxaFalhaSubstituicao = {read=PM_GetTaxaFalhaSubstituicao, write=PM_SetTaxaFalhaSubstituicao };
	  //extras FKM
	  __property bool       Finalizada            = {read=PD.finalizada,      write=PD.finalizada};
	  __property bool       Bloqueada             = {read=PD.bloqueada,       write=PD.bloqueada};
	  __property unsigned int Grupo               = {read=PD.grupo,           write=PD.grupo};

   public:
                        __fastcall  VTObra(void) {};
      virtual           __fastcall ~VTObra(void) {};
      virtual void      __fastcall  AddAcao(VTAcao *acao)=0;
      virtual VTAcao*   __fastcall  AddAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original)=0;
      virtual VTAcao*   __fastcall  AddAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTAcao *owner=NULL)=0;
      virtual VTAcao*   __fastcall  AddAcaoComposta(void)=0;
      virtual VTAcao*   __fastcall  AddAcaoInsere(VTEqpto *eqpto, VTAcao *owner=NULL)=0;
	  virtual VTAcao*   __fastcall  AddAcaoRetira(VTEqpto *eqpto, VTAcao *owner=NULL)=0;
	  virtual VTItemObra* __fastcall AddItemObra(void)=0;
	  virtual bool		  __fastcall AddItemObra(VTItemObra *item)=0;
	  virtual bool		  __fastcall AddRede(VTRede *rede)=0;
      virtual void      __fastcall  CancelaRedo(void) = 0;
      virtual void      __fastcall  CancelaUndo(void) = 0;
      virtual void      __fastcall  Clear(void) = 0;
	  virtual VTObra*   __fastcall  Clone(TDuplicacao &duplicacao) = 0;
	  virtual VTItemObra* __fastcall CriaItemObra(void)=0;
      virtual void      __fastcall  DefineActions(TAction *ActionUndo, TAction *ActionRedo)=0;
	  virtual void      __fastcall  DeleteRedo(VTAcao *acao) = 0;
	  virtual bool    	__fastcall  DeleteItemObra(VTItemObra *itemObra)=0;
      virtual VTItemObra* __fastcall  ExisteItemObra(int id) = 0;
	  virtual void      __fastcall  FundeAcoes(void) = 0;
	  virtual void      __fastcall  IniciaLisRede(TList *lisEXT=NULL) = 0;
      virtual VTAcao*   __fastcall  LastAcao(void)=0;
	  virtual VTAcao*   __fastcall  LastAcao(int tipo, VTEqpto *eqpto)=0;
      virtual TList*    __fastcall  LisRede(void) = 0;
      virtual VTAcao*   __fastcall  Redo(bool valida_acao_fixa=false)=0;
      virtual VTAcao*   __fastcall  Undo(bool valida_acao_fixa=false)=0;

   protected:  //métodos acessados via property
		virtual bool       __fastcall PM_GetAlteraTaxaFalha(void) = 0;
      virtual int        __fastcall PM_GetAnoAtivacao(void) = 0;
		virtual int        __fastcall PM_GetAnoInicio(void) = 0;
		virtual TColor     __fastcall PM_GetColor(void) = 0;
		virtual bool       __fastcall PM_GetColorEnabled(void) = 0;
		virtual double     __fastcall PM_GetCusto(void) = 0;
		virtual TDateTime  __fastcall PM_GetDataAtivacao(void) = 0;
      virtual TDateTime  __fastcall PM_GetDataInicio(void) = 0;
      virtual bool       __fastcall PM_GetFixa(void) = 0;
      virtual int        __fastcall PM_GetId(void) = 0;
      virtual bool       __fastcall PM_GetIncorporada(void) = 0;
      virtual TList*     __fastcall PM_GetLisAcao(void) = 0;
	  virtual TList*     __fastcall PM_GetLisObject(void) = 0;
	  virtual TList*     __fastcall PM_GetLisItemObra(void) = 0;
      virtual VTLixeira* __fastcall PM_GetLixeira(void) = 0;
      virtual int        __fastcall PM_GetModalResult(void) = 0;
      virtual AnsiString __fastcall PM_GetNome(void) = 0;
      virtual double     __fastcall PM_GetObEspecial(void) = 0;
      virtual VTObras*   __fastcall PM_GetObras(void) = 0;
      virtual double     __fastcall PM_GetPartOtr(void) = 0;
      virtual AnsiString __fastcall PM_GetResponsavel(void) = 0;
      virtual double     __fastcall PM_GetTaxaFalhaInsercao(void) = 0;
      virtual double     __fastcall PM_GetTaxaFalhaSubstituicao(void) = 0;
      virtual int        __fastcall PM_GetTipo(void) = 0;
      virtual bool       __fastcall PM_GetVisible(void) = 0;
      //
      virtual void       __fastcall PM_SetAlteraTaxaFalha(bool altera) = 0;
		virtual void       __fastcall PM_SetColor(TColor color) = 0;
		virtual void       __fastcall PM_SetColorEnabled(bool enabled) = 0;
		virtual void       __fastcall PM_SetCusto(double custo) = 0;
      virtual void       __fastcall PM_SetDataAtivacao(TDateTime data) = 0;
      virtual void       __fastcall PM_SetDataInicio(TDateTime data) = 0;
      virtual void       __fastcall PM_SetFixa(bool fixa) = 0;
      virtual void       __fastcall PM_SetId(int id) = 0;
      virtual void       __fastcall PM_SetIncorporada(bool incorporada) = 0;
	  virtual void       __fastcall PM_SetModalResult(int modal_result) = 0;
      virtual void       __fastcall PM_SetNome(AnsiString nome) = 0;
      virtual void       __fastcall PM_SetObEspecial(double ob_esp) = 0;
      virtual void       __fastcall PM_SetObras(VTObras *obras) = 0;
      virtual void       __fastcall PM_SetPartOtr(double part_otr) = 0;
      virtual void       __fastcall PM_SetResponsavel(AnsiString responsavel) = 0;
      virtual void       __fastcall PM_SetTaxaFalhaInsercao(double taxa) = 0;
      virtual void       __fastcall PM_SetTaxaFalhaSubstituicao(double taxa) = 0;
      virtual void       __fastcall PM_SetTipo(int tipo) = 0;
	  virtual void       __fastcall PM_SetVisible(bool visible) = 0;

	  protected://dados acessados via property
      struct   {
               VTObras    *obras;      //objeto externo
               int        id;
               int        tipo;
               AnsiString nome;
               AnsiString responsavel;
               TDateTime  data_inicio;
               TDateTime  data_ativacao;
               double     custo;
               double     ob_esp;     //obrigação especial (%)
               double     part_otr;   //participação OTR (%)
               bool       visible;
               bool       fixa;
               bool       incorporada;
					int        modal_result;
					TColor     color;
					bool       color_enabled;
               //dados para cálculo de continuidade
               bool       altera_taxa_falha;
               double     taxa_falha_insercao;
               double     taxa_falha_substituicao;
					//IMPORTANTE: lisOBJ não pode ficar dentro de PD em razão da função Clone()
			   //TList     *lisOBJ;     //lista p/ objetos externos associados à Obra
               bool bloqueada;
				bool finalizada;
			   unsigned int grupo;
			   }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe VTObra
//---------------------------------------------------------------------------
VTObra* NewObjObra(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

